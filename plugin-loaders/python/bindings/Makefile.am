## Process this file with automake to produce Makefile.in

noinst_LTLIBRARIES = \
	scrivi.la

nodist_scrivi_la_SOURCES = 	\
	scrivi.c			\
	scriviutils.c		\
	scrivicommands.c

scrivi_la_LDFLAGS = \
	-module -avoid-version

scrivi_la_LIBADD = \
	$(PYTHON_LIB_LOC)       \
	$(PYTHON_LIBS)		\
	$(PYTHON_EXTRA_LIBS)	\
	$(PYGTK_LIBS)

scrivi_la_CFLAGS = \
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	-I$(top_srcdir)/scrivi				\
	-I$(top_builddir)/scrivi				\
	-I$(top_srcdir)/plugin-loaders/python		\
	-I$(top_builddir)/plugin-loaders/python		\
	$(SCRIVI_CFLAGS)					\
	$(NO_STRICT_ALIASING_CFLAGS)			\
	$(PYGTK_CFLAGS)					\
	$(PYTHON_CFLAGS)				\
	$(AM_CFLAGS)

$(top_builddir)/scrivi/scrivi-enum-types.h:
	cd $(top_builddir)/scrivi && $(MAKE) scrivi-enum-types.h

scrivi.c: scrivi.defs scrivi.override scriviplugin.override scrivimessage.override $(top_builddir)/scrivi/scrivi-enum-types.h
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register $(PYGTK_DEFSDIR)/pango-types.defs \
		--register $(PYGTK_DEFSDIR)/gdk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--register $(PYGTK_DEFSDIR)/gtksourceview2.defs \
		--register $(PYGOBJECT_DEFSDIR)/gio-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

scriviutils.c: scriviutils.defs scriviutils.override
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

scrivicommands.c: scrivicommands.defs scrivicommands.override scrivi.defs
	( cd $(srcdir) && $(PYGTK_CODEGEN) \
		--register scrivi.defs \
		--register $(PYGTK_DEFSDIR)/gtk-types.defs \
		--override $*.override \
		--prefix py$* $(<F) ) > $@ 

BINDING_SCRIVI_HEADERS_SRCDIR_IN = \
	scrivi/scrivi-app.h		\
	scrivi/scrivi-document.h		\
	scrivi/scrivi-encodings.h		\
	scrivi/scrivi-plugin.h		\
	plugin-loaders/python/scrivi-plugin-python.h \
	scrivi/scrivi-view.h		\
	scrivi/scrivi-statusbar.h		\
	scrivi/scrivi-tab.h 		\
	scrivi/scrivi-panel.h 		\
	scrivi/scrivi-window.h 		\
	scrivi/scrivi-help.h		\
	scrivi/scrivi-debug.h		\
	scrivi/scrivi-message-type.h	\
	scrivi/scrivi-message.h		\
	scrivi/scrivi-message-bus.h	\
	scrivi/scrivi-language-manager.h

BINDING_UTILS_HEADERS_SRCDIR_IN = \
	scrivi/scrivi-utils.h

BINDING_COMMANDS_HEADERS_SRCDIR_IN = \
	scrivi/scrivi-commands.h

BINDING_HEADERS_BUILDDIR_IN = 

BINDING_SCRIVI_HEADERS_SRCDIR	:= $(addprefix $(top_srcdir)/,$(BINDING_SCRIVI_HEADERS_SRCDIR_IN))
BINDING_UTILS_HEADERS_SRCDIR	:= $(addprefix $(top_srcdir)/,$(BINDING_UTILS_HEADERS_SRCDIR_IN))
BINDING_COMMANDS_HEADERS_SRCDIR	:= $(addprefix $(top_srcdir)/,$(BINDING_COMMANDS_HEADERS_SRCDIR_IN))

BINDING_HEADERS_BUILDDIR	:= $(addprefix $(top_builddir)/,$(BINDING_HEADERS_BUILDDIR_IN))

regenerate-python-binding:
	$(PYGTK_H2DEF) $(sort $(BINDING_SCRIVI_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > scrivi.defs.new
	$(PYGTK_H2DEF) $(sort $(BINDING_UTILS_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > scriviutils.defs.new
	$(PYGTK_H2DEF) $(sort $(BINDING_COMMANDS_HEADERS_SRCDIR) $(BINDING_HEADERS_BUILDDIR)) > scrivicommands.defs.new

BUILT_SOURCES = \
	scrivi.c		\
	scriviutils.c	\
	scrivicommands.c

EXTRA_DIST = \
	scrivi.override		\
	scrivi.defs		\
	scriviutils.override	\
	scriviutils.defs		\
	scrivicommands.override 	\
	scrivicommands.defs	\
	scrivimessage.override	\
	scriviplugin.override

CLEANFILES = $(BUILT_SOURCES)

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
