/* vim: set sw=8: -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * scrivi-spell-checker-dialog.h
 * This file is part of scrivi
 *
 * Copyright (C) 2002 Paolo Maggi 
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

#ifndef __SCRIVI_SPELL_CHECKER_DIALOG_H__
#define __SCRIVI_SPELL_CHECKER_DIALOG_H__

#include <gtk/gtk.h>
#include "scrivi-spell-checker.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_SPELL_CHECKER_DIALOG            (scrivi_spell_checker_dialog_get_type ())
#define SCRIVI_SPELL_CHECKER_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_SPELL_CHECKER_DIALOG, ScriviSpellCheckerDialog))
#define SCRIVI_SPELL_CHECKER_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_SPELL_CHECKER_DIALOG, ScriviSpellCheckerDialog))
#define SCRIVI_IS_SPELL_CHECKER_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_SPELL_CHECKER_DIALOG))
#define SCRIVI_IS_SPELL_CHECKER_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_SPELL_CHECKER_DIALOG))
#define SCRIVI_SPELL_CHECKER_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_SPELL_CHECKER_DIALOG, ScriviSpellCheckerDialog))


typedef struct _ScriviSpellCheckerDialog ScriviSpellCheckerDialog;

typedef struct _ScriviSpellCheckerDialogClass ScriviSpellCheckerDialogClass;

struct _ScriviSpellCheckerDialogClass 
{
	GtkWindowClass parent_class;

	/* Signals */
	void		(*ignore)		(ScriviSpellCheckerDialog *dlg,
						 const gchar *word);
	void		(*ignore_all)		(ScriviSpellCheckerDialog *dlg,
						 const gchar *word);
	void		(*change)		(ScriviSpellCheckerDialog *dlg,
						 const gchar *word, 
						 const gchar *change_to);
	void		(*change_all)		(ScriviSpellCheckerDialog *dlg,
						 const gchar *word, 
						 const gchar *change_to);
	void		(*add_word_to_personal)	(ScriviSpellCheckerDialog *dlg,
						 const gchar *word);

};

GType        		 scrivi_spell_checker_dialog_get_type	(void) G_GNUC_CONST;

/* Constructors */
GtkWidget		*scrivi_spell_checker_dialog_new		(const gchar *data_dir);
GtkWidget		*scrivi_spell_checker_dialog_new_from_spell_checker 
								(ScriviSpellChecker *spell,
								 const gchar *data_dir);

void 			 scrivi_spell_checker_dialog_set_spell_checker
								(ScriviSpellCheckerDialog *dlg,
								 ScriviSpellChecker *spell);
void			 scrivi_spell_checker_dialog_set_misspelled_word 
								(ScriviSpellCheckerDialog *dlg, 
								 const gchar* word, 
								 gint len);

void 			 scrivi_spell_checker_dialog_set_completed 
								(ScriviSpellCheckerDialog *dlg);
								
G_END_DECLS

#endif  /* __SCRIVI_SPELL_CHECKER_DIALOG_H__ */

