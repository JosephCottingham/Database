#include "LeafPage.hpp"

LeafPage::LeafPage(PageHeader header, std::vector<uint8_t> rawBody): header(header), rawBody(rawBody), availabilityMap(nullptr) {
    SerializeUtil serializer;

    size_t location = 0;
    slotDirectory.reserve(header.getRecordCount()); // Reserve the space for the slot directory as to prevent reallocation.
    
    // Populate the Slot Directory
    for (size_t i = 0; i < header.getRecordCount(); i++) {
        PageSlot slot;
        serializer.deserializeFromVector(rawBody, location, slot.recordAdr);
        serializer.deserializeFromVector(rawBody, location, slot.recordSize);
        serializer.deserializeFromVector(rawBody, location, slot.recordPk);
        slotDirectory.push_back(slot);
    }
}

LeafPage::LeafPage(PageHeader header): header(header), availabilityMap(nullptr) {
    this->header.setType(PageType::LEAF);
}

std::vector<uint8_t> LeafPage::getSerialized() {
    SerializeUtil serializer;
    std::vector<uint8_t> serialized;
    size_t location = 0;

    // Serialize the Slot Directory
    for (size_t i = 0; i < slotDirectory.size(); i++) {
        serializer.serializeIntoVector(serialized, location, slotDirectory[i].recordAdr);
        serializer.serializeIntoVector(serialized, location, slotDirectory[i].recordSize);
        serializer.serializeIntoVector(serialized, location, slotDirectory[i].recordPk);
    }
    return serialized;
}

std::vector<uint8_t> LeafPage::getRecord(uint64_t recordPk) {
    PageSlot slot = getSlot(recordPk);
    return std::vector<uint8_t>(rawBody.begin() + slot.recordAdr, rawBody.begin() + slot.recordAdr + slot.recordSize);
}

int8_t LeafPage::insertRecord(uint64_t recordPk, std::vector<uint8_t> record) {
    // Check if the record already exists
    PageSlot slot = getSlot(recordPk);
    if (slot.recordPk == recordPk) throw std::runtime_error("Record already exists in page");
    

    // Check if the record can fit in the page
    uint64_t writeAdrEnd = getWriteAdr(record.size());

    slot.recordPk = recordPk;
    slot.recordSize = record.size();
    slot.recordAdr = writeAdrEnd;

    // Insert the slot into the slot directory
    slotDirectory.push_back(slot);
    // sort the slot directory by record pk
    std::sort(slotDirectory.begin(), slotDirectory.end(),
              [](const PageSlot& a, const PageSlot& b) { return a.recordPk < b.recordPk; });

    // write the record to the raw body. Note that we the adress of the record is the end of the record.
    uint64_t writeAdrStart = writeAdrEnd - record.size();
    rawBody.insert(rawBody.begin() + writeAdrStart, record.begin(), record.end());
    
    // Update the header
    header.setRecordCount(header.getRecordCount() + 1);
    if (writeAdrStart < header.getLowestWrittenAdr()) {
        header.setLowestWrittenAdr(writeAdrStart);
    }

    return 0;
}

PageSlot LeafPage::getSlot(uint64_t recordPk) {
    // TODO Implement binary search
    for (size_t i = 0; i < slotDirectory.size(); i++) {
        if (slotDirectory[i].recordPk == recordPk) {
            return slotDirectory[i];
        }
    }
    throw std::runtime_error("Record not found in page");
}

void LeafPage::populateAvailabilityMap() {
    // Clear the availability map and recreate it
    if (availabilityMap != nullptr) delete this->availabilityMap;
    this->availabilityMap = new std::multimap<size_t, uint64_t>();

    // Sort the slot directory by record address
    std::vector<PageSlot> tempSlotDirectory = slotDirectory;
    std::sort(tempSlotDirectory.begin(), tempSlotDirectory.end(),
              [](const PageSlot& a, const PageSlot& b) { return a.recordAdr > b.recordAdr; });

    // Add the gaps between slots to the availability map (Note that we populate the page in reverse so we generate the gaps in reverse order as well)
    uint64_t previousEndAdr = HEADER_SIZE;
    for (const PageSlot& slot : tempSlotDirectory) {
        uint64_t gapSize = previousEndAdr - slot.recordAdr;
        if (gapSize > 0) {
            availabilityMap->insert({gapSize, previousEndAdr});
        }
        previousEndAdr = slot.recordAdr - slot.recordSize;
    }

    // Add the gap between the last slot and the end of the page to the availability map
    uint64_t gapSize = previousEndAdr - (HEADER_SIZE + (SLOT_SIZE * slotDirectory.size()));
    if (gapSize > 0) {
        availabilityMap->insert({gapSize, previousEndAdr});
    }
    updateLargestContinuous();
}

void LeafPage::updateLargestContinuous() {
    if (availabilityMap == nullptr) populateAvailabilityMap();

    // Update the largest continuous space
    if (availabilityMap->size() > 0) {
        this->header.setLargestContinuous(availabilityMap->rbegin()->first);
    } else {
        this->header.setLargestContinuous(PAGE_SIZE - HEADER_SIZE - (SLOT_SIZE * slotDirectory.size()));
    }
}

uint64_t LeafPage::getWriteAdr(size_t recordSize) {
    if (this->availabilityMap == nullptr) populateAvailabilityMap();
    
    // Find the first disk area that can fit the record
    std::map<size_t, uint64_t>::iterator availabilitySpace = this->availabilityMap->upper_bound(recordSize);

    if (availabilitySpace == this->availabilityMap->end()) {
        throw std::runtime_error("Record does not fit in page");
    }

    size_t adrToWriteSize = availabilitySpace->first;
    uint64_t adrToWrite = availabilitySpace->second;

    // std::cout << "Adr to write: " << adrToWrite  << "with size: " << adrToWriteSize << std::endl;

    uint64_t lowestAdr = this->header.getLowestWrittenAdr();
    if ((adrToWrite + recordSize) < lowestAdr) {
        lowestAdr = adrToWrite + recordSize;
    }

    // Confirm that the page can fit another slot
    if ((this->slotDirectory.size() * SLOT_SIZE) + HEADER_SIZE >= lowestAdr) {
        throw std::runtime_error("No space to write new slot to page.");
    }

    return adrToWrite;
}
