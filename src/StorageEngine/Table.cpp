#include "Table.hpp"

// Constructor
Table::Table() {
    // Default Constructor
    throw std::runtime_error("Default constructor not allowed");
};

Table::Table(std::filesystem::path filePath) : filePath(filePath) {
    // Load metadata from disk
    readHeader();
};

Table::Table(std::filesystem::path basePath, std::string name, std::string primaryKey, std::vector<Column> columns) {
    // Create new table
    tableHeader.name = name;
    tableHeader.primaryKey = primaryKey;
    tableHeader.columns = columns;
    tableHeader.columnCount = columns.size();
    tableHeader.columnSizeInBytes = 0;
    for (const Column column : columns) {
        tableHeader.columnSizeInBytes += column.sizeInBytes;
    }
    tableHeader.pageCount = 0;
    filePath = basePath / "data" / (name + ".data");
    writeHeader();
    createNewPage(PageType::LEAF);
};

uint8_t Table::insertRecord(std::vector<uint8_t> recordData){
    Page page = getPage(1); // Get root page.
    page.header.freeSpaceAdr -= tableHeader.columnSizeInBytes;
    
    PageSloter pageSloter = {page.header.freeSpaceAdr, tableHeader.columnSizeInBytes};
    page.header.pageSloters.push_back(pageSloter);

    Record record = {page.header.freeSpaceAdr, recordData};
    page.data.push_back(record);

    writePage(page);

    return 0;
}

Page Table::getPage(uint32_t pageId) {
    confirmFileOpen();
    uint32_t pageAdr = pageId-1 + HEADER_SIZE;

    // Read the page from disk
    dataFile.seekg(pageAdr);
    std::vector<uint8_t> serializedPage(PAGE_SIZE);
    dataFile.read(reinterpret_cast<char*>(&serializedPage[0]), PAGE_SIZE);

    return deserializePage(serializedPage);
}

std::string Table::getName() {
    return this->tableHeader.name;
};

int Table::openFile() {
    std::cout << "Open file: " << filePath << std::endl;
    dataFile.open(filePath.string(), std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    if (!dataFile) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return 1;
    }
    return 0;
}

int Table::closeFile() {
    dataFile.close();
    return 0;
}

int Table::confirmFileOpen() {
    if (!dataFile.is_open()) {
        std::cerr << "File is not open: " << filePath << std::endl;
        if (openFile() == 1) throw std::runtime_error("Unable to open file");
        return 1;
    }
    return 0;
}

// Private method to load metadata from disk
void Table::readHeader() {
    std::cout << "Read header from disk..." << filePath << std::endl;
    confirmFileOpen();

    dataFile.seekg(0);
    std::vector<uint8_t> serializeHeader(HEADER_SIZE);
    dataFile.read(reinterpret_cast<char*>(&serializeHeader[0]), HEADER_SIZE);

    this->tableHeader = deserializeHeader(serializeHeader);
}

// Private method to save metadata to disk
void Table::writeHeader() {
    std::cout << "Write header to disk..." << filePath << std::endl;
    confirmFileOpen();

    // return to the beginning of the file and write the header
    dataFile.seekg(0);
    dataFile.write(reinterpret_cast<char*>(&serializeHeader()[0]), HEADER_SIZE);
}


std::vector<uint8_t> Table::serializeHeader() {
    // Header Structure:
    // | Name Length | Name | Primary Key Length | Primary Key | Page Count | Column 1 | Column 2 | ... | Column N |
    // Column Structure:
    // | Name Length | Name | Data Type Length | Data Type | Size in Bytes |

    std::vector<uint8_t> serializedHeader(HEADER_SIZE);
    size_t location = 0;

    // Table name
    serializeIntoVector(serializedHeader, location, tableHeader.name);

    // Primary key
    serializeIntoVector(serializedHeader, location, tableHeader.primaryKey);

    // Column Size in Bytes
    serializeIntoVector(serializedHeader, location, tableHeader.columnSizeInBytes);

    // Column Count
    serializeIntoVector(serializedHeader, location, tableHeader.columnCount);

    // Page Count
    serializeIntoVector(serializedHeader, location, tableHeader.pageCount);

    // Columns
    for (const Column column : tableHeader.columns) {
        serializeIntoVector(serializedHeader, location, column.name);

        std::string dataType = getDataTypeAsString(column.datatype);
        serializeIntoVector(serializedHeader, location, dataType);

        serializeIntoVector(serializedHeader, location, column.sizeInBytes);
    }

    return serializedHeader;
}

TableHeader Table::deserializeHeader(std::vector<uint8_t> serializedHeader) {
    std::cout << "Deserializing table header..." << std::endl;
    TableHeader tableHeader;
    size_t location = 0;

    // Table name
    std::cout << "Location: " << location << " | Table name: ";
    deserializeFromVector(serializedHeader, location, tableHeader.name);
    std::cout << tableHeader.name << std::endl;

    // Primary key
    std::cout << "Location: " << location << " | Primary key: ";
    deserializeFromVector(serializedHeader, location, tableHeader.primaryKey);
    std::cout << tableHeader.primaryKey << std::endl;

    // Column Size in Bytes
    std::cout << "Location: " << location << " | Column Size in Bytes: ";
    deserializeFromVector(serializedHeader, location, tableHeader.columnSizeInBytes);
    std::cout << std::to_string(tableHeader.columnSizeInBytes) << std::endl;

    // Column Count
    std::cout << "Location: " << location << " | Column Count: ";
    deserializeFromVector(serializedHeader, location, tableHeader.columnCount);
    std::cout << tableHeader.columnCount << std::endl;

    // Page Count
    std::cout << "Location: " << location << " | Page Count: ";
    deserializeFromVector(serializedHeader, location, tableHeader.pageCount);
    std::cout << tableHeader.pageCount << std::endl;

    // Columns
    uint32_t columnCount = 0;
    while (columnCount < tableHeader.columnCount) {
        columnCount++;
        Column column;
        std::cout << "Location: " << location << " | Column Name: ";
        // Column Name
        deserializeFromVector(serializedHeader, location, column.name);
        std::cout << column.name << std::endl;
        
        // Data Type
        std::cout << "Location: " << location << " | Data Type: ";
        std::string dataType;
        deserializeFromVector(serializedHeader, location, dataType);
        std::cout << dataType << std::endl;
        column.datatype = getDataTypeFromString(dataType);

        // Size in Bytes
        std::cout << "Location: " << location << " | Size in Bytes: ";
        deserializeFromVector(serializedHeader, location, column.sizeInBytes);
        std::cout << std::to_string(column.sizeInBytes) << std::endl;

        tableHeader.columns.push_back(column);
    }

    return tableHeader;
}

Page Table::createNewPage(PageType pageType) {
    confirmFileOpen();
    uint64_t fileSize = (PAGE_SIZE * tableHeader.pageCount) + HEADER_SIZE;

    // Update the table header with the new page count
    tableHeader.pageCount += 1;
    writeHeader();

    // Create a new page
    // The freeSpaceAdr is the end of the page
    PageHeader pageHeader = {tableHeader.pageCount, PAGE_SIZE, pageType, {}};
    Page page = {pageHeader, {}};
    
    // Write the page to disk
    writePage(page);
    
    return page;
}

void Table::writePage(Page page) {
    confirmFileOpen();
    uint8_t pageNumber = page.header.pageCount;
    std::vector<uint8_t> serializedPage = serializePage(page);

    // Write the page to disk
    // The page number is 1 indexed, however we want to write from the beginning of the page and the end.
    dataFile.seekg(HEADER_SIZE + (pageNumber-1 * PAGE_SIZE));
    dataFile.write(reinterpret_cast<char*>(&serializedPage[0]), PAGE_SIZE);
}

std::vector<uint8_t> Table::serializePage(Page page) {
    // Page Structure:
    // | Page Count | Free Space Address | Page Slot 1 | Page Slot 2 | ... | Page Slot N | Data |
    // Page Slot Structure:
    // | Record Address | Record Size |
    // Data Structure:
    // | Record N | ... | Record 2 | Record 1 |

    std::vector<uint8_t> serializedPage(PAGE_SIZE);
    size_t location = 0;

    // Page Count
    serializeIntoVector(serializedPage, location, page.header.pageCount);

    // Free Space Address
    serializeIntoVector(serializedPage, location, page.header.freeSpaceAdr);

    // Page Type
    serializeIntoVector(serializedPage, location, static_cast<uint8_t>(page.header.pageType));

    // Page Sloters
    for (const PageSloter pageSloter : page.header.pageSloters) {
        // Record Address
        serializeIntoVector(serializedPage, location, pageSloter.recordAdr);

        // Record Size
        serializeIntoVector(serializedPage, location, pageSloter.recordSize);
    }

    // Location tracking stop hear as we will be writing the data based on the record address
    // Data
    for (const Record record : page.data) {
        // Record Data
        uint32_t location = record.recordAdr;
        serializeNoSizeIntoVector(serializedPage, location, record.data);
    }

    return serializedPage;
}

Page Table::deserializePage(std::vector<uint8_t> serializedPage) {
    Page page;
    size_t location = 0;

    // Page Count
    deserializeFromVector(serializedPage, location, page.header.pageCount);

    // Free Space Address
    deserializeFromVector(serializedPage, location, page.header.freeSpaceAdr);

    // Page Type
    uint8_t pageType;
    deserializeFromVector(serializedPage, location, pageType);
    page.header.pageType = static_cast<PageType>(pageType);

    // Page Sloters
    while (location < serializedPage.size()) {
        PageSloter pageSloter;
        // Record Address
        deserializeFromVector(serializedPage, location, pageSloter.recordAdr);
        // Record Size
        deserializeFromVector(serializedPage, location, pageSloter.recordSize);
        page.header.pageSloters.push_back(pageSloter);
    }

    for (const PageSloter pageSloter : page.header.pageSloters) {
        Record record;
        record.recordAdr = pageSloter.recordAdr;
        record.data.resize(pageSloter.recordSize);
        deserializeNoSizeIntoVector(serializedPage, record.recordAdr, record.data);
        page.data.push_back(record);
    }
    
    return page;
}

void Table::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, std::string data) {
    size_t dataLength = data.length();
    if ((location+dataLength+1) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    if (dataLength > 255) throw std::runtime_error("Data is too long, max length is 255 characters");
    serializedData[location] = static_cast<uint8_t>(dataLength & 0xFF);
    location += 1;

    std::copy(data.begin(), data.end(), serializedData.begin() + location);
    location += dataLength;
}

void Table::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t data) {
    if ((location+sizeof(uint32_t)) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        serializedData[location + i] = static_cast<uint8_t>((data >> (i * 8)) & 0xFF);
    }
    location += sizeof(uint32_t);
}

void Table::serializeIntoVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t data) {
    if ((location+1) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());
    serializedData[location] = static_cast<uint8_t>(data);
    location += 1;
}

void Table::serializeNoSizeIntoVector(std::vector<uint8_t>& serializedData, uint32_t& location, std::vector<uint8_t> data) {
    size_t dataLength = data.size();
    if ((location+dataLength) > serializedData.size()) throw std::runtime_error("Data is too long to add to vector of length: " + serializedData.size());

    std::copy(data.begin(), data.end(), serializedData.begin() + location);
}

void Table::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, std::string& data) {
    size_t dataLength = static_cast<size_t>(serializedData[location]);
    location += 1;

    data = std::string(serializedData.begin() + location, serializedData.begin() + location + dataLength);
    location += dataLength;
}

void Table::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint32_t& data) {
    data = 0;
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        data |= static_cast<uint32_t>(serializedData[location + i]) << (i * 8);
    }
    location += sizeof(uint32_t);
}

void Table::deserializeFromVector(std::vector<uint8_t>& serializedData, size_t& location, uint8_t& data) {
    data = serializedData[location];
    location += 1;
}

void Table::deserializeNoSizeIntoVector(std::vector<uint8_t>& serializedData, uint32_t& location, std::vector<uint8_t> data) {
    size_t dataLength = data.size();
    if ((location+dataLength) > serializedData.size()) throw std::runtime_error("Read out of bounds for lenth: " + serializedData.size());

    std::copy(serializedData.begin() + location, serializedData.begin() + location + dataLength, data.begin());
}
