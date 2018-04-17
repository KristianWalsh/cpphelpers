#ifndef SERIALPORT_HPP_SD6HPFN1
#define SERIALPORT_HPP_SD6HPFN1

#include <stddef.h>
#include <inttypes.h>
#include <termios.h>

class SerialPort 
{
private:
  int fd;
  struct termios oldTerminalSettings;

public:
  enum ParitySetting {  kNoParity, kEvenParity, kOddParity };

 SerialPort( const char* devicePath, uint32_t baud=9600, uint8_t databits=8, ParitySetting parity=kNoParity, uint8_t stopBits=1);
 ~SerialPort();

 bool good() const { return fd!=-1; }

 void writeByte( unsigned char byte ) { writeBytes(&byte,1); };
 void writeBytes( const unsigned char* bytes, size_t howMany );
 void waitForRead( void );
 bool checkForRead( uint32_t timeoutMicroseconds = 1 );
 uint8_t readByte( void );
 size_t readBytes( unsigned char* buffer, size_t maxToRead, uint32_t timeoutMicroseconds =1 );
 void flushReceiver( );
};

#endif /* end of include guard: SERIALPORT_HPP_SD6HPFN1 */
