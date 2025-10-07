# CMake 最小版本
cmake_minimum_required(VERSION 3.10)
project(xrtrader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Include directories
include_directories(
	src
	third/longport_sdk/include
	third/spdlog/include
)

# Source files
set(SRC_FILES
	src/main.cpp
	src/data/asset_fetcher.cpp
	src/data/quote_fetcher.cpp
)

# Library paths
link_directories(
	third/longport_sdk/libs/aarch64-linux/release
)

# Add executable
add_executable(test ${SRC_FILES})

# Link libraries
target_link_libraries(test
	longport_cpp
	longport_c
	pthread
	dl
)

# Set RPATH so the loader can find the .so at runtime
set_target_properties(test PROPERTIES
	BUILD_WITH_INSTALL_RPATH TRUE
	INSTALL_RPATH "$ORIGIN/../third/longport_sdk/libs/aarch64-linux/release"
	SKIP_BUILD_RPATH FALSE
	SKIP_INSTALL_RPATH FALSE
)
