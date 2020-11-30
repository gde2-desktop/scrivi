/*
 * scrivi-document-loader.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyright (C) 2007 - Paolo Maggi, Steve Frécinaux
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
 * Modified by the scrivi Team, 2005-2007. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifndef __SCRIVI_DOCUMENT_LOADER_H__
#define __SCRIVI_DOCUMENT_LOADER_H__

#include <scrivi/scrivi-document.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_DOCUMENT_LOADER              (scrivi_document_loader_get_type())
#define SCRIVI_DOCUMENT_LOADER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_DOCUMENT_LOADER, ScriviDocumentLoader))
#define SCRIVI_DOCUMENT_LOADER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_DOCUMENT_LOADER, ScriviDocumentLoaderClass))
#define SCRIVI_IS_DOCUMENT_LOADER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_DOCUMENT_LOADER))
#define SCRIVI_IS_DOCUMENT_LOADER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_DOCUMENT_LOADER))
#define SCRIVI_DOCUMENT_LOADER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_DOCUMENT_LOADER, ScriviDocumentLoaderClass))

/* Private structure type */
typedef struct _ScriviDocumentLoaderPrivate ScriviDocumentLoaderPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviDocumentLoader ScriviDocumentLoader;

struct _ScriviDocumentLoader 
{
	GObject object;

	ScriviDocument		 *document;
	gboolean		  used;

	/* Info on the current file */
	GFileInfo		 *info;
	gchar			 *uri;
	const ScriviEncoding	 *encoding;
	const ScriviEncoding	 *auto_detected_encoding;
	ScriviDocumentNewlineType  auto_detected_newline_type;
};

/*
 * Class definition
 */
typedef struct _ScriviDocumentLoaderClass ScriviDocumentLoaderClass;

struct _ScriviDocumentLoaderClass 
{
	GObjectClass parent_class;

	/* Signals */
	void (* loading) (ScriviDocumentLoader *loader,
			  gboolean             completed,
			  const GError        *error);

	/* VTable */
	void			(* load)		(ScriviDocumentLoader *loader);
	gboolean		(* cancel)		(ScriviDocumentLoader *loader);
	goffset			(* get_bytes_read)	(ScriviDocumentLoader *loader);
};

/*
 * Public methods
 */
GType 		 	 scrivi_document_loader_get_type		(void) G_GNUC_CONST;

/* If enconding == NULL, the encoding will be autodetected */
ScriviDocumentLoader 	*scrivi_document_loader_new 		(ScriviDocument       *doc,
								 const gchar         *uri,
								 const ScriviEncoding *encoding);

void			 scrivi_document_loader_loading		(ScriviDocumentLoader *loader,
								 gboolean             completed,
								 GError              *error);

void			 scrivi_document_loader_load		(ScriviDocumentLoader *loader);
#if 0
gboolean		 scrivi_document_loader_load_from_stdin	(ScriviDocumentLoader *loader);
#endif		 
gboolean		 scrivi_document_loader_cancel		(ScriviDocumentLoader *loader);

ScriviDocument		*scrivi_document_loader_get_document	(ScriviDocumentLoader *loader);

/* Returns STDIN_URI if loading from stdin */
#define STDIN_URI "stdin:" 
const gchar		*scrivi_document_loader_get_uri		(ScriviDocumentLoader *loader);

const ScriviEncoding	*scrivi_document_loader_get_encoding	(ScriviDocumentLoader *loader);

ScriviDocumentNewlineType scrivi_document_loader_get_newline_type (ScriviDocumentLoader *loader);

goffset			 scrivi_document_loader_get_bytes_read	(ScriviDocumentLoader *loader);

/* You can get from the info: content_type, time_modified, standard_size, access_can_write 
   and also the metadata*/
GFileInfo		*scrivi_document_loader_get_info		(ScriviDocumentLoader *loader);

G_END_DECLS

#endif  /* __SCRIVI_DOCUMENT_LOADER_H__  */
