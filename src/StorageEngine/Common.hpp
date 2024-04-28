#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iostream>

// Enum to represent the different types of data that can be stored in a column
enum class DataType {
    INT,
    FLOAT,
    STRING
};

// File directory constants
// const std::string DATA_DIR = "data";
// const std::string INDEX_DIR = "index";
// const std::string LOG_DIR = "log";

// tableHeader for a column in a table
struct Column {
    std::string name;
    DataType datatype;
    uint8_t sizeInBytes;
};

// Table tableHeader
struct TableHeader {
    std::string name;
    std::string primaryKey;
    std::uint32_t columnSizeInBytes;
    std::uint32_t columnCount;
    std::vector<Column> columns;
    std::uint32_t pageCount;
};

// Page Header
struct PageSloter {
    std::uint32_t recordAdr;
    std::uint32_t recordSize;
};

enum class PageType {
    ROOT,
    INNER,
    LEAF
};

// Page Header
struct PageHeader {
    std::uint32_t pageCount;
    std::uint32_t freeSpaceAdr;
    PageType pageType;
    std::vector<PageSloter> pageSloters;
};

// Record
struct Record {
    std::uint32_t recordAdr;
    std::vector<uint8_t> data;
};

// Page
struct Page {
    PageHeader header;
    std::vector<Record> data;
};

// Returns the size in bytes of a given datatype, note that this allows for a more dynamic approach with custom types
size_t getSizeInBytes(DataType datatype);

// returns the datatype as a string
std::string getDataTypeAsString(DataType datatype);

// returns the datatype from a string
DataType getDataTypeFromString(const std::string& datatype);

#endif // COMMON_H