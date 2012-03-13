# DOXYGEN_BIN=$$findFile(doxygen)
DOXYGEN_BIN=doxygen

QMAKE_EXTRA_TARGETS += doc
doc.target = doc
isEmpty(DOXYGEN_BIN) {
    doc.commands = @echo "Unable to detect doxygen in PATH"
} else {
    doc.commands = @$${DOXYGEN_BIN} doc/lookalikedoxy.cfg  ;
    doc.commands+= ./doc/xmlize.pl ;


# install rules

    htmldocs.files = ./doc/html/*.html ./doc/html/*.css
    htmldocs.path = /usr/share/doc/lookalike-doc
    htmldocs.CONFIG += no_check_exist

    docimages.files = ./doc/html/*.png ./doc/html/*.gif
    docimages.path = /usr/share/doc/lookalike-doc
    docimages.CONFIG += no_check_exist

    docmaps.files = ./doc/html/*.map
    docmaps.path = /usr/share/doc/lookalike-doc
    docmaps.CONFIG += no_check_exist

  INSTALLS += htmldocs docimages docmaps
}
doc.depends = FORCE

