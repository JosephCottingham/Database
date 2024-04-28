#include "Common.hpp"


// Returns the size in bytes of a given datatype, note that this allows for a more dynamic approach with custom types
size_t getSizeInBytes(DataType datatype) {
    switch (datatype) {
        case DataType::INT:
            return sizeof(int);
        case DataType::FLOAT:
            return sizeof(float);
        case DataType::STRING:
            return 255; // Max supported size for DB
    }
    return -1;
}

std::string getDataTypeAsString(DataType datatype) {
    switch (datatype) {
        case DataType::INT: return "INT";
        case DataType::FLOAT: return "FLOAT";
        case DataType::STRING: return "STRING";
    }
    return "UNKNOWN";
}

DataType getDataTypeFromString(const std::string& datatype) {
    if (datatype == "INT") return DataType::INT;
    if (datatype == "FLOAT") return DataType::FLOAT;
    if (datatype == "STRING") return DataType::STRING;
    throw std::invalid_argument("Invalid datatype: " + datatype);
}