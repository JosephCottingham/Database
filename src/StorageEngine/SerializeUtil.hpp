#ifndef SerializeUtil_H
#define SerializeUtil_H

#include <vector>
#include <string>
#include <stdexcept>

class SerializeUtil {
public:
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::string data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint64_t data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::vector<uint8_t> data);

    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, std::string& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint64_t& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, std::vector<uint8_t> data);
};

#endif // SerializeUtil_H