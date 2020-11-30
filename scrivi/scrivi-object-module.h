/*
 * scrivi-object-module.h
 * This file is part of scrivi
 *
 * Copyright (C) 2005 - Paolo Maggi
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
 
/* This is a modified version of scrivi-module.h from Epiphany source code.
 * Here the original copyright assignment:
 *
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
 *
 */

/*
 * Modified by the scrivi Team, 2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id: scrivi-module.h 6263 2008-05-05 10:52:10Z sfre $
 */
 
#ifndef __SCRIVI_OBJECT_MODULE_H__
#define __SCRIVI_OBJECT_MODULE_H__

#include <glib-object.h>
#include <gmodule.h>
#include <stdarg.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_OBJECT_MODULE		(scrivi_object_module_get_type ())
#define SCRIVI_OBJECT_MODULE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_OBJECT_MODULE, ScriviObjectModule))
#define SCRIVI_OBJECT_MODULE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_OBJECT_MODULE, ScriviObjectModuleClass))
#define SCRIVI_IS_OBJECT_MODULE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_OBJECT_MODULE))
#define SCRIVI_IS_OBJECT_MODULE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_OBJECT_MODULE))
#define SCRIVI_OBJECT_MODULE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_OBJECT_MODULE, ScriviObjectModuleClass))

typedef struct _ScriviObjectModule 		ScriviObjectModule;
typedef struct _ScriviObjectModulePrivate	ScriviObjectModulePrivate;

struct _ScriviObjectModule
{
	GTypeModule parent;

	ScriviObjectModulePrivate *priv;
};

typedef struct _ScriviObjectModuleClass ScriviObjectModuleClass;

struct _ScriviObjectModuleClass
{
	GTypeModuleClass parent_class;

	/* Virtual class methods */
	void		 (* garbage_collect)	();
};

GType		 scrivi_object_module_get_type			(void) G_GNUC_CONST;

ScriviObjectModule *scrivi_object_module_new			(const gchar *module_name,
								 const gchar *path,
								 const gchar *type_registration,
								 gboolean     resident);

GObject		*scrivi_object_module_new_object			(ScriviObjectModule *module, 
								 const gchar	   *first_property_name,
								 ...);

GType		 scrivi_object_module_get_object_type		(ScriviObjectModule *module);
const gchar	*scrivi_object_module_get_path			(ScriviObjectModule *module);
const gchar	*scrivi_object_module_get_module_name		(ScriviObjectModule *module);
const gchar 	*scrivi_object_module_get_type_registration 	(ScriviObjectModule *module);

G_END_DECLS

#endif
