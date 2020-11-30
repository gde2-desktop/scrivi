# Spell checker plugin
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = 							\
	-I$(top_srcdir)						\
	$(SCRIVI_CFLAGS) 					\
	$(ENCHANT_CFLAGS)					\
	$(WARN_CFLAGS)

BUILT_SOURCES = 					\
	scrivi-spell-marshal.c				\
	scrivi-spell-marshal.h

plugin_LTLIBRARIES = libspell.la

libspell_la_SOURCES = \
	scrivi-spell-plugin.c 				\
	scrivi-spell-plugin.h 				\
	scrivi-spell-checker.c 				\
	scrivi-spell-checker.h				\
	scrivi-spell-checker-dialog.c			\
	scrivi-spell-checker-dialog.h			\
	scrivi-spell-checker-language.c			\
	scrivi-spell-checker-language.h			\
	scrivi-spell-language-dialog.c			\
	scrivi-spell-language-dialog.h			\
	scrivi-automatic-spell-checker.c			\
	scrivi-automatic-spell-checker.h			\
	scrivi-spell-utils.c				\
	scrivi-spell-utils.h				\
	$(BUILT_SOURCES)			

libspell_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS) 
libspell_la_LIBADD  = $(SCRIVI_LIBS) $(ENCHANT_LIBS)

uidir = $(SCRIVI_PLUGINS_DATA_DIR)/spell
ui_DATA = spell-checker.ui languages-dialog.ui scrivi-spell-setup-dialog.ui

scrivi-spell-marshal.h: scrivi-spell-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) $(GLIB_GENMARSHAL) $< --header --prefix=scrivi_marshal > $@

scrivi-spell-marshal.c: scrivi-spell-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) echo "#include \"scrivi-spell-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=scrivi_marshal >> $@

plugin_in_files = spell.scrivi-plugin.desktop.in

%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

@INTLTOOL_XML_NOMERGE_RULE@
spell_gschema_in = org.gde2.scrivi.plugins.spell.gschema.xml.in
gsettings_SCHEMAS = $(spell_gschema_in:.xml.in=.xml)
@GSETTINGS_RULES@


EXTRA_DIST = 					\
	$(ui_DATA)				\
	$(plugin_in_files)			\
	scrivi-spell-marshal.list    	        \
	$(spell_gschema_in)

CLEANFILES = $(BUILT_SOURCES) $(plugin_DATA) $(gsettings_SCHEMAS)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
