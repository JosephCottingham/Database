#ifndef TableHeader_H
#define TableHeader_H

#include <vector>
#include <string>

#include "SerializeUtil.hpp"
#include "PageCommon.hpp"

class TableHeader {
    public:
        TableHeader();
        TableHeader(std::string name, std::vector<ColumnMetadata> columns);
        TableHeader(std::vector<uint8_t> rawData);
        std::vector<uint8_t> getSerialized();
        std::vector<ColumnMetadata> getColumns();

        uint64_t getPkIdSquence() { return this->pkIdSquence; }
        void incrementPkIdSequence() { this->pkIdSquence++; }

        static const size_t HEADER_SIZE = 4096000;
        static const uint64_t MAGIC_NUMBER = 0x4E4F4D42;

    private:
        std::string name;
        uint8_t columnCount;
        std::vector<ColumnMetadata> columns;
        uint64_t pageCount;
        uint64_t pkIdSquence;
};


#endif // TableHeader_H