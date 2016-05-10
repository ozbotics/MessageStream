/** @file MessageStream.h 
  *  Copyright (c) 2016 Ozbotics 
  *  Distributed under the MIT license (see LICENSE)
  */ 
#ifndef I2C_MESSAGE_STREAM_H
  #define I2C_MESSAGE_STREAM_H

#include <Arduino.h>
#include <Wire.h>
#include <MessageStream.h>

/**
 * Implements MessageStream over I2C
 *
 * Provides the MessageStream interface over I2C
 */
class I2cMessageStream : public MessageStream {
  protected:
    byte _myI2cAddr;       /**< protected variable  _myI2cAddr     my i2c address (if any) */ 
    byte _targetI2cAddr;   /**< protected variable  _targetI2cAddr my target address (if any) */ 
  
  public:
   /**
    * Constructor
    *
    * @param myI2cAddr My i2c address (if any) 
    * @param targetI2cAddr My i2c address (if any) 
    */
    I2cMessageStream(byte myI2cAddr = 0, byte targetI2cAddr = 0);
    
   /**
    * Called when a Request is sent
    *
    * @param request The outgoing request string
    */
    void writeRequest(char * request);

   /**
    * Called when a Request is incoming
    */
    void receiveRequest();
    
    
   /**
    * Called when a Response is outgoing
    */
    void sendResponse();
    
   /**
    * Called when a Reaponae is incoming
    *
    * subclasses are required to implement readResponse()
    *
    * @param response The incoming response string
    * @param responseSize The expected size of the response string
    */
    MessageStatus readResponse(char * response,  byte responseSize);
    
   /**
    * get the I2C address of the target
    *
    * @return the i2c address of the target
    */
    byte getI2cAddress() { return _targetI2cAddr; }
};



#endif