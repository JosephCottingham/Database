#ifndef PAGEHEADER_H
#define PAGEHEADER_H

#include <vector>

#include "SerializeUtil.hpp"
#include "PageCommon.hpp"

class PageHeader {

public:
    PageHeader(std::vector<uint8_t> rawHeader);
    PageHeader(uint32_t id, PageType type, uint64_t leftPage, uint64_t rightPage);
    size_t PageHeader::serializeIntoVector(std::vector<uint8_t> rawData);

    uint64_t getMagicNumber() { return magicNumber; }
    uint64_t getId() { return id; }
    uint64_t getLowestWrittenAdr() { return lowestWrittenAdr; }
    PageType getType() { return type; }
    uint32_t getLargestContinuous() { return largestContinuous; }
    uint64_t getLeftPage() { return leftPage; }
    uint64_t getRightPage() { return rightPage; }
    uint64_t getCheckSum() { return checkSum; }
    uint64_t getRecordCount() { return recordCount; }

    void setMagicNumber(uint64_t magicNumber) { this->magicNumber = magicNumber; }
    void setId(uint64_t id) { this->id = id; }
    void setLowestWrittenAdr(uint64_t lowestWrittenAdr) { this->lowestWrittenAdr = lowestWrittenAdr; }
    void setType(PageType type) { this->type = type; }
    void setLargestContinuous(uint32_t largestContinuous) { this->largestContinuous = largestContinuous; }
    void setLeftPage(uint64_t leftPage) { this->leftPage = leftPage; }
    void setRightPage(uint64_t rightPage) { this->rightPage = rightPage; }
    void setCheckSum(uint64_t checkSum) { this->checkSum = checkSum; }
    void setRecordCount(uint64_t recordCount) { this->recordCount = recordCount; }

private:
    std::vector<uint8_t> rawHeader;
    
    uint64_t magicNumber;
    uint64_t id;
    PageType type;
    uint64_t lowestWrittenAdr;
    uint32_t largestContinuous;
    uint64_t leftPage;
    uint64_t rightPage;
    uint64_t checkSum;
    uint64_t recordCount;

};

#endif // PAGEHEADER_H