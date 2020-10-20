# Findlibxml2.cmake
#
# Finds libxml2
#
# This will define the following variables
#
# libxml2_FOUND
# libxml2_INCLUDE_DIR
# libxml2_LIBRARY
#
# and the following imported targets
#
# libxml2::libxml2
#
# The following variables can be set as arugments
#
# libxml2_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)

PKG_CHECK_MODULES(_libxml2_PC QUIET libxml-2.0)

FIND_PATH(
    libxml2_INCLUDE_DIR
    NAMES libxml/tree.h
    PATHS
        ${libxml2_ROOT_DIR}
        ${_libxml2_PC_INCLUDE_DIRS}
    PATH_SUFFIXES 
        include
)

FIND_LIBRARY(
    libxml2_LIBRARY
    NAMES xml2
    PATHS
        ${libxml2_ROOT_DIR}
        ${_libxml2_PC_LIBRARY_DIRS}
    PATH_SUFFIXES 
        lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    libxml2
    REQUIRED_VARS
        libxml2_INCLUDE_DIR
        libxml2_LIBRARY       
)

IF(libxml2_FOUND)
    ADD_LIBRARY(libxml2::libxml2 INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        libxml2::libxml2
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${libxml2_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${libxml2_LIBRARY}"
    )
ENDIF()