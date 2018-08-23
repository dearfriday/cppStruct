




add_library(btree
        src/binaryTree.cpp
        include/tree/binaryTree.hpp)

target_include_directories(btree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")