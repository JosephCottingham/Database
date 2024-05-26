#include "TableHeader.hpp"

TableHeader::TableHeader() {
    this->name = "";
    this->columnCount = 0;
    this->pageCount = 0;
    this->pkIdSquence = 0;
}

TableHeader::TableHeader(std::string name, std::vector<ColumnMetadata> columns) {
    this->name = name;
    this->columns = columns;
    this->columnCount = static_cast<uint8_t>(columns.size());
    this->pageCount = 0;
    this->pkIdSquence = 0;
}

TableHeader::TableHeader(std::vector<uint8_t> rawData) {
    size_t location = 0;
    SerializeUtil serializer;
    
    uint64_t magicNumber;
    serializer.deserializeFromVector(rawData, location, magicNumber);
    if (magicNumber != MAGIC_NUMBER) throw std::runtime_error("Invalid magic number");

    serializer.deserializeFromVector(rawData, location, name);
    serializer.deserializeFromVector(rawData, location, columnCount);
    for (int i = 0; i < columnCount; i++) {
        ColumnMetadata column;
        serializer.deserializeFromVector(rawData, location, column.name);
        uint8_t datatype;
        serializer.deserializeFromVector(rawData, location, datatype);
        column.datatype = static_cast<DataType>(datatype);
        serializer.deserializeFromVector(rawData, location, column.sizeInBytes);
        columns.push_back(column);
    }
    serializer.deserializeFromVector(rawData, location, pageCount);

    serializer.deserializeFromVector(rawData, location, pkIdSquence);
}

std::vector<uint8_t> TableHeader::getSerialized() {
    std::vector<uint8_t> serializedData(HEADER_SIZE);
    size_t location = 0;
    SerializeUtil serializer;

    serializer.serializeIntoVector(serializedData, location, MAGIC_NUMBER);
    serializer.serializeIntoVector(serializedData, location, name);
    serializer.serializeIntoVector(serializedData, location, columnCount);
    for (int i = 0; i < columnCount; i++) {
        serializer.serializeIntoVector(serializedData, location, columns[i].name);
        uint8_t datatype = static_cast<uint8_t>(columns[i].datatype);
        serializer.serializeIntoVector(serializedData, location, datatype);
        serializer.serializeIntoVector(serializedData, location, columns[i].sizeInBytes);
    }
    serializer.serializeIntoVector(serializedData, location, pageCount);

    serializer.serializeIntoVector(serializedData, location, pkIdSquence);

    return serializedData;
}