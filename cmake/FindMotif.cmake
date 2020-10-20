# FindMotif.cmake
#
# Finds Motif
#
# This will define the following variables
#
# Motif_FOUND
# Motif_INCLUDE_DIR
# Motif_Xm_LIBRARY
# Motif_Xt_LIBRARY
# Motif_Mrm_LIBRARY
# Motif_X11_LIBRARY
#
# and the following imported targets
#
# Motif::Motif
#
# The following variables can be set as arugments
#
# Motif_ROOT_DIR
#

FIND_PATH(
    Motif_INCLUDE_DIR
    NAMES Xm/DrawP.h
    PATHS
        ${Motif_ROOT_DIR}
    PATH_SUFFIXES 
        include
)

FIND_LIBRARY(
    Motif_Xm_LIBRARY
    NAMES Xm
    PATHS
        ${Motif_ROOT_DIR}
    PATH_SUFFIXES 
        lib
)

FIND_LIBRARY(
    Motif_Xt_LIBRARY
    NAMES Xt
    PATHS
        ${Motif_ROOT_DIR}
    PATH_SUFFIXES 
        lib
)

FIND_LIBRARY(
    Motif_Mrm_LIBRARY
    NAMES Mrm
    PATHS
        ${Motif_ROOT_DIR}
    PATH_SUFFIXES 
        lib
)

FIND_LIBRARY(
    Motif_X11_LIBRARY
    NAMES X11
    PATHS
        ${Motif_ROOT_DIR}
    PATH_SUFFIXES 
        lib
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Motif
    REQUIRED_VARS
        Motif_INCLUDE_DIR
        Motif_Xm_LIBRARY
        Motif_Xt_LIBRARY
        Motif_Mrm_LIBRARY
        Motif_X11_LIBRARY
)

IF(Motif_FOUND)
    SET(Motif_LIBRARIES
        ${Motif_Xm_LIBRARY}
        ${Motif_Xt_LIBRARY}
        ${Motif_Mrm_LIBRARY}
        ${Motif_X11_LIBRARY}
    )
    
    ADD_LIBRARY(Motif::Motif INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(
        Motif::Motif
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Motif_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${Motif_LIBRARIES}"
    )
ENDIF()