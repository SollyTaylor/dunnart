TEMPLATE = lib
TARGET = cola
CONFIG += staticlib
DEPENDPATH += .. .
INCLUDEPATH += .. \
    ../libvpsc
include(../common_options.qmake)
CONFIG -= qt

#LIBS += -Wl,-undefined -Wl,dynamic_lookup

LIBS += -L$$DESTDIR -lvpsc -Tp
DEFINES += NOMINMAX

# Input
SOURCES += cola.cpp \
    colafd.cpp \
    conjugate_gradient.cpp \
    gradient_projection.cpp \
    straightener.cpp \
    connected_components.cpp \
    convex_hull.cpp \
    cluster.cpp \
    compound_constraints.cpp \
    output_svg.cpp \
    cc_clustercontainmentconstraints.cpp \
    cc_nonoverlapconstraints.cpp \
    box.cpp \
    shapepair.cpp \
    pseudorandom.cpp
HEADERS += cola.h \
    cluster.h \
    commondefs.h \
    compound_constraints.h \
    gradient_projection.h \
    sparse_matrix.h \
    straightener.h \
    output_svg.h \
    shortest_paths.h \
    cc_clustercontainmentconstraints.h \
    cc_nonoverlapconstraints.h \
    unused.h \
    box.h \
    shapepair.h \
    pseudorandom.h \
    config.h
