# changecase plugin
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(SCRIVI_CFLAGS) 				\
	$(WARN_CFLAGS)

plugin_LTLIBRARIES = libchangecase.la

libchangecase_la_SOURCES = \
	scrivi-changecase-plugin.h	\
	scrivi-changecase-plugin.c

libchangecase_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libchangecase_la_LIBADD = $(SCRIVI_LIBS)

uidir = $(SCRIVI_PLUGINS_DATA_DIR)/changecase
ui_DATA =

plugin_in_files = changecase.scrivi-plugin.desktop.in

%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

EXTRA_DIST = $(ui_DATA) $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)


-include $(top_srcdir)/git.mk
