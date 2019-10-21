


add_executable(test_tree_tt
        main.cpp)
target_include_directories(test_tree_tt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_tree_tt PUBLIC btree)



add_executable(test_insert
        testInsert.cpp)
target_include_directories(test_insert PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_insert PUBLIC btree)