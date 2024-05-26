#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <stack>

#include "Page.hpp"
#include "TableHeader.hpp"
#include "PageHandler.hpp"

struct TableConfig {
    std::filesystem::path basePath;
    std::string name;
    std::vector<ColumnMetadata> columns;
    TableConfig() {}
    TableConfig(std::filesystem::path basePath, std::string name, std::vector<ColumnMetadata> columns) {
        this->basePath = basePath;
        this->name = name;
        this->columns = columns;
    }
};

struct TableData {
    std::vector<ColumnMetadata> columns;
    std::vector<uint8_t> data;
};

class Table {
public:
    // Constructor
    Table(std::filesystem::path filePath);
    Table(TableConfig config);
    ~Table();
    std::string getName();
    uint8_t Table::insertRecord(std::vector<uint8_t> record);
    TableData Table::getRecord(uint64_t recordId);
    
private:
    // Private member variables
    PageHandler* pageHandler;

    // Private methods
    Page getPageByRecordId(uint64_t recordId);
};
#endif // TABLE_H