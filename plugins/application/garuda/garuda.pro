
QT           += xml svg network
TEMPLATE      = lib
CONFIG       += qt plugin
TARGET        = $$qtLibraryTarget(org.dunnart.GarudaApplicationPlugin)

include(../../../common_options.qmake)
include(../application_plugin_options.pri)

HEADERS       = \
    appgarudaclient.h \
    openincompatiblesoftwarewidget.h \

SOURCES       = plugin.cpp \
    appgarudaclient.cpp \
    openincompatiblesoftwarewidget.cpp \

FORMS += \
    openincompatiblesoftwarewidget.ui


# Copies the given files to the destination directory
defineTest(copyToDestDir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$RESOURCESDIR

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $(MKDIR) $$quote($$DDIR) $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

RESOURCE_FILES = \
    $$PWD/../../../libdunnartcanvas/doc/DunnartIcon64.png \
    $$PWD/../../../libdunnartcanvas/doc/DunnartScreen.png

copyToDestDir($$RESOURCE_FILES)


win32: {
CONFIG += windows
LIBS += -L$$DESTDIR -ldunnartcanvas -ltopology -lcola -lvpsc -logdf -lavoid -lpsapi

}
