




add_library(btree
        src/binaryTree.cpp
        include/tree/binaryTree.hpp
        include/tree/bpptree.hpp
        src/bpptree.cpp
        )
set_target_properties(btree PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(btree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")




add_subdirectory(test)