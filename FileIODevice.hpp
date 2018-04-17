#ifndef FileIODevice_hpp__aQd7129
#define FileIODevice_hpp__aQd7129

#include "IODevice.hpp"
#include <stdio.h>

 class FileIODevice
  : public IODevice
{
private:
  std::string filename;
  int fd;
  bool eof;

public:
  FileIODevice(const char* c);
  virtual ~FileIODevice();
  virtual bool open( int mode=ReadOnly );
  virtual void close( );
  virtual bool isOpen() const { return (fd!=-1); }

  virtual bool atEnd();
  virtual size_t pos() const;
  virtual bool seek( size_t where);

  virtual size_t read( uint8_t * data, size_t maxSize );

  virtual bool waitForReadyRead( int millisecs);
  virtual size_t write( const uint8_t * data, size_t count );

  enum { ReadOnly=1, WriteOnly=2, ReadWrite=ReadOnly|WriteOnly, Append=4, Truncate=8 };

};


#endif