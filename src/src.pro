 ========================= #
# Compilation configuration #
# ========================= #

include(../common.pri)

TEMPLATE    = app

contains( debug, yes ) {
     message( "Configuring for debug build ..." )
     CONFIG += debug warn_on
     contains( opt, yes ) {
          message( "Debugging built is incompatible with optimized built ..." )
     }
} else {
     message( "Configuring for release build ..." )
     CONFIG += release warn_on
     DEFINES += QT_NO_DEBUG_OUTPUT
#     DEFINES += QT_NO_WARNING_OUTPUT
     contains( opt, yes ) {
          message( "Configuring for optimized build ..." )
          #More CXX flags to be added here, if needed (see Qt doc).
     }
}

contains( log, yes ) {
     message( "Look Alike is being built with logging support ..." )
     DEFINES += LOOKALIKE_LOGGING
}

contains( launcher, no ) {
     message( "Look Alike is being built without launcher support ..." )
} else {
     # Add config flag for applauncherd (mbooster)
     CONFIG += meegotouch-boostable
}

# Avoid automatic casts from QString to QUrl. Dangerous!!!
DEFINES += QT_NO_URL_CAST_FROM_STRING

# Treat warnings as errors (Do not remove this!)
# Hide all the library symbols by default (use FACEAUTORECOGNIZER_EXPORT to export them)
QMAKE_CFLAGS_WARN_OFF = $$QMAKE_CFLAGS_WARN_ON
QMAKE_CXXFLAGS_WARN_OFF = $$QMAKE_CXXFLAGS_WARN_ON
QMAKE_LFLAGS += -Wl,--as-needed
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS_DEBUG += -O0 -ggdb
#QMAKE_CXXFLAGS_DEBUG +=  -Wunreachable-code
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_DISTCLEAN += *.o moc_* Makefile*

target.path = $$PREFIX/bin
INSTALLS += target

OBJECTS_DIR = build/
MOC_DIR = build/

CONFIG +=                   \
    faceplatform            \
    gallerycore             \
    link_pkgconfig          \
    meegotouch              \
    qtsparql                \
    qtsparql-tracker-live   \
    quillmetadata

SOURCES +=                      \
    facedatabaseprovider.cpp    \
    facetrackerprovider.cpp     \
    facetrackerproxy.cpp        \
    lookalikemain.cpp           \
    lookalikemain_p.cpp         \
    main.cpp                    \
    nullthumbnailer.cpp         \
    trackercontentprovider.cpp  \
    trackercontentprovider_p.cpp

HEADERS +=                      \
    enforce.h                   \
    facedatabaseprovider.h      \
    facetrackerprovider.h       \
    facetrackerproxy.h          \
    lookalikemain.h             \
    lookalikemain_p.h           \
    nullthumbnailer.h           \
    trackercontentprovider.h    \
    trackercontentprovider_p.h

message( "Settings:" )
message( "C Compiler:" $$QMAKE_CC )
message( "C++ Compiler:" $$QMAKE_CXX )
message( "C++ Warn On:" $$QMAKE_CXXFLAGS_WARN_ON )
message( "C++ Warn Off:" $$QMAKE_CXXFLAGS_WARN_OFF )
message( "C++ Debug flags:" $$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_DEBUG )
message( "C++ Release flags:" $$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_RELEASE  )
message( "Linker flags:" $$QMAKE_LFLAGS )
message( "LIBS flags:" $$QMAKE_LIBS )
