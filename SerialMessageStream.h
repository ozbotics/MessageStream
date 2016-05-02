#ifndef SERIAL_MESSAGE_STREAM_H
#define SERIAL_MESSAGE_STREAM_H

#include <Arduino.h>
#include <MessageStream.h>
// #define DEBUG_MESSAGE_STREAM

class SerialMessageStream : public MessageStream {
  protected:
    HardwareSerial * _stream;
    byte _countResponse = 0;
    byte _countRequest = 0;
    char _requestAccumulator[_requestBufferSize];
  
  public:
    SerialMessageStream(HardwareSerial * stream) : _stream(stream), MessageStream() { }

    void writeRequest(char * request) {
      _stream->write(request);
      _responseBuffer[0] = PENDING_MESSAGE_STATUS;
    } 
    
    void receiveRequest() { 
      //byte countRequest = 0;
      bool _requestAccumulated = false;
      
      while(_stream->available()) {
        if (_countRequest < (_requestBufferSize +1)) {
          _requestAccumulator[_countRequest] = _stream->read();

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
//        else {
//          _stream->read(); // ignore over-run
//        }
        
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

    
    
    void sendResponse() { 
      _stream->write(_responseBuffer);
      _stream->write('\n');
      
      _setStatus(NO_DATA_MESSAGE_STATUS);
    }

    void writeResponse(char * response, byte _status = SUCCESS_MESSAGE_STATUS){
      MessageStream::writeResponse(response, _status);
      sendResponse();
    }
    
    void receiveResponse() {
      char c;
      
      while(_stream->available()) {
        c = _stream->read();
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