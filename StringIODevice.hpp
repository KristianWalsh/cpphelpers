#ifndef StringIODevice_hpp_9120kx
#define StringIODevice_hpp_9120kx

#include "IODevice.hpp"

#include <string>

class StringIODevice
  : public IODevice
{
private:
  std::string buffer;
  size_t position;

public:
  StringIODevice( const std::string& buffer_);
  StringIODevice( const char* cstring);
  StringIODevice( const char* data, size_t len);

  virtual bool isOpen() const { return true; }
  virtual bool readyToRead() { return !atEnd(); }
  virtual bool atEnd() { return position>=buffer.length(); }
  virtual size_t read( uint8_t * data, size_t maxSize );
  virtual bool waitForReadyRead( int millisecs) { return !atEnd();}

  virtual size_t pos() const { return position; }
  virtual bool seek( size_t where);

  virtual size_t write( const uint8_t * data, size_t count );

};


#endif