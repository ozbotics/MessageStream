#ifndef MESSAGE_STREAM_H
#define MESSAGE_STREAM_H

#include <Arduino.h>
//#include <Wire.h>

class MessageStream {
  public:
    MessageStream() {
      _available = false;
    }
        
    enum MessageStatus {
      SUCCESS_MESSAGE_STATUS = 1,
      FAILED_MESSAGE_STATUS  = 2,
      PENDING_MESSAGE_STATUS = 254,
      NO_DATA_MESSAGE_STATUS = 255 
    };

    
    
    void readRequest(char * request) { 
      //Serial.print(F("Reading request: "));
      //Serial.println(_requestBuffer);
      
      strcpy(request, _requestBuffer);
      _available = false;
    };
    
    virtual void writeResponse(char * response, byte status = SUCCESS_MESSAGE_STATUS) { 
      _responseBuffer[0] = status;
      strcpy(_responseBuffer+1, response);
      _setStatus(status);
    }
      
    virtual void receiveRequest() =0;

    virtual void writeRequest(char * request) =0;
    
    virtual MessageStatus readResponse(char * response,  byte responseSize);
    
    bool isAvailable() {
      return _available;
    }
    
  protected:
    static const byte _requestBufferSize = 32;
    static const byte _responseBufferSize = 32;
    
    char _requestBuffer[_requestBufferSize];
    char _responseBuffer[_responseBufferSize];
    
    volatile bool _available;
    
    byte _status = NO_DATA_MESSAGE_STATUS;

    void _setStatus(byte status) { _status = status; }
    
};

#endif