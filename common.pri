TARGET = "lookalike"

unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /opt/$${TARGET}
    }
    BINDIR = $$PREFIX/bin
    DATADIR =/usr/share
    PKGDATADIR =$$PREFIX/share

    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
}

# Relative path definitions for gallery core
USER_XDG_DATA_HOME="/home/user/.local/share"
GALLERYCORE_DATA_ROOT_DIR="gallerycore"
GALLERYCORE_DATA_DATA_DIR="data"
FACE_RECOGNITION_DATABASE_FILENAME="faces.db"
GALLERYCORE_USER="gallerycoredata-user"
GALLERYCORE_GROUP="gallerycoredata-users"
REGULAR_USER="user"
REGULAR_GROUP="users"
