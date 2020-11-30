#include "scrivi-message-bus.h"

#include <string.h>
#include <stdarg.h>
#include <gobject/gvaluecollector.h>

/**
 * ScriviMessageCallback:
 * @bus: the #ScriviMessageBus on which the message was sent
 * @message: the #ScriviMessage which was sent
 * @userdata: the supplied user data when connecting the callback
 *
 * Callback signature used for connecting callback functions to be called
 * when a message is received (see scrivi_message_bus_connect()).
 *
 */

/**
 * SECTION:scrivi-message-bus
 * @short_description: internal message communication bus
 * @include: scrivi/scrivi-message-bus.h
 *
 * scrivi has a communication bus very similar to DBus. Its primary use is to
 * allow easy communication between plugins, but it can also be used to expose
 * scrivi functionality to external applications by providing DBus bindings for
 * the internal scrivi message bus.
 *
 * There are two different communication busses available. The default bus
 * (see scrivi_message_bus_get_default()) is an application wide communication
 * bus. In addition, each #ScriviWindow has a separate, private bus
 * (see scrivi_window_get_message_bus()). This makes it easier for plugins to
 * communicate to other plugins in the same window.
 *
 * The concept of the message bus is very simple. You can register a message
 * type on the bus, specified as a Method at a specific Object Path with a
 * certain set of Method Arguments. You can then connect callback functions
 * for this message type on the bus. Whenever a message with the Object Path
 * and Method for which callbacks are connected is sent over the bus, the
 * callbacks are called. There is no distinction between Methods and Signals
 * (signals are simply messages where sender and receiver have switched places).
 *
 * <example>
 * <title>Registering a message type</title>
 * <programlisting>
 * ScriviMessageBus *bus = scrivi_message_bus_get_default ();
 *
 * // Register 'method' at '/plugins/example' with one required
 * // string argument 'arg1'
 * ScriviMessageType *message_type = scrivi_message_bus_register ("/plugins/example", "method", 
 *                                                              0, 
 *                                                              "arg1", G_TYPE_STRING,
 *                                                              NULL);
 * </programlisting>
 * </example>
 * <example>
 * <title>Connecting a callback</title>
 * <programlisting>
 * static void
 * example_method_cb (ScriviMessageBus *bus,
 *                    ScriviMessage    *message,
 *                    gpointer         userdata)
 * {
 * 	gchar *arg1 = NULL;
 *	
 * 	scrivi_message_get (message, "arg1", &arg1, NULL);
 * 	g_message ("Evoked /plugins/example.method with: %s", arg1);
 * 	g_free (arg1);
 * }
 *
 * ScriviMessageBus *bus = scrivi_message_bus_get_default ();
 * 
 * guint id = scrivi_message_bus_connect (bus, 
 *                                       "/plugins/example", "method",
 *                                       example_method_cb,
 *                                       NULL,
 *                                       NULL);
 *                                        
 * </programlisting>
 * </example>
 * <example>
 * <title>Sending a message</title>
 * <programlisting>
 * ScriviMessageBus *bus = scrivi_message_bus_get_default ();
 *
 * scrivi_message_bus_send (bus, 
 *                         "/plugins/example", "method", 
 *                         "arg1", "Hello World", 
 *                         NULL);
 * </programlisting>
 * </example>
 */
 
#define SCRIVI_MESSAGE_BUS_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), SCRIVI_TYPE_MESSAGE_BUS, ScriviMessageBusPrivate))

typedef struct
{
	gchar *object_path;
	gchar *method;

	GList *listeners;
} Message;

typedef struct
{
	guint id;
	gboolean blocked;

	GDestroyNotify destroy_data;
	ScriviMessageCallback callback;
	gpointer userdata;
} Listener;

typedef struct
{
	Message *message;
	GList *listener;
} IdMap;

struct _ScriviMessageBusPrivate
{
	GHashTable *messages;
	GHashTable *idmap;

	GList *message_queue;
	guint idle_id;

	guint next_id;
	
	GHashTable *types; /* mapping from identifier to ScriviMessageType */
};

/* signals */
enum
{
	DISPATCH,
	REGISTERED,
	UNREGISTERED,
	LAST_SIGNAL
};

static guint message_bus_signals[LAST_SIGNAL] = { 0 };

static void scrivi_message_bus_dispatch_real (ScriviMessageBus *bus,
				 	     ScriviMessage    *message);

G_DEFINE_TYPE(ScriviMessageBus, scrivi_message_bus, G_TYPE_OBJECT)

static void
listener_free (Listener *listener)
{
	if (listener->destroy_data)
		listener->destroy_data (listener->userdata);

	g_free (listener);
}

static void
message_free (Message *message)
{
	g_free (message->method);
	g_free (message->object_path);
	
	g_list_foreach (message->listeners, (GFunc)listener_free, NULL);
	g_list_free (message->listeners);
	
	g_free (message);
}

static void
message_queue_free (GList *queue)
{
	g_list_foreach (queue, (GFunc)g_object_unref, NULL);
	g_list_free (queue);
}

static void
scrivi_message_bus_finalize (GObject *object)
{
	ScriviMessageBus *bus = SCRIVI_MESSAGE_BUS (object);
	
	if (bus->priv->idle_id != 0)
		g_source_remove (bus->priv->idle_id);
	
	message_queue_free (bus->priv->message_queue);

	g_hash_table_destroy (bus->priv->messages);
	g_hash_table_destroy (bus->priv->idmap);
	g_hash_table_destroy (bus->priv->types);
	
	G_OBJECT_CLASS (scrivi_message_bus_parent_class)->finalize (object);
}

static void
scrivi_message_bus_class_init (ScriviMessageBusClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = scrivi_message_bus_finalize;
	
	klass->dispatch = scrivi_message_bus_dispatch_real;

	/**
	 * ScriviMessageBus::dispatch:
	 * @bus: a #ScriviMessageBus
	 * @message: the #ScriviMessage to dispatch
	 *
	 * The "dispatch" signal is emitted when a message is to be dispatched.
	 * The message is dispatched in the default handler of this signal. 
	 * Primary use of this signal is to customize the dispatch of a message
	 * (for instance to automatically dispatch all messages over DBus).
	 *2
	 */
	message_bus_signals[DISPATCH] =
   		g_signal_new ("dispatch",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviMessageBusClass, dispatch),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__OBJECT,
			      G_TYPE_NONE,
			      1,
			      SCRIVI_TYPE_MESSAGE);

	/**
	 * ScriviMessageBus::registered:
	 * @bus: a #ScriviMessageBus
	 * @message_type: the registered #ScriviMessageType
	 *
	 * The "registered" signal is emitted when a message has been registered
	 * on the bus.
	 *
	 */
	message_bus_signals[REGISTERED] =
   		g_signal_new ("registered",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviMessageBusClass, registered),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__BOXED,
			      G_TYPE_NONE,
			      1,
			      SCRIVI_TYPE_MESSAGE_TYPE);

	/**
	 * ScriviMessageBus::unregistered:
	 * @bus: a #ScriviMessageBus
	 * @message_type: the unregistered #ScriviMessageType
	 *
	 * The "unregistered" signal is emitted when a message has been 
	 * unregistered from the bus.
	 *
	 */
	message_bus_signals[UNREGISTERED] =
   		g_signal_new ("unregistered",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (ScriviMessageBusClass, unregistered),
			      NULL, NULL,
			      g_cclosure_marshal_VOID__BOXED,
			      G_TYPE_NONE,
			      1,
			      SCRIVI_TYPE_MESSAGE_TYPE);

	g_type_class_add_private (object_class, sizeof(ScriviMessageBusPrivate));
}

static Message *
message_new (ScriviMessageBus *bus,
	     const gchar     *object_path,
	     const gchar     *method)
{
	Message *message = g_new (Message, 1);
	
	message->object_path = g_strdup (object_path);
	message->method = g_strdup (method);
	message->listeners = NULL;

	g_hash_table_insert (bus->priv->messages, 
			     scrivi_message_type_identifier (object_path, method),
			     message);
	return message;
}

static Message *
lookup_message (ScriviMessageBus *bus,
	       const gchar      *object_path,
	       const gchar      *method,
	       gboolean          create)
{
	gchar *identifier;
	Message *message;
	
	identifier = scrivi_message_type_identifier (object_path, method);
	message = (Message *)g_hash_table_lookup (bus->priv->messages, identifier);
	g_free (identifier);

	if (!message && !create)
		return NULL;
	
	if (!message)
		message = message_new (bus, object_path, method);
	
	return message;
}

static guint
add_listener (ScriviMessageBus      *bus,
	      Message		   *message,
	      ScriviMessageCallback  callback,
	      gpointer		    userdata,
	      GDestroyNotify        destroy_data)
{
	Listener *listener;
	IdMap *idmap;
	
	listener = g_new (Listener, 1);
	listener->id = ++bus->priv->next_id;
	listener->callback = callback;
	listener->userdata = userdata;
	listener->blocked = FALSE;
	listener->destroy_data = destroy_data;

	message->listeners = g_list_append (message->listeners, listener);
	
	idmap = g_new (IdMap, 1);
	idmap->message = message;
	idmap->listener = g_list_last (message->listeners);

	g_hash_table_insert (bus->priv->idmap, GINT_TO_POINTER (listener->id), idmap);	
	return listener->id;
}

static void
remove_listener (ScriviMessageBus *bus,
		 Message         *message,
		 GList		 *listener)
{
	Listener *lst;
	
	lst = (Listener *)listener->data;
	
	/* remove from idmap */
	g_hash_table_remove (bus->priv->idmap, GINT_TO_POINTER (lst->id));
	listener_free (lst);

	/* remove from list of listeners */
	message->listeners = g_list_delete_link (message->listeners, listener);
	
	if (!message->listeners)
	{
		/* remove message because it does not have any listeners */
		g_hash_table_remove (bus->priv->messages, message);
	}
}

static void
block_listener (ScriviMessageBus *bus,
		Message		*message,
		GList		*listener)
{
	Listener *lst;
	
	lst = (Listener *)listener->data;
	lst->blocked = TRUE;
}

static void
unblock_listener (ScriviMessageBus *bus,
		  Message	  *message,
		  GList		  *listener)
{
	Listener *lst;
	
	lst = (Listener *)listener->data;
	lst->blocked = FALSE;
}

static void
dispatch_message_real (ScriviMessageBus *bus,
		       Message         *msg,
		       ScriviMessage    *message)
{
	GList *item;
	
	for (item = msg->listeners; item; item = item->next)
	{
		Listener *listener = (Listener *)item->data;
		
		if (!listener->blocked)
			listener->callback (bus, message, listener->userdata);
	}
}

static void
scrivi_message_bus_dispatch_real (ScriviMessageBus *bus,
				 ScriviMessage    *message)
{
	const gchar *object_path;
	const gchar *method;
	Message *msg;
	
	object_path = scrivi_message_get_object_path (message);
	method = scrivi_message_get_method (message);

	msg = lookup_message (bus, object_path, method, FALSE);
	
	if (msg)
		dispatch_message_real (bus, msg, message);
}

static void
dispatch_message (ScriviMessageBus *bus,
		  ScriviMessage    *message)
{
	g_signal_emit (bus, message_bus_signals[DISPATCH], 0, message);	
}

static gboolean
idle_dispatch (ScriviMessageBus *bus)
{
	GList *list;
	GList *item;
	
	/* make sure to set idle_id to 0 first so that any new async messages
	   will be queued properly */
	bus->priv->idle_id = 0;

	/* reverse queue to get correct delivery order */
	list = g_list_reverse (bus->priv->message_queue);
	bus->priv->message_queue = NULL;
	
	for (item = list; item; item = item->next)
	{
		ScriviMessage *msg = SCRIVI_MESSAGE (item->data);
		
		dispatch_message (bus, msg);
	}
	
	message_queue_free (list);
	return FALSE;
}

typedef void (*MatchCallback) (ScriviMessageBus *, Message *, GList *);

static void
process_by_id (ScriviMessageBus  *bus,
	       guint	         id,
	       MatchCallback     processor)
{
	IdMap *idmap;
	
	idmap = (IdMap *)g_hash_table_lookup (bus->priv->idmap, GINT_TO_POINTER (id));
	
	if (idmap == NULL)
	{
		g_warning ("No handler registered with id `%d'", id);
		return;
	}
		
	processor (bus, idmap->message, idmap->listener);
}

static void
process_by_match (ScriviMessageBus      *bus,
	          const gchar          *object_path,
	          const gchar          *method,
	          ScriviMessageCallback  callback,
	          gpointer              userdata,
	          MatchCallback         processor)
{
	Message *message;
	GList *item;
	
	message = lookup_message (bus, object_path, method, FALSE);
	
	if (!message)
	{
		g_warning ("No such handler registered for %s.%s", object_path, method);
		return;
	}
	
	for (item = message->listeners; item; item = item->next)
	{
		Listener *listener = (Listener *)item->data;
		
		if (listener->callback == callback && 
		    listener->userdata == userdata)
		{
			processor (bus, message, item);
			return;
		}
	}
	
	g_warning ("No such handler registered for %s.%s", object_path, method);
}

static void
scrivi_message_bus_init (ScriviMessageBus *self)
{
	self->priv = SCRIVI_MESSAGE_BUS_GET_PRIVATE (self);
	
	self->priv->messages = g_hash_table_new_full (g_str_hash,
						      g_str_equal,
						      (GDestroyNotify)g_free,
						      (GDestroyNotify)message_free);

	self->priv->idmap = g_hash_table_new_full (g_direct_hash,
	 					   g_direct_equal,
	 					   NULL,
	 					   (GDestroyNotify)g_free);
	 					   
	self->priv->types = g_hash_table_new_full (g_str_hash,
						   g_str_equal,
						   (GDestroyNotify)g_free,
						   (GDestroyNotify)scrivi_message_type_unref);
}

/**
 * scrivi_message_bus_get_default:
 *
 * Get the default application #ScriviMessageBus.
 *
 * Return value: the default #ScriviMessageBus
 *
 */
ScriviMessageBus *
scrivi_message_bus_get_default (void)
{
	static ScriviMessageBus *default_bus = NULL;
	
	if (G_UNLIKELY (default_bus == NULL))
	{
		default_bus = g_object_new (SCRIVI_TYPE_MESSAGE_BUS, NULL);
		g_object_add_weak_pointer (G_OBJECT (default_bus),
				           (gpointer) &default_bus);
	}
	
	return default_bus;
}

/**
 * scrivi_message_bus_new:
 * 
 * Create a new message bus. Use scrivi_message_bus_get_default() to get the
 * default, application wide, message bus. Creating a new bus is useful for
 * associating a specific bus with for instance a #ScriviWindow.
 *
 * Return value: a new #ScriviMessageBus
 *
 */
ScriviMessageBus *
scrivi_message_bus_new (void)
{
	return SCRIVI_MESSAGE_BUS (g_object_new (SCRIVI_TYPE_MESSAGE_BUS, NULL));
}

/**
 * scrivi_message_bus_lookup:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 *
 * Get the registered #ScriviMessageType for @method at @object_path. The 
 * returned #ScriviMessageType is owned by the bus and should not be unreffed.
 *
 * Return value: the registered #ScriviMessageType or %NULL if no message type
 *               is registered for @method at @object_path
 *
 */
ScriviMessageType *
scrivi_message_bus_lookup (ScriviMessageBus *bus,
			  const gchar	  *object_path,
			  const gchar	  *method)
{
	gchar *identifier;
	ScriviMessageType *message_type;
	
	g_return_val_if_fail (SCRIVI_IS_MESSAGE_BUS (bus), NULL);
	g_return_val_if_fail (object_path != NULL, NULL);
	g_return_val_if_fail (method != NULL, NULL);

	identifier = scrivi_message_type_identifier (object_path, method);
	message_type = SCRIVI_MESSAGE_TYPE (g_hash_table_lookup (bus->priv->types, identifier));
	
	g_free (identifier);
	return message_type;
}

/**
 * scrivi_message_bus_register:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method to register
 * @num_optional: the number of optional arguments
 * @...: NULL terminated list of key/gtype method argument pairs
 *
 * Register a message on the bus. A message must be registered on the bus before
 * it can be send. This function registers the type arguments for @method at 
 * @object_path. The arguments are specified with the variable arguments which 
 * should contain pairs of const gchar *key and GType terminated by %NULL. The 
 * last @num_optional arguments are registered as optional (and are thus not
 * required when sending a message).
 *
 * This function emits a #ScriviMessageBus::registered signal.
 *
 * Return value: the registered #ScriviMessageType. The returned reference is
 *               owned by the bus. If you want to keep it alive after
 *               unregistering, use scrivi_message_type_ref().
 *
 */
ScriviMessageType *
scrivi_message_bus_register (ScriviMessageBus *bus,
			    const gchar     *object_path,
			    const gchar	    *method,
			    guint	     num_optional,
			    ...)
{
	gchar *identifier;
	gpointer data;
	va_list var_args;
	ScriviMessageType *message_type;

	g_return_val_if_fail (SCRIVI_IS_MESSAGE_BUS (bus), NULL);
	g_return_val_if_fail (scrivi_message_type_is_valid_object_path (object_path), NULL);
	
	if (scrivi_message_bus_is_registered (bus, object_path, method))
	{
		g_warning ("Message type for '%s.%s' is already registered", object_path, method);
		return NULL;
	}

	identifier = scrivi_message_type_identifier (object_path, method);
	data = g_hash_table_lookup (bus->priv->types, identifier);
	
	va_start (var_args, num_optional);
	message_type = scrivi_message_type_new_valist (object_path, 
						      method,
						      num_optional,
						      var_args);
	va_end (var_args);
	
	if (message_type)
	{
		g_hash_table_insert (bus->priv->types, identifier, message_type);
		g_signal_emit (bus, message_bus_signals[REGISTERED], 0, message_type);
	}
	else
	{
		g_free (identifier);
	}
	
	return message_type;	
}

static void
scrivi_message_bus_unregister_real (ScriviMessageBus  *bus,
				   ScriviMessageType *message_type,
				   gboolean          remove_from_store)
{
	gchar *identifier;
	
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));

	identifier = scrivi_message_type_identifier (scrivi_message_type_get_object_path (message_type), 
						    scrivi_message_type_get_method (message_type));
	
	/* Keep message type alive for signal emission */
	scrivi_message_type_ref (message_type);

	if (!remove_from_store || g_hash_table_remove (bus->priv->types, identifier))
		g_signal_emit (bus, message_bus_signals[UNREGISTERED], 0, message_type);
	
	scrivi_message_type_unref (message_type);
	g_free (identifier);
}

/**
 * scrivi_message_bus_unregister:
 * @bus: a #ScriviMessageBus
 * @message_type: the #ScriviMessageType to unregister
 *
 * Unregisters a previously registered message type. This is especially useful 
 * for plugins which should unregister message types when they are deactivated.
 *
 * This function emits the #ScriviMessageBus::unregistered signal.
 *
 */
void
scrivi_message_bus_unregister (ScriviMessageBus  *bus,
			      ScriviMessageType *message_type)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	scrivi_message_bus_unregister_real (bus, message_type, TRUE);
}

typedef struct 
{
	ScriviMessageBus *bus;
	const gchar *object_path;
} UnregisterInfo;

static gboolean
unregister_each (const gchar      *identifier,
		 ScriviMessageType *message_type,
		 UnregisterInfo   *info)
{
	if (strcmp (scrivi_message_type_get_object_path (message_type),
		    info->object_path) == 0)
	{	
		scrivi_message_bus_unregister_real (info->bus, message_type, FALSE);
		return TRUE;
	}
	
	return FALSE;
}

/**
 * scrivi_message_bus_unregister_all:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 *
 * Unregisters all message types for @object_path. This is especially useful for
 * plugins which should unregister message types when they are deactivated.
 *
 * This function emits the #ScriviMessageBus::unregistered signal for all
 * unregistered message types.
 *
 */
void
scrivi_message_bus_unregister_all (ScriviMessageBus *bus,
			          const gchar     *object_path)
{
	UnregisterInfo info = {bus, object_path};

	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (object_path != NULL);

	g_hash_table_foreach_remove (bus->priv->types, 
				     (GHRFunc)unregister_each,
				     &info);
}

/**
 * scrivi_message_bus_is_registered:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 *
 * Check whether a message type @method at @object_path is registered on the 
 * bus.
 *
 * Return value: %TRUE if the @method at @object_path is a registered message 
 *               type on the bus
 *
 */
gboolean
scrivi_message_bus_is_registered (ScriviMessageBus	*bus,
				 const gchar	*object_path,
				 const gchar	*method)
{
	gchar *identifier;
	gboolean ret;
	
	g_return_val_if_fail (SCRIVI_IS_MESSAGE_BUS (bus), FALSE);
	g_return_val_if_fail (object_path != NULL, FALSE);
	g_return_val_if_fail (method != NULL, FALSE);

	identifier = scrivi_message_type_identifier (object_path, method);
	ret = g_hash_table_lookup (bus->priv->types, identifier) != NULL;
	
	g_free(identifier);
	return ret;
}

typedef struct
{
	ScriviMessageBusForeach func;
	gpointer userdata;
} ForeachInfo;

static void
foreach_type (const gchar      *key,
	      ScriviMessageType *message_type,
	      ForeachInfo      *info)
{
	scrivi_message_type_ref (message_type);
	info->func (message_type, info->userdata);
	scrivi_message_type_unref (message_type);
}

/**
 * scrivi_message_bus_foreach:
 * @bus: the #ScriviMessagebus
 * @func: the callback function
 * @userdata: the user data to supply to the callback function
 *
 * Calls @func for each message type registered on the bus
 *
 */
void 
scrivi_message_bus_foreach (ScriviMessageBus        *bus,
			   ScriviMessageBusForeach  func,
			   gpointer		   userdata)
{
	ForeachInfo info = {func, userdata};
	
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (func != NULL);

	g_hash_table_foreach (bus->priv->types, (GHFunc)foreach_type, &info);
}

/**
 * scrivi_message_bus_connect:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: function to be called when message @method at @object_path is sent
 * @userdata: userdata to use for the callback
 * @destroy_data: function to evoke with @userdata as argument when @userdata
 *                needs to be freed
 *
 * Connect a callback handler to be evoked when message @method at @object_path
 * is sent over the bus.
 *
 * Return value: the callback identifier
 *
 */
guint
scrivi_message_bus_connect (ScriviMessageBus	*bus, 
		           const gchar		*object_path,
		           const gchar		*method,
		           ScriviMessageCallback  callback,
		           gpointer		 userdata,
		           GDestroyNotify	 destroy_data)
{
	Message *message;

	g_return_val_if_fail (SCRIVI_IS_MESSAGE_BUS (bus), 0);
	g_return_val_if_fail (object_path != NULL, 0);
	g_return_val_if_fail (method != NULL, 0);
	g_return_val_if_fail (callback != NULL, 0);
	
	/* lookup the message and create if it does not exist yet */
	message = lookup_message (bus, object_path, method, TRUE);
	
	return add_listener (bus, message, callback, userdata, destroy_data);
}

/**
 * scrivi_message_bus_disconnect:
 * @bus: a #ScriviMessageBus
 * @id: the callback id as returned by scrivi_message_bus_connect()
 *
 * Disconnects a previously connected message callback.
 *
 */
void
scrivi_message_bus_disconnect (ScriviMessageBus *bus,
			      guint            id)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_id (bus, id, remove_listener);
}

/**
 * scrivi_message_bus_disconnect_by_func:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: the connected callback
 * @userdata: the userdata with which the callback was connected
 *
 * Disconnects a previously connected message callback by matching the 
 * provided callback function and userdata. See also 
 * scrivi_message_bus_disconnect().
 *
 */
void
scrivi_message_bus_disconnect_by_func (ScriviMessageBus      *bus,
				      const gchar	   *object_path,
				      const gchar	   *method,
				      ScriviMessageCallback  callback,
				      gpointer		    userdata)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_match (bus, object_path, method, callback, userdata, remove_listener);
}

/**
 * scrivi_message_bus_block:
 * @bus: a #ScriviMessageBus
 * @id: the callback id
 *
 * Blocks evoking the callback specified by @id. Unblock the callback by
 * using scrivi_message_bus_unblock().
 *
 */
void
scrivi_message_bus_block (ScriviMessageBus *bus,
			 guint		  id)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_id (bus, id, block_listener);
}

/**
 * scrivi_message_bus_block_by_func:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: the callback to block
 * @userdata: the userdata with which the callback was connected
 *
 * Blocks evoking the callback that matches provided @callback and @userdata.
 * Unblock the callback using scrivi_message_unblock_by_func().
 *
 */
void
scrivi_message_bus_block_by_func (ScriviMessageBus      *bus,
				 const gchar	      *object_path,
				 const gchar	      *method,
				 ScriviMessageCallback  callback,
				 gpointer	       userdata)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_match (bus, object_path, method, callback, userdata, block_listener);
}

/**
 * scrivi_message_bus_unblock:
 * @bus: a #ScriviMessageBus
 * @id: the callback id
 *
 * Unblocks the callback specified by @id.
 *
 */
void
scrivi_message_bus_unblock (ScriviMessageBus *bus,
			   guint	    id)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_id (bus, id, unblock_listener);
}

/**
 * scrivi_message_bus_unblock_by_func:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: the callback to block
 * @userdata: the userdata with which the callback was connected
 *
 * Unblocks the callback that matches provided @callback and @userdata.
 *
 */
void
scrivi_message_bus_unblock_by_func (ScriviMessageBus      *bus,
				   const gchar	        *object_path,
				   const gchar	        *method,
				   ScriviMessageCallback  callback,
				   gpointer	         userdata)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	
	process_by_match (bus, object_path, method, callback, userdata, unblock_listener);
}

static gboolean
validate_message (ScriviMessage *message)
{
	if (!scrivi_message_validate (message))
	{
		g_warning ("Message '%s.%s' is invalid", scrivi_message_get_object_path (message),
							 scrivi_message_get_method (message));
		return FALSE;
	}
	
	return TRUE;
}

static void
send_message_real (ScriviMessageBus *bus,
		   ScriviMessage    *message)
{
	if (!validate_message (message))
	{
		return;
	}
	
	bus->priv->message_queue = g_list_prepend (bus->priv->message_queue, 
						   g_object_ref (message));

	if (bus->priv->idle_id == 0)
		bus->priv->idle_id = g_idle_add_full (G_PRIORITY_HIGH,
						      (GSourceFunc)idle_dispatch,
						      bus,
						      NULL);
}

/**
 * scrivi_message_bus_send_message:
 * @bus: a #ScriviMessageBus
 * @message: the message to send
 *
 * This sends the provided @message asynchronously over the bus. To send
 * a message synchronously, use scrivi_message_bus_send_message_sync(). The 
 * convenience function scrivi_message_bus_send() can be used to easily send
 * a message without constructing the message object explicitly first.
 *
 */
void
scrivi_message_bus_send_message (ScriviMessageBus *bus,
			        ScriviMessage    *message)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (SCRIVI_IS_MESSAGE (message));
	
	send_message_real (bus, message);
}

static void
send_message_sync_real (ScriviMessageBus *bus,
                        ScriviMessage    *message)
{
	if (!validate_message (message))
	{
		return;
	}
	
	dispatch_message (bus, message);
}

/**
 * scrivi_message_bus_send_message_sync:
 * @bus: a #ScriviMessageBus
 * @message: the message to send
 *
 * This sends the provided @message synchronously over the bus. To send
 * a message asynchronously, use scrivi_message_bus_send_message(). The 
 * convenience function scrivi_message_bus_send_sync() can be used to easily send
 * a message without constructing the message object explicitly first.
 *
 */
void
scrivi_message_bus_send_message_sync (ScriviMessageBus *bus,
			             ScriviMessage    *message)
{
	g_return_if_fail (SCRIVI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (SCRIVI_IS_MESSAGE (message));

	send_message_sync_real (bus, message);	
}

static ScriviMessage *
create_message (ScriviMessageBus *bus,
		const gchar     *object_path,
		const gchar     *method,
		va_list          var_args)
{
	ScriviMessageType *message_type;
	
	message_type = scrivi_message_bus_lookup (bus, object_path, method);
	
	if (!message_type)
	{
		g_warning ("Could not find message type for '%s.%s'", object_path, method);
		return NULL;
	}

	return scrivi_message_type_instantiate_valist (message_type, 
						      var_args);
}

/**
 * scrivi_message_bus_send:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @...: NULL terminated list of key/value pairs
 *
 * This provides a convenient way to quickly send a message @method at 
 * @object_path asynchronously over the bus. The variable argument list 
 * specifies key (string) value pairs used to construct the message arguments. 
 * To send a message synchronously use scrivi_message_bus_send_sync().
 *
 */
void
scrivi_message_bus_send (ScriviMessageBus *bus,
			const gchar     *object_path,
			const gchar     *method,
			...)
{
	va_list var_args;
	ScriviMessage *message;
	
	va_start (var_args, method);

	message = create_message (bus, object_path, method, var_args);
	
	if (message)
	{
		send_message_real (bus, message);
		g_object_unref (message);
	}
	else
	{
		g_warning ("Could not instantiate message");
	}

	va_end (var_args);
}

/**
 * scrivi_message_bus_send_sync:
 * @bus: a #ScriviMessageBus
 * @object_path: the object path
 * @method: the method
 * @...: NULL terminated list of key/value pairs
 *
 * This provides a convenient way to quickly send a message @method at 
 * @object_path synchronously over the bus. The variable argument list 
 * specifies key (string) value pairs used to construct the message 
 * arguments. To send a message asynchronously use scrivi_message_bus_send().
 *
 * Return value: the constructed #ScriviMessage. The caller owns a reference
 *               to the #ScriviMessage and should call g_object_unref() when
 *               it is no longer needed
 */
ScriviMessage *
scrivi_message_bus_send_sync (ScriviMessageBus *bus,
			     const gchar     *object_path,
			     const gchar     *method,
			     ...)
{
	va_list var_args;
	ScriviMessage *message;
	
	va_start (var_args, method);
	message = create_message (bus, object_path, method, var_args);
	
	if (message)
		send_message_sync_real (bus, message);

	va_end (var_args);
	
	return message;
}

// ex:ts=8:noet:
