# trailsave plugin
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(SCRIVI_CFLAGS) 				\
	$(WARN_CFLAGS)

plugin_LTLIBRARIES = libtrailsave.la

libtrailsave_la_SOURCES = \
	scrivi-trail-save-plugin.h	\
	scrivi-trail-save-plugin.c

libtrailsave_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libtrailsave_la_LIBADD  = $(SCRIVI_LIBS)

plugin_in_files = trailsave.scrivi-plugin.desktop.in

%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)


-include $(top_srcdir)/git.mk
