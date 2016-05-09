/** @file SerialMessageStream.h 
  *  Copyright (c) 2016 Ozbotics 
  *  Distributed under the MIT license (see LICENSE)
  */ 
#ifndef SERIAL_MESSAGE_STREAM_H
 #define SERIAL_MESSAGE_STREAM_H

#include <Arduino.h>
#include <MessageStream.h>
//#define DEBUG_MESSAGE_STREAM

/**
 * Implements MessageStream over Serial
 *
 * Provides the MessageStream interface over Serial
 */
class SerialMessageStream : public MessageStream {
  protected:
    HardwareSerial& _serial;                        /**< protected variable  _serial The serial stream */ 
    byte _countResponse = 0;                        /**< protected variable  _countResponse The response string length */ 
    byte _countRequest = 0;                         /**< protected variable  _countRequest The request string length */ 
    char _requestAccumulator[_requestBufferSize];   /**< protected variable  _requestAccumulator Buffer to accumulate incoming string */ 
  
  public:
   /**
    * Constructor
    *
    * @param serial The serial stream
   */
    SerialMessageStream(HardwareSerial& serial) : _serial(serial), MessageStream() { }

   /**
    * Called when a Request is sent
    *
    * @param request The outgoing request string
    */
    void writeRequest(char * request) {
#ifdef DEBUG_MESSAGE_STREAM
Serial.print(F("SerialMessageStream::writeRequest Atempting to writeRequest: "));      
Serial.println(request);      
#endif

      _serial.println(request);
      
      _responseBuffer[0] = PENDING_MESSAGE_STATUS;
    } 
    
   /**
    * Called when a Request is incoming
    */
    void receiveRequest() { 
      bool _requestAccumulated = false;
      
      while(_serial.available()) {
        if (_countRequest < (_requestBufferSize +1)) {
          _requestAccumulator[_countRequest] = _serial.read();

          #ifdef DEBUG_MESSAGE_STREAM
          Serial.print(_requestAccumulator[_countRequest]);
          Serial.print(" ");
          Serial.print((byte) _requestAccumulator[_countRequest]);
          Serial.print(", ");
          #endif

         
          if (_requestAccumulator[_countRequest] == '\n') {
            _requestAccumulated = true;
            _available = true;
          }
          
          _countRequest++;
        }
      }

      if (_requestAccumulated == true) {
        _requestAccumulator[_countRequest] = 0;
        
        strcpy(_requestBuffer, _requestAccumulator);
        
        _countRequest = 0;
        _setStatus(PENDING_MESSAGE_STATUS);

#ifdef DEBUG_MESSAGE_STREAM
        Serial.print(F("Handled receiveRequest, got "));
        Serial.println(_requestBuffer);
#endif
      }
    }

    
   /**
    * Called when a Response is outgoing
    */
    void sendResponse() { 
      _serial.write(_responseBuffer);
      _serial.write('\n');
      
      _setStatus(NO_DATA_MESSAGE_STATUS);
    }

    
   /**
    * Called to send a Response
    */
    void writeResponse(char * response, byte _status = SUCCESS_MESSAGE_STATUS){
      MessageStream::writeResponse(response, _status);
      sendResponse();
    }
    
   /**
    * Called when a Response is incoming
    */
    void receiveResponse() {
      char c;
      
      while(_serial.available()) {
        c = _serial.read();
        if (_countResponse < (_responseBufferSize +1)) {
          _responseBuffer[_countResponse] = c;
          _countResponse++;
        }

        if (c == '\n') {
          _available = true;
        }
       
      }
      
      if (_available) {      
        _responseBuffer[_countResponse] = 0; // null terminate
        
#ifdef DEBUG_MESSAGE_STREAM
        Serial.print(F("receiveResponse: "));
        Serial.println(_responseBuffer);
#endif
      }
    }

   /**
    * Called when a Reaponae is incoming
    *
    * subclasses are required to implement readResponse()
    *
    * @param response The incoming response string
    * @param responseSize The expected size of the response string
    */
    MessageStatus readResponse(char * response,  byte responseSize) {
#ifdef DEBUG_MESSAGE_STREAM
      Serial.print(F("readResponse: "));
      Serial.print((byte) _responseBuffer[0]);
      Serial.print(F(", "));
      Serial.println(_responseBuffer+1);
#endif

      MessageStatus status = (MessageStatus) _responseBuffer[0];
      strcpy(response, _responseBuffer+1);
      
      _responseBuffer[0] = NO_DATA_MESSAGE_STATUS;
      _available = false;
    }
    

};

#endif