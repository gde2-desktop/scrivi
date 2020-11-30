/*
 * scrivi-plugin-info.h
 * This file is part of scrivi
 *
 * Copyright (C) 2002-2005 - Paolo Maggi 
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
 * Modified by the scrivi Team, 2002-2007. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifndef __SCRIVI_PLUGIN_INFO_H__
#define __SCRIVI_PLUGIN_INFO_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_PLUGIN_INFO			(scrivi_plugin_info_get_type ())
#define SCRIVI_PLUGIN_INFO(obj)			((ScriviPluginInfo *) (obj))

typedef struct _ScriviPluginInfo			ScriviPluginInfo;

GType		 scrivi_plugin_info_get_type		(void) G_GNUC_CONST;

gboolean 	 scrivi_plugin_info_is_active		(ScriviPluginInfo *info);
gboolean 	 scrivi_plugin_info_is_available		(ScriviPluginInfo *info);
gboolean	 scrivi_plugin_info_is_configurable	(ScriviPluginInfo *info);

const gchar	*scrivi_plugin_info_get_module_name	(ScriviPluginInfo *info);

const gchar	*scrivi_plugin_info_get_name		(ScriviPluginInfo *info);
const gchar	*scrivi_plugin_info_get_description	(ScriviPluginInfo *info);
const gchar	*scrivi_plugin_info_get_icon_name	(ScriviPluginInfo *info);
const gchar    **scrivi_plugin_info_get_authors		(ScriviPluginInfo *info);
const gchar	*scrivi_plugin_info_get_website		(ScriviPluginInfo *info);
const gchar	*scrivi_plugin_info_get_copyright	(ScriviPluginInfo *info);
const gchar	*scrivi_plugin_info_get_version		(ScriviPluginInfo *info);

G_END_DECLS

#endif /* __SCRIVI_PLUGIN_INFO_H__ */

