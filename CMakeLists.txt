CMAKE_MINIMUM_REQUIRED(VERSION 3.10 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

PROJECT(
    MDSplus
)

INCLUDE(Version)

# Dependencies

FIND_PACKAGE(Python3 REQUIRED COMPONENTS Interpreter)
FIND_PACKAGE(libxml2 REQUIRED)

# Initial Configuration

MESSAGE(STATUS "Running deploy/gen_messages.py")

EXECUTE_PROCESS(
    COMMAND ${Python3_EXECUTABLE} deploy/gen_messages.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

MESSAGE(STATUS "Running deploy/gen_compound_opcbuiltins.py")

EXECUTE_PROCESS(
    COMMAND ${Python3_EXECUTABLE} deploy/gen_compound_opcbuiltins.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

MESSAGE(STATUS "Running deploy/yylex.gen tdishr")

EXECUTE_PROCESS(
    COMMAND deploy/yylex.gen tdishr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

MESSAGE(STATUS "deploy/yylex.gen tdishr")

EXECUTE_PROCESS(
    COMMAND deploy/yylex.gen tdishr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/include/mdsplus/mdsconfig.h.in
    ${CMAKE_BINARY_DIR}/include/mdsplus/mdsconfig.h
)

# Libraries

ADD_SUBDIRECTORY(mdsshr)

ADD_SUBDIRECTORY(treeshr)

# Install

SET(CMAKE_INSTALL_PREFIX "/usr/local/mdsplus")

INSTALL(
    DIRECTORY include/
    DESTINATION include
)

INSTALL(
    TARGETS
        MdsShr
        TreeShr
)