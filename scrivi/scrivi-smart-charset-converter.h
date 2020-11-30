/*
 * scrivi-smart-charset-converter.h
 * This file is part of scrivi
 *
 * Copyright (C) 2009 - Ignacio Casal Quinteiro
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

#ifndef __SCRIVI_SMART_CHARSET_CONVERTER_H__
#define __SCRIVI_SMART_CHARSET_CONVERTER_H__

#include <glib-object.h>

#include "scrivi-encodings.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_SMART_CHARSET_CONVERTER		(scrivi_smart_charset_converter_get_type ())
#define SCRIVI_SMART_CHARSET_CONVERTER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_SMART_CHARSET_CONVERTER, ScriviSmartCharsetConverter))
#define SCRIVI_SMART_CHARSET_CONVERTER_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_SMART_CHARSET_CONVERTER, ScriviSmartCharsetConverter const))
#define SCRIVI_SMART_CHARSET_CONVERTER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_SMART_CHARSET_CONVERTER, ScriviSmartCharsetConverterClass))
#define SCRIVI_IS_SMART_CHARSET_CONVERTER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_SMART_CHARSET_CONVERTER))
#define SCRIVI_IS_SMART_CHARSET_CONVERTER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_SMART_CHARSET_CONVERTER))
#define SCRIVI_SMART_CHARSET_CONVERTER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_SMART_CHARSET_CONVERTER, ScriviSmartCharsetConverterClass))

typedef struct _ScriviSmartCharsetConverter		ScriviSmartCharsetConverter;
typedef struct _ScriviSmartCharsetConverterClass		ScriviSmartCharsetConverterClass;
typedef struct _ScriviSmartCharsetConverterPrivate	ScriviSmartCharsetConverterPrivate;

struct _ScriviSmartCharsetConverter
{
	GObject parent;
	
	ScriviSmartCharsetConverterPrivate *priv;
};

struct _ScriviSmartCharsetConverterClass
{
	GObjectClass parent_class;
};

GType scrivi_smart_charset_converter_get_type (void) G_GNUC_CONST;

ScriviSmartCharsetConverter	*scrivi_smart_charset_converter_new		(GSList *candidate_encodings);

const ScriviEncoding		*scrivi_smart_charset_converter_get_guessed	(ScriviSmartCharsetConverter *smart);

guint				 scrivi_smart_charset_converter_get_num_fallbacks(ScriviSmartCharsetConverter *smart);

G_END_DECLS

#endif /* __SCRIVI_SMART_CHARSET_CONVERTER_H__ */
