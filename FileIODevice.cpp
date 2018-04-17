
#include "FileIODevice.hpp"
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


FileIODevice::FileIODevice(const char* filename_)
: filename(filename_), fd(-1), eof(true)
{

}
///////////////////////////////////////////////////////////////////////

bool FileIODevice::open(int mode)
{
  int posixmode = 0; 
  int posixperms = 0644;  // octal

  switch (mode&ReadWrite)
  {
    case ReadOnly:
      posixmode=O_RDONLY;
      break;

    case ReadWrite:
      posixmode=O_RDWR|O_CREAT;
      break;

    case WriteOnly:
      posixmode=O_WRONLY|O_CREAT;
      break;

    default:
      posixmode = -1;
  }

  if (posixmode!=-1)
  {
    if (mode&Truncate)
    {
      posixmode|=O_TRUNC;
    }

    if (mode&Append)
    {
      posixmode|=O_APPEND;
    }
    fd = ::open(filename.c_str(), posixmode, posixperms);
    //printf("::open(\"%s\",%d) = %d",filename.c_str(),posixmode,fd);
    eof=(fd==-1);
  }
  return (fd!=-1);
}
///////////////////////////////////////////////////////////////////////

FileIODevice::~FileIODevice()
{
  close();
}

///////////////////////////////////////////////////////////////////////

void FileIODevice::close()
{
  if (fd!=-1)
  {
    ::close(fd);
    fd=-1;
  }
}


///////////////////////////////////////////////////////////////////////

bool FileIODevice::atEnd()
{
  return eof;

}

///////////////////////////////////////////////////////////////////////

size_t FileIODevice::pos() const
{
  size_t realPos =0;
  if (isOpen())
  {
   realPos = lseek(fd, 0, SEEK_CUR);
  }
  return realPos;
}

///////////////////////////////////////////////////////////////////////

bool FileIODevice::seek( size_t where)
{
  bool r = false;
  if (isOpen())
  {
    size_t realPos = lseek(fd, where, SEEK_SET);
    r = (realPos==where);
    eof=!r;
  }
  return r;
}

///////////////////////////////////////////////////////////////////////


size_t FileIODevice::read( uint8_t * data, size_t maxSize )
{
  size_t ret = 0;
  if (isOpen())
  {
    ret = ::read(fd,data,maxSize);
    eof=(ret<maxSize);
    //printf("%d read from fd %d, at eof?%s\n",ret,fd,eof?"yes":"no");
  }
  return ret;
}

///////////////////////////////////////////////////////////////////////


bool FileIODevice::waitForReadyRead( int millisecs)
{
  struct timeval timeout;
  timeout.tv_sec = millisecs/1000;
  timeout.tv_usec = ((long)millisecs%1000)*1000;

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(fd,&readfds);
  int avail = select(fd+1, &readfds, 0, 0, &timeout);
  return (avail !=-1);
}

///////////////////////////////////////////////////////////////////////

size_t FileIODevice::write( const uint8_t * data, size_t count )
{
  size_t amount = 0;
  if (isOpen())
  {
    amount = ::write(fd, data, count);
  }
  return amount;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
