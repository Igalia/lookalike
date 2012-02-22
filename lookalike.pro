SUBDIRS     = src
TEMPLATE    = subdirs
CONFIG      += ordered

# Style
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
meegotouch.files = conf/lookalike.css
meegotouch.path = $${M_THEME_DIR}/base/meegotouch/lookalike/style
