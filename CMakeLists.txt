cmake_minimum_required(VERSION 3.17)
project(project)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Build GraphViewerCpp
set(GRAPHVIEWERCPP_PATH lib/GraphViewerCpp)
add_subdirectory(${GRAPHVIEWERCPP_PATH})
include_directories(${GRAPHVIEWERCPP_PATH}/include)

file(GLOB TP7_GRAPHVIEWER_FILES CONFIGURE_DEPENDS
        "TP7_graphviewer/*.cpp"
        )

file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "src/*.cpp"
        )

add_executable(TP7_graphviewer
        ${TP7_GRAPHVIEWER_FILES}
        )

add_executable(Project src/main.cpp ${SRC_FILES})

add_executable(parkingTags src/parkingsTags/main.cpp)


target_link_libraries(TP7_graphviewer graphviewer)