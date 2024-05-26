#ifndef LEAFPAGE_H
#define LEAFPAGE_H

#include <vector>
#include <map>
#include <algorithm>

#include "SerializeUtil.hpp"
#include "PageCommon.hpp"
#include "PageHeader.hpp"

class LeafPage {
    // Add your class members and methods here

public:
    LeafPage(PageHeader header, std::vector<uint8_t> rawBody);
    LeafPage(PageHeader header);

    std::vector<uint8_t> getSerialized();
    std::vector<uint8_t> getRecord(uint64_t recordPk);
    int8_t insertRecord(uint64_t recordPk, std::vector<uint8_t> record);

private:
    PageHeader header;
    std::vector<uint8_t> rawBody;
    std::vector<PageSlot> slotDirectory;
    std::multimap<size_t, uint64_t>* availabilityMap;

    PageSlot getSlot(uint64_t recordPk);
    void updateLargestContinuous();
    uint64_t getWriteAdr(size_t recordSize);
    void populateAvailabilityMap();

};

#endif // LEAFPAGE_H