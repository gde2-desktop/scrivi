/*
 * scrivi-history-entry.h
 * This file is part of scrivi
 *
 * Copyright (C) 2006 - Paolo Borelli
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
 * Modified by the scrivi Team, 2006. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id$
 */

#ifndef __SCRIVI_HISTORY_ENTRY_H__
#define __SCRIVI_HISTORY_ENTRY_H__


G_BEGIN_DECLS

#define SCRIVI_TYPE_HISTORY_ENTRY             (scrivi_history_entry_get_type ())
#define SCRIVI_HISTORY_ENTRY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_HISTORY_ENTRY, ScriviHistoryEntry))
#define SCRIVI_HISTORY_ENTRY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_HISTORY_ENTRY, ScriviHistoryEntryClass))
#define SCRIVI_IS_HISTORY_ENTRY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_HISTORY_ENTRY))
#define SCRIVI_IS_HISTORY_ENTRY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_HISTORY_ENTRY))
#define SCRIVI_HISTORY_ENTRY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_HISTORY_ENTRY, ScriviHistoryEntryClass))


typedef struct _ScriviHistoryEntry        ScriviHistoryEntry;
typedef struct _ScriviHistoryEntryClass   ScriviHistoryEntryClass;
typedef struct _ScriviHistoryEntryPrivate ScriviHistoryEntryPrivate;

struct _ScriviHistoryEntryClass
{
	GtkComboBoxTextClass parent_class;
};

struct _ScriviHistoryEntry
{
	GtkComboBoxText parent_instance;

	ScriviHistoryEntryPrivate *priv;
};

GType		 scrivi_history_entry_get_type	(void) G_GNUC_CONST;

GtkWidget	*scrivi_history_entry_new		(const gchar       *history_id,
							 gboolean           enable_completion);

void		 scrivi_history_entry_prepend_text	(ScriviHistoryEntry *entry,
							 const gchar       *text);

void		 scrivi_history_entry_append_text	(ScriviHistoryEntry *entry,
							 const gchar       *text);

void		 scrivi_history_entry_clear		(ScriviHistoryEntry *entry);

void		 scrivi_history_entry_set_history_length	(ScriviHistoryEntry *entry,
							 guint              max_saved);

guint		 scrivi_history_entry_get_history_length	(ScriviHistoryEntry *gentry);

gchar		*scrivi_history_entry_get_history_id	(ScriviHistoryEntry *entry);

void             scrivi_history_entry_set_enable_completion 
							(ScriviHistoryEntry *entry,
							 gboolean           enable);
							 
gboolean         scrivi_history_entry_get_enable_completion 
							(ScriviHistoryEntry *entry);

GtkWidget	*scrivi_history_entry_get_entry		(ScriviHistoryEntry *entry);

typedef gchar * (* ScriviHistoryEntryEscapeFunc) (const gchar *str);
void		scrivi_history_entry_set_escape_func	(ScriviHistoryEntry *entry,
							 ScriviHistoryEntryEscapeFunc escape_func);

G_END_DECLS

#endif /* __SCRIVI_HISTORY_ENTRY_H__ */
