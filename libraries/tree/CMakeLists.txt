




add_library(btree
        src/binaryTree.cpp
        include/tree/binaryTree.hpp
        include/tree/bpptree.hpp
        )
set_target_properties(btree PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(btree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")




add_subdirectory(test)