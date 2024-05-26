#ifndef INNERPAGE_H
#define INNERPAGE_H

#include <vector>

#include "SerializeUtil.hpp"
#include "PageCommon.hpp"
#include "PageHeader.hpp"

class InnerPage {
    // Add your class members and methods here

public:
    InnerPage(PageHeader header, std::vector<uint8_t> rawBody);
    InnerPage(PageHeader header);
    
    std::vector<uint8_t> getSerialized();
    PagePointer getPagePointer(uint64_t recordPk);

private:
    PageHeader header;
    std::vector<uint8_t> rawBody;
    std::vector<PagePointer> pagePointers;

};

#endif // INNERPAGE_H