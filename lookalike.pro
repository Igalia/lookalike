# ========================= #
# Compilation configuration #
# ========================= #

include(common.pri)

SUBDIRS = src
TEMPLATE = subdirs
CONFIG += ordered
QMAKE_DISTCLEAN += Makefile*

contains( launcher, no ) {
     INVOKERTYPEOPTION = "e"
} else {
#     INVOKERTYPEOPTION = "m"
     INVOKERTYPEOPTION = "e"
}

# message( "Defines:" )
# message( "PREFIX:" $$PREFIX )
# message( "DATADIR:" $$DATADIR )
# message( "PKGDATADIR:" $$PKGDATADIR )

QMAKE_SUBSTITUTES += \
      desktopfiles/$${TARGET}.desktop.in \
      dbus/com.igalia.$${TARGET}.service.in \
      debian/lintian-overrides.in \
      debian/$${TARGET}.aegis.in \
      debian/$${TARGET}.lintian-overrides.in \
      debian/$${TARGET}.install.in

#MAKE INSTALL

INSTALLS += meegotouch service script desktop icon80 splash theme

# Style
include($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
meegotouch.files = conf/$${TARGET}.css
meegotouch.path = $${M_THEME_DIR}/base/meegotouch/$${TARGET}/style

# Icons
theme.files = art/icon-m-*.png
theme.path = $${M_THEME_DIR}/base/meegotouch/$${TARGET}/icons
theme.CONFIG += no_check_exist

# Service files
service.files = dbus/com.igalia.$${TARGET}.service
service.path = $$DATADIR/dbus-1/services/

# Desktop files
desktop.files = desktopfiles/$${TARGET}.desktop
desktop.path = $$DATADIR/applications

icon80.path = $$DATADIR/icons/hicolor/80x80/apps
icon80.files += art/icon-l-$${TARGET}.png

splash.path = $$PKGDATADIR/splash/
splash.files += art/$${TARGET}-splash-landscape.png
splash.files += art/$${TARGET}-splash-portrait.png

# Script files ??????????????
script.files = $${TARGET}
script.path = $$BINDIR

