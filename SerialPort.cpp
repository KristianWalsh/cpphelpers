#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <iomanip>

#include "SerialPort.hpp"

#ifndef B7200
#define B7200 (7200)
#endif
#ifndef B14400
#define B14400 (14400)
#endif
#ifndef B28800
#define B28800 (28800)
#endif
#ifndef B76800
#define B76800 (76800)
#endif

SerialPort::SerialPort( const char* devicePath, uint32_t baud, uint8_t databits, ParitySetting parity, uint8_t stopBits)
{
  struct termios newtio;
  int bitflags[ 4 ] = { CS5, CS6, CS7, CS8 }; 
  
  
  newtio.c_iflag = 0;
  newtio.c_oflag = 0;
  newtio.c_cflag = 0;
  newtio.c_lflag = 0;

  if((fd = open(devicePath, O_NONBLOCK| O_RDWR | O_NOCTTY )) < 0){
    printf("ERROR on opening file %s", devicePath);    
    return;
  }

  tcgetattr(fd,&oldTerminalSettings); /* save current port settings */
  uint32_t baudrate = B9600;

  switch(baud)
  {
    case 1200:
      baudrate = B1200;
      break;          
    case 2400:
      baudrate = B2400;
      break;        
    case 7200:
      baudrate= B7200;
      break; 
    case 9600:
      baudrate = B9600;
      break;  
    case 14400:
      baudrate= B14400;
      break;
    case 19200:
      baudrate = B19200;
      break;      
    case 28800:
      baudrate= B28800;
      break;
    case 38400:
      baudrate = B38400;
      break;        
    case 57600:
      baudrate = B57600;
      break;        
    case 76800:
      baudrate= B76800;
      break;
    case 115200:
      baudrate= B115200;
      break;
    case 230400:
      baudrate= B230400;
      break;
   }

  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag =  CLOCAL | CREAD;
  cfsetospeed( &newtio, baudrate );
  cfsetispeed( &newtio, baudrate );

  // set data word length
  if (databits>8)
  {
    databits = 8;
  }
  else
  if (databits<5)
  {
    databits = 5;
  }
  newtio.c_cflag |= bitflags[databits-5];

  newtio.c_iflag = PARMRK;//IGNPAR;

    // add 2nd stop bit if requested to.
  if ( stopBits==2)
  {
    newtio.c_cflag |=  CSTOPB;
  }

    // set parity
  if ( parity != kNoParity)
  {
    newtio.c_cflag |=  PARENB;
    if (parity==kOddParity)
    {
      newtio.c_cflag |= PARODD;  
    }
  }
  
  newtio.c_oflag = 0;
 
  /* set input mode (non-canonical, no echo,...) */
  newtio.c_lflag = 0;
 
  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 0;   /* blocking read until 1 chars received */
  
  tcflush(fd, TCIFLUSH);
  tcflush(fd, TCOFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

}

SerialPort::~SerialPort( )
{
  if (good())
  {
    tcsetattr(fd,TCSANOW,&oldTerminalSettings);
    close(fd);
  }
}

void SerialPort::writeBytes(const unsigned char* bytes, size_t howMany)
{
  write( fd, bytes, howMany );
}

void SerialPort::waitForRead(void)
{
  fd_set fileDescriptors;
  FD_ZERO( &fileDescriptors );
  FD_SET( fd, &fileDescriptors );
  select(fd+1, &fileDescriptors, NULL, NULL, NULL);
}

bool SerialPort::checkForRead(uint32_t timeoutMicroseconds)
{
  fd_set fileDescriptors;
  FD_ZERO( &fileDescriptors );
  FD_SET( fd, &fileDescriptors );
  struct timeval timeout;
  timeout.tv_usec = timeoutMicroseconds%1000000L;    // microseconds
  timeout.tv_sec = timeoutMicroseconds/1000000L;    // seconds
  return (select(fd+1, &fileDescriptors, NULL, NULL, &timeout) != 0);
}

uint8_t SerialPort::readByte(void)
{
  uint8_t result=0;
  read(fd, &result, 1);

  return result;
}

size_t SerialPort::readBytes(unsigned char* buffer, size_t howMany, uint32_t timeoutMicroseconds){
  size_t haveRead = 0;
  while(haveRead < howMany){
    if(checkForRead(timeoutMicroseconds) == false){
      break;
    }
    *buffer++ = readByte();
    ++haveRead;  
  }

  return haveRead;
}

void SerialPort::flushReceiver(void)
{
  tcflush( fd, TCIFLUSH);
}
