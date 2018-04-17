#include <string.h>
#include "StringIODevice.hpp"

///////////////////////////////////////////////////////////////////////

StringIODevice::StringIODevice( const std::string& buffer_)
  : buffer(buffer_), position(0)
{

}

///////////////////////////////////////////////////////////////////////

StringIODevice::StringIODevice( const char* data, size_t len)
  : buffer(data, len), position(0)
{

}

///////////////////////////////////////////////////////////////////////

StringIODevice::StringIODevice( const char* cstring)
  : buffer(cstring), position(0)
{

}

///////////////////////////////////////////////////////////////////////

bool StringIODevice::seek( size_t where )
{
  if (where>buffer.length())
  {
    where=buffer.length();
  }
  position= where;
  return !atEnd();
}

///////////////////////////////////////////////////////////////////////

size_t StringIODevice::read( uint8_t * data, size_t maxSize )
{
  if (atEnd())
  {
    return 0;
  }
  size_t readAmount = position+maxSize<buffer.length()? maxSize : buffer.length()-position;
  memcpy( data, buffer.data()+position, readAmount);
  position+=readAmount;
  return (readAmount==maxSize);
}

///////////////////////////////////////////////////////////////////////


size_t StringIODevice::write( const uint8_t * data, size_t count )
{
  if (position>=buffer.length())
  {
    buffer.append( (const char*)data, count );
  }
  else
  {
    size_t repLen = position+count<buffer.length()? count : buffer.length()-position;
    buffer.replace(position, repLen, (const char*)data, count);
  }
  position+=count;
  return count;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


