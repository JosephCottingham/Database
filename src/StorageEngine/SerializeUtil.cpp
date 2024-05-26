#include "SerializeUtil.hpp"

void SerializeUtil::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::string data) {
    size_t dataLength = data.length();
    if ((location+dataLength+1) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    if (dataLength > 255) throw std::runtime_error("Data is too long, max length is 255 characters");
    serializedData[location] = static_cast<uint8_t>(dataLength & 0xFF);
    location += 1;

    std::copy(data.begin(), data.end(), serializedData.begin() + location);
    location += dataLength;
}

void SerializeUtil::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t data) {
    if ((location+sizeof(uint32_t)) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        serializedData[location + i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFF);
    }
    location += sizeof(uint32_t);
}

void SerializeUtil::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t data) {
    if ((location+1) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    serializedData[location] = static_cast<uint8_t>(data);
    location += 1;
}

void SerializeUtil::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::vector<uint8_t> data) {
    size_t dataLength = data.size();
    if ((location+dataLength) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());

    std::copy(data.begin(), data.end(), serializedData.begin() + location);
}

void SerializeUtil::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, std::string& data) {
    size_t dataLength = static_cast<size_t>(serializedData[location]);
    location += 1;

    data = std::string(serializedData.begin() + location, serializedData.begin() + location + dataLength);
    location += dataLength;
}

void SerializeUtil::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint64_t& data) {
    data = 0;
    for (size_t i = 0; i < sizeof(uint64_t); ++i) {
        data |= static_cast<uint64_t>(serializedData[location + i]) << (i * 8);
    }
    location += sizeof(uint64_t);
}

void SerializeUtil::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t& data) {
    data = 0;
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        data |= static_cast<uint32_t>(serializedData[location + i]) << (i * 8);
    }
    location += sizeof(uint32_t);
}

void SerializeUtil::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t& data) {
    data = serializedData[location];
    location += 1;
}

void SerializeUtil::deserializeFromVector(std::vector<uint8_t>& serializedData, uint32_t& location, std::vector<uint8_t> data) {
    size_t dataLength = data.size();
    if ((location+dataLength) > serializedData.size()) throw std::runtime_error("Read out of bounds for lenth: " + serializedData.size());

    std::copy(serializedData.begin() + location, serializedData.begin() + location + dataLength, data.begin());
}
