/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * scrivi-spell-checker.h
 * This file is part of scrivi
 *
 * Copyright (C) 2002-2006 Paolo Maggi
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
 * Modified by the scrivi Team, 2002. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 */

#ifndef __SCRIVI_SPELL_CHECKER_H__
#define __SCRIVI_SPELL_CHECKER_H__

#include <glib.h>
#include <glib-object.h>

#include "scrivi-spell-checker-language.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_SPELL_CHECKER            (scrivi_spell_checker_get_type ())
#define SCRIVI_SPELL_CHECKER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_SPELL_CHECKER, ScriviSpellChecker))
#define SCRIVI_SPELL_CHECKER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_SPELL_CHECKER, ScriviSpellChecker))
#define SCRIVI_IS_SPELL_CHECKER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_SPELL_CHECKER))
#define SCRIVI_IS_SPELL_CHECKER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_SPELL_CHECKER))
#define SCRIVI_SPELL_CHECKER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_SPELL_CHECKER, ScriviSpellChecker))

typedef struct _ScriviSpellChecker ScriviSpellChecker;

typedef struct _ScriviSpellCheckerClass ScriviSpellCheckerClass;

struct _ScriviSpellCheckerClass
{
	GObjectClass parent_class;

	/* Signals */
	void (*add_word_to_personal) (ScriviSpellChecker               *spell,
				      const gchar                     *word,
				      gint                             len);

	void (*add_word_to_session)  (ScriviSpellChecker               *spell,
				      const gchar                     *word,
				      gint                             len);

	void (*set_language)         (ScriviSpellChecker               *spell,
				      const ScriviSpellCheckerLanguage *lang);

	void (*clear_session)	     (ScriviSpellChecker               *spell);
};


GType        		 scrivi_spell_checker_get_type		(void) G_GNUC_CONST;

/* Constructors */
ScriviSpellChecker	*scrivi_spell_checker_new		(void);

gboolean		 scrivi_spell_checker_set_language 	(ScriviSpellChecker               *spell,
								 const ScriviSpellCheckerLanguage *lang);
const ScriviSpellCheckerLanguage
			*scrivi_spell_checker_get_language 	(ScriviSpellChecker               *spell);

gboolean		 scrivi_spell_checker_check_word 	(ScriviSpellChecker               *spell,
								 const gchar                     *word,
								 gssize                           len);

GSList 			*scrivi_spell_checker_get_suggestions 	(ScriviSpellChecker               *spell,
								 const gchar                     *word,
								 gssize                           len);

gboolean		 scrivi_spell_checker_add_word_to_personal
								(ScriviSpellChecker               *spell,
								 const gchar                     *word,
								 gssize                           len);

gboolean		 scrivi_spell_checker_add_word_to_session
								(ScriviSpellChecker               *spell,
								 const gchar                     *word,
								 gssize                           len);

gboolean		 scrivi_spell_checker_clear_session 	(ScriviSpellChecker               *spell);

gboolean		 scrivi_spell_checker_set_correction 	(ScriviSpellChecker               *spell,
								 const gchar                     *word,
								 gssize                           w_len,
								 const gchar                     *replacement,
								 gssize                           r_len);
G_END_DECLS

#endif  /* __SCRIVI_SPELL_CHECKER_H__ */

