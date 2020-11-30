/*
 * scrivi-search-commands.c
 * This file is part of scrivi
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi
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
 * Modified by the scrivi Team, 1998-2006. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 *
 * $Id$
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#if GTK_CHECK_VERSION (3, 0, 0)
#define GTK_OBJECT G_OBJECT
#endif

#include "scrivi-commands.h"
#include "scrivi-debug.h"
#include "scrivi-statusbar.h"
#include "scrivi-window.h"
#include "scrivi-window-private.h"
#include "scrivi-utils.h"
#include "dialogs/scrivi-search-dialog.h"

#define SCRIVI_SEARCH_DIALOG_KEY		"scrivi-search-dialog-key"
#define SCRIVI_LAST_SEARCH_DATA_KEY	"scrivi-last-search-data-key"

typedef struct _LastSearchData LastSearchData;
struct _LastSearchData
{
	gint x;
	gint y;
};

static void
last_search_data_free (LastSearchData *data)
{
	g_slice_free (LastSearchData, data);
}

static void
last_search_data_restore_position (ScriviSearchDialog *dlg)
{
	LastSearchData *data;

	data = g_object_get_data (G_OBJECT (dlg), SCRIVI_LAST_SEARCH_DATA_KEY);

	if (data != NULL)
	{
		gtk_window_move (GTK_WINDOW (dlg),
				 data->x,
				 data->y);
	}
}

static void
last_search_data_store_position (ScriviSearchDialog *dlg)
{
	LastSearchData *data;
	
	data = g_object_get_data (G_OBJECT (dlg), SCRIVI_LAST_SEARCH_DATA_KEY);
	
	if (data == NULL)
	{
		data = g_slice_new (LastSearchData);
		
		g_object_set_data_full (G_OBJECT (dlg),
					SCRIVI_LAST_SEARCH_DATA_KEY,
					data,
					(GDestroyNotify) last_search_data_free);
	}
	
	gtk_window_get_position (GTK_WINDOW (dlg),
				 &data->x,
				 &data->y);
}

/* Use occurrences only for Replace All */
static void
text_found (ScriviWindow *window,
	    gint         occurrences)
{
	if (occurrences > 1)
	{
		scrivi_statusbar_flash_message (SCRIVI_STATUSBAR (window->priv->statusbar),
					       window->priv->generic_message_cid,
					       ngettext("Found and replaced %d occurrence",
					     	        "Found and replaced %d occurrences",
					     	        occurrences),
					       occurrences);
	}
	else
	{
		if (occurrences == 1)
			scrivi_statusbar_flash_message (SCRIVI_STATUSBAR (window->priv->statusbar),
						       window->priv->generic_message_cid,
						       _("Found and replaced one occurrence"));
		else
			scrivi_statusbar_flash_message (SCRIVI_STATUSBAR (window->priv->statusbar),
						       window->priv->generic_message_cid,
						       " ");
	}
}

#define MAX_MSG_LENGTH 40
static void
text_not_found (ScriviWindow *window,
		const gchar *text)
{
	gchar *searched;
	
	searched = scrivi_utils_str_end_truncate (text, MAX_MSG_LENGTH);

	scrivi_statusbar_flash_message (SCRIVI_STATUSBAR (window->priv->statusbar),
				       window->priv->generic_message_cid,
				       /* Translators: %s is replaced by the text
				          entered by the user in the search box */
				       _("\"%s\" not found"), searched);
	g_free (searched);
}

static gboolean
run_search (ScriviView   *view,
	    gboolean     wrap_around,
	    gboolean     search_backwards)
{
	ScriviDocument *doc;
	GtkTextIter start_iter;
	GtkTextIter match_start;
	GtkTextIter match_end;
	gboolean found = FALSE;

	doc = SCRIVI_DOCUMENT (gtk_text_view_get_buffer (GTK_TEXT_VIEW (view)));

	if (!search_backwards)
	{
		gtk_text_buffer_get_selection_bounds (GTK_TEXT_BUFFER (doc),
						      NULL,
						      &start_iter);

		found = scrivi_document_search_forward (doc,
						       &start_iter,
						       NULL,
						       &match_start,
						       &match_end);
	}
	else
	{
		gtk_text_buffer_get_selection_bounds (GTK_TEXT_BUFFER (doc),
						      &start_iter,
						      NULL);

		found = scrivi_document_search_backward (doc,
						        NULL,
						        &start_iter,
						        &match_start,
						        &match_end);
	}

	if (!found && wrap_around)
	{
		if (!search_backwards)
			found = scrivi_document_search_forward (doc,
							       NULL,
							       NULL, /* FIXME: set the end_inter */
							       &match_start,
							       &match_end);
		else
			found = scrivi_document_search_backward (doc,
							        NULL, /* FIXME: set the start_inter */
							        NULL, 
							        &match_start,
							        &match_end);
	}
	
	if (found)
	{
		gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER (doc),
					      &match_start);

		gtk_text_buffer_move_mark_by_name (GTK_TEXT_BUFFER (doc),
						   "selection_bound",
						   &match_end);

		scrivi_view_scroll_to_cursor (view);
	}
	else
	{
		gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER (doc),
					      &start_iter);
	}

	return found;
}

static void
do_find (ScriviSearchDialog *dialog,
	 ScriviWindow       *window)
{
	ScriviView *active_view;
	ScriviDocument *doc;
	gchar *search_text;
	const gchar *entry_text;
	gboolean match_case;
	gboolean entire_word;
	gboolean wrap_around;
	gboolean search_backwards;
	gboolean parse_escapes;
	guint flags = 0;
	guint old_flags = 0;
	gboolean found;

	/* TODO: make the dialog insensitive when all the tabs are closed
	 * and assert here that the view is not NULL */
	active_view = scrivi_window_get_active_view (window);
	if (active_view == NULL)
		return;

	doc = SCRIVI_DOCUMENT (gtk_text_view_get_buffer (GTK_TEXT_VIEW (active_view)));

	match_case = scrivi_search_dialog_get_match_case (dialog);
	entire_word = scrivi_search_dialog_get_entire_word (dialog);
	search_backwards = scrivi_search_dialog_get_backwards (dialog);
	wrap_around = scrivi_search_dialog_get_wrap_around (dialog);
	parse_escapes = scrivi_search_dialog_get_parse_escapes (dialog);

	if (!parse_escapes) {
		entry_text = scrivi_utils_escape_search_text (scrivi_search_dialog_get_search_text (dialog));
	} else {
		entry_text = scrivi_search_dialog_get_search_text (dialog);
	}

	SCRIVI_SEARCH_SET_CASE_SENSITIVE (flags, match_case);
	SCRIVI_SEARCH_SET_ENTIRE_WORD (flags, entire_word);

	search_text = scrivi_document_get_search_text (doc, &old_flags);

	if ((search_text == NULL) ||
	    (strcmp (search_text, entry_text) != 0) ||
	    (flags != old_flags))
	{
		scrivi_document_set_search_text (doc, entry_text, flags);
	}

	g_free (search_text);
	
	found = run_search (active_view,
			    wrap_around,
			    search_backwards);

	if (found)
		text_found (window, 0);
	else {
		if (!parse_escapes) {
			text_not_found (window, scrivi_utils_unescape_search_text (entry_text));
		} else {
			text_not_found (window, entry_text);
		}
	}

	gtk_dialog_set_response_sensitive (GTK_DIALOG (dialog),
					   SCRIVI_SEARCH_DIALOG_REPLACE_RESPONSE,
					   found);
}

/* FIXME: move in scrivi-document.c and share it with scrivi-view */
static gboolean
get_selected_text (GtkTextBuffer  *doc,
		   gchar         **selected_text,
		   gint           *len)
{
	GtkTextIter start, end;

	g_return_val_if_fail (selected_text != NULL, FALSE);
	g_return_val_if_fail (*selected_text == NULL, FALSE);

	if (!gtk_text_buffer_get_selection_bounds (doc, &start, &end))
	{
		if (len != NULL)
			len = 0;

		return FALSE;
	}

	*selected_text = gtk_text_buffer_get_slice (doc, &start, &end, TRUE);

	if (len != NULL)
		*len = g_utf8_strlen (*selected_text, -1);

	return TRUE;
}

static void
replace_selected_text (GtkTextBuffer *buffer,
		       const gchar   *replace)
{
	g_return_if_fail (gtk_text_buffer_get_selection_bounds (buffer, NULL, NULL));
	g_return_if_fail (replace != NULL);

	gtk_text_buffer_begin_user_action (buffer);

	gtk_text_buffer_delete_selection (buffer, FALSE, TRUE);

	gtk_text_buffer_insert_at_cursor (buffer, replace, strlen (replace));

	gtk_text_buffer_end_user_action (buffer);
}

static void
do_replace (ScriviSearchDialog *dialog,
	    ScriviWindow       *window)
{
	ScriviDocument *doc;
	const gchar *search_entry_text;
	const gchar *replace_entry_text;
	gchar *unescaped_search_text;
	gchar *unescaped_replace_text;
	gchar *selected_text = NULL;
	gboolean match_case;
	gboolean parse_escapes;

	doc = scrivi_window_get_active_document (window);
	if (doc == NULL)
		return;

	parse_escapes = scrivi_search_dialog_get_parse_escapes (dialog);
	if (!parse_escapes) {
		search_entry_text = scrivi_utils_escape_search_text (scrivi_search_dialog_get_search_text (dialog));
	} else {
		search_entry_text = scrivi_search_dialog_get_search_text (dialog);
	}
	g_return_if_fail ((search_entry_text) != NULL);
	g_return_if_fail ((*search_entry_text) != '\0');

	/* replace text may be "", we just delete */
	if (!parse_escapes) {
		replace_entry_text = scrivi_utils_escape_search_text (scrivi_search_dialog_get_replace_text (dialog));
	} else {
		replace_entry_text = scrivi_search_dialog_get_replace_text (dialog);
	}
	g_return_if_fail ((replace_entry_text) != NULL);

	unescaped_search_text = scrivi_utils_unescape_search_text (search_entry_text);

	get_selected_text (GTK_TEXT_BUFFER (doc), 
			   &selected_text, 
			   NULL);

	match_case = scrivi_search_dialog_get_match_case (dialog);

	if ((selected_text == NULL) ||
	    (match_case && (strcmp (selected_text, unescaped_search_text) != 0)) || 
	    (!match_case && !g_utf8_caselessnmatch (selected_text,
						    unescaped_search_text, 
						    strlen (selected_text), 
						    strlen (unescaped_search_text)) != 0))
	{
		do_find (dialog, window);
		g_free (unescaped_search_text);
		g_free (selected_text);	

		return;
	}

	unescaped_replace_text = scrivi_utils_unescape_search_text (replace_entry_text);	
	replace_selected_text (GTK_TEXT_BUFFER (doc), unescaped_replace_text);

	g_free (unescaped_search_text);
	g_free (selected_text);
	g_free (unescaped_replace_text);
	
	do_find (dialog, window);
}

static void
do_replace_all (ScriviSearchDialog *dialog,
		ScriviWindow       *window)
{
	ScriviView *active_view;
	ScriviDocument *doc;
	const gchar *search_entry_text;
	const gchar *replace_entry_text;
	gboolean match_case;
	gboolean entire_word;
	gboolean parse_escapes;
	guint flags = 0;
	gint count;

	active_view = scrivi_window_get_active_view (window);
	if (active_view == NULL)
		return;

	doc = SCRIVI_DOCUMENT (gtk_text_view_get_buffer (GTK_TEXT_VIEW (active_view)));

	parse_escapes = scrivi_search_dialog_get_parse_escapes (dialog);
	if (!parse_escapes) {
		search_entry_text = scrivi_utils_escape_search_text(scrivi_search_dialog_get_search_text (dialog));
	} else {
		search_entry_text = scrivi_search_dialog_get_search_text (dialog);
	}
	g_return_if_fail ((search_entry_text) != NULL);
	g_return_if_fail ((*search_entry_text) != '\0');

	/* replace text may be "", we just delete all occurrencies */
	if (!parse_escapes) {
		replace_entry_text = scrivi_utils_escape_search_text (scrivi_search_dialog_get_replace_text (dialog));
	} else {
		replace_entry_text = scrivi_search_dialog_get_replace_text (dialog);
	}
	g_return_if_fail ((replace_entry_text) != NULL);

	match_case = scrivi_search_dialog_get_match_case (dialog);
	entire_word = scrivi_search_dialog_get_entire_word (dialog);

	SCRIVI_SEARCH_SET_CASE_SENSITIVE (flags, match_case);
	SCRIVI_SEARCH_SET_ENTIRE_WORD (flags, entire_word);

	count = scrivi_document_replace_all (doc, 
					    search_entry_text,
					    replace_entry_text,
					    flags);

	if (count > 0)
	{
		text_found (window, count);
	}
	else
	{
		if (!parse_escapes) {
			text_not_found (window, scrivi_utils_unescape_search_text (search_entry_text));
		} else {
			text_not_found (window, search_entry_text);
		}
	}

	gtk_dialog_set_response_sensitive (GTK_DIALOG (dialog),
					   SCRIVI_SEARCH_DIALOG_REPLACE_RESPONSE,
					   FALSE);
}

static void
search_dialog_response_cb (ScriviSearchDialog *dialog,
			   gint               response_id,
			   ScriviWindow       *window)
{
	scrivi_debug (DEBUG_COMMANDS);

	switch (response_id)
	{
		case SCRIVI_SEARCH_DIALOG_FIND_RESPONSE:
			do_find (dialog, window);
			break;
		case SCRIVI_SEARCH_DIALOG_REPLACE_RESPONSE:
			do_replace (dialog, window);
			break;
		case SCRIVI_SEARCH_DIALOG_REPLACE_ALL_RESPONSE:
			do_replace_all (dialog, window);
			break;
		default:
			last_search_data_store_position (dialog);
			gtk_widget_hide (GTK_WIDGET (dialog));
	}
}

static gboolean
search_dialog_delete_event_cb (GtkWidget   *widget,
			       GdkEventAny *event,
			       gpointer     user_data)
{
	scrivi_debug (DEBUG_COMMANDS);

	/* prevent destruction */
	return TRUE;
}

static void
search_dialog_destroyed (ScriviWindow       *window,
			 ScriviSearchDialog *dialog)
{
	scrivi_debug (DEBUG_COMMANDS);

	g_object_set_data (G_OBJECT (window),
			   SCRIVI_SEARCH_DIALOG_KEY,
			   NULL);
	g_object_set_data (G_OBJECT (dialog),
			   SCRIVI_LAST_SEARCH_DATA_KEY,
			   NULL);
}

static GtkWidget *
create_dialog (ScriviWindow *window, gboolean show_replace)
{
	GtkWidget *dialog;

	dialog = scrivi_search_dialog_new (GTK_WINDOW (window), show_replace);

	g_signal_connect (dialog,
			  "response",
			  G_CALLBACK (search_dialog_response_cb),
			  window);
	g_signal_connect (dialog,
			 "delete-event",
			 G_CALLBACK (search_dialog_delete_event_cb),
			 NULL);

	g_object_set_data (G_OBJECT (window),
			   SCRIVI_SEARCH_DIALOG_KEY,
			   dialog);

	g_object_weak_ref (G_OBJECT (dialog),
			   (GWeakNotify) search_dialog_destroyed,
			   window);

	return dialog;
}

void
_scrivi_cmd_search_find (GtkAction   *action,
			ScriviWindow *window)
{
	gpointer data;
	GtkWidget *search_dialog;
	ScriviDocument *doc;
	gboolean selection_exists;
	gboolean parse_escapes;
	gchar *find_text = NULL;
	const gchar *search_text = NULL;
	gint sel_len;

	scrivi_debug (DEBUG_COMMANDS);

	data = g_object_get_data (G_OBJECT (window), SCRIVI_SEARCH_DIALOG_KEY);

	if (data == NULL)
	{
		search_dialog = create_dialog (window, FALSE);
	}
	else
	{
		g_return_if_fail (SCRIVI_IS_SEARCH_DIALOG (data));
		
		search_dialog = GTK_WIDGET (data);
		
		/* turn the dialog into a find dialog if needed */
		if (scrivi_search_dialog_get_show_replace (SCRIVI_SEARCH_DIALOG (search_dialog)))
			scrivi_search_dialog_set_show_replace (SCRIVI_SEARCH_DIALOG (search_dialog),
							      FALSE);
	}

	doc = scrivi_window_get_active_document (window);
	g_return_if_fail (doc != NULL);

	selection_exists = get_selected_text (GTK_TEXT_BUFFER (doc),
					      &find_text,
					      &sel_len);

	if (selection_exists && find_text != NULL && sel_len < 80)
	{
		/*
		 * Special case: if the currently selected text
		 * is the same as the unescaped search text and
		 * escape sequence parsing is activated, use the
		 * same old search text. (Without this, if you e.g.
		 * search for '\n' in escaped mode and then open
		 * the search dialog again, you'll get an unprintable
		 * single-character literal '\n' in the "search for"
		 * box).
		 */
		parse_escapes = scrivi_search_dialog_get_parse_escapes (SCRIVI_SEARCH_DIALOG (search_dialog));
		search_text = scrivi_search_dialog_get_search_text (SCRIVI_SEARCH_DIALOG (search_dialog));
		if (!(search_text != NULL
		      && !strcmp(scrivi_utils_unescape_search_text(search_text), find_text)
		      && parse_escapes)) {
			/* General case */
			scrivi_search_dialog_set_search_text (SCRIVI_SEARCH_DIALOG (search_dialog),
							     find_text);
		}
		g_free (find_text);
	}
	else
	{
		g_free (find_text);
	}

	gtk_widget_show (search_dialog);
	last_search_data_restore_position (SCRIVI_SEARCH_DIALOG (search_dialog));
	scrivi_search_dialog_present_with_time (SCRIVI_SEARCH_DIALOG (search_dialog),
					       GDK_CURRENT_TIME);
}

void
_scrivi_cmd_search_replace (GtkAction   *action,
			   ScriviWindow *window)
{
	gpointer data;
	GtkWidget *replace_dialog;
	ScriviDocument *doc;
	gboolean selection_exists;
	gboolean parse_escapes;
	gchar *find_text = NULL;
	const gchar *search_text = NULL;
	gint sel_len;

	scrivi_debug (DEBUG_COMMANDS);

	data = g_object_get_data (G_OBJECT (window), SCRIVI_SEARCH_DIALOG_KEY);

	if (data == NULL)
	{
		replace_dialog = create_dialog (window, TRUE);
	}
	else
	{
		g_return_if_fail (SCRIVI_IS_SEARCH_DIALOG (data));
		
		replace_dialog = GTK_WIDGET (data);
		
		/* turn the dialog into a find dialog if needed */
		if (!scrivi_search_dialog_get_show_replace (SCRIVI_SEARCH_DIALOG (replace_dialog)))
			scrivi_search_dialog_set_show_replace (SCRIVI_SEARCH_DIALOG (replace_dialog),
							      TRUE);
	}

	doc = scrivi_window_get_active_document (window);
	g_return_if_fail (doc != NULL);

	selection_exists = get_selected_text (GTK_TEXT_BUFFER (doc),
					      &find_text,
					      &sel_len);

	if (selection_exists && find_text != NULL && sel_len < 80)
	{
		/*
		 * Special case: if the currently selected text
		 * is the same as the unescaped search text and
		 * escape sequence parsing is activated, use the
		 * same old search text. (Without this, if you e.g.
		 * search for '\n' in escaped mode and then open
		 * the search dialog again, you'll get an unprintable
		 * single-character literal '\n' in the "search for"
		 * box).
		 */
		parse_escapes = scrivi_search_dialog_get_parse_escapes (SCRIVI_SEARCH_DIALOG (replace_dialog));
		search_text = scrivi_search_dialog_get_search_text (SCRIVI_SEARCH_DIALOG (replace_dialog));
		if (!(search_text != NULL
		      && !strcmp(scrivi_utils_unescape_search_text(search_text), find_text)
		      && parse_escapes)) {
			/* General case */
			scrivi_search_dialog_set_search_text (SCRIVI_SEARCH_DIALOG (replace_dialog),
							     find_text);
		}
		g_free (find_text);
	}
	else
	{
		g_free (find_text);
	}

	gtk_widget_show (replace_dialog);
	last_search_data_restore_position (SCRIVI_SEARCH_DIALOG (replace_dialog));
	scrivi_search_dialog_present_with_time (SCRIVI_SEARCH_DIALOG (replace_dialog),
					       GDK_CURRENT_TIME);
}

static void
do_find_again (ScriviWindow *window,
	       gboolean     backward)
{
	ScriviView *active_view;
	gboolean wrap_around = TRUE;
	gpointer data;
	
	active_view = scrivi_window_get_active_view (window);
	g_return_if_fail (active_view != NULL);

	data = g_object_get_data (G_OBJECT (window), SCRIVI_SEARCH_DIALOG_KEY);
	
	if (data != NULL)
		wrap_around = scrivi_search_dialog_get_wrap_around (SCRIVI_SEARCH_DIALOG (data));
	
	run_search (active_view,
		    wrap_around,
		    backward);
}

void
_scrivi_cmd_search_find_next (GtkAction   *action,
			     ScriviWindow *window)
{
	scrivi_debug (DEBUG_COMMANDS);

	do_find_again (window, FALSE);
}

void
_scrivi_cmd_search_find_prev (GtkAction   *action,
			     ScriviWindow *window)
{
	scrivi_debug (DEBUG_COMMANDS);

	do_find_again (window, TRUE);
}

void
_scrivi_cmd_search_clear_highlight (GtkAction   *action,
				   ScriviWindow *window)
{
	ScriviDocument *doc;

	scrivi_debug (DEBUG_COMMANDS);

	doc = scrivi_window_get_active_document (window);
	scrivi_document_set_search_text (SCRIVI_DOCUMENT (doc),
					"",
					SCRIVI_SEARCH_DONT_SET_FLAGS);
}

void
_scrivi_cmd_search_goto_line (GtkAction   *action,
			     ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	if (active_view == NULL)
		return;

	/* Focus the view if needed: we need to focus the view otherwise 
	   activating the binding for goto line has no effect */
	gtk_widget_grab_focus (GTK_WIDGET (active_view));


	/* goto line is builtin in ScriviView, just activate
	 * the corrisponding binding.
	 */
	gtk_bindings_activate (GTK_OBJECT (active_view),
			       GDK_KEY_i,
			       GDK_CONTROL_MASK);
}

void
_scrivi_cmd_search_incremental_search (GtkAction   *action,
				      ScriviWindow *window)
{
	ScriviView *active_view;

	scrivi_debug (DEBUG_COMMANDS);

	active_view = scrivi_window_get_active_view (window);
	if (active_view == NULL)
		return;

	/* Focus the view if needed: we need to focus the view otherwise 
	   activating the binding for incremental search has no effect */
	gtk_widget_grab_focus (GTK_WIDGET (active_view));
	
	/* incremental search is builtin in ScriviView, just activate
	 * the corrisponding binding.
	 */
	gtk_bindings_activate (GTK_OBJECT (active_view),
			       GDK_KEY_k,
			       GDK_CONTROL_MASK);
}
