/*
 * scrivi-gio-document-loader.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyright (C) 2007 - Paolo Maggi, Steve Frécinaux
 * Copyright (C) 2008 - Jesse van den Kieboom
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
 * Modified by the scrivi Team, 2005-2008. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifndef __SCRIVI_GIO_DOCUMENT_LOADER_H__
#define __SCRIVI_GIO_DOCUMENT_LOADER_H__

#include <scrivi/scrivi-document.h>
#include "scrivi-document-loader.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_GIO_DOCUMENT_LOADER              (scrivi_gio_document_loader_get_type())
#define SCRIVI_GIO_DOCUMENT_LOADER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_GIO_DOCUMENT_LOADER, ScriviGioDocumentLoader))
#define SCRIVI_GIO_DOCUMENT_LOADER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_GIO_DOCUMENT_LOADER, ScriviGioDocumentLoaderClass))
#define SCRIVI_IS_GIO_DOCUMENT_LOADER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_GIO_DOCUMENT_LOADER))
#define SCRIVI_IS_GIO_DOCUMENT_LOADER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_GIO_DOCUMENT_LOADER))
#define SCRIVI_GIO_DOCUMENT_LOADER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_GIO_DOCUMENT_LOADER, ScriviGioDocumentLoaderClass))

/* Private structure type */
typedef struct _ScriviGioDocumentLoaderPrivate ScriviGioDocumentLoaderPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviGioDocumentLoader ScriviGioDocumentLoader;

struct _ScriviGioDocumentLoader
{
	ScriviDocumentLoader loader;

	/*< private > */
	ScriviGioDocumentLoaderPrivate *priv;
};

/*
 * Class definition
 */
typedef ScriviDocumentLoaderClass ScriviGioDocumentLoaderClass;

/*
 * Public methods
 */
GType 		 	 scrivi_gio_document_loader_get_type	(void) G_GNUC_CONST;

G_END_DECLS

#endif  /* __SCRIVI_GIO_DOCUMENT_LOADER_H__  */
