/*
 * scrivi-document.h
 * This file is part of scrivi
 *
 * Copyright (C) 1998, 1999 Alex Roberts, Evan Lawrence
 * Copyright (C) 2000, 2001 Chema Celorio, Paolo Maggi 
 * Copyright (C) 2002-2005 Paolo Maggi 
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
 
#ifndef __SCRIVI_DOCUMENT_H__
#define __SCRIVI_DOCUMENT_H__

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksourcebuffer.h>

#include <scrivi/scrivi-encodings.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_DOCUMENT              (scrivi_document_get_type())
#define SCRIVI_DOCUMENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_DOCUMENT, ScriviDocument))
#define SCRIVI_DOCUMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_DOCUMENT, ScriviDocumentClass))
#define SCRIVI_IS_DOCUMENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_DOCUMENT))
#define SCRIVI_IS_DOCUMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_DOCUMENT))
#define SCRIVI_DOCUMENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_DOCUMENT, ScriviDocumentClass))

#define SCRIVI_METADATA_ATTRIBUTE_POSITION "metadata::scrivi-position"
#define SCRIVI_METADATA_ATTRIBUTE_ENCODING "metadata::scrivi-encoding"
#define SCRIVI_METADATA_ATTRIBUTE_LANGUAGE "metadata::scrivi-language"

typedef enum
{
	SCRIVI_DOCUMENT_NEWLINE_TYPE_LF,
	SCRIVI_DOCUMENT_NEWLINE_TYPE_CR,
	SCRIVI_DOCUMENT_NEWLINE_TYPE_CR_LF
} ScriviDocumentNewlineType;

#define SCRIVI_DOCUMENT_NEWLINE_TYPE_DEFAULT SCRIVI_DOCUMENT_NEWLINE_TYPE_LF

typedef enum
{
	SCRIVI_SEARCH_DONT_SET_FLAGS	= 1 << 0, 
	SCRIVI_SEARCH_ENTIRE_WORD	= 1 << 1,
	SCRIVI_SEARCH_CASE_SENSITIVE	= 1 << 2,
	SCRIVI_SEARCH_PARSE_ESCAPES	= 1 << 3

} ScriviSearchFlags;

/**
 * ScriviDocumentSaveFlags:
 * @SCRIVI_DOCUMENT_SAVE_IGNORE_MTIME: save file despite external modifications.
 * @SCRIVI_DOCUMENT_SAVE_IGNORE_BACKUP: write the file directly without attempting to backup.
 * @SCRIVI_DOCUMENT_SAVE_PRESERVE_BACKUP: preserve previous backup file, needed to support autosaving.
 */
typedef enum
{
	SCRIVI_DOCUMENT_SAVE_IGNORE_MTIME 	= 1 << 0,
	SCRIVI_DOCUMENT_SAVE_IGNORE_BACKUP	= 1 << 1,
	SCRIVI_DOCUMENT_SAVE_PRESERVE_BACKUP	= 1 << 2
} ScriviDocumentSaveFlags;

/* Private structure type */
typedef struct _ScriviDocumentPrivate    ScriviDocumentPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviDocument           ScriviDocument;
 
struct _ScriviDocument
{
	GtkSourceBuffer buffer;
	
	/*< private > */
	ScriviDocumentPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviDocumentClass 	ScriviDocumentClass;

struct _ScriviDocumentClass
{
	GtkSourceBufferClass parent_class;

	/* Signals */ // CHECK: ancora da rivedere

	void (* cursor_moved)		(ScriviDocument    *document);

	/* Document load */
	void (* load)			(ScriviDocument       *document,
					 const gchar         *uri,
					 const ScriviEncoding *encoding,
					 gint                 line_pos,
					 gboolean             create);

	void (* loading)		(ScriviDocument    *document,
					 goffset	   size,
					 goffset	   total_size);

	void (* loaded)			(ScriviDocument    *document,
					 const GError     *error);

	/* Document save */
	void (* save)			(ScriviDocument          *document,
					 const gchar            *uri,
					 const ScriviEncoding    *encoding,
					 ScriviDocumentSaveFlags  flags);

	void (* saving)			(ScriviDocument    *document,
					 goffset	   size,
					 goffset	   total_size);

	void (* saved)  		(ScriviDocument    *document,
					 const GError     *error);

	void (* search_highlight_updated)
					(ScriviDocument    *document,
					 GtkTextIter      *start,
					 GtkTextIter      *end);
};


#define SCRIVI_DOCUMENT_ERROR scrivi_document_error_quark ()

enum
{
	SCRIVI_DOCUMENT_ERROR_EXTERNALLY_MODIFIED,
	SCRIVI_DOCUMENT_ERROR_CANT_CREATE_BACKUP,
	SCRIVI_DOCUMENT_ERROR_TOO_BIG,
	SCRIVI_DOCUMENT_ERROR_ENCODING_AUTO_DETECTION_FAILED,
	SCRIVI_DOCUMENT_ERROR_CONVERSION_FALLBACK,
	SCRIVI_DOCUMENT_NUM_ERRORS
};

GQuark		 scrivi_document_error_quark	(void);

GType		 scrivi_document_get_type      	(void) G_GNUC_CONST;

ScriviDocument   *scrivi_document_new 		(void);

GFile		*scrivi_document_get_location	(ScriviDocument       *doc);

gchar		*scrivi_document_get_uri 	(ScriviDocument       *doc);
void		 scrivi_document_set_uri		(ScriviDocument       *doc,
						 const gchar 	     *uri);

gchar		*scrivi_document_get_uri_for_display
						(ScriviDocument       *doc);
gchar		*scrivi_document_get_short_name_for_display
					 	(ScriviDocument       *doc);

void		 scrivi_document_set_short_name_for_display
						(ScriviDocument       *doc,
						 const gchar         *name);

gchar		*scrivi_document_get_content_type
					 	(ScriviDocument       *doc);

void		 scrivi_document_set_content_type
					 	(ScriviDocument       *doc,
					 	 const gchar         *content_type);

gchar		*scrivi_document_get_mime_type 	(ScriviDocument       *doc);

gboolean	 scrivi_document_get_readonly 	(ScriviDocument       *doc);

void		 scrivi_document_load 		(ScriviDocument       *doc,
						 const gchar         *uri,
						 const ScriviEncoding *encoding,
						 gint                 line_pos,
						 gboolean             create); 

gboolean	 scrivi_document_insert_file	(ScriviDocument       *doc,
						 GtkTextIter         *iter, 
						 const gchar         *uri, 
						 const ScriviEncoding *encoding);

gboolean	 scrivi_document_load_cancel	(ScriviDocument       *doc);

void		 scrivi_document_save 		(ScriviDocument       *doc,
						 ScriviDocumentSaveFlags flags);

void		 scrivi_document_save_as 	(ScriviDocument       *doc,	
						 const gchar         *uri, 
						 const ScriviEncoding *encoding,
						 ScriviDocumentSaveFlags flags);

gboolean	 scrivi_document_is_untouched 	(ScriviDocument       *doc);
gboolean	 scrivi_document_is_untitled 	(ScriviDocument       *doc);

gboolean	 scrivi_document_is_local	(ScriviDocument       *doc);

gboolean	 scrivi_document_get_deleted	(ScriviDocument       *doc);

gboolean	 scrivi_document_goto_line 	(ScriviDocument       *doc, 
						 gint                 line);

gboolean	 scrivi_document_goto_line_offset(ScriviDocument *doc,
						 gint           line,
						 gint           line_offset);

void		 scrivi_document_set_search_text	(ScriviDocument       *doc,
						 const gchar         *text,
						 guint                flags);
						 
gchar		*scrivi_document_get_search_text	(ScriviDocument       *doc,
						 guint               *flags);

gboolean	 scrivi_document_get_can_search_again
						(ScriviDocument       *doc);

gboolean	 scrivi_document_search_forward	(ScriviDocument       *doc,
						 const GtkTextIter   *start,
						 const GtkTextIter   *end,
						 GtkTextIter         *match_start,
						 GtkTextIter         *match_end);
						 
gboolean	 scrivi_document_search_backward	(ScriviDocument       *doc,
						 const GtkTextIter   *start,
						 const GtkTextIter   *end,
						 GtkTextIter         *match_start,
						 GtkTextIter         *match_end);

gint		 scrivi_document_replace_all 	(ScriviDocument       *doc,
				            	 const gchar         *find, 
						 const gchar         *replace, 
					    	 guint                flags);

void 		 scrivi_document_set_language 	(ScriviDocument       *doc,
						 GtkSourceLanguage   *lang);
GtkSourceLanguage 
		*scrivi_document_get_language 	(ScriviDocument       *doc);

const ScriviEncoding 
		*scrivi_document_get_encoding	(ScriviDocument       *doc);

void		 scrivi_document_set_enable_search_highlighting 
						(ScriviDocument       *doc,
						 gboolean             enable);

gboolean	 scrivi_document_get_enable_search_highlighting
						(ScriviDocument       *doc);

void		 scrivi_document_set_newline_type (ScriviDocument           *doc,
						  ScriviDocumentNewlineType newline_type);

ScriviDocumentNewlineType
		 scrivi_document_get_newline_type (ScriviDocument *doc);

gchar		*scrivi_document_get_metadata	(ScriviDocument *doc,
						 const gchar   *key);

void		 scrivi_document_set_metadata	(ScriviDocument *doc,
						 const gchar   *first_key,
						 ...);

/* 
 * Non exported functions
 */
void		 _scrivi_document_set_readonly 	(ScriviDocument       *doc,
						 gboolean             readonly);

glong		 _scrivi_document_get_seconds_since_last_save_or_load 
						(ScriviDocument       *doc);

/* Note: this is a sync stat: use only on local files */
gboolean	_scrivi_document_check_externally_modified
						(ScriviDocument       *doc);

void		_scrivi_document_search_region   (ScriviDocument       *doc,
						 const GtkTextIter   *start,
						 const GtkTextIter   *end);
						  
/* Search macros */
#define SCRIVI_SEARCH_IS_DONT_SET_FLAGS(sflags) ((sflags & SCRIVI_SEARCH_DONT_SET_FLAGS) != 0)
#define SCRIVI_SEARCH_SET_DONT_SET_FLAGS(sflags,state) ((state == TRUE) ? \
(sflags |= SCRIVI_SEARCH_DONT_SET_FLAGS) : (sflags &= ~SCRIVI_SEARCH_DONT_SET_FLAGS))

#define SCRIVI_SEARCH_IS_ENTIRE_WORD(sflags) ((sflags & SCRIVI_SEARCH_ENTIRE_WORD) != 0)
#define SCRIVI_SEARCH_SET_ENTIRE_WORD(sflags,state) ((state == TRUE) ? \
(sflags |= SCRIVI_SEARCH_ENTIRE_WORD) : (sflags &= ~SCRIVI_SEARCH_ENTIRE_WORD))

#define SCRIVI_SEARCH_IS_CASE_SENSITIVE(sflags) ((sflags &  SCRIVI_SEARCH_CASE_SENSITIVE) != 0)
#define SCRIVI_SEARCH_SET_CASE_SENSITIVE(sflags,state) ((state == TRUE) ? \
(sflags |= SCRIVI_SEARCH_CASE_SENSITIVE) : (sflags &= ~SCRIVI_SEARCH_CASE_SENSITIVE))

#define SCRIVI_SEARCH_IS_PARSE_ESCAPES(sflags) ((sflags &  SCRIVI_SEARCH_PARSE_ESCAPES) != 0)
#define SCRIVI_SEARCH_SET_PARSE_ESCAPES(sflags,state) ((state == TRUE) ? \
(sflags |= SCRIVI_SEARCH_PARSE_ESCAPES) : (sflags &= ~SCRIVI_SEARCH_PARSE_ESCAPES))

typedef GMountOperation *(*ScriviMountOperationFactory)(ScriviDocument *doc, 
						       gpointer       userdata);

void		 _scrivi_document_set_mount_operation_factory
						(ScriviDocument	            *doc,
						 ScriviMountOperationFactory  callback,
						 gpointer	             userdata);
GMountOperation
		*_scrivi_document_create_mount_operation
						(ScriviDocument	     *doc);

G_END_DECLS

#endif /* __SCRIVI_DOCUMENT_H__ */
