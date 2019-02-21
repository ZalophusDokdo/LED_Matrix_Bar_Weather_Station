/* =========================================================================
 *  Author: Zalophus Dokdo (https://zddh.blogspot.com)
 *  Date: 31/08/2017
 *  License: GPL v2
 * =========================================================================
 *  LED Matrix Bar Weather Station
 * =========================================================================
 *  WeMos D1 mini I2C Library
 * =========================================================================
 */
#ifndef ESP_I2C_H
#define ESP_I2C_H

#include "core_esp8266_si2c.c"

//////////////////////////////////////////////
/* Services Section I2C bus routines        */
//////////////////////////////////////////////

// 1000ms default read timeout (modify with "I2CDEV_DEFAULT_READ_TIMEOUT")
#define I2CDEV_DEFAULT_READ_TIMEOUT     1000
static uint16_t readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;

#define BUFFER_LENGTH   32

uint8_t rxBuffer[BUFFER_LENGTH];
uint8_t rxBufferIndex   = 0;
uint8_t rxBufferLength  = 0;

uint8_t txAddress       = 0;
uint8_t txBuffer[BUFFER_LENGTH];
uint8_t txBufferIndex   = 0;
uint8_t txBufferLength  = 0;

uint8_t transmitting    = 0;

unsigned char i2c_writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop)
{
  unsigned int i;
  if(!twi_write_start()) return 4;                            // line busy
  if(!twi_write_byte(((address << 1) | 0) & 0xFF)) return 2;  // received NACK on transmit of address
  for(i=0; i<len; i++)
  {
    if(!twi_write_byte(buf[i])) return 3;                     // received NACK on transmit of data
  }
  SCL_LOW();
  twi_delay(twi_dcount);
  if(sendStop) twi_write_stop();
  return 0;
}

unsigned char i2c_readFrom(unsigned char address, unsigned char* buf, unsigned int len, unsigned char sendStop)
{
  unsigned int i;
  if(!twi_write_start()) return 4;                            // line busy
  if(!twi_write_byte(((address << 1) | 1) & 0xFF)) return 2;  // received NACK on transmit of address
  for(i=0; i<(len-1); i++)
  {
    buf[i] = twi_read_byte(false);
  }
  buf[len-1] = twi_read_byte(true);
  SCL_LOW();
  twi_delay(twi_dcount);
  if(sendStop) twi_write_stop();
  return 0;
}

void i2c_flush() {
  rxBufferIndex  = 0;
  rxBufferLength = 0;
  txBufferIndex  = 0;
  txBufferLength = 0;
}

void i2c_begin(int sda, int scl, uint32_t frequency) {
  twi_init(sda, scl);
  twi_setClock(frequency);
  i2c_flush();
}

void i2c_beginTransmission(uint8_t address) {
  transmitting   = 1;
  txAddress      = address;
  txBufferIndex  = 0;
  txBufferLength = 0;
}

size_t i2c_write(uint8_t data) {
  if(transmitting) {
    if(txBufferLength >= BUFFER_LENGTH) return 0;
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    txBufferLength = txBufferIndex;
  }
  return 1;
}

uint8_t i2c_endTransmission(uint8_t sendStop) {
  int8_t ret     = i2c_writeTo(txAddress, txBuffer, txBufferLength, sendStop);
  txBufferIndex  = 0;
  txBufferLength = 0;
  transmitting   = 0;
  return ret;
}

size_t i2c_requestFrom(uint8_t address, size_t size, bool sendStop) {
  if(size > BUFFER_LENGTH) {
    size = BUFFER_LENGTH;
  }
  size_t ret    = (i2c_readFrom(address, rxBuffer, size, sendStop) == 0)?size:0;
  rxBufferIndex  = 0;
  rxBufferLength = ret;
  return ret;
}

int i2c_available(void) {
  int result = rxBufferLength - rxBufferIndex;

  if (!result) {
    // yielding here will not make more data "available",
    // but it will prevent the system from going into WDT reset
    optimistic_yield(1000);
  }
  return result;
}

int i2c_read(void) {
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

int8_t i2c_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout=readTimeout)
{
    int8_t count = 0;
    uint32_t t1 = millis();
    for (uint8_t k = 0; k < length; k += min(length, BUFFER_LENGTH))
    {
      i2c_beginTransmission(devAddr);
      i2c_write(regAddr);
      i2c_endTransmission(false);
      
      i2c_beginTransmission(devAddr);
      i2c_requestFrom(devAddr, (uint8_t) min(length - k, BUFFER_LENGTH), true);
      
      for (; i2c_available() && (timeout == 0 || millis() - t1 < timeout); count++)
      {
        data[count] = i2c_read();
      }
    }
    // check for timeout
    if (timeout > 0 && millis() - t1 >= timeout && count < length) count = -1;  // timeout
    return count;
} 

int8_t i2c_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout=readTimeout) 
{
    return i2c_ReadBytes(devAddr, regAddr, 1, data, timeout);
}

bool i2c_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t* data) 
{
    uint8_t status = 0;
    i2c_beginTransmission(devAddr);
    i2c_write(regAddr);                    // send address
    for (uint8_t i = 0; i < length; i++) {
      i2c_write((uint8_t) data[i]);
    }
    status = i2c_endTransmission(true);
    return status == 0;
}

bool i2c_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) 
{
    return i2c_WriteBytes(devAddr, regAddr, 1, &data);
}

bool i2c_WriteBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) 
{
    uint8_t b;
    i2c_ReadByte(devAddr, regAddr, &b, 0);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return i2c_WriteByte(devAddr, regAddr, b);
}

bool i2c_WriteBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    uint8_t b;
    if (i2c_ReadByte(devAddr, regAddr, &b, 0) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);  // shift data into correct position
        data &= mask;                      // zero all non-important bits in data
        b &= ~(mask);                      // zero all important bits in existing byte
        b |= data;                         // combine data with existing byte
        return i2c_WriteByte(devAddr, regAddr, b);
    } else {
        return false;
    }
}

int8_t i2c_ReadBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout=readTimeout) 
{
    uint8_t b;
    uint8_t count = i2c_ReadByte(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}

int8_t i2c_ReadBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout=readTimeout) 
{
    uint8_t count, b;
    if ((count = i2c_ReadByte(devAddr, regAddr, &b, timeout)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    return count;
}

#endif  // ESP_I2C_H
