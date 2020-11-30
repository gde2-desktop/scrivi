desktopdir 	 = $(datadir)/applications
desktop_in_files = scrivi.desktop.in
desktop_DATA 	 = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

@INTLTOOL_XML_RULE@
appdatadir = $(datadir)/appdata
appdata_in_files = scrivi.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)

gsettings_SCHEMAS = org.gde2.scrivi.gschema.xml
@GSETTINGS_RULES@

man_MANS = scrivi.1

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = scrivi.pc

bugreportdir = $(libexecdir)/scrivi
bugreport_SCRIPTS = scrivi-bugreport.sh

EXTRA_DIST = 			\
	$(appdata_in_files)	\
	$(desktop_in_files)	\
	$(man_MANS)			\
	scrivi.pc.in			\
	scrivi-bugreport.sh.in

CLEANFILES =	 			\
	$(desktop_DATA)			\
	$(gsettings_SCHEMAS)	\
	$(pkgconfig_DATA)		\
	$(appdata_DATA)




-include $(top_srcdir)/git.mk
