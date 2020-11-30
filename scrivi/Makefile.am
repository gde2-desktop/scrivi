## Process this file with automake to produce Makefile.in
SUBDIRS = dialogs smclient

bin_PROGRAMS = scrivi

noinst_LTLIBRARIES = libscrivi.la

AM_CPPFLAGS =								\
	-I$(top_srcdir)							\
	-I$(srcdir)							\
	-I$(srcdir)/smclient						\
	$(SCRIVI_CFLAGS)							\
	$(WARN_CFLAGS)							\
	-DDATADIR=\""$(datadir)"\"					\
	-DLIBDIR=\""$(libdir)"\"

scrivi_SOURCES = \
	scrivi.c

scrivi_LDADD = libscrivi.la $(SCRIVI_LIBS) $(EGG_SMCLIENT_LIBS)

scrivi_LDFLAGS = -export-dynamic -no-undefined -export-symbols-regex "^[[^_]].*"

libscrivi_la_LDFLAGS = -export-dynamic -no-undefined -export-symbols-regex "^[[^_]].*"

libscrivi_la_LIBADD = \
	dialogs/libdialogs.la		\
	smclient/libeggsmclient.la

# SCRIVI_LIBS must be the last to ensure correct order on some platforms
libscrivi_la_LIBADD += $(SCRIVI_LIBS) -lICE

BUILT_SOURCES = 			\
	scrivi-enum-types.c		\
	scrivi-enum-types.h		\
	scrivi-marshal.c			\
	scrivi-marshal.h


NOINST_H_FILES =			\
	scrivi-close-button.h		\
	scrivi-dirs.h			\
	scrivi-document-input-stream.h	\
	scrivi-document-loader.h		\
	scrivi-document-output-stream.h	\
	scrivi-document-saver.h		\
	scrivi-documents-panel.h		\
	scrivi-gio-document-loader.h	\
	scrivi-gio-document-saver.h	\
	scrivi-history-entry.h		\
	scrivi-io-error-message-area.h	\
	scrivi-language-manager.h	\
	scrivi-object-module.h		\
	scrivi-plugin-info.h		\
	scrivi-plugin-info-priv.h	\
	scrivi-plugin-loader.h		\
	scrivi-plugin-manager.h		\
	scrivi-plugins-engine.h		\
	scrivi-prefs-manager-private.h	\
	scrivi-print-job.h		\
	scrivi-print-preview.h		\
	scrivi-session.h			\
	scrivi-smart-charset-converter.h	\
	scrivi-style-scheme-manager.h	\
	scrivi-tab-label.h		\
	plugde2xtregion.h		\
	scrivi-ui.h			\
	scrivi-window-private.h

INST_H_FILES =				\
	scrivi-app.h			\
	scrivi-commands.h		\
	scrivi-debug.h			\
	scrivi-document.h 		\
	scrivi-encodings.h		\
	scrivi-encodings-combo-box.h	\
	scrivi-file-chooser-dialog.h	\
	scrivi-help.h 			\
	scrivi-message-bus.h		\
	scrivi-message-type.h		\
	scrivi-message.h			\
	scrivi-notebook.h		\
	scrivi-panel.h			\
	scrivi-plugin.h			\
	scrivi-prefs-manager-app.h	\
	scrivi-prefs-manager.h		\
	scrivi-progress-message-area.h	\
	scrivi-statusbar.h		\
	scrivi-status-combo-box.h	\
	scrivi-tab.h 			\
	scrivi-utils.h 			\
	scrivi-view.h 			\
	scrivi-window.h

if !ENABLE_GVFS_METADATA
INST_H_FILES += scrivi-metadata-manager.h
endif

headerdir = $(prefix)/include/scrivi

header_DATA = 				\
	$(INST_H_FILES)


libscrivi_la_SOURCES = 			\
	$(BUILT_SOURCES)		\
	$(BACON_FILES)			\
	$(POSIXIO_FILES)		\
	scrivi-app.c			\
	scrivi-close-button.c		\
	scrivi-commands-documents.c	\
	scrivi-commands-edit.c		\
	scrivi-commands-file.c		\
	scrivi-commands-file-print.c	\
	scrivi-commands-help.c		\
	scrivi-commands-search.c		\
	scrivi-commands-view.c		\
	scrivi-debug.c			\
	scrivi-dirs.c			\
	scrivi-document.c 		\
	scrivi-document-input-stream.c	\
	scrivi-document-loader.c		\
	scrivi-document-output-stream.c	\
	scrivi-gio-document-loader.c	\
	scrivi-document-saver.c		\
	scrivi-gio-document-saver.c	\
	scrivi-documents-panel.c		\
	scrivi-encodings.c		\
	scrivi-encodings-combo-box.c	\
	scrivi-file-chooser-dialog.c	\
	scrivi-help.c			\
	scrivi-history-entry.c		\
	scrivi-io-error-message-area.c	\
	scrivi-language-manager.c	\
	scrivi-message-bus.c		\
	scrivi-message-type.c		\
	scrivi-message.c			\
	scrivi-object-module.c		\
	scrivi-notebook.c		\
	scrivi-panel.c			\
	scrivi-plugin-info.c		\
	scrivi-plugin.c			\
	scrivi-plugin-loader.c		\
	scrivi-plugin-manager.c		\
	scrivi-plugins-engine.c		\
	scrivi-prefs-manager-app.c	\
	scrivi-prefs-manager.c		\
	scrivi-prefs-manager-private.h	\
	scrivi-print-job.c		\
	scrivi-print-preview.c		\
	scrivi-progress-message-area.c	\
	scrivi-session.c			\
	scrivi-smart-charset-converter.c	\
	scrivi-statusbar.c		\
	scrivi-status-combo-box.c	\
	scrivi-style-scheme-manager.c	\
	scrivi-tab.c 			\
	scrivi-tab-label.c		\
	scrivi-utils.c 			\
	scrivi-view.c 			\
	scrivi-window.c			\
	plugde2xtregion.c		\
	$(NOINST_H_FILES)		\
	$(INST_H_FILES)

if !ENABLE_GVFS_METADATA
libscrivi_la_SOURCES += scrivi-metadata-manager.c
endif

scrivi-enum-types.h: scrivi-enum-types.h.template $(INST_H_FILES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template scrivi-enum-types.h.template $(INST_H_FILES)) > $@

scrivi-enum-types.c: scrivi-enum-types.c.template $(INST_H_FILES) $(GLIB_MKENUMS)
	$(AM_V_GEN) (cd $(srcdir) && $(GLIB_MKENUMS) --template scrivi-enum-types.c.template $(INST_H_FILES)) > $@

scrivi-marshal.h: scrivi-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) $(GLIB_GENMARSHAL) $< --header --prefix=scrivi_marshal > $@

scrivi-marshal.c: scrivi-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) echo "#include \"scrivi-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=scrivi_marshal >> $@

uidir = $(datadir)/scrivi/ui/
ui_DATA = 				\
	scrivi-ui.xml			\
	scrivi-print-preferences.ui

EXTRA_DIST = 				\
	$(ui_DATA)			\
	scrivi-enum-types.h.template	\
	scrivi-enum-types.c.template	\
	scrivi-marshal.list		\
	scrivi.rc

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

BACON_DIR=$(srcdir)/../../libbacon/src/
BACON_FILES=bacon-message-connection.h bacon-message-connection.c

regenerate-built-sources:
	BACONFILES="$(BACON_FILES)" BACONDIR="$(BACON_DIR)" $(top_srcdir)/scrivi/update-from-bacon.sh

-include $(top_srcdir)/git.mk
