cmake_minimum_required (VERSION 3.8)
project (test-sptool)

set (CMAKE_CXX_STANDART 17)
set (CMAKE_CXX_STANDART_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp test-sptool.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features (${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(test-sptool gmock)
target_include_directories(test-sptool PRIVATE "./../spcore")
target_link_libraries(test-sptool spcore)
target_link_libraries(test-sptool ${Boost_LIBRARIES})

add_test(NAME test_sptool COMMAND $<TARGET_FILE:test-sptool>)