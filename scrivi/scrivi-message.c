#include "scrivi-message.h"
#include "scrivi-message-type.h"

#include <string.h>
#include <gobject/gvaluecollector.h>

/**
 * SECTION:scrivi-message
 * @short_description: message bus message object
 * @include: scrivi/scrivi-message.h
 *
 * Communication on a #ScriviMessageBus is done through messages. Messages are
 * sent over the bus and received by connecting callbacks on the message bus.
 * A #ScriviMessage is an instantiation of a #ScriviMessageType, containing
 * values for the arguments as specified in the message type.
 *
 * A message can be seen as a method call, or signal emission depending on
 * who is the sender and who is the receiver. There is no explicit distinction
 * between methods and signals.
 */
#define SCRIVI_MESSAGE_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SCRIVI_TYPE_MESSAGE, ScriviMessagePrivate))

enum {
	PROP_0,

	PROP_OBJECT_PATH,
	PROP_METHOD,
	PROP_TYPE
};

struct _ScriviMessagePrivate
{
	ScriviMessageType *type;
	gboolean valid;

	GHashTable *values;
};

G_DEFINE_TYPE (ScriviMessage, scrivi_message, G_TYPE_OBJECT)

static void
scrivi_message_finalize (GObject *object)
{
	ScriviMessage *message = SCRIVI_MESSAGE (object);
	
	scrivi_message_type_unref (message->priv->type);
	g_hash_table_destroy (message->priv->values);

	G_OBJECT_CLASS (scrivi_message_parent_class)->finalize (object);
}

static void
scrivi_message_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
	ScriviMessage *msg = SCRIVI_MESSAGE (object);

	switch (prop_id)
	{
		case PROP_OBJECT_PATH:
			g_value_set_string (value, scrivi_message_type_get_object_path (msg->priv->type));
			break;
		case PROP_METHOD:
			g_value_set_string (value, scrivi_message_type_get_method (msg->priv->type));
			break;
		case PROP_TYPE:
			g_value_set_boxed (value, msg->priv->type);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
scrivi_message_set_property (GObject      *object,
			    guint         prop_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
	ScriviMessage *msg = SCRIVI_MESSAGE (object);

	switch (prop_id)
	{
		case PROP_TYPE:
			msg->priv->type = SCRIVI_MESSAGE_TYPE (g_value_dup_boxed (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static GValue *
add_value (ScriviMessage *message,
	   const gchar  *key)
{
	GValue *value;
	GType type = scrivi_message_type_lookup (message->priv->type, key);
	
	if (type == G_TYPE_INVALID)
		return NULL;
	
	value = g_new0 (GValue, 1);
	g_value_init (value, type);
	g_value_reset (value);

	g_hash_table_insert (message->priv->values, g_strdup (key), value);
	
	return value;
}

static void
scrivi_message_class_init (ScriviMessageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	
	object_class->finalize = scrivi_message_finalize;
	object_class->get_property = scrivi_message_get_property;
	object_class->set_property = scrivi_message_set_property;
	
	/**
	 * ScriviMessage:object_path:
	 *
	 * The messages object path (e.g. /scrivi/object/path).
	 *
	 */
	g_object_class_install_property (object_class, PROP_OBJECT_PATH,
					 g_param_spec_string ("object-path",
							      "OBJECT_PATH",
							      "The message object path",
							      NULL,
							      G_PARAM_READABLE |
							      G_PARAM_STATIC_STRINGS));

	/**
	 * ScriviMessage:method:
	 *
	 * The messages method.
	 *
	 */
	g_object_class_install_property (object_class, PROP_METHOD,
					 g_param_spec_string ("method",
							      "METHOD",
							      "The message method",
							      NULL,
							      G_PARAM_READABLE |
							      G_PARAM_STATIC_STRINGS));
	
	/**
	 * ScriviMEssage:type:
	 *
	 * The message type.
	 *
	 */
	g_object_class_install_property (object_class, PROP_TYPE,
					 g_param_spec_boxed ("type",
					 		     "TYPE",
					 		     "The message type",
					 		     SCRIVI_TYPE_MESSAGE_TYPE,
					 		     G_PARAM_READWRITE |
					 		     G_PARAM_CONSTRUCT_ONLY |
					 		     G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof(ScriviMessagePrivate));
}

static void
destroy_value (GValue *value)
{
	g_value_unset (value);
	g_free (value);
}

static void
scrivi_message_init (ScriviMessage *self)
{
	self->priv = SCRIVI_MESSAGE_GET_PRIVATE (self);

	self->priv->values = g_hash_table_new_full (g_str_hash,
						    g_str_equal,
						    (GDestroyNotify)g_free,
						    (GDestroyNotify)destroy_value);
}

static gboolean
set_value_real (GValue 	     *to, 
		const GValue *from)
{
	GType from_type;
	GType to_type;
	
	from_type = G_VALUE_TYPE (from);
	to_type = G_VALUE_TYPE (to);

	if (!g_type_is_a (from_type, to_type))
	{		
		if (!g_value_transform (from, to))
		{
			g_warning ("%s: Unable to make conversion from %s to %s",
				   G_STRLOC,
				   g_type_name (from_type),
				   g_type_name (to_type));
			return FALSE;
		}
		
		return TRUE;
	}
	
	g_value_copy (from, to);
	return TRUE;
}

inline static GValue *
value_lookup (ScriviMessage *message,
	      const gchar  *key,
	      gboolean	    create)
{
	GValue *ret = (GValue *)g_hash_table_lookup (message->priv->values, key);
	
	if (!ret && create)
		ret = add_value (message, key);
	
	return ret;
}

/**
 * scrivi_message_get_method:
 * @message: the #ScriviMessage
 *
 * Get the message method.
 *
 * Return value: the message method
 *
 */
const gchar *
scrivi_message_get_method (ScriviMessage *message)
{
	g_return_val_if_fail (SCRIVI_IS_MESSAGE (message), NULL);
	
	return scrivi_message_type_get_method (message->priv->type);
}

/**
 * scrivi_message_get_object_path:
 * @message: the #ScriviMessage
 *
 * Get the message object path.
 *
 * Return value: the message object path
 *
 */
const gchar *
scrivi_message_get_object_path (ScriviMessage *message)
{
	g_return_val_if_fail (SCRIVI_IS_MESSAGE (message), NULL);
	
	return scrivi_message_type_get_object_path (message->priv->type);
}

/**
 * scrivi_message_set:
 * @message: the #ScriviMessage
 * @...: a %NULL terminated variable list of key/value pairs
 *
 * Set values of message arguments. The supplied @var_args should contain
 * pairs of keys and argument values.
 *
 */
void
scrivi_message_set (ScriviMessage *message,
		   ...)
{
	va_list ap;

	g_return_if_fail (SCRIVI_IS_MESSAGE (message));

	va_start (ap, message);
	scrivi_message_set_valist (message, ap);
	va_end (ap);
}

/**
 * scrivi_message_set_valist:
 * @message: the #ScriviMessage
 * @var_args: a %NULL terminated variable list of key/value pairs
 *
 * Set values of message arguments. The supplied @var_args should contain
 * pairs of keys and argument values.
 *
 */
void
scrivi_message_set_valist (ScriviMessage *message,
			  va_list	var_args)
{
	const gchar *key;

	g_return_if_fail (SCRIVI_IS_MESSAGE (message));

	while ((key = va_arg (var_args, const gchar *)) != NULL)
	{
		/* lookup the key */
		GValue *container = value_lookup (message, key, TRUE);
		GValue value = {0,};
		gchar *error = NULL;
		
		if (!container)
		{
			g_warning ("%s: Cannot set value for %s, does not exist", 
				   G_STRLOC,
				   key);
			
			/* skip value */
			va_arg (var_args, gpointer);
			continue;
		}
		
		g_value_init (&value, G_VALUE_TYPE (container));
		G_VALUE_COLLECT (&value, var_args, 0, &error);
		
		if (error)
		{
			g_warning ("%s: %s", G_STRLOC, error);
			continue;
		}

		set_value_real (container, &value);
		g_value_unset (&value);
	}
}

/**
 * scrivi_message_set_value:
 * @message: the #ScriviMessage
 * @key: the argument key
 * @value: (out): the argument value
 *
 * Set value of message argument @key to @value.
 *
 */
void
scrivi_message_set_value (ScriviMessage *message,
			 const gchar  *key,
			 GValue	      *value)
{
	GValue *container;
	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	container = value_lookup (message, key, TRUE);
	
	if (!container)
	{
		g_warning ("%s: Cannot set value for %s, does not exist", 
			   G_STRLOC, 
			   key);
		return;
	}
	
	set_value_real (container, value);
}

/**
 * scrivi_message_set_valuesv:
 * @message: the #ScriviMessage
 * @keys: (array-length=n_values): keys to set values for
 * @values: (array-length=n_values): values to set
 * @n_values: number of arguments to set values for
 *
 * Set message argument values.
 *
 */
void
scrivi_message_set_valuesv (ScriviMessage	 *message,
			   const gchar	**keys,
			   GValue        *values,
			   gint		  n_values)
{
	gint i;
	
	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	for (i = 0; i < n_values; i++)
	{
		scrivi_message_set_value (message, keys[i], &values[i]);
	}
}

/* FIXME this is an issue for introspection */
/**
 * scrivi_message_get:
 * @message: the #ScriviMessage
 * @...: a %NULL variable argument list of key/value container pairs
 *
 * Get values of message arguments. The supplied @var_args should contain
 * pairs of keys and pointers to variables which are set to the argument
 * value for the specified key.
 *
 */
void 
scrivi_message_get (ScriviMessage	*message,
		   ...)
{
	va_list ap;

	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	va_start (ap, message);
	scrivi_message_get_valist (message, ap);
	va_end (ap);
}

/**
 * scrivi_message_get_valist:
 * @message: the #ScriviMessage
 * @var_args: a %NULL variable argument list of key/value container pairs
 *
 * Get values of message arguments. The supplied @var_args should contain
 * pairs of keys and pointers to variables which are set to the argument
 * value for the specified key.
 *
 */
void
scrivi_message_get_valist (ScriviMessage *message,
			  va_list 	var_args)
{
	const gchar *key;

	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	while ((key = va_arg (var_args, const gchar *)) != NULL)
	{
		GValue *container;
		GValue copy = {0,};
		gchar *error = NULL;

		container = value_lookup (message, key, FALSE);
	
		if (!container)
		{		
			/* skip value */
			va_arg (var_args, gpointer);
			continue;
		}
		
		/* copy the value here, to be sure it isn't tainted */
		g_value_init (&copy, G_VALUE_TYPE (container));
		g_value_copy (container, &copy);
		
		G_VALUE_LCOPY (&copy, var_args, 0, &error);
		
		if (error)
		{
			g_warning ("%s: %s", G_STRLOC, error);
			g_free (error);
			
			/* purposely leak the value here, because it might
			   be in a bad state */
			continue;
		}
		
		g_value_unset (&copy);
	}
}

/**
 * scrivi_message_get_value:
 * @message: the #ScriviMessage
 * @key: the argument key
 * @value: (out): value return container
 *
 * Get the value of a specific message argument. @value will be initialized
 * with the correct type.
 *
 */
void 
scrivi_message_get_value (ScriviMessage *message,
			 const gchar  *key,
			 GValue	      *value)
{
	GValue *container;
	
	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	container = value_lookup (message, key, FALSE);
	
	if (!container)
	{
		g_warning ("%s: Invalid key `%s'",
			   G_STRLOC,
			   key);
		return;
	}
	
	g_value_init (value, G_VALUE_TYPE (container));
	set_value_real (value, container);
}

/**
 * scrivi_message_get_key_type:
 * @message: the #ScriviMessage
 * @key: the argument key
 *
 * Get the type of a message argument.
 *
 * Return value: the type of @key
 *
 */
GType 
scrivi_message_get_key_type (ScriviMessage    *message,
			    const gchar	    *key)
{
	g_return_val_if_fail (SCRIVI_IS_MESSAGE (message), G_TYPE_INVALID);
	g_return_val_if_fail (message->priv->type != NULL, G_TYPE_INVALID);

	return scrivi_message_type_lookup (message->priv->type, key);
}

/**
 * scrivi_message_has_key:
 * @message: the #ScriviMessage
 * @key: the argument key
 *
 * Check whether the message has a specific key.
 *
 * Return value: %TRUE if @message has argument @key
 *
 */
gboolean
scrivi_message_has_key (ScriviMessage *message,
		       const gchar  *key)
{
	g_return_val_if_fail (SCRIVI_IS_MESSAGE (message), FALSE);
	
	return value_lookup (message, key, FALSE) != NULL;
}

typedef struct
{
	ScriviMessage *message;
	gboolean valid;	
} ValidateInfo;

static void
validate_key (const gchar  *key,
	      GType         type,
	      gboolean	    required,
	      ValidateInfo *info)
{
	GValue *value;
	
	if (!info->valid || !required)
		return;
	
	value = value_lookup (info->message, key, FALSE);
	
	if (!value)
		info->valid = FALSE;
}

/**
 * scrivi_message_validate:
 * @message: the #ScriviMessage
 *
 * Validates the message arguments according to the message type.
 *
 * Return value: %TRUE if the message is valid
 *
 */
gboolean
scrivi_message_validate (ScriviMessage *message)
{
	ValidateInfo info = {message, TRUE};

	g_return_val_if_fail (SCRIVI_IS_MESSAGE (message), FALSE);
	g_return_val_if_fail (message->priv->type != NULL, FALSE);
	
	if (!message->priv->valid)
	{
		scrivi_message_type_foreach (message->priv->type, 
					    (ScriviMessageTypeForeach)validate_key,
					    &info);

		message->priv->valid = info.valid;
	}
	
	return message->priv->valid;
}

// ex:ts=8:noet:
