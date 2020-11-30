#ifndef __SCRIVI_MESSAGE_H__
#define __SCRIVI_MESSAGE_H__

#include <glib-object.h>
#include <stdarg.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_MESSAGE			(scrivi_message_get_type ())
#define SCRIVI_MESSAGE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_MESSAGE, ScriviMessage))
#define SCRIVI_MESSAGE_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_MESSAGE, ScriviMessage const))
#define SCRIVI_MESSAGE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_MESSAGE, ScriviMessageClass))
#define SCRIVI_IS_MESSAGE(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_MESSAGE))
#define SCRIVI_IS_MESSAGE_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_MESSAGE))
#define SCRIVI_MESSAGE_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_MESSAGE, ScriviMessageClass))

typedef struct _ScriviMessage		ScriviMessage;
typedef struct _ScriviMessageClass	ScriviMessageClass;
typedef struct _ScriviMessagePrivate	ScriviMessagePrivate;

struct _ScriviMessage {
	GObject parent;
	
	ScriviMessagePrivate *priv;
};

struct _ScriviMessageClass {
	GObjectClass parent_class;
};

GType scrivi_message_get_type (void) G_GNUC_CONST;

struct _ScriviMessageType scrivi_message_get_message_type (ScriviMessage *message);

void scrivi_message_get			(ScriviMessage	 *message,
					 ...) G_GNUC_NULL_TERMINATED;
void scrivi_message_get_valist		(ScriviMessage	 *message,
					 va_list 	  var_args);
void scrivi_message_get_value		(ScriviMessage	 *message,
					 const gchar	 *key,
					 GValue		 *value);

void scrivi_message_set			(ScriviMessage	 *message,
					 ...) G_GNUC_NULL_TERMINATED;
void scrivi_message_set_valist		(ScriviMessage	 *message,
					 va_list	  	  var_args);
void scrivi_message_set_value		(ScriviMessage	 *message,
					 const gchar 	 *key,
					 GValue		 *value);
void scrivi_message_set_valuesv		(ScriviMessage	 *message,
					 const gchar	**keys,
					 GValue		 *values,
					 gint		  n_values);

const gchar *scrivi_message_get_object_path (ScriviMessage	*message);
const gchar *scrivi_message_get_method	(ScriviMessage	 *message);

gboolean scrivi_message_has_key		(ScriviMessage	 *message,
					 const gchar     *key);

GType scrivi_message_get_key_type 	(ScriviMessage    *message,
			    		 const gchar     *key);

gboolean scrivi_message_validate		(ScriviMessage	 *message);


G_END_DECLS

#endif /* __SCRIVI_MESSAGE_H__ */

// ex:ts=8:noet:
