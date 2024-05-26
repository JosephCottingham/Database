#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "Table.hpp"

class StorageEngine {
public:
    StorageEngine();
    StorageEngine(const std::string& basePath);
    int createTable(std::string tableName, std::vector<ColumnMetadata> columns);
    int StorageEngine::insertRecord(std::string& tableName, std::vector<uint8_t> recordData);
    TableData StorageEngine::readRecord(std::string& tableName, uint64_t recordId);

private:
    std::string basePath = ".db";
    std::unordered_map<std::string, Table*> tables;
    void populateTables();
};

#endif // STORAGE_ENGINE_H