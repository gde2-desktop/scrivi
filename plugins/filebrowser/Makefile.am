# filebrowser

plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = \
	-I$(top_srcdir) 						\
	$(SCRIVI_CFLAGS) 						\
	$(WARN_CFLAGS)

BUILT_SOURCES = \
	scrivi-file-browser-enum-types.h		\
	scrivi-file-browser-enum-types.c		\
	scrivi-file-browser-marshal.h 		\
	scrivi-file-browser-marshal.c

plugin_LTLIBRARIES = libfilebrowser.la

NOINST_H_FILES = \
	scrivi-file-bookmarks-store.h 		\
	scrivi-file-browser-store.h 		\
	scrivi-file-browser-view.h 		\
	scrivi-file-browser-widget.h 		\
	scrivi-file-browser-error.h		\
	scrivi-file-browser-utils.h		\
	scrivi-file-browser-plugin.h		\
	scrivi-file-browser-messages.h

libfilebrowser_la_SOURCES = \
	$(BUILT_SOURCES) 			\
	scrivi-file-bookmarks-store.c 		\
	scrivi-file-browser-store.c 		\
	scrivi-file-browser-view.c 		\
	scrivi-file-browser-widget.c 		\
	scrivi-file-browser-utils.c 		\
	scrivi-file-browser-plugin.c		\
	scrivi-file-browser-messages.c		\
	$(NOINST_H_FILES)

libfilebrowser_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libfilebrowser_la_LIBADD = $(SCRIVI_LIBS)

# UI files (if you use ui for your plugin, list those files here)
uidir = $(SCRIVI_PLUGINS_DATA_DIR)/filebrowser
ui_DATA = scrivi-file-browser-widget-ui.xml

plugin_in_files = filebrowser.scrivi-plugin.desktop.in

%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

scrivi-file-browser-enum-types.h: scrivi-file-browser-enum-types.h.template $(NOINST_H_FILES) $(GLIB_MKENUMS)
	(cd $(srcdir) && $(GLIB_MKENUMS) --template scrivi-file-browser-enum-types.h.template $(NOINST_H_FILES)) > $@

scrivi-file-browser-enum-types.c: scrivi-file-browser-enum-types.c.template scrivi-file-browser-enum-register.c.template $(NOINST_H_FILES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && \
	$(GLIB_MKENUMS) --template scrivi-file-browser-enum-types.c.template $(NOINST_H_FILES) && \
	$(GLIB_MKENUMS) --template scrivi-file-browser-enum-register.c.template $(NOINST_H_FILES)) > $@

scrivi-file-browser-marshal.h: scrivi-file-browser-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) $(GLIB_GENMARSHAL) $< --header --prefix=scrivi_file_browser_marshal > $@

scrivi-file-browser-marshal.c: scrivi-file-browser-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) echo "#include \"scrivi-file-browser-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=scrivi_file_browser_marshal >> $@

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

@INTLTOOL_XML_NOMERGE_RULE@
filebrowser_gschema_in = org.gde2.scrivi.plugins.filebrowser.gschema.xml.in
gsettings_SCHEMAS = $(filebrowser_gschema_in:.xml.in=.xml)
@GSETTINGS_RULES@

EXTRA_DIST = \
	$(ui_DATA)					\
	$(plugin_in_files)				\
	$(filebrowser_gschema_in)			\
	scrivi-file-browser-enum-types.h.template	\
	scrivi-file-browser-enum-types.c.template	\
	scrivi-file-browser-enum-register.c.template	\
	scrivi-file-browser-marshal.list

CLEANFILES = \
	$(plugin_DATA)		\
	$(gsettings_SCHEMAS_in)		\
	$(gsettings_SCHEMAS)		\
	$(BUILT_SOURCES)

DISTCLEANFILES = \
	$(plugin_DATA)		\
	$(gsettings_SCHEMAS_in)		\
	$(gsettings_SCHEMAS)		\
	$(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
