/** @file SDFileMessageStream.h 
  *  Copyright (c) 2017 Ozbotics 
  *  Distributed under the MIT license (see LICENSE)
  */ 
#ifndef SDFILE_MESSAGE_STREAM_H
 #define SDFILE_MESSAGE_STREAM_H

#include <Arduino.h>
#include <MessageStream.h>

//#define DEBUG_SDFILE_MESSAGE_STREAM

/**
 * Implements MessageStream to SD File
 *
 * Provides the MessageStream interface to SD File
 * (Write access only)
 */
class SDFileMessageStream : public MessageStream {
  protected:
    byte _countResponse = 0;                        /**< protected variable  _countResponse The response string length */ 
    byte _countRequest = 0;                         /**< protected variable  _countRequest The request string length */ 
    char _requestAccumulator[_requestBufferSize];   /**< protected variable  _requestAccumulator Buffer to accumulate incoming string */ 
    char* _filename;
    
  public:
   /**
    * Constructor
    *
    * @param serial The serial stream
   */
    SDFileMessageStream(char* filename) : _filename(filename), MessageStream() { }

   /**
    * Called when a Request is sent
    *
    * @param request The outgoing request string
    */
    void writeRequest(char * request) {
#ifdef DEBUG_SDFILE_MESSAGE_STREAM
Serial.print(F("SDFileMessageStream::writeRequest Attempting to writeRequest: "));      
Serial.println(request);      
#endif

      File file = SD.open(_filename, FILE_WRITE);
      
      if (file) {
        file.println(request);
        file.close();
        // print to the serial port too:
#ifdef DEBUG_SDFILE_MESSAGE_STREAM
        Serial.print("Writing to SD :");
        Serial.println(request);
#endif
      }
      else {
#ifdef DEBUG_SDFILE_MESSAGE_STREAM
        Serial.println("Failed to open SD file");
#endif
      }
      
      _responseBuffer[0] = PENDING_MESSAGE_STATUS;
    } 
    
   /**
    * Called when a Request is incoming
    */
    void receiveRequest() {}

    
   /**
    * Called when a Response is outgoing
    */
    void sendResponse() {}

    
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
    void receiveResponse() {}

   /**
    * Called when a Reaponae is incoming
    *
    * subclasses are required to implement readResponse()
    *
    * @param response The incoming response string
    * @param responseSize The expected size of the response string
    */
    MessageStatus readResponse(char * response,  byte responseSize) {}
    

};

#endif