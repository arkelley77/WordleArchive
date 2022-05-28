# CMakeList.txt : CMake project for WordleArchive, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set the project name and version
project (WordleArchive VERSION 1.0)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)


# Add source to this project's executable.
add_executable (WordleArchive "WordleArchive.cpp" "WordleArchive.h" "color.cpp" "strtoupper.cpp" "word_dict.cpp" "word_list.cpp" "wordle_statistics.cpp")

add_custom_command(
        TARGET WordleArchive POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                ${CMAKE_CURRENT_BINARY_DIR}/files)
add_custom_command(
        TARGET WordleArchive POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/WordleArchive/files/word_list.txt
                ${CMAKE_CURRENT_BINARY_DIR}/files/word_list.txt)
add_custom_command(
        TARGET WordleArchive POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/WordleArchive/files/wordles.txt
                ${CMAKE_CURRENT_BINARY_DIR}/files/wordles.txt)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WordleArchive PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
