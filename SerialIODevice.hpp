#ifndef SerialIODevice_HPP_xo123
#define SerialIODevice_HPP_xo123

#include "IODevice.hpp"
#include "SerialPort.hpp"

class SerialIODevice
: public IODevice, public SerialPort 
{
private:
  int intercharTime;

public:
  SerialIODevice( const char* devicePath, uint32_t baud=9600, int32_t intercharMillisecs=-1, uint8_t databits=8, SerialPort::ParitySetting parity=SerialPort::kNoParity, uint8_t stopBits=1);
  virtual ~SerialIODevice();

  virtual bool isOpen() const { return good(); }
  virtual bool atEnd() { return false; }

  virtual size_t read( uint8_t * data, size_t maxSize );

  virtual bool waitForReadyRead( int millisecs);

  virtual size_t write( const uint8_t * data, size_t count );

 }; 

#endif
