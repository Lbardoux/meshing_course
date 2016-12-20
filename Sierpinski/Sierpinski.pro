#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T03:27:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sierpinski
TEMPLATE = app
INCLUDEPATH += includes/ \
               sources/ \
               sources/file_io/ \
               sources/mesh/ \
               sources/predicats/ \
               sources/mesh/plugins \
               includes/iterators/ \
               includes/predicats/ \
               includes/mesh/ \
               includes/mesh/plugins/ \


SOURCES += sources/gasket.cpp \
           sources/gldisplay.cpp \
           sources/main.cpp \
           sources/mainwindow.cpp \
           sources/file_io/file_io.cpp \
           sources/mesh/Mesh.cpp \
           sources/mesh/Triangle.cpp \
           sources/predicats/predicats.cpp \
    sources/mesh/plugins/OffLoader.cpp \
    sources/mesh/TopoTriangle.cpp \
    sources/mesh/plugins/neighbors.cpp \
    sources/CallBackglBegin.cpp

HEADERS  += includes/gasket.h \
            includes/gldisplay.h \
            includes/mainwindow.h \
            sources/file_io/file_io.hpp \
            includes/iterators/MeshCirculator.hpp \
            includes/iterators/MeshIterator.hpp \
            includes/iterators/TriangleCirculator.hpp \
            includes/iterators/TriangleIterator.hpp \
            includes/iterators/VertexCirculator.hpp \
            includes/iterators/VertexIterator.hpp \
            includes/mesh/Mesh.hpp \
            includes/mesh/TopoTriangle.hpp \
            includes/mesh/Triangle.hpp \
            includes/mesh/Vertex3D.hpp \
            includes/predicats/predicats.hpp \
            includes/predicats/struct_predicats.hpp \
    includes/mesh/plugins/OffLoader.hpp \
    includes/mesh/plugins/neighbors.hpp \
    includes/mesh/plugins/common.hpp \
    includes/CallBackglBegin.hpp \
    includes/logs.hpp

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
