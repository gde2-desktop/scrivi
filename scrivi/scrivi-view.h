/*
 * scrivi-view.h
 * This file is part of scrivi
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2005 Paolo Maggi  
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA 02110-1301, USA. 
 */
 
/*
 * Modified by the scrivi Team, 1998-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 */

#ifndef __SCRIVI_VIEW_H__
#define __SCRIVI_VIEW_H__

#include <gtk/gtk.h>

#include <scrivi/scrivi-document.h>
#include <gtksourceview/gtksourceview.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_VIEW            (scrivi_view_get_type ())
#define SCRIVI_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_VIEW, ScriviView))
#define SCRIVI_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_VIEW, ScriviViewClass))
#define SCRIVI_IS_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_VIEW))
#define SCRIVI_IS_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_VIEW))
#define SCRIVI_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_VIEW, ScriviViewClass))

/* Private structure type */
typedef struct _ScriviViewPrivate	ScriviViewPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviView		ScriviView;

struct _ScriviView
{
	GtkSourceView view;

	/*< private > */
	ScriviViewPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviViewClass		ScriviViewClass;

struct _ScriviViewClass
{
	GtkSourceViewClass parent_class;

	/* FIXME: Do we need placeholders ? */

	/* Key bindings */
	gboolean (* start_interactive_search)	(ScriviView       *view);
	gboolean (* start_interactive_goto_line)(ScriviView       *view);
	gboolean (* reset_searched_text)	(ScriviView       *view);

	void	 (* drop_uris)			(ScriviView	 *view,
						 gchar          **uri_list);
};

/*
 * Public methods
 */
GType		 scrivi_view_get_type     	(void) G_GNUC_CONST;

GtkWidget	*scrivi_view_new			(ScriviDocument   *doc);

void		 scrivi_view_cut_clipboard 	(ScriviView       *view);
void		 scrivi_view_copy_clipboard 	(ScriviView       *view);
void		 scrivi_view_paste_clipboard	(ScriviView       *view);
void		 scrivi_view_delete_selection	(ScriviView       *view);
void		 scrivi_view_select_all		(ScriviView       *view);

void		 scrivi_view_scroll_to_cursor 	(ScriviView       *view);

void 		 scrivi_view_set_font		(ScriviView       *view,
						 gboolean         def,
						 const gchar     *font_name);

G_END_DECLS

#endif /* __SCRIVI_VIEW_H__ */
