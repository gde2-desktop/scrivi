# C plugin loader

SUBDIRS = bindings
loaderdir = $(libdir)/scrivi/plugin-loaders

AM_CPPFLAGS = \
	-I$(top_srcdir) 						\
	-I$(top_builddir)						\
	-I$(top_srcdir)/scrivi						\
	-I$(top_builddir)/scrivi						\
	$(SCRIVI_CFLAGS) 						\
	$(WARN_CFLAGS)							\
	$(DISABLE_DEPRECATED_CFLAGS)					\
	$(PYGTK_CFLAGS)							\
	$(PYTHON_CFLAGS)						\
	-DSCRIVI_PLUGINS_LIBS_DIR=\"$(SCRIVI_PLUGINS_LIBS_DIR)\"		\
	-DSCRIVI_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

loader_LTLIBRARIES = libpythonloader.la


NOINST_H_FILES = \
	scrivi-plugin-loader-python.h	\
	scrivi-plugin-python.h

libpythonloader_la_SOURCES = \
	scrivi-plugin-loader-python.c 	\
	scrivi-plugin-python.c		\
	$(NOINST_H_FILES)

libpythonloader_la_LDFLAGS = $(LOADER_LIBTOOL_FLAGS)
libpythonloader_la_LIBADD = \
	$(SCRIVI_LIBS) 			\
	bindings/scrivi.la

-include $(top_srcdir)/git.mk
