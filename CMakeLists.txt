cmake_minimum_required(VERSION 3.22.1)

# -------------- Set C++ standard and architecture settings ----------------

set(CMAKE_CXX_STANDARD 20)
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64") # Required for M1 (Apple Silicon) MacBooks
endif()

# ---------------------- Configure the actual project ----------------------

project(SkydivingVideoEditor)
message(STATUS "Configuring project ${PROJECT_NAME}")

# Find all required libraries
message(STATUS "Ensuring all required 3rd party libraries can be found...")
set(HOMEBREW_INCLUDE_DIR "/opt/homebrew/include")
set(HOMEBREW_LIB_DIR "/opt/homebrew/lib")
find_library(AVCODEC_LIBRARY
    avcodec
    PATHS ${HOMEBREW_LIB_DIR}
    REQUIRED
)
find_library(AVFORMAT_LIBRARY
    avformat
    PATHS ${HOMEBREW_LIB_DIR}
    REQUIRED
)
message(STATUS "Found all required 3rd party libraries")

# Configure executable generation
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Video.cpp
)

# Configure includes
target_include_directories(${PROJECT_NAME}
    PRIVATE ${HOMEBREW_INCLUDE_DIR} # TODO perhaps not ideal to do this alongside find_library()
)

# Configure libraries
target_link_libraries(${PROJECT_NAME}
    ${AVCODEC_LIBRARY}
    ${AVFORMAT_LIBRARY}
)
