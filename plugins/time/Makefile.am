# time plugin
plugindir = $(SCRIVI_PLUGINS_LIBS_DIR)

AM_CPPFLAGS = \
	-I$(top_srcdir) 				\
	$(SCRIVI_CFLAGS) 				\
	$(WARN_CFLAGS)

plugin_LTLIBRARIES = libtime.la

libtime_la_SOURCES = \
	scrivi-time-plugin.h	\
	scrivi-time-plugin.c

libtime_la_LDFLAGS = $(PLUGIN_LIBTOOL_FLAGS)
libtime_la_LIBADD  = $(SCRIVI_LIBS)

uidir = $(SCRIVI_PLUGINS_DATA_DIR)/time
ui_DATA = \
	scrivi-time-dialog.ui				\
	scrivi-time-setup-dialog.ui

plugin_in_files = time.scrivi-plugin.desktop.in

%.scrivi-plugin: %.scrivi-plugin.desktop.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*po) ; $(INTLTOOL_MERGE) $(top_srcdir)/po $< $@ -d -u -c $(top_builddir)/po/.intltool-merge-cache

plugin_DATA = $(plugin_in_files:.scrivi-plugin.desktop.in=.scrivi-plugin)

@INTLTOOL_XML_NOMERGE_RULE@
time_gschema_in = org.gde2.scrivi.plugins.time.gschema.xml.in
gsettings_SCHEMAS = $(time_gschema_in:.xml.in=.xml)
@GSETTINGS_RULES@

EXTRA_DIST = $(ui_DATA) $(plugin_in_files) $(time_gschema_in)

CLEANFILES = $(plugin_DATA) $(gsettings_SCHEMAS_in) $(gsettings_SCHEMASn)
DISTCLEANFILES = $(plugin_DATA) $(gsettings_SCHEMAS_in) $(gsettings_SCHEMAS)


-include $(top_srcdir)/git.mk
