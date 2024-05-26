#ifndef PageCommon_H
#define PageCommon_H

static const uint64_t MAGIC_NUMBER = 0x4E4F4D42;
static const size_t HEADER_SIZE = 512;
static const size_t PAGE_SIZE = 4096000;
static const size_t SLOT_SIZE = 16;

enum class PageType {
    ROOT,
    INNER,
    LEAF
};

struct PageSlot {
    uint32_t recordAdr;
    uint32_t recordSize;
    uint64_t recordPk;
};

struct PagePointer {
    uint64_t pageId;
    uint64_t pk;
};

// struct PageHeader {
//     uint64_t magicNumber;
//     uint64_t id;
//     uint64_t location;
//     PageType type;
//     uint32_t largestContinuous;
//     uint64_t leftPage;
//     uint64_t rightPage;
//     uint64_t checkSum;
//     uint64_t recordCount;
// };


enum class DataType: uint8_t {
    INT=0,
    FLOAT=1,
    STRING=2,
    PRIMARY_KEY=3 // Special type for primary key (uint64_t) Primary key is always int64_t to simplify the implementation.
};

// Returns the size in bytes of a given datatype, note that this allows for a more dynamic approach with custom types
size_t getSizeInBytes(DataType datatype) {
    switch (datatype) {
        case DataType::INT:
            return sizeof(int);
        case DataType::FLOAT:
            return sizeof(float);
        case DataType::STRING:
            return 255; // Max supported size for DB
        case DataType::PRIMARY_KEY:
            return sizeof(uint64_t);
    }
    return -1;
}

struct ColumnMetadata {
    std::string name;
    DataType datatype;
    uint8_t sizeInBytes;
};

#endif // PageCommon_H