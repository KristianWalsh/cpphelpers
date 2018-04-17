#include "Configuration.hpp"


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


bool Configuration::Item::toBoolean() const
{
  bool b=false;
  if (good && str.length()>0)
  {
    b=(str.length()==4 && (str[0]&0xdf)=='T' && (str[1]&0xdf)=='R' && (str[2]&0xdf)=='U' && (str[3]&0xdf)=='E');

    if (!b)
    {
      b=(atoi(str.c_str())!=0);
    }
  }
  return b;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


bool Configuration::hasKey( const std::string& fullpath ) const
{
  std::map <std::string,Item>::const_iterator place = dict.find(fullpath);
  return ( place!=dict.end() );
}

///////////////////////////////////////////////////////////////////////

const Configuration::Item& Configuration::operator[]( const std::string& fullpath ) const
{
  static Item defaultItem;

  std::map <std::string,Item>::const_iterator place = dict.find(fullpath);
  if ( place!=dict.end() )
  {
    return place->second;
  }
  else
  {
    return defaultItem;
  }
}

///////////////////////////////////////////////////////////////////////

void Configuration::set(const char* fullpath, const char* value)
{
  dict.insert( std::pair<std::string,Item>(std::string(fullpath), Item(std::string(value))));
}

///////////////////////////////////////////////////////////////////////

void Configuration::mergeFile( IODevice& source )
{
  static std::string whitespace(" \t");
  static std::string whitespaceAndEquals("\t =");

  std::string currentSection;

  if (!source.isOpen())
  {
    return;
  }
  

  std::string line;
  while ( source.atEnd()==false  ) 
  {
    line = source.readLine();
    if (!line.size() || line[0]==';' || line[0]=='#')
    { // blank or commented line
      continue;
    }
  
    if (line[0]=='[')
    { // new section
      std::string::size_type sectionNameEnd=line.find_first_of(']', 1);
      if (sectionNameEnd!=std::string::npos)
      {
        currentSection = line.substr( 1, sectionNameEnd-1 );
      }
      continue;
    }
    
    std::string::size_type keyStart=line.find_first_not_of(whitespace);
    std::string::size_type keyEnd=line.find_first_of(whitespaceAndEquals,keyStart);
    if (keyStart==std::string::npos || keyEnd==std::string::npos)
    { // skip line - no key, or key occupies whole line (thus, no value)
      continue;
    }

    
    std::string::size_type valueStart=line.find_first_not_of(whitespaceAndEquals,keyEnd);
    if (valueStart==std::string::npos)
    { // skip line - no value
      continue;
    }

    std::string key( line.substr( keyStart, keyEnd-keyStart) );

    std::string value;//( line.substr( valueStart, std::string::npos ));
      
    bool escaped = false;
    for (std::string::iterator i = line.begin()+valueStart; i!= line.end(); ++i)
    {
      if ( !escaped && *i=='\\' )
      {
        escaped=true;
        continue;
      }
      else
      if (escaped)
      {
        switch (*i)
        {
          case 'n':
            value.append( 1, 0x0A );
            break;
        
          default:
            value.append(1, *i);
            break;
        }
        escaped = false;
      }
      else
      {
        value.append(1, *i);
      }
    }
  
    std::string fullKey=currentSection;
    if (fullKey.length()>0) { fullKey.append(1,'/'); }
    fullKey.append(key);
    //printf("Config %s = '%s'\n", fullKey.c_str(),value.c_str());
    dict[fullKey].str = value;  // can access private 'str' because Item declared Configuration class as a friend
  }
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
