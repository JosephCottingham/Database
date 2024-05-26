#include "StorageEngine.hpp"

StorageEngine::StorageEngine() {
    populateTables();
};

StorageEngine::StorageEngine(const std::string& basePath) {
    this->basePath = basePath;
    populateTables();
};

int StorageEngine::createTable(std::string tableName, std::vector<ColumnMetadata> columns) {
    if (tables.find(tableName) != tables.end()) return -1;
    for (ColumnMetadata column : columns) {
        size_t size = getSizeInBytes(column.datatype);
        if (size > 255) throw std::runtime_error("Column type size is too large, max size is 255 bytes");
        column.sizeInBytes = static_cast<uint8_t>(size);
        std::cout << "Column: " << column.name << " Size: " << column.sizeInBytes << std::endl;
    }
    TableConfig config(basePath, tableName, columns);
    config.basePath = basePath;

    tables[tableName] = &Table(config);
    return 0;
};

int StorageEngine::insertRecord(std::string& tableName, std::vector<uint8_t> recordData) {
    if (tables.find(tableName) == tables.end()) return -1;
    Table* table = tables[tableName];
    return table->insertRecord(recordData);
};

TableData StorageEngine::readRecord(std::string& tableName, uint64_t recordId) {
    if (tables.find(tableName) == tables.end()) throw std::runtime_error("Table not found");
    Table* table = tables[tableName];
    return table->getRecord(recordId);
};

void StorageEngine::populateTables() {
    // Load tables metadata from disk
    for (const auto& entry : std::filesystem::directory_iterator(basePath + "/" + "data")) {
        std::cout << entry.path() << std::endl;

        Table table = Table(entry.path());
        tables[table.getName()] = &table;
    }
};