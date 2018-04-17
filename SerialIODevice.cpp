#include <stdio.h>
#include "SerialIODevice.hpp"

///////////////////////////////////////////////////////////////////////

SerialIODevice::SerialIODevice( const char* devicePath, uint32_t baud, int32_t intercharMillisecs, uint8_t databits, SerialPort::ParitySetting parity, uint8_t stopBits)
  : SerialPort(devicePath,baud,databits,parity,stopBits)
{
  if (intercharMillisecs<0)
  {
    intercharTime = (1000000.0/(double)baud)*200.0; // wait roughly 20 character times between chars.
  }
  else
  {
    intercharTime = intercharMillisecs*1000;
  }
  printf("Interchar time = (1000000/%d)*60=%d usec\n", baud,intercharTime);
}


///////////////////////////////////////////////////////////////////////

SerialIODevice::~SerialIODevice( )
{

}


///////////////////////////////////////////////////////////////////////

size_t SerialIODevice::read( uint8_t * data, size_t maxSize )
{
  if (isOpen())
  {
    return SerialPort::readBytes( data, maxSize, intercharTime);
  }
  else
  {
    return 0;
  }
}

///////////////////////////////////////////////////////////////////////

bool SerialIODevice::waitForReadyRead( int millisecs)
{
  return SerialPort::checkForRead( ((uint32_t)millisecs)*1000 );
}

///////////////////////////////////////////////////////////////////////

size_t SerialIODevice::write( const uint8_t * data, size_t count )
{
  std::string dbg((const char*)data,count);
  printf("SERIAL --->>>%s",dbg.c_str());

  size_t amt = 0;
  if (isOpen())
  {
    writeBytes(data,count);
    amt = count;
  }
  return amt;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
