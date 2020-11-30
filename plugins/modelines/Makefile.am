# Modelines Plugin
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(SCRIVI_CFLAGS) 				\
	$(WARN_CFLAGS)

modelinesdir = $(SCRIVI_PLUGINS_DATA_DIR)/modelines
modelines_DATA = \
	language-mappings

plugin_LTLIBRARIES = libmodelines.la

libmodelines_la_SOURCES = \
	scrivi-modeline-plugin.h				\
	scrivi-modeline-plugin.c				\
	modeline-parser.h				\
	modeline-parser.c

libmodelines_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libmodelines_la_LIBADD  = $(SCRIVI_LIBS)

plugin_in_files = modelines.scrivi-plugin.desktop.in
%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

EXTRA_DIST = \
	$(plugin_in_files)				\
	$(modelines_DATA)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)


-include $(top_srcdir)/git.mk
