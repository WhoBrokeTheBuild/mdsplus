
SET(SOURCES
    ListTree.c
    XmdsBells.c
    XmdsComplain.c
    XmdsDeviceSetup.c
    XmdsDigChans.c
    XmdsDisplay.c
    XmdsExpr.c
    XmdsGetPutNid.c
    XmdsGetSetOptionIdx.c
    XmdsInitialize.c
    XmdsInput.c
    XmdsInputs.c
    XmdsNidOptionMenu.c
    XmdsOkButtons.c
    XmdsOnOffToggleButton.c
    XmdsPath.c
    XmdsPopupMenuPosition.c
    XmdsSetSubvalues.c
    XmdsSupport.c
    XmdsUsageIcons.c
    XmdsWavedraw.c
    XmdsWaveform.c
    XmdsWidgetCallbacks.c
    XmdsXdBox.c
    XmdsXdBoxDialog.c
    XmdsXdBoxDialogButton.c
    XmdsXdBoxOnOffButton.c
)

ADD_LIBRARY(
    XmdsShr
    SHARED
    ${SOURCES}
)

TARGET_INCLUDE_DIRECTORIES(
    XmdsShr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

SET_TARGET_PROPERTIES(
    XmdsShr
    PROPERTIES
        PREFIX ""
)

TARGET_LINK_LIBRARIES(
    XmdsShr
    PUBLIC
        MdsShr
        TreeShr
        TdiShr
        Motif::Motif
)