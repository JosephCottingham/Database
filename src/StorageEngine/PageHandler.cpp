#include "PageHandler.hpp"

PageHandler::PageHandler(std::filesystem::path filePath) {
    this->filePath = filePath;
    openFile();
}

PageHandler::~PageHandler() {
    closeFile();
}

Page PageHandler::getPage(uint64_t pageId) {
    std::cout << "Get page from disk..." << filePath << std::endl;
    confirmFileOpen();

    uint64_t offset = (pageId * PAGE_SIZE) + HEADER_SIZE;
    std::vector<uint8_t> rawData = readData(offset, PAGE_SIZE);
    Page page = Page(rawData, getTableHeader().getColumns());

    return page;
}

int8_t PageHandler::setPage(Page page) {
    std::cout << "Set page to disk..." << filePath << std::endl;
    confirmFileOpen();

    uint64_t offset = (page.getPageId() * PAGE_SIZE) + HEADER_SIZE;
    std::vector<uint8_t> serializedPage = page.getSerialized();
    writeData(serializedPage, offset);

    return 0;
}

TableHeader PageHandler::getTableHeader() {
    if (tableHeader == nullptr) {
        std::vector<uint8_t> rawData = readData(0, TableHeader::HEADER_SIZE);
        tableHeader = new TableHeader(rawData);
    }
    return *tableHeader;
}

int8_t PageHandler::setTableHeader(TableHeader tableHeader) {
    this->tableHeader = &tableHeader;
    writeData(tableHeader.getSerialized(), 0);
    return 0;
}

std::vector<uint8_t> PageHandler::readData(uint64_t offset, size_t size) {
    std::cout << "Read data from disk..." << filePath << std::endl;
    confirmFileOpen();

    dataFile.seekg(offset);
    std::vector<uint8_t> rawData(size);
    dataFile.read(reinterpret_cast<char*>(&rawData[0]), rawData.size());

    return rawData;
}

int8_t PageHandler::writeData(std::vector<uint8_t> data, uint64_t offset) {
    std::cout << "Write data to disk..." << filePath << std::endl;
    confirmFileOpen();

    dataFile.seekp(offset);
    dataFile.write(reinterpret_cast<char*>(&data[0]), data.size());

    return 0;
}

int8_t PageHandler::openFile() {
    std::cout << "Open file: " << filePath << std::endl;
    dataFile.open(filePath.string(), std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    if (!dataFile) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return 1;
    }
    return 0;
}

int8_t PageHandler::closeFile() {
    dataFile.close();
    return 0;
}

int8_t PageHandler::confirmFileOpen() {
    if (!dataFile.is_open()) {
        std::cerr << "File is not open: " << filePath << std::endl;
        if (openFile() == 1) throw std::runtime_error("Unable to open file");
        return 1;
    }
    return 0;
}
