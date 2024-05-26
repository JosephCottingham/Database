#include "Page.hpp"

Page::~Page() {
    if (this->innerPage != nullptr) delete this->innerPage;
    if (this->leafPage != nullptr) delete this->leafPage;
}

Page::Page(std::vector<uint8_t> rawData, std::vector<ColumnMetadata> columnMetadata):
    rawData(rawData),
    innerPage(nullptr),
    leafPage(nullptr),
    header(PageHeader(rawData)) {

    std::vector<uint8_t> tempBody(rawData.begin() + HEADER_SIZE, rawData.end());

    if (this->header.getType() == PageType::INNER){
        this->innerPage = new InnerPage(this->header, tempBody);
    } else if (this->header.getType() == PageType::LEAF) {
        this->leafPage = new LeafPage(this->header, tempBody);
    } else {
        throw std::invalid_argument("Invalid page type");
    }
}

Page::Page(uint32_t id, PageType type, uint64_t leftPage, uint64_t rightPage): header(id, type, leftPage, rightPage) {

    this->header.setMagicNumber(MAGIC_NUMBER);

    if (type == PageType::INNER) {
        this->innerPage = new InnerPage(this->header);
    } else if (type == PageType::LEAF) {
        this->leafPage = new LeafPage(this->header);
    } else {
        throw std::invalid_argument("Invalid page type");
    }
}
