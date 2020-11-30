/*
 * scrivi-print-job.h
 * This file is part of scrivi
 *
 * Copyright (C) 2000-2001 Chema Celorio, Paolo Maggi
 * Copyright (C) 2002-2008 Paolo Maggi  
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

#ifndef __SCRIVI_PRINT_JOB_H__
#define __SCRIVI_PRINT_JOB_H__

#include <gtk/gtk.h>
#include <scrivi/scrivi-view.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define SCRIVI_TYPE_PRINT_JOB              (scrivi_print_job_get_type())
#define SCRIVI_PRINT_JOB(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SCRIVI_TYPE_PRINT_JOB, ScriviPrintJob))
#define SCRIVI_PRINT_JOB_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SCRIVI_TYPE_PRINT_JOB, ScriviPrintJobClass))
#define SCRIVI_IS_PRINT_JOB(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SCRIVI_TYPE_PRINT_JOB))
#define SCRIVI_IS_PRINT_JOB_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_PRINT_JOB))
#define SCRIVI_PRINT_JOB_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SCRIVI_TYPE_PRINT_JOB, ScriviPrintJobClass))


typedef enum
{
	SCRIVI_PRINT_JOB_STATUS_INIT,
	SCRIVI_PRINT_JOB_STATUS_PAGINATING,
	SCRIVI_PRINT_JOB_STATUS_DRAWING,
	SCRIVI_PRINT_JOB_STATUS_DONE
} ScriviPrintJobStatus;

typedef enum
{
	SCRIVI_PRINT_JOB_RESULT_OK,
	SCRIVI_PRINT_JOB_RESULT_CANCEL,
	SCRIVI_PRINT_JOB_RESULT_ERROR	
} ScriviPrintJobResult;

/* Private structure type */
typedef struct _ScriviPrintJobPrivate ScriviPrintJobPrivate;

/*
 * Main object structure
 */
typedef struct _ScriviPrintJob ScriviPrintJob;


struct _ScriviPrintJob 
{
	GObject parent;
	
	/* <private> */
	ScriviPrintJobPrivate *priv;
};

/*
 * Class definition
 */
typedef struct _ScriviPrintJobClass ScriviPrintJobClass;

struct _ScriviPrintJobClass 
{
	GObjectClass parent_class;

        /* Signals */
	void (* printing) (ScriviPrintJob       *job,
	                   ScriviPrintJobStatus  status);

	void (* show_preview) (ScriviPrintJob   *job,
	                       GtkWidget       *preview);

        void (*done)      (ScriviPrintJob       *job,
		           ScriviPrintJobResult  result,
                           const GError        *error);
};

/*
 * Public methods
 */
GType			 scrivi_print_job_get_type		(void) G_GNUC_CONST;

ScriviPrintJob		*scrivi_print_job_new			(ScriviView                *view);

void			 scrivi_print_job_set_export_filename	(ScriviPrintJob            *job,
								 const gchar              *filename);

GtkPrintOperationResult	 scrivi_print_job_print			(ScriviPrintJob            *job,
								 GtkPrintOperationAction   action,
								 GtkPageSetup             *page_setup,
								 GtkPrintSettings         *settings,
								 GtkWindow                *parent,
								 GError                  **error);

void			 scrivi_print_job_cancel			(ScriviPrintJob            *job);

const gchar		*scrivi_print_job_get_status_string	(ScriviPrintJob            *job);

gdouble			 scrivi_print_job_get_progress		(ScriviPrintJob            *job);

GtkPrintSettings	*scrivi_print_job_get_print_settings	(ScriviPrintJob            *job);

GtkPageSetup		*scrivi_print_job_get_page_setup		(ScriviPrintJob            *job);

G_END_DECLS

#endif /* __SCRIVI_PRINT_JOB_H__ */
