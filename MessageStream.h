/** @file MessageStream.h 
  *  Copyright (c) 2016 Ozbotics 
  *  Distributed under the MIT license (see LICENSE)
  */ 
#ifndef MESSAGE_STREAM_H
 #define MESSAGE_STREAM_H

#include <Arduino.h>

/**
 * The base of all MessageStreams
 *
 * Requires that subclasses implement receiveRequest() & writeRequest()
 *
 * Provides uniform interface to transmit messages along a stream
 *  Regardless of the underlying communications protocol
 */
class MessageStream {
  public:
   /**
    * Constructor
    */
    MessageStream() {
      _available = false;
    }
    
   /**
    * MessageStatus
    */
    enum MessageStatus {
      SUCCESS_MESSAGE_STATUS = 1,   /**< MessageStatus::SUCCESS_MESSAGE_STATUS = 1 */
      FAILED_MESSAGE_STATUS  = 2,   /**< MessageStatus::FAILED_MESSAGE_STATUS  = 2 */
      PENDING_MESSAGE_STATUS = 254, /**< MessageStatus::PENDING_MESSAGE_STATUS = 254 */
      NO_DATA_MESSAGE_STATUS = 255  /**< MessageStatus::NO_DATA_MESSAGE_STATUS = 255 */
    };
    
   /**
    * read an incoming request
    *
    * @param request Buffer for incoming request string
    */
    void readRequest(char * request) { 
      strcpy(request, _requestBuffer);
      _available = false;
    };
    
   /**
    * write a response
    *
    * @param response Outgoing response string
    * @param status The MessageStatus, defualts to SUCCESS_MESSAGE_STATUS
    */
    virtual void writeResponse(char * response, byte status = SUCCESS_MESSAGE_STATUS) { 
      _responseBuffer[0] = status;
      strcpy(_responseBuffer+1, response);
      _setStatus(status);
    }

   /**
    * Called when a Request is incoming
    *
    * subclasses are required to implement receiveRequest()
    */
    virtual void receiveRequest() =0;

   /**
    * Called when a Request is sent
    *
    * subclasses are required to implement writeRequest()
    *
    * @param request The outgoing request string
    */
    virtual void writeRequest(char * request) =0;
    
   /**
    * Called when a Reaponae is incoming
    *
    * subclasses are required to implement readResponse()
    *
    * @param response The incoming response string
    * @param responseSize The expected size of the response string
    */
    virtual MessageStatus readResponse(char * response,  byte responseSize) =0;
    
   /**
    * is there a Message available?
    *
    * @return true if a message is available
    */
    bool isAvailable() {
      return _available;
    }
    
  protected:
    static const byte _requestBufferSize = 32;   /**< protected const  _requestBufferSize The size of the RequestBuffer */ 
    static const byte _responseBufferSize = 32;  /**< protected const  _responseBufferSize The size of the ResponseBufferSize */
    
    char _requestBuffer[_requestBufferSize];     /**< protected const  _requestBufferSize The Request Buffer */ 
    char _responseBuffer[_responseBufferSize];   /**< protected const  _requestBufferSize The Response Buffer */ 
    
    volatile bool _available;                    /**< protected volatile variable  _available Message is available */ 
    
    byte _status = NO_DATA_MESSAGE_STATUS;       /**< protected variable  _status Current MessageStatus */ 

   /**
    * is there a Message available?
    *
    * @param status The new MessageStatus
    */
    void _setStatus(byte status) { _status = status; }
    
};

#endif