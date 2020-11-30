/*
 * scrivi-document-input-stream.h
 * This file is part of scrivi
 *
 * Copyright (C) 2010 - Ignacio Casal Quinteiro
 *
 * scrivi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * scrivi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with scrivi; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

#ifndef __SCRIVI_DOCUMENT_INPUT_STREAM_H__
#define __SCRIVI_DOCUMENT_INPUT_STREAM_H__

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "scrivi-document.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_DOCUMENT_INPUT_STREAM		(scrivi_document_input_stream_get_type ())
#define SCRIVI_DOCUMENT_INPUT_STREAM(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM, ScriviDocumentInputStream))
#define SCRIVI_DOCUMENT_INPUT_STREAM_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM, ScriviDocumentInputStream const))
#define SCRIVI_DOCUMENT_INPUT_STREAM_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM, ScriviDocumentInputStreamClass))
#define SCRIVI_IS_DOCUMENT_INPUT_STREAM(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM))
#define SCRIVI_IS_DOCUMENT_INPUT_STREAM_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM))
#define SCRIVI_DOCUMENT_INPUT_STREAM_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_DOCUMENT_INPUT_STREAM, ScriviDocumentInputStreamClass))

typedef struct _ScriviDocumentInputStream	ScriviDocumentInputStream;
typedef struct _ScriviDocumentInputStreamClass	ScriviDocumentInputStreamClass;
typedef struct _ScriviDocumentInputStreamPrivate	ScriviDocumentInputStreamPrivate;

struct _ScriviDocumentInputStream
{
	GInputStream parent;
	
	ScriviDocumentInputStreamPrivate *priv;
};

struct _ScriviDocumentInputStreamClass
{
	GInputStreamClass parent_class;
};

GType				 scrivi_document_input_stream_get_type		(void) G_GNUC_CONST;

GInputStream			*scrivi_document_input_stream_new		(GtkTextBuffer           *buffer,
										 ScriviDocumentNewlineType type);

gsize				 scrivi_document_input_stream_get_total_size	(ScriviDocumentInputStream *stream);

gsize				 scrivi_document_input_stream_tell		(ScriviDocumentInputStream *stream);

G_END_DECLS

#endif /* __SCRIVI_DOCUMENT_INPUT_STREAM_H__ */
