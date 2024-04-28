file(GLOB CPP_SOURCES "*.cpp")
file(GLOB HPP_SOURCES "*.hpp")

add_library(StorageEngine ${CPP_SOURCES} ${HPP_SOURCES})