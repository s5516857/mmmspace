cmake_minimum_required(VERSION 3.10)
project(SpaceshipShooter)

# Setting the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setting the vcpkg path
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif()

# Setting CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")


# Finding dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Includes catalogue
include_directories(include ${IMGUI_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS})

# source file
file(GLOB SOURCES "src/*.cpp")

# Adding Executable Files
add_executable(SpaceshipShooter ${SOURCES})

# link library
target_link_libraries(SpaceshipShooter PRIVATE glfw glm::glm glad::glad imgui::imgui)

# Setting the Include Directory
# target_include_directories(SpaceshipShooter PRIVATE include)

# Adding test files
file(GLOB TEST_SOURCES "test/*.cpp")

# Adding a test executable
add_executable(SpaceshipShooterTest ${TEST_SOURCES})

# Linking Test Libraries
target_link_libraries(SpaceshipShooterTest PRIVATE GTest::gtest GTest::gtest_main glfw glm::glm glad::glad imgui::imgui)
