
QT           += xml svg
TEMPLATE      = lib
CONFIG       += qt plugin
TARGET        = $$qtLibraryTarget(org.dunnart.UMLShapesPlugin)

include(../../../common_options.qmake)
include(../shape_plugin_options.pri)

HEADERS       = umlclass.h
SOURCES       = plugin.cpp umlclass.cpp


win32: {
CONFIG += windows
LIBS += -L$$DESTDIR -ldunnartcanvas -ltopology -lcola -lvpsc -logdf -lavoid -lpsapi

}
