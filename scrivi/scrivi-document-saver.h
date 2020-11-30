/*
 * scrivi-document-saver.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyrhing (C) 2007 - Paolo Maggi, Steve Frécinaux
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

#ifndef __SCRIVI_DOCUMENT_SAVER_H__
#define __SCRIVI_DOCUMENT_SAVER_H__

#include <scrivi/scrivi-document.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_DOCUMENT_SAVER              (scrivi_document_saver_get_type())
#define SCRIVI_DOCUMENT_SAVER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_DOCUMENT_SAVER, ScriviDocumentSaver))
#define SCRIVI_DOCUMENT_SAVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_DOCUMENT_SAVER, ScriviDocumentSaverClass))
#define SCRIVI_IS_DOCUMENT_SAVER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_DOCUMENT_SAVER))
#define SCRIVI_IS_DOCUMENT_SAVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_DOCUMENT_SAVER))
#define SCRIVI_DOCUMENT_SAVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_DOCUMENT_SAVER, ScriviDocumentSaverClass))

/*
 * Main object structure
 */
typedef struct _ScriviDocumentSaver ScriviDocumentSaver;

struct _ScriviDocumentSaver 
{
	GObject object;

	/*< private >*/
	GFileInfo		 *info;
	ScriviDocument		 *document;
	gboolean		  used;

	gchar			 *uri;
	const ScriviEncoding      *encoding;
	ScriviDocumentNewlineType  newline_type;

	ScriviDocumentSaveFlags    flags;

	gboolean		  keep_backup;
};

/*
 * Class definition
 */
typedef struct _ScriviDocumentSaverClass ScriviDocumentSaverClass;

struct _ScriviDocumentSaverClass 
{
	GObjectClass parent_class;

	/* Signals */
	void (* saving) (ScriviDocumentSaver *saver,
			 gboolean             completed,
			 const GError        *error);

	/* VTable */
	void			(* save)		(ScriviDocumentSaver *saver,
							 GTimeVal           *old_mtime);
	goffset			(* get_file_size)	(ScriviDocumentSaver *saver);
	goffset			(* get_bytes_written)	(ScriviDocumentSaver *saver);
};

/*
 * Public methods
 */
GType 		 	 scrivi_document_saver_get_type		(void) G_GNUC_CONST;

/* If enconding == NULL, the encoding will be autodetected */
ScriviDocumentSaver 	*scrivi_document_saver_new 		(ScriviDocument           *doc,
								 const gchar             *uri,
								 const ScriviEncoding     *encoding,
								 ScriviDocumentNewlineType newline_type,
								 ScriviDocumentSaveFlags   flags);

void			 scrivi_document_saver_saving		(ScriviDocumentSaver *saver,
								 gboolean            completed,
								 GError             *error);
void			 scrivi_document_saver_save		(ScriviDocumentSaver  *saver,
								 GTimeVal            *old_mtime);

#if 0
void			 scrivi_document_saver_cancel		(ScriviDocumentSaver  *saver);
#endif

ScriviDocument		*scrivi_document_saver_get_document	(ScriviDocumentSaver  *saver);

const gchar		*scrivi_document_saver_get_uri		(ScriviDocumentSaver  *saver);

/* If backup_uri is NULL no backup will be made */
const gchar		*scrivi_document_saver_get_backup_uri	(ScriviDocumentSaver  *saver);
void			*scrivi_document_saver_set_backup_uri	(ScriviDocumentSaver  *saver,
							 	 const gchar         *backup_uri);

/* Returns 0 if file size is unknown */
goffset			 scrivi_document_saver_get_file_size	(ScriviDocumentSaver  *saver);

goffset			 scrivi_document_saver_get_bytes_written	(ScriviDocumentSaver  *saver);

GFileInfo		*scrivi_document_saver_get_info		(ScriviDocumentSaver  *saver);

G_END_DECLS

#endif  /* __SCRIVI_DOCUMENT_SAVER_H__  */
