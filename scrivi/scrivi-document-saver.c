/*
 * scrivi-document-saver.c
 * This file is part of scrivi
 *
 * Copyright (C) 2005-2006 - Paolo Borelli and Paolo Maggi
 * Copyright (C) 2007 - Paolo Borelli, Paolo Maggi, Steve Frécinaux
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
 * Modified by the scrivi Team, 2005-2006. See the AUTHORS file for a
 * list of people on the scrivi Team.
 * See the ChangeLog files for a list of changes.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <glib/gi18n.h>

#include "scrivi-document-saver.h"
#include "scrivi-debug.h"
#include "scrivi-prefs-manager.h"
#include "scrivi-marshal.h"
#include "scrivi-utils.h"
#include "scrivi-enum-types.h"
#include "scrivi-gio-document-saver.h"

G_DEFINE_ABSTRACT_TYPE(ScriviDocumentSaver, scrivi_document_saver, G_TYPE_OBJECT)

/* Signals */

enum {
	SAVING,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

/* Properties */

enum {
	PROP_0,
	PROP_DOCUMENT,
	PROP_URI,
	PROP_ENCODING,
	PROP_NEWLINE_TYPE,
	PROP_FLAGS
};

static void
scrivi_document_saver_set_property (GObject      *object,
				   guint         prop_id,
				   const GValue *value,
				   GParamSpec   *pspec)
{
	ScriviDocumentSaver *saver = SCRIVI_DOCUMENT_SAVER (object);

	switch (prop_id)
	{
		case PROP_DOCUMENT:
			g_return_if_fail (saver->document == NULL);
			saver->document = g_value_get_object (value);
			break;
		case PROP_URI:
			g_return_if_fail (saver->uri == NULL);
			saver->uri = g_value_dup_string (value);
			break;
		case PROP_ENCODING:
			g_return_if_fail (saver->encoding == NULL);
			saver->encoding = g_value_get_boxed (value);
			break;
		case PROP_NEWLINE_TYPE:
			saver->newline_type = g_value_get_enum (value);
			break;
		case PROP_FLAGS:
			saver->flags = g_value_get_flags (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
scrivi_document_saver_get_property (GObject    *object,
				   guint       prop_id,
				   GValue     *value,
				   GParamSpec *pspec)
{
	ScriviDocumentSaver *saver = SCRIVI_DOCUMENT_SAVER (object);

	switch (prop_id)
	{
		case PROP_DOCUMENT:
			g_value_set_object (value, saver->document);
			break;
		case PROP_URI:
			g_value_set_string (value, saver->uri);
			break;
		case PROP_ENCODING:
			g_value_set_boxed (value, saver->encoding);
			break;
		case PROP_NEWLINE_TYPE:
			g_value_set_enum (value, saver->newline_type);
			break;
		case PROP_FLAGS:
			g_value_set_flags (value, saver->flags);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
scrivi_document_saver_finalize (GObject *object)
{
	ScriviDocumentSaver *saver = SCRIVI_DOCUMENT_SAVER (object);

	g_free (saver->uri);

	G_OBJECT_CLASS (scrivi_document_saver_parent_class)->finalize (object);
}

static void
scrivi_document_saver_dispose (GObject *object)
{
	ScriviDocumentSaver *saver = SCRIVI_DOCUMENT_SAVER (object);

	if (saver->info != NULL)
	{
		g_object_unref (saver->info);
		saver->info = NULL;
	}

	G_OBJECT_CLASS (scrivi_document_saver_parent_class)->dispose (object);
}

static void 
scrivi_document_saver_class_init (ScriviDocumentSaverClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = scrivi_document_saver_finalize;
	object_class->dispose = scrivi_document_saver_dispose;
	object_class->set_property = scrivi_document_saver_set_property;
	object_class->get_property = scrivi_document_saver_get_property;

	g_object_class_install_property (object_class,
					 PROP_DOCUMENT,
					 g_param_spec_object ("document",
							      "Document",
							      "The ScriviDocument this ScriviDocumentSaver is associated with",
							      SCRIVI_TYPE_DOCUMENT,
							      G_PARAM_READWRITE |
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
					 PROP_URI,
					 g_param_spec_string ("uri",
							      "URI",
							      "The URI this ScriviDocumentSaver saves the document to",
							      "",
							      G_PARAM_READWRITE |
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
					 PROP_ENCODING,
					 g_param_spec_boxed ("encoding",
							     "URI",
							     "The encoding of the saved file",
							     SCRIVI_TYPE_ENCODING,
							     G_PARAM_READWRITE |
							     G_PARAM_CONSTRUCT_ONLY |
							     G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
					 PROP_NEWLINE_TYPE,
					 g_param_spec_enum ("newline-type",
					                    "Newline type",
					                    "The accepted types of line ending",
					                    SCRIVI_TYPE_DOCUMENT_NEWLINE_TYPE,
					                    SCRIVI_DOCUMENT_NEWLINE_TYPE_LF,
					                    G_PARAM_READWRITE |
					                    G_PARAM_STATIC_NAME |
					                    G_PARAM_STATIC_BLURB |
					                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
					 PROP_FLAGS,
					 g_param_spec_flags ("flags",
							     "Flags",
							     "The flags for the saving operation",
							     SCRIVI_TYPE_DOCUMENT_SAVE_FLAGS,
							     0,
							     G_PARAM_READWRITE |
							     G_PARAM_CONSTRUCT_ONLY));

	signals[SAVING] =
		g_signal_new ("saving",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviDocumentSaverClass, saving),
			      NULL, NULL,
			      scrivi_marshal_VOID__BOOLEAN_POINTER,
			      G_TYPE_NONE,
			      2,
			      G_TYPE_BOOLEAN,
			      G_TYPE_POINTER);
}

static void
scrivi_document_saver_init (ScriviDocumentSaver *saver)
{
	saver->used = FALSE;
}

ScriviDocumentSaver *
scrivi_document_saver_new (ScriviDocument           *doc,
			  const gchar             *uri,
			  const ScriviEncoding     *encoding,
			  ScriviDocumentNewlineType newline_type,
			  ScriviDocumentSaveFlags   flags)
{
	ScriviDocumentSaver *saver;
	GType saver_type;

	g_return_val_if_fail (SCRIVI_IS_DOCUMENT (doc), NULL);

	saver_type = SCRIVI_TYPE_GIO_DOCUMENT_SAVER;

	if (encoding == NULL)
		encoding = scrivi_encoding_get_utf8 ();

	saver = SCRIVI_DOCUMENT_SAVER (g_object_new (saver_type,
						    "document", doc,
						    "uri", uri,
						    "encoding", encoding,
						    "newline_type", newline_type,
						    "flags", flags,
						    NULL));

	return saver;
}

void
scrivi_document_saver_saving (ScriviDocumentSaver *saver,
			     gboolean            completed,
			     GError             *error)
{
	/* the object will be unrefed in the callback of the saving
	 * signal, so we need to prevent finalization.
	 */
	if (completed)
	{
		g_object_ref (saver);
	}

	g_signal_emit (saver, signals[SAVING], 0, completed, error);

	if (completed)
	{
		if (error == NULL)
			scrivi_debug_message (DEBUG_SAVER, "save completed");
		else
			scrivi_debug_message (DEBUG_SAVER, "save failed");

		g_object_unref (saver);
	}
}

void
scrivi_document_saver_save (ScriviDocumentSaver     *saver,
			   GTimeVal               *old_mtime)
{
	scrivi_debug (DEBUG_SAVER);

	g_return_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver));
	g_return_if_fail (saver->uri != NULL && strlen (saver->uri) > 0);

	g_return_if_fail (saver->used == FALSE);
	saver->used = TRUE;

	// CHECK:
	// - sanity check a max len for the uri?
	// report async (in an idle handler) or sync (bool ret)
	// async is extra work here, sync is special casing in the caller

	/* never keep backup of autosaves */
	if ((saver->flags & SCRIVI_DOCUMENT_SAVE_PRESERVE_BACKUP) != 0)
		saver->keep_backup = FALSE;
	else
		saver->keep_backup = scrivi_prefs_manager_get_create_backup_copy ();

	SCRIVI_DOCUMENT_SAVER_GET_CLASS (saver)->save (saver, old_mtime);
}

ScriviDocument *
scrivi_document_saver_get_document (ScriviDocumentSaver *saver)
{
	g_return_val_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver), NULL);

	return saver->document;
}

const gchar *
scrivi_document_saver_get_uri (ScriviDocumentSaver *saver)
{
	g_return_val_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver), NULL);

	return saver->uri;
}

/* Returns 0 if file size is unknown */
goffset
scrivi_document_saver_get_file_size (ScriviDocumentSaver *saver)
{
	g_return_val_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver), 0);

	return SCRIVI_DOCUMENT_SAVER_GET_CLASS (saver)->get_file_size (saver);
}

goffset
scrivi_document_saver_get_bytes_written (ScriviDocumentSaver *saver)
{
	g_return_val_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver), 0);

	return SCRIVI_DOCUMENT_SAVER_GET_CLASS (saver)->get_bytes_written (saver);
}

GFileInfo *
scrivi_document_saver_get_info (ScriviDocumentSaver *saver)
{
	g_return_val_if_fail (SCRIVI_IS_DOCUMENT_SAVER (saver), NULL);

	return saver->info;
}
