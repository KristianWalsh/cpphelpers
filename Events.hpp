#ifndef __EVENTS_HPP__
#define __EVENTS_HPP__

#include <vector>
#include <map>
#include <inttypes.h>
#include <typeinfo>

#include "LockableObject.hpp"

typedef const std::type_info* EventType;

/*
    Events
    ------

      Event-passing mechanism.


    Usage:  


      while (1)
      {
        EventDispatcher::process();
        // any objects implementing PeriodicallyCalled could also be called here.
        ob1.periodic();
        ob2.periodic();
        usleep(1202057);  // delay is there to avoid idle-spinning. Set as appropriate to needs.
      }

 */




  /*!
    Event is the base class for any sendable event.
  */
class Event
  {
  public:
    virtual ~Event( void );
  };






  /*!
    EventDispatcher handles the transfer of events to their registered responders.  
  */
class EventDispatcher
  : public LockableObject
{
public:
  /*!
    Base class for event conduits. These are created by the EventResponder mix-in classes, so this class
    can be safely ignored by end-user code.
  */
  class EventConduit
    {
    public:
      virtual void callObjectEventHandler( const Event* eventData ) = 0;
      virtual ~EventConduit( )  { };  // keeping GCC happy here by adding an explicit destructor
    };

private:
  typedef std::vector<EventDispatcher::EventConduit*> ResponderListForEvent;
  typedef std::map<EventType, ResponderListForEvent> EventRegistryForSender;
  typedef std::map<void*, EventRegistryForSender> EventRegistry;  // sender --> events
  EventRegistry allResponders; 

  struct EventQueueEntry
  {
    const Event* event;
    void* sender;
    EventQueueEntry() : event(0), sender(0) {}
    EventQueueEntry(const Event* e, void* s=0) : event(e),sender(s) {}
  };

  typedef std::vector< EventQueueEntry> EventQueue;
  EventQueue eventQueue;
  EventQueue deferredEventQueue; // filled by postEventLater; events that are to be processed next time processEvents is called.
      
  EventDispatcher( );
  ~EventDispatcher();
  
  void processEvents_( void );

public:
  static EventDispatcher& GetInstance( void );
  void subscribe( EventType theEventType, EventConduit* responderConduit, void* onlyFromThisSender=0 ); // subscribe to sender==0 means "subscribe to broadcasts"
  void unsubscribe( EventType theEventType, EventConduit* responderObject, void* fromThisSender=0 ); // 0 means broadcasts
  void unsubscribeAll( EventType theEventType, EventConduit* responderObject );


    /*
      post an Event to the queue.  This event will be added to the end of the event queue and
      will be processed, in order, by the current event-queue processing operation if one
      is in progress (i.e., if this event was posted in response to receiving another) 
      (see also postEventLater())

      if sender is non-NULL, then the event distribution is selective: only those event responders 
      that have subscribed to events from "sender" will receive this event.
      If sender is NULL, then the event is a broadcast, and is sent to all objects that 
      implement EventResponder< typeof eventObject >
    */
  void postEvent( const Event* eventObject, void* sender );

    /*!
      Sometimes, event handlers (called from processEvents) do not want an event they post to be processed right now, as
      doing so could cause an endless cycle of event, response-event, event...

      Events posted by this function will not be handled until the NEXT call to processEvents. 
      In all other respects, it functions the same as postEvent()
    */
  void postEventLater( const Event* eventObject, void* sender );

    /* 
      broadcast() and send() are convenience static methods for the broadcast and specific-listeners versions 
      of postEvent().  broadcastLater() and sendLater() are the equivalent convenience methods for postEventLater()
    */

    /*!
      broadcast() queues the event to be sent to all listeners to that event.

      Callers to broadcast pass ownership of 'theEvent' to EventDispatcher; callers
      must not attempt to delete 'theObject' after calling PostEvent! 
    */
  static void broadcast( const Event* theObject ) { instance().postEvent(theObject,0); }

    // broadcastLater() quees
  static void broadcastLater( const Event* theObject ) { instance().postEventLater(theObject,0); }

    /*!
      send differs from 'broadcast' in that the sender is specified, and only those
      objects that have subscribed to that specific sender will recieve notifications. 
        As with 'broadcast', ownership of "theObject" is passed to EventDispatcher.
    */
  static void send( const Event* theObject, void* sender  ) { instance().postEvent(theObject, sender); }
  static void sendLater( const Event* theObject, void* sender  ) { instance().postEventLater(theObject, sender); }

    /*!
      Normally called only by the main application loop, this causes
      all events currently in the queue to be processed.
      
      process() will also process any events which were posted by event responders
      in response those in the queue when process() was called
    */
  static void process( void ) { instance().processEvents_(); }

    /* For constructing EventResponders */
  enum Mode { AcceptBroadcasts, NoBroadcasts };

  static EventDispatcher& instance();

};




    /*! 
    @discussion 
      EventResponder is a mix-in class which provides a mechanism for responding to ONE subclass of Event. 
      For a client object to respond to more than one Event,
     different EventResponder templates must be mixed in. (eg. "class MyMonitor: public EventResponder<SignalDetectedEvent>, 
     EventResponder<WindowMovedEvent>, etc. etc.")
     EVENTOBJECT is a class which must be derived from Event
  */
template<class EVENTOBJECT> class EventResponder 
{
private:
      /*! 
      An EventConduit object carries events from EventDispatcher to recipients of the event 
      (i.e., the mixin-suppied virtual function RespondToEvent( EVENTOBJECT& ):
    */
  class ResponderEventConduit : public EventDispatcher::EventConduit
  {
    private:
      EventResponder* targetObject;
    
    public:
      ResponderEventConduit( EventResponder<EVENTOBJECT>* target ) : targetObject( target ) { };
      
      virtual void callObjectEventHandler( const Event* eventData )
      {
        targetObject->respond( *(reinterpret_cast<const EVENTOBJECT*>(eventData)) );  // this is a virtual function call, and so will get the right handler.
      };
  };

ResponderEventConduit myConduit;

public:
    /*!
      Constructor installs the event handler
    */
  EventResponder( enum EventDispatcher::Mode m=EventDispatcher::AcceptBroadcasts )
    : myConduit( this )
  {
    if (m==EventDispatcher::AcceptBroadcasts)
    {
      subscribe( *this, 0);
      //EventDispatcher::instance().subscribe( &typeid(EVENTOBJECT), &myConduit, 0 );
    }
  }


  /*!
    Destructor removes event handler
  */
  virtual ~EventResponder( )
  {
    EventDispatcher::instance().unsubscribeAll( &typeid(EVENTOBJECT), &myConduit );
  };

  void subscribe( EventResponder<EVENTOBJECT>& self, void* sender ) { EventDispatcher::instance().subscribe( &typeid(EVENTOBJECT), &myConduit, sender ); }
  void unsubscribe( EventResponder<EVENTOBJECT>& self, void* sender ) { EventDispatcher::instance().unsubscribe( &typeid(EVENTOBJECT), &myConduit, sender ); }

/*! 
  The action to be taken must be implemented here by class which takes this mixin. 
  */
  virtual void respond( const EVENTOBJECT& theEvent ) =0;
};


/*!
  Shorthand macros for defining events
*/
#define DECLARE_EVENT( EVENTNAME ) class EVENTNAME : public Event { public: EVENTNAME(){ };  virtual ~EVENTNAME(){ }; };

  /*! Value events carry a single data parameter, and have a read-only accessor to retrieve this */
#define DECLARE_VALUE_EVENT( EVENTNAME, VALUETYPE, VALUENAME  ) class EVENTNAME : public Event { private: VALUETYPE value; public: EVENTNAME( const VALUETYPE& newVal ) : value( newVal ) { };  virtual ~EVENTNAME(){ }; const VALUETYPE& VALUENAME(void) const { return value; }; };

/*! Value pointer events carry a pointer to a single data parameter (no copy is made in the event object ), and have a read-only accessor to retrieve this */
#define DECLARE_VALUE_PTR_EVENT( EVENTNAME, VALUETYPE, VALUENAME  ) class EVENTNAME : public Event { private: const VALUETYPE* value; public: EVENTNAME( const VALUETYPE& newVal ) : value( &newVal ) { };  virtual ~EVENTNAME(){ }; const VALUETYPE& VALUENAME(void) const { return *value; }; };


#define EVENT_SUBSCRIBE( SUBSCRIBER, EVENTTYPE, SENDER ) (SUBSCRIBER).subscribe( *(static_cast< EventResponder< EVENTTYPE >* >(& (SUBSCRIBER ))), (void *)( SENDER ) )
#define EVENT_UNSUBSCRIBE( SUBSCRIBER, EVENTTYPE, SENDER ) (SUBSCRIBER).unsubscribe( *(static_cast<EventResponder< EVENTTYPE >*>( & (SUBSCRIBER ))), (void *)( SENDER ) )

#endif
