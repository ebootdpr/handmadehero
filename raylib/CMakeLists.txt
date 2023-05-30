cmake_minimum_required(VERSION 3.0.0)
project(itsatrap VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(itsatrap src/main.cpp)
#target_link_libraries(itsatrap libraylib.so)
target_link_libraries(itsatrap /usr/local/lib/libraylib.so.450)


#target_include_directories(itsatrap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#add_library(add_library add.cpp)
#target_link_libraries(itsatrap add_library)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
