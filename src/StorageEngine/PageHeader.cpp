#include "PageHeader.hpp"

PageHeader::PageHeader(std::vector<uint8_t> rawHeader) {
    this->rawHeader = rawHeader;
    
    SerializeUtil serializer;
    size_t location = 0;

    serializer.deserializeFromVector(rawHeader, location, magicNumber);
    if (magicNumber != MAGIC_NUMBER) throw std::runtime_error("Invalid magic number");

    serializer.deserializeFromVector(rawHeader, location, id);

    uint8_t tempType;
    serializer.deserializeFromVector(rawHeader, location, tempType);
    type = static_cast<PageType>(tempType);

    serializer.deserializeFromVector(rawHeader, location, lowestWrittenAdr);
    serializer.deserializeFromVector(rawHeader, location, largestContinuous);

    serializer.deserializeFromVector(rawHeader, location, leftPage);
    serializer.deserializeFromVector(rawHeader, location, rightPage);
    
    serializer.deserializeFromVector(rawHeader, location, checkSum);
    serializer.deserializeFromVector(rawHeader, location, recordCount);
}

PageHeader::PageHeader(uint32_t id, PageType type, uint64_t leftPage, uint64_t rightPage) {
    this->id = id;
    this->type = type;
    this->leftPage = leftPage;
    this->rightPage = rightPage;
    this->magicNumber = MAGIC_NUMBER;
    this->recordCount = 0;
    this->lowestWrittenAdr = PAGE_SIZE;                // Default to last byte of page
    this->largestContinuous = PAGE_SIZE - HEADER_SIZE; // Default to all bytes after header
    this->checkSum = 0;                                // TODO, must implement checksum based on page body.
}


size_t PageHeader::serializeIntoVector(std::vector<uint8_t> rawData) {
    if (rawData.size() <= HEADER_SIZE) throw std::runtime_error("Invalid header size");

    SerializeUtil serializer;
    size_t location = 0;

    serializer.serializeIntoVector(rawData, location, this->magicNumber);

    serializer.serializeIntoVector(rawData, location, this->id);

    uint8_t tempType = static_cast<uint8_t>(this->type);
    serializer.serializeIntoVector(rawData, location, tempType);

    serializer.serializeIntoVector(rawData, location, this->largestContinuous);

    serializer.serializeIntoVector(rawData, location, this->leftPage);
    serializer.serializeIntoVector(rawData, location, this->rightPage);

    serializer.serializeIntoVector(rawData, location, this->checkSum);
    serializer.serializeIntoVector(rawData, location, this->recordCount);

    

    return location;
}