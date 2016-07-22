
QT           += xml svg
TEMPLATE      = lib
CONFIG       += qt plugin
TARGET        = $$qtLibraryTarget(org.dunnart.BuiltinGMLFileIOPlugin)

include(../../../common_options.qmake)
include(../fileio_plugin_options.pri)

HEADERS       =
SOURCES       = plugin.cpp

win32: {
CONFIG += windows
LIBS += -L$$DESTDIR -ldunnartcanvas -ltopology -lcola -lvpsc -logdf -lavoid -lpsapi

}

