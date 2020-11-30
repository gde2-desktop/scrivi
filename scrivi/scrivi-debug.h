/*
 * scrivi-debug.h
 * This file is part of scrivi
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002 - 2005 Paolo Maggi  
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
 * Modified by the scrivi Team, 1998-2005. See the AUTHORS file for a 
 * list of people on the scrivi Team.  
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifndef __SCRIVI_DEBUG_H__
#define __SCRIVI_DEBUG_H__

#include <glib.h>

/*
 * Set an environmental var of the same name to turn on
 * debugging output. Setting SCRIVI_DEBUG will turn on all
 * sections.
 */
typedef enum {
	SCRIVI_NO_DEBUG       = 0,
	SCRIVI_DEBUG_VIEW     = 1 << 0,
	SCRIVI_DEBUG_SEARCH   = 1 << 1,
	SCRIVI_DEBUG_PRINT    = 1 << 2,
	SCRIVI_DEBUG_PREFS    = 1 << 3,
	SCRIVI_DEBUG_PLUGINS  = 1 << 4,
	SCRIVI_DEBUG_TAB      = 1 << 5,
	SCRIVI_DEBUG_DOCUMENT = 1 << 6,
	SCRIVI_DEBUG_COMMANDS = 1 << 7,
	SCRIVI_DEBUG_APP      = 1 << 8,
	SCRIVI_DEBUG_SESSION  = 1 << 9,
	SCRIVI_DEBUG_UTILS    = 1 << 10,
	SCRIVI_DEBUG_METADATA = 1 << 11,
	SCRIVI_DEBUG_WINDOW   = 1 << 12,
	SCRIVI_DEBUG_LOADER   = 1 << 13,
	SCRIVI_DEBUG_SAVER    = 1 << 14
} ScriviDebugSection;


/* FIXME this is an issue for introspection */
#define	DEBUG_VIEW	SCRIVI_DEBUG_VIEW,    __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_SEARCH	SCRIVI_DEBUG_SEARCH,  __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PRINT	SCRIVI_DEBUG_PRINT,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PREFS	SCRIVI_DEBUG_PREFS,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_PLUGINS	SCRIVI_DEBUG_PLUGINS, __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_TAB	SCRIVI_DEBUG_TAB,     __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_DOCUMENT	SCRIVI_DEBUG_DOCUMENT,__FILE__, __LINE__, G_STRFUNC
#define	DEBUG_COMMANDS	SCRIVI_DEBUG_COMMANDS,__FILE__, __LINE__, G_STRFUNC
#define	DEBUG_APP	SCRIVI_DEBUG_APP,     __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_SESSION	SCRIVI_DEBUG_SESSION, __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_UTILS	SCRIVI_DEBUG_UTILS,   __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_METADATA	SCRIVI_DEBUG_METADATA,__FILE__, __LINE__, G_STRFUNC
#define	DEBUG_WINDOW	SCRIVI_DEBUG_WINDOW,  __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_LOADER	SCRIVI_DEBUG_LOADER,  __FILE__, __LINE__, G_STRFUNC
#define	DEBUG_SAVER	SCRIVI_DEBUG_SAVER,   __FILE__, __LINE__, G_STRFUNC

void scrivi_debug_init (void);

void scrivi_debug (ScriviDebugSection  section,
		  const gchar       *file,
		  gint               line,
		  const gchar       *function);

void scrivi_debug_message (ScriviDebugSection  section,
			  const gchar       *file,
			  gint               line,
			  const gchar       *function,
			  const gchar       *format, ...) G_GNUC_PRINTF(5, 6);


#endif /* __SCRIVI_DEBUG_H__ */
