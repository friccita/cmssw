// $Id: EventDistributor.h,v 1.2 2009/06/10 08:15:22 dshpakov Exp $
/// @file: EventDistributor.h 

#ifndef StorageManager_EventDistributor_h
#define StorageManager_EventDistributor_h

#include "EventFilter/StorageManager/interface/ErrorStreamConfigurationInfo.h"
#include "EventFilter/StorageManager/interface/ErrorStreamSelector.h"
#include "EventFilter/StorageManager/interface/EventQueueCollection.h"
#include "EventFilter/StorageManager/interface/EventConsumerRegistrationInfo.h"
#include "EventFilter/StorageManager/interface/EventConsumerSelector.h"
#include "EventFilter/StorageManager/interface/EventStreamConfigurationInfo.h"
#include "EventFilter/StorageManager/interface/EventStreamSelector.h"
#include "EventFilter/StorageManager/interface/DQMEventSelector.h"
#include "EventFilter/StorageManager/interface/I2OChain.h"
#include "EventFilter/StorageManager/interface/QueueID.h"
#include "EventFilter/StorageManager/interface/SharedResources.h"

#include "boost/shared_ptr.hpp"


namespace stor {

  /**
   * Distributes complete events to appropriate queues
   *
   * It receives complete events in form of I2OChains and
   * distributes it to the appropriate queues by checking
   * the I2O message type and the trigger bits in the event
   * header.
   *
   * $Author: dshpakov $
   * $Revision: 1.2 $
   * $Date: 2009/06/10 08:15:22 $
   */

  class EventDistributor
  {
  public:

    EventDistributor(SharedResourcesPtr sr);

    ~EventDistributor();

    /**
     * Add the event given as I2OChain to the appropriate queues
     */
    void addEventToRelevantQueues( I2OChain& );

    /**
     * Returns true if no further events can be processed,
     * e.g. the StreamQueue is full
     */
    const bool full() const;

    /**
     * Registers a new consumer
     */
    void registerEventConsumer( const EventConsumerRegistrationInfo* );

    /**
     * Registers a new DQM consumer
     */
    void registerDQMEventConsumer( const DQMEventConsumerRegistrationInfo* );

    /**
     * Registers the full set of event streams.
     */
    void registerEventStreams( const EvtStrConfigListPtr );

    /**
     * Registers the full set of error event streams.
     */
    void registerErrorStreams( const ErrStrConfigListPtr );

    /**
     * Clears out all existing event and error streams.
     */
    void clearStreams();

    /**
     * Returns the number of streams that have been configured.
     */
    unsigned int configuredStreamCount() const;

    /**
     * Returns the number of streams that have been configured and initialized.
     */
    unsigned int initializedStreamCount() const;

    /**
     * Clears out all existing consumer registrations.
     */
    void clearConsumers();

    /**
     * Returns the number of consumers that have been configured.
     */
    unsigned int configuredConsumerCount() const;

    /**
     * Returns the number of consumers that have been configured and initialized.
     */
    unsigned int initializedConsumerCount() const;

    /**
       Updates staleness info for consumers.
    */
    void checkForStaleConsumers();

  private:

    void tagCompleteEventForQueues( I2OChain& );

    SharedResourcesPtr _sharedResources;

    typedef std::vector<EventStreamSelector> EvtSelList;
    EvtSelList _eventStreamSelectors;

    typedef std::vector<DQMEventSelector> DQMEvtSelList;
    DQMEvtSelList _dqmEventSelectors;

    typedef std::vector<ErrorStreamSelector> ErrSelList;
    ErrSelList _errorStreamSelectors;

    typedef std::vector<EventConsumerSelector> ConsSelList;
    ConsSelList _eventConsumerSelectors;

    // temporary
    std::vector<unsigned char> _tempEventArea;

  };
  
} // namespace stor

#endif // StorageManager_EventDistributor_h 


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
