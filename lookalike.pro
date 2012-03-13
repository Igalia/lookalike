# ========================= #
# Compilation configuration #
# ========================= #

include(common.pri)

SUBDIRS = src
TEMPLATE = subdirs
CONFIG += ordered
QMAKE_DISTCLEAN += Makefile*

# Style
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
meegotouch.files = conf/$${TARGET}.css
meegotouch.path = $${M_THEME_DIR}/base/meegotouch/$${TARGET}/style

icon64.path = $$DATADIR/icons/hicolor/64x64/apps
icon64.files += art/icon-l-$${TARGET}.png

splash.path = $$DATADIR/$${TARGET}/
splash.files += art/$${TARGET}-splash-landscape.png
splash.files += art/$${TARGET}-splash-portrait.png

# Script files ??????????????
script.files = $${TARGET}
script.path = $$BINDIR

INSTALLS += meegotouch script icon64 splash
