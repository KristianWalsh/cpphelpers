#ifndef IODevice_hpp__aid7129
#define IODevice_hpp__aid7129
#include <stddef.h>
#include <stdint.h>
#include <string>
/*
  Basic interface for any file-like I/O device.

    (heavily influenced by Qt's IODevice)
*/


class IODevice
{
public:
  virtual bool isOpen() const =0;
  virtual bool readyToRead();
  virtual bool atEnd()=0;

  virtual size_t read( uint8_t * data, size_t maxSize )=0;
  virtual bool getChar( char* result );
  virtual std::string readLine( size_t maxSize=0 );

  virtual bool waitForReadyRead( int millisecs)=0;

  virtual size_t pos() const { return 0; }
  virtual bool seek( size_t where) { return false; }

  virtual size_t write( const uint8_t * data, size_t count )=0;
  virtual bool putChar( char c );
};


#endif