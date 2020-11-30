uidir = $(datadir)/scrivi/ui/

AM_CPPFLAGS = 							\
	-I$(top_srcdir)						\
	-I$(top_builddir)					\
	-I$(top_srcdir)/scrivi 					\
	-I$(top_builddir)/scrivi					\
	$(SCRIVI_CFLAGS) 					\
	$(WARN_CFLAGS)

noinst_LTLIBRARIES = libdialogs.la

libdialogs_la_SOURCES = 			\
	scrivi-preferences-dialog.h		\
	scrivi-preferences-dialog.c		\
	scrivi-close-confirmation-dialog.c 	\
	scrivi-close-confirmation-dialog.h 	\
	scrivi-encodings-dialog.c		\
	scrivi-encodings-dialog.h		\
	scrivi-search-dialog.h			\
	scrivi-search-dialog.c

ui_DATA =					\
	scrivi-encodings-dialog.ui		\
	scrivi-preferences-dialog.ui		\
	scrivi-search-dialog-gtk3.ui     \
	scrivi-search-dialog.ui

EXTRA_DIST = $(ui_DATA)

-include $(top_srcdir)/git.mk
