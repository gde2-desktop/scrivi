#ifndef __SCRIVI_FILE_BROWSER_UTILS_H__
#define __SCRIVI_FILE_BROWSER_UTILS_H__

#include <scrivi/scrivi-window.h>
#include <gio/gio.h>

GdkPixbuf *scrivi_file_browser_utils_pixbuf_from_theme     (gchar const *name,
                                                           GtkIconSize size);

GdkPixbuf *scrivi_file_browser_utils_pixbuf_from_icon	  (GIcon * icon,
                                                           GtkIconSize size);
GdkPixbuf *scrivi_file_browser_utils_pixbuf_from_file	  (GFile * file,
                                                           GtkIconSize size);

gchar * scrivi_file_browser_utils_file_basename		  (GFile * file);
gchar * scrivi_file_browser_utils_uri_basename             (gchar const * uri);

gboolean scrivi_file_browser_utils_confirmation_dialog     (ScriviWindow * window,
                                                           GtkMessageType type,
                                                           gchar const *message,
		                                           gchar const *secondary, 
		                                           gchar const * button_stock, 
		                                           gchar const * button_label);

#endif /* __SCRIVI_FILE_BROWSER_UTILS_H__ */

// ex:ts=8:noet:
