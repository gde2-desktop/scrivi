# External Tools plugin
SUBDIRS = tools data scripts
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

plugin_in_files = externaltools.scrivi-plugin.desktop.in
%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

EXTRA_DIST = $(plugin_in_files)

CLEANFILES = $(plugin_DATA)
DISTCLEANFILES = $(plugin_DATA)

-include $(top_srcdir)/git.mk
