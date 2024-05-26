#include "InnerPage.hpp"

InnerPage::InnerPage(PageHeader header, std::vector<uint8_t> rawBody): header(header), rawBody(rawBody) {
    SerializeUtil serializer;

    size_t location = 0; 
    for (size_t i = 0; i < header.getRecordCount(); i++) {
        // PagePointer pagePointer;
        PagePointer tempPagePointer; 
        serializer.deserializeFromVector(rawBody, location, tempPagePointer.pageId);
        serializer.deserializeFromVector(rawBody, location, tempPagePointer.pk);

        pagePointers.push_back(tempPagePointer);
    }
}

InnerPage::InnerPage(PageHeader header): header(header) {}

std::vector<uint8_t> InnerPage::getSerialized() {
    SerializeUtil serializer;
    std::vector<uint8_t> serialized;
    size_t location = 0;

    for (size_t i = 0; i < pagePointers.size(); i++) {
        serializer.serializeIntoVector(serialized, location, pagePointers[i].pageId);
        serializer.serializeIntoVector(serialized, location, pagePointers[i].pk);
    }

    return serialized;
}

PagePointer InnerPage::getPagePointer(uint64_t recordPk) {
    // Do binary search for where recordPk should be less then or equal to the recordPk in the pagePointers but greater then the previous recordPk.
    int left = 0;
    int right = pagePointers.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (pagePointers[mid].pk == recordPk) {
            return pagePointers[mid];
        } else if (pagePointers[mid].pk < recordPk) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    throw std::runtime_error("Record not found in page");
}