#include "StorageEngine.hpp"

StorageEngine::StorageEngine() {
    populateTables();
};

StorageEngine::StorageEngine(const std::string& basePath) {
    this->basePath = basePath;
    populateTables();
};

int StorageEngine::createTable(std::string& tableName, std::string& primaryKey, std::vector<Column>& tableSchema) {
    if (tables.find(tableName) != tables.end()) return -1;
    for (Column& column : tableSchema) {
        size_t size = getSizeInBytes(column.datatype);
        if (size > 255) throw std::runtime_error("Column type size is too large, max size is 255 bytes");
        column.sizeInBytes = static_cast<uint8_t>(size);
        std::cout << "Column: " << column.name << " Size: " << column.sizeInBytes << std::endl;
    }
    tables[tableName] = &Table(basePath, tableName, primaryKey, tableSchema);
    return 0;
};

int StorageEngine::insertRecord(std::string& tableName, std::vector<uint8_t> recordData) {
    if (tables.find(tableName) == tables.end()) return -1;
    Table* table = tables[tableName];
    return table->insertRecord(recordData);
};

void StorageEngine::populateTables() {
    // Load tables metadata from disk
    for (const auto& entry : std::filesystem::directory_iterator(basePath + "/" + "data")) {
        std::cout << entry.path() << std::endl;

        Table table = Table(entry.path());
        tables[table.getName()] = &table;
    }
};