CATALOGNAME = lookalike
SOURCEDIR = ../src
TRANSLATIONDIR = $$PWD
LRELEASE_OPTIONS = -idbased
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
TRANSLATION_INSTALLDIR=$${M_TRANSLATION_DIR}
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

