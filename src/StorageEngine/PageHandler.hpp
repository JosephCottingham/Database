#ifndef PAGE_HANDLER_HPP
#define PAGE_HANDLER_HPP

#include <fstream>
#include <iostream>
#include <filesystem>

#include "Page.hpp"
#include "TableHeader.hpp"

class PageHandler {
public:
    // Constructor
    PageHandler(std::filesystem::path filePath);

    // Destructor
    ~PageHandler();

    // Method to handle a page
    Page getPage(uint64_t pageId);
    int8_t setPage(Page page);
    TableHeader getTableHeader();
    int8_t setTableHeader(TableHeader tableHeader);

private:
    // Private members and methods
    std::fstream dataFile;
    std::filesystem::path filePath;
    TableHeader* tableHeader;

    int8_t writeData(std::vector<uint8_t> data, uint64_t offset);
    std::vector<uint8_t> readData(uint64_t offset, size_t size);
    int8_t openFile();
    int8_t closeFile();
    int8_t confirmFileOpen();
};

#endif // PAGE_HANDLER_HPP