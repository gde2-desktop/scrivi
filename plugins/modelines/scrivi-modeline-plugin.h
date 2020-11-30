/*
 * scrivi-modeline-plugin.h
 * Emacs, Kate and Vim-style modelines support for scrivi.
 * 
 * Copyright (C) 2005-2007 - Steve Frécinaux <code@istique.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __SCRIVI_MODELINE_PLUGIN_H__
#define __SCRIVI_MODELINE_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_MODELINE_PLUGIN		(scrivi_modeline_plugin_get_type ())
#define SCRIVI_MODELINE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_MODELINE_PLUGIN, ScriviModelinePlugin))
#define SCRIVI_MODELINE_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_MODELINE_PLUGIN, ScriviModelinePluginClass))
#define SCRIVI_IS_MODELINE_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_MODELINE_PLUGIN))
#define SCRIVI_IS_MODELINE_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_MODELINE_PLUGIN))
#define SCRIVI_MODELINE_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_MODELINE_PLUGIN, ScriviModelinePluginClass))

/* Private structure type */
typedef ScriviPluginClass	ScriviModelinePluginClass;
typedef ScriviPlugin		ScriviModelinePlugin;

GType	scrivi_modeline_plugin_get_type		(void) G_GNUC_CONST;

G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCRIVI_MODELINE_PLUGIN_H__ */
