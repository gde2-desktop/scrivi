/*
 * scrivi-gio-document-saver.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyrhing (C) 2007 - Paolo Maggi, Steve Frécinaux
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
 * Modified by the scrivi Team, 2005-2007. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 */

#ifndef __SCRIVI_GIO_DOCUMENT_SAVER_H__
#define __SCRIVI_GIO_DOCUMENT_SAVER_H__

#include <scrivi/scrivi-document-saver.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_GIO_DOCUMENT_SAVER              (scrivi_gio_document_saver_get_type())
#define SCRIVI_GIO_DOCUMENT_SAVER(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_GIO_DOCUMENT_SAVER, ScriviGioDocumentSaver))
#define SCRIVI_GIO_DOCUMENT_SAVER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_GIO_DOCUMENT_SAVER, ScriviGioDocumentSaverClass))
#define SCRIVI_IS_GIO_DOCUMENT_SAVER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_GIO_DOCUMENT_SAVER))
#define SCRIVI_IS_GIO_DOCUMENT_SAVER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_GIO_DOCUMENT_SAVER))
#define SCRIVI_GIO_DOCUMENT_SAVER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_GIO_DOCUMENT_SAVER, ScriviGioDocumentSaverClass))

/* Private structure type */
typedef struct _ScriviGioDocumentSaverPrivate ScriviGioDocumentSaverPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviGioDocumentSaver ScriviGioDocumentSaver;

struct _ScriviGioDocumentSaver 
{
	ScriviDocumentSaver saver;

	/*< private > */
	ScriviGioDocumentSaverPrivate *priv;
};

/*
 * Class definition
 */
typedef ScriviDocumentSaverClass ScriviGioDocumentSaverClass;

/*
 * Public methods
 */
GType 		 	 scrivi_gio_document_saver_get_type	(void) G_GNUC_CONST;

G_END_DECLS

#endif  /* __SCRIVI_GIO_DOCUMENT_SAVER_H__  */
