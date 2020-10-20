
SET(SOURCES
    dummies.c
    TreeAddNode.c
    TreeAddTag.c
    TreeCallHook.c
    TreeCleanDatafile.c
    TreeCreatePulseFile.c
    TreeDeleteNode.c
    TreeDeletePulseFile.c
    TreeDoMethod.c
    TreeFindNode.c
    TreeFindTag.c
    TreeFindTagWild.c
    TreeGetDbi.c
    TreeGetNci.c
    TreeGetRecord.c
    TreeGetSetShotId.c
    TreeOpen.c
    TreePerf.c
    TreePutRecord.c
    TreeRenameNode.c
    TreeRemoveNodesTags.c
    TreeSegments.c
    TreeSerializeNci.c
    TreeSetDbi.c
    TreeSetDefault.c
    TreeSetNci.c
    TreeThreadStatic.c
    TreeVerify.c
    RemoteAccess.c
    lex.yytreepath.c 
)

ADD_LIBRARY(
    TreeShr
    SHARED
    ${SOURCES}
)

SET_TARGET_PROPERTIES(
    TreeShr
    PROPERTIES
        PREFIX ""
)

TARGET_INCLUDE_DIRECTORIES(
    TreeShr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

TARGET_LINK_LIBRARIES(
    TreeShr
    PUBLIC
        MdsShr
)