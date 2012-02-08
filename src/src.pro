TEMPLATE    = app
TARGET      = lookalike

contains(debug, yes) {
    CONFIG += debug warn_on
}

# Treat warnings as errors (Do not remove this!)
# Hide all the library symbols by default (use FACEAUTORECOGNIZER_EXPORT to export them)
QMAKE_CFLAGS_WARN_OFF = $$QMAKE_CFLAGS_WARN_ON
QMAKE_CXXFLAGS_WARN_OFF = $$QMAKE_CXXFLAGS_WARN_ON
QMAKE_LFLAGS += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS_DEBUG += -O0 -ggdb
#QMAKE_CXXFLAGS_DEBUG +=  -Wunreachable-code
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log  coverage *.o moc_* Makefile* doc/html/*

target.path = /usr/bin
INSTALLS += target

OBJECTS_DIR = build/
MOC_DIR = build/

CONFIG +=           \
    gallerycore     \
    link_pkgconfig  \
    meegotouch      \
    faceplatform    \
    qtsparql

SOURCES += \
    main.cpp \
    facedatabaseprovider.cpp \
    nullthumbnailer.cpp \
    lookalikemain.cpp \
    lookalikemain_p.cpp

HEADERS += \
    facedatabaseprovider.h \
    nullthumbnailer.h \
    lookalikemain.h \
    lookalikemain_p.h

message( "Settings:" )
message( "C Compiler:" $$QMAKE_CC )
message( "C++ Compiler:" $$QMAKE_CXX )
message( "C++ Warn On:" $$QMAKE_CXXFLAGS_WARN_ON )
message( "C++ Warn Off:" $$QMAKE_CXXFLAGS_WARN_OFF )
message( "C++ Debug flags:" $$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_DEBUG )
message( "C++ Release flags:" $$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_RELEASE  )
message( "Linker flags:" $$QMAKE_LFLAGS )
message( "LIBS flags:" $$QMAKE_LIBS )
















