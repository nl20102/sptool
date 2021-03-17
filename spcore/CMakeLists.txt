project (spcore CXX)

set (CMAKE_CXX_STANDART 17)
set (CMAKE_CXX_STANDART_REQUIRED ON)

file(GLOB folder_source *.cpp)
file(GLOB folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})
message("spcore folder_source: " ${folder_source})
include_directories(${Boost_INCLUDE_DIR})

add_library (${PROJECT_NAME} ${folder_source} ${folder_header})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features (${PROJECT_NAME} PRIVATE cxx_std_17)
if(WIN32)
  target_link_libraries(${PROJECT_NAME} ws2_32)
endif()