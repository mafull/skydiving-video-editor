cmake_minimum_required(VERSION 3.22.1)

project(SkydivingVideoEditor)
message(STATUS "Configuring project ${PROJECT_NAME}")

add_executable(${PROJECT_NAME}
    test.cpp
)

add_subdirectory(MyLib)

# target_include_directories(${PROJECT_NAME} PUBLIC MyLib)
target_link_libraries(${PROJECT_NAME}
    MyLib
)
