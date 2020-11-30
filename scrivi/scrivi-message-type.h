#ifndef __SCRIVI_MESSAGE_TYPE_H__
#define __SCRIVI_MESSAGE_TYPE_H__

#include <glib-object.h>
#include <stdarg.h>

#include "scrivi-message.h"

G_BEGIN_DECLS

#define SCRIVI_TYPE_MESSAGE_TYPE			(scrivi_message_type_get_type ())
#define SCRIVI_MESSAGE_TYPE(x)			((ScriviMessageType *)(x))

typedef void (*ScriviMessageTypeForeach)		(const gchar *key, 
						 GType 	      type, 
						 gboolean     required, 
						 gpointer     user_data);

typedef struct _ScriviMessageType			ScriviMessageType;

GType scrivi_message_type_get_type 		 (void) G_GNUC_CONST;

gboolean scrivi_message_type_is_supported 	 (GType type);
gchar *scrivi_message_type_identifier		 (const gchar *object_path,
						  const gchar *method);
gboolean scrivi_message_type_is_valid_object_path (const gchar *object_path);

ScriviMessageType *scrivi_message_type_new	 (const gchar *object_path, 
						  const gchar *method,
						  guint	      num_optional,
						  ...) G_GNUC_NULL_TERMINATED;
ScriviMessageType *scrivi_message_type_new_valist	 (const gchar *object_path,
						  const gchar *method,
						  guint	      num_optional,
						  va_list      va_args);

void scrivi_message_type_set			 (ScriviMessageType *message_type,
						  guint		   num_optional,
						  ...) G_GNUC_NULL_TERMINATED;
void scrivi_message_type_set_valist		 (ScriviMessageType *message_type,
						  guint		   num_optional,
						  va_list	           va_args);

ScriviMessageType *scrivi_message_type_ref 	 (ScriviMessageType *message_type);
void scrivi_message_type_unref			 (ScriviMessageType *message_type);


ScriviMessage *scrivi_message_type_instantiate_valist (ScriviMessageType *message_type,
				       		     va_list	      va_args);
ScriviMessage *scrivi_message_type_instantiate 	 (ScriviMessageType *message_type,
				       		  ...) G_GNUC_NULL_TERMINATED;

const gchar *scrivi_message_type_get_object_path	 (ScriviMessageType *message_type);
const gchar *scrivi_message_type_get_method	 (ScriviMessageType *message_type);

GType scrivi_message_type_lookup			 (ScriviMessageType *message_type,
						  const gchar      *key);
						 
void scrivi_message_type_foreach 		 (ScriviMessageType 	  *message_type,
						  ScriviMessageTypeForeach  func,
						  gpointer	   	   user_data);

G_END_DECLS

#endif /* __SCRIVI_MESSAGE_TYPE_H__ */

// ex:ts=8:noet:
