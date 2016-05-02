#include <MessageStream.h>
#include "I2CMessageStream.h"

//#define DEBUG_MESSAGE_STREAM

I2cMessageStream::I2cMessageStream(byte myI2cAddr, byte targetI2cAddr) : _myI2cAddr(myI2cAddr), _targetI2cAddr(targetI2cAddr), MessageStream() {
  if (TWCR == 0) {   // check that Wire.begin has not already been called
    if (_myI2cAddr > 0) {
      Wire.begin(_myI2cAddr);
    }
    else {
      Wire.begin();
    }
  }
}

void I2cMessageStream::writeRequest(char * request) {
  if (_targetI2cAddr > 0) {
    Wire.beginTransmission(_targetI2cAddr);
    Wire.write(request);
    Wire.endTransmission();
  }

#ifdef DEBUG_MESSAGE_STREAM
  Serial.print(F("writeRequest: "));
  Serial.println(request);
#endif
} 

void I2cMessageStream::receiveRequest() { 
  byte count = 0;
  while(Wire.available()) {
    if (count < (_requestBufferSize +1)) {
      _requestBuffer[count] = Wire.read();
      count++;
    }
    else {
      Wire.read(); // ignore over-run
    }

  }
  _requestBuffer[count] = 0; // null terminate
  
  _available = true;
  _setStatus(PENDING_MESSAGE_STATUS);
  
}

void I2cMessageStream::sendResponse() { 
  Wire.write(_responseBuffer);
  
  _setStatus(NO_DATA_MESSAGE_STATUS);
}

MessageStream::MessageStatus I2cMessageStream::readResponse(char * response,  byte responseSize) {
  byte count = 0;
  MessageStatus status; 
  
  Wire.requestFrom((int)_targetI2cAddr, (int)responseSize+1);

  if (Wire.available()) {
    status = (MessageStatus) Wire.read();
  }
  
  while (Wire.available()) {
    if (count < responseSize) {
      response[count] = Wire.read();
      
      if (response[count] < 0) {
        response[count] = 0;
      }

      count++;
    }
    else {
      Wire.read(); // ignore over-run
    }

  }
  response[count] = 0; // null terminate
 
  // debug
#ifdef DEBUG_MESSAGE_STREAM
  Serial.print(F("readResponse: "));
  Serial.println(response);
#endif 
 
  return status;
}
