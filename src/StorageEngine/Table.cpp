#include "Table.hpp"

Table::~Table() {
    delete pageHandler;
}

// Constructor with path, used for opening existing tables
Table::Table(std::filesystem::path filePath) {
    this->pageHandler = new PageHandler(filePath);
};

// Constructor with config, used for creating new tables
Table::Table(TableConfig config) {
    std::string extension = ".db";
    this->pageHandler = new PageHandler(config.basePath / "data" / (config.name + extension));
    this->pageHandler->setTableHeader(TableHeader(config.name, config.columns));
}


uint8_t Table::insertRecord(std::vector<uint8_t> record) {
    uint64_t recordId = this->pageHandler->getTableHeader().getPkIdSquence();
    this->pageHandler->getTableHeader().incrementPkIdSequence();
    Page page = getPageByRecordId(recordId);
    

    return 0;
};

TableData Table::getRecord(uint64_t recordId) {
    TableData tableData;
    Page page = getPageByRecordId(recordId);

    std::vector<uint8_t> rawData = page.getLeafPage()->getRecord(recordId);
    
    tableData.columns = pageHandler->getTableHeader().getColumns();
    tableData.data = rawData;

    return tableData;
};

Page Table::getPageByRecordId(uint64_t recordId) {
    std::stack<uint64_t> pageBreadcrumbs;
    Page page = pageHandler->getPage(0);
    while (page.getType() == PageType::INNER) {
        pageBreadcrumbs.push(page.getPageId());
        PagePointer PagePointer = page.getInnerPage()->getPagePointer(recordId);
        page = pageHandler->getPage(PagePointer.pageId);
    }
    page.setBreadcrumbs(pageBreadcrumbs);
    return page;
};