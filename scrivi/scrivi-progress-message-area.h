/*
 * scrivi-progress-message-area.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi 
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
 * Modified by the scrivi Team, 2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_PROGRESS_MESSAGE_AREA_H__
#define __SCRIVI_PROGRESS_MESSAGE_AREA_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_PROGRESS_MESSAGE_AREA              (scrivi_progress_message_area_get_type())
#define SCRIVI_PROGRESS_MESSAGE_AREA(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PROGRESS_MESSAGE_AREA, ScriviProgressMessageArea))
#define SCRIVI_PROGRESS_MESSAGE_AREA_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PROGRESS_MESSAGE_AREA, ScriviProgressMessageAreaClass))
#define SCRIVI_IS_PROGRESS_MESSAGE_AREA(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PROGRESS_MESSAGE_AREA))
#define SCRIVI_IS_PROGRESS_MESSAGE_AREA_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PROGRESS_MESSAGE_AREA))
#define SCRIVI_PROGRESS_MESSAGE_AREA_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PROGRESS_MESSAGE_AREA, ScriviProgressMessageAreaClass))

/* Private structure type */
typedef struct _ScriviProgressMessageAreaPrivate ScriviProgressMessageAreaPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviProgressMessageArea ScriviProgressMessageArea;

struct _ScriviProgressMessageArea 
{
	GtkInfoBar parent;

	/*< private > */
	ScriviProgressMessageAreaPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviProgressMessageAreaClass ScriviProgressMessageAreaClass;

struct _ScriviProgressMessageAreaClass 
{
	GtkInfoBarClass parent_class;
};

/*
 * Public methods
 */
GType 		 scrivi_progress_message_area_get_type 		(void) G_GNUC_CONST;

GtkWidget	*scrivi_progress_message_area_new      		(const gchar              *stock_id,
								 const gchar              *markup,
								 gboolean                  has_cancel);

void		 scrivi_progress_message_area_set_stock_image	(ScriviProgressMessageArea *area,
								 const gchar              *stock_id);

void		 scrivi_progress_message_area_set_markup		(ScriviProgressMessageArea *area,
								 const gchar              *markup);

void		 scrivi_progress_message_area_set_text		(ScriviProgressMessageArea *area,
								 const gchar              *text);

void		 scrivi_progress_message_area_set_fraction	(ScriviProgressMessageArea *area,
								 gdouble                   fraction);

void		 scrivi_progress_message_area_pulse		(ScriviProgressMessageArea *area);
								 

G_END_DECLS

#endif  /* __SCRIVI_PROGRESS_MESSAGE_AREA_H__  */
