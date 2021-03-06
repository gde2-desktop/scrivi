/*
 * scrivi-spell-plugin.h
 * 
 * Copyright (C) 2002-2005 Paolo Maggi 
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
 *
 * $Id$
 */

#ifndef __SCRIVI_SPELL_PLUGIN_H__
#define __SCRIVI_SPELL_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <scrivi/scrivi-plugin.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_SPELL_PLUGIN		(scrivi_spell_plugin_get_type ())
#define SCRIVI_SPELL_PLUGIN(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCRIVI_TYPE_SPELL_PLUGIN, ScriviSpellPlugin))
#define SCRIVI_SPELL_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), SCRIVI_TYPE_SPELL_PLUGIN, ScriviSpellPluginClass))
#define SCRIVI_IS_SPELL_PLUGIN(o)	(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCRIVI_TYPE_SPELL_PLUGIN))
#define SCRIVI_IS_SPELL_PLUGIN_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCRIVI_TYPE_SPELL_PLUGIN))
#define SCRIVI_SPELL_PLUGIN_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCRIVI_TYPE_SPELL_PLUGIN, ScriviSpellPluginClass))

/* Private structure type */
typedef struct _ScriviSpellPluginPrivate	ScriviSpellPluginPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviSpellPlugin	ScriviSpellPlugin;

struct _ScriviSpellPlugin
{
	ScriviPlugin parent_instance;

	ScriviSpellPluginPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviSpellPluginClass	ScriviSpellPluginClass;

struct _ScriviSpellPluginClass
{
	ScriviPluginClass parent_class;
};

/*
 * Public methods
 */
GType	scrivi_spell_plugin_get_type		(void) G_GNUC_CONST;

/* All the plugins must implement this function */
G_MODULE_EXPORT GType register_scrivi_plugin (GTypeModule *module);

G_END_DECLS

#endif /* __SCRIVI_SPELL_PLUGIN_H__ */
