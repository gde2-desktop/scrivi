#ifndef __SCRIVI_MESSAGE_BUS_H__
#define __SCRIVI_MESSAGE_BUS_H__

#include <glib-object.h>
#include <scrivi/scrivi-message.h>
#include <scrivi/scrivi-message-type.h>

G_BEGIN_DECLS

#define SCRIVI_TYPE_MESSAGE_BUS			(scrivi_message_bus_get_type ())
#define SCRIVI_MESSAGE_BUS(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_MESSAGE_BUS, ScriviMessageBus))
#define SCRIVI_MESSAGE_BUS_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCRIVI_TYPE_MESSAGE_BUS, ScriviMessageBus const))
#define SCRIVI_MESSAGE_BUS_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SCRIVI_TYPE_MESSAGE_BUS, ScriviMessageBusClass))
#define SCRIVI_IS_MESSAGE_BUS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCRIVI_TYPE_MESSAGE_BUS))
#define SCRIVI_IS_MESSAGE_BUS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCRIVI_TYPE_MESSAGE_BUS))
#define SCRIVI_MESSAGE_BUS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SCRIVI_TYPE_MESSAGE_BUS, ScriviMessageBusClass))

typedef struct _ScriviMessageBus		ScriviMessageBus;
typedef struct _ScriviMessageBusClass	ScriviMessageBusClass;
typedef struct _ScriviMessageBusPrivate	ScriviMessageBusPrivate;

struct _ScriviMessageBus {
	GObject parent;
	
	ScriviMessageBusPrivate *priv;
};

struct _ScriviMessageBusClass {
	GObjectClass parent_class;
	
	void (*dispatch)		(ScriviMessageBus  *bus,
					 ScriviMessage     *message);
	void (*registered)		(ScriviMessageBus  *bus,
					 ScriviMessageType *message_type);
	void (*unregistered)		(ScriviMessageBus  *bus,
					 ScriviMessageType *message_type);
};

typedef void (* ScriviMessageCallback) 	(ScriviMessageBus *bus,
					 ScriviMessage	 *message,
					 gpointer	  userdata);

typedef void (* ScriviMessageBusForeach) (ScriviMessageType *message_type,
					 gpointer	   userdata);

GType scrivi_message_bus_get_type (void) G_GNUC_CONST;

ScriviMessageBus *scrivi_message_bus_get_default	(void);
ScriviMessageBus *scrivi_message_bus_new		(void);

/* registering messages */
ScriviMessageType *scrivi_message_bus_lookup	(ScriviMessageBus 	*bus,
						 const gchar		*object_path,
						 const gchar		*method);
ScriviMessageType *scrivi_message_bus_register	(ScriviMessageBus		*bus,
					   	 const gchar 		*object_path,
					  	 const gchar		*method,
					  	 guint		 	 num_optional,
					  	 ...) G_GNUC_NULL_TERMINATED;

void scrivi_message_bus_unregister	  (ScriviMessageBus	*bus,
					   ScriviMessageType	*message_type);

void scrivi_message_bus_unregister_all	  (ScriviMessageBus	*bus,
					   const gchar		*object_path);

gboolean scrivi_message_bus_is_registered  (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method);

void scrivi_message_bus_foreach		  (ScriviMessageBus        *bus,
					   ScriviMessageBusForeach  func,
					   gpointer		   userdata);


/* connecting to message events */		   
guint scrivi_message_bus_connect	 	  (ScriviMessageBus	*bus, 
					   const gchar		*object_path,
					   const gchar		*method,
					   ScriviMessageCallback	 callback,
					   gpointer		 userdata,
					   GDestroyNotify        destroy_data);

void scrivi_message_bus_disconnect	  (ScriviMessageBus	*bus,
					   guint		 id);

void scrivi_message_bus_disconnect_by_func (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method,
					   ScriviMessageCallback	 callback,
					   gpointer		 userdata);

/* blocking message event callbacks */
void scrivi_message_bus_block		  (ScriviMessageBus	*bus,
					   guint		 id);
void scrivi_message_bus_block_by_func	  (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method,
					   ScriviMessageCallback	 callback,
					   gpointer		 userdata);

void scrivi_message_bus_unblock		  (ScriviMessageBus	*bus,
					   guint		 id);
void scrivi_message_bus_unblock_by_func	  (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method,
					   ScriviMessageCallback	 callback,
					   gpointer		 userdata);

/* sending messages */
void scrivi_message_bus_send_message	  (ScriviMessageBus	*bus,
					   ScriviMessage		*message);
void scrivi_message_bus_send_message_sync  (ScriviMessageBus	*bus,
					   ScriviMessage		*message);
					  
void scrivi_message_bus_send		  (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method,
					   ...) G_GNUC_NULL_TERMINATED;
ScriviMessage *scrivi_message_bus_send_sync (ScriviMessageBus	*bus,
					   const gchar		*object_path,
					   const gchar		*method,
					   ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __SCRIVI_MESSAGE_BUS_H__ */

// ex:ts=8:noet:
