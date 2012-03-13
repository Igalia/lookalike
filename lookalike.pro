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

# Script files ??????????????
script.files = $${TARGET}
script.path = $$BINDIR

INSTALLS += meegotouch script
