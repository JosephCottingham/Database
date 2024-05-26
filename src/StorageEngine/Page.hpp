#ifndef Page_H
#define Page_H

#include <vector>
#include <string>
#include <stdexcept>
#include <stack>

#include "SerializeUtil.hpp"
#include "PageCommon.hpp"
#include "InnerPage.hpp"
#include "LeafPage.hpp"
#include "PageHeader.hpp"

class Page {
public:
    Page(uint32_t pageId, PageType type, uint64_t leftPage, uint64_t rightPage);
    Page(std::vector<uint8_t> rawData, std::vector<ColumnMetadata> columnMetadata);
    ~Page();
    std::vector<uint8_t> getSerialized();
    PageType getType();
    InnerPage* getInnerPage() { return this->innerPage; };
    LeafPage* getLeafPage() { return this->leafPage; };
    uint64_t getPageId() { return this->header.getId(); }
    void setBreadcrumbs(std::stack<uint64_t> breadcrumbs) { this->breadcrumbs = breadcrumbs; }
    std::stack<uint64_t> getBreadcrumbs() { return this->breadcrumbs; }

private:
    std::vector<uint8_t> rawData;
    PageHeader header;
    InnerPage* innerPage;
    LeafPage* leafPage;
    std::stack<uint64_t> breadcrumbs;
};

#endif // Page_H