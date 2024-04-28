#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

#include "common.hpp"

class Table {
public:
    // Constructor
    Table();
    Table(std::filesystem::path);
    Table(std::filesystem::path, std::string name, std::string primaryKey, std::vector<Column> columns);
    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;
    std::string getName();
    uint8_t Table::insertRecord(std::vector<uint8_t> record);
    
private:
    // Private member variables
    std::fstream dataFile;
    const size_t HEADER_SIZE = 4096000;
    //                      3435973836
    const size_t PAGE_SIZE = 4096000;
    TableHeader tableHeader;
    std::filesystem::path filePath;

    // Handle file statuses
    int Table::openFile();
    int Table::closeFile();
    int Table::confirmFileOpen();

    // Private method to load metadata from disk
    void readHeader();

    // Private method to save metadata to disk
    void writeHeader();

    // Create new page
    Page createNewPage(PageType pageType);
    Page getPage(uint32_t pageId);

    // Write page to disk
    void writePage(Page page);

    // Read page from disk
    Page readPage(uint8_t pageNumber);

    // Serialization methods
    std::vector<uint8_t> serializeHeader();
    std::vector<uint8_t> serializePage(Page page);

    // Deserialization methods
    TableHeader deserializeHeader(std::vector<uint8_t> header);
    Page deserializePage(std::vector<uint8_t> serializedPage);

    // Serialization helper methods
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::string data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t data);
    void serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t data);
    void serializeNoSizeIntoVector(std::vector<uint8_t>& serializedData, uint32_t& location, std::vector<uint8_t> data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, std::string& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t& data);
    void deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t& data);
    void deserializeNoSizeIntoVector(std::vector<uint8_t>& serializedData, uint32_t& location, std::vector<uint8_t> data);
};
#endif // TABLE_H