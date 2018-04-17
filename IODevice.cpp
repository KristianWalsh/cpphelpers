#include "IODevice.hpp"
#include "ASCIICodes.hpp"

///////////////////////////////////////////////////////////////////////

std::string IODevice::readLine( size_t maxSize )
{
  std::string result;
  char c;
  while (getChar(&c))
  {
    if (c==ASCII::CR || c==ASCII::LF )
    {
      break;
    }
    result.append(1,c);
    if (result.length()==maxSize) break; // note: testing this here implements the "zero means no maximum" rule
  }
  return result;
}

///////////////////////////////////////////////////////////////////////

bool IODevice::getChar( char* result )
{
  size_t amount = read((uint8_t*)result,1);
  return (amount==1);
}

///////////////////////////////////////////////////////////////////////

bool IODevice::putChar( char c )
{
  uint8_t v =c;
  size_t amount = write(&v,1);
  return (amount==1);
}

///////////////////////////////////////////////////////////////////////

bool IODevice::readyToRead()
{
  return waitForReadyRead(0);
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
