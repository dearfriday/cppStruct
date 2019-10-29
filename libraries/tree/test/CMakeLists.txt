


add_executable(test_tree_tt
        main.cpp)
target_include_directories(test_tree_tt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include ${Boost_INCLUDE_DIRS})
target_link_libraries(test_tree_tt PUBLIC btree ${Boost_LIBRARIES})

message(STATUS "BOOST_LIBRARIES ${Boost_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}")


add_executable(test_insert
        testInsert.cpp)
target_include_directories(test_insert PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_insert PUBLIC btree)


add_executable(test_dg
        testDg.cpp)
target_include_directories(test_dg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_dg PUBLIC btree)


add_executable(test_bplus
        testTreePlus.cpp)
target_include_directories(test_bplus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test_bplus PUBLIC btree)