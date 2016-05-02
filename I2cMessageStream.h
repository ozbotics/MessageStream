#ifndef I2C_MESSAGE_STREAM_H
  #define I2C_MESSAGE_STREAM_H

#include <Arduino.h>
#include <Wire.h>
#include <MessageStream.h>

class I2cMessageStream : public MessageStream {
  protected:
    byte _myI2cAddr;
    byte _targetI2cAddr;
  
  public:
    I2cMessageStream(byte myI2cAddr = 0, byte targetI2cAddr = 0);
    void writeRequest(char * request);
    void receiveRequest();
    void sendResponse();
    MessageStatus readResponse(char * response,  byte responseSize);
    byte getI2cAddress() { return _targetI2cAddr; }
};



#endif