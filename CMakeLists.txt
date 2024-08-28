cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ObjectAttributeViewer VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/attr.cpp
    src/AttributeDocs.cpp
    src/AttributeEditor.cpp
    src/AttributeListing.cpp
    src/include.cpp
    src/InspectorPopup.cpp
    src/GameObjectWrapper.cpp
    src/ObjectView.cpp
    src/ValueContainer.cpp
    src/JsonTextArea.cpp
    src/NodeExitTracker.cpp
    # Add any extra C++ source files here
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
