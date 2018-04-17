#ifndef Configuration_HPP_YAM1B1
#define Configuration_HPP_YAM1B1

#include <stdlib.h>
#include "IODevice.hpp"
#include <map>

/*
  Configuration
  -------------

  A simple settings/configuration manager.

  Class Configuration manages a set of "Configuration::Item" objects, indexed by string keys.

  Can also import INI-formatted data from any object implementing InputStream interface.

  All config data is text internally, but can be presented as integer/boolean if necessary using 
  methods on Item.

  Usage:

    Configuration c;
      // set defaults
    c.add("setting1","default1");
    c.add("setting2","2");
    c.add("setting3","default3");

    cout<<c["setting1"];  // outputs "default1"
    int x = c["setting2"].toInteger(); // x == 2

    FileIODevice file("/etc/mysettings.config");
    file.open();
    c.mergeFile(file);  // read settings from a config file
    file.close();

    cout<<c["setting1"];  // outputs file's value for setting1, or still "default1" if file didn't define it.


*/

class Configuration
{
public:
  class Item
  {
  friend class Configuration;

  private:
    std::string str;
    bool good;
  public:
    Item():str(),good(false) {};
    Item(const std::string& s):str(s),good(true) {};
    const std::string toString() const { return str; };
    const char* toCString() const { return str.c_str(); };
    bool toBoolean() const;
    int toInteger() const { return atoi(str.c_str()); };
    virtual bool exists() { return good; };
  };

  typedef std::map <std::string,Item> Dictionary;

private:
  Dictionary dict;

public:
  void mergeFile( IODevice& source ); // settings read from the stream "source" will overwrite their like-named equivalents in this config

    // change the value of a setting
  void set(const char* fullpath, const char* value);

    // check if key is in file.  path is "section/key" for INI-file format ('[section]\n key=value')
  bool hasKey(const char* fullpath ) const { return hasKey(std::string(fullpath)); }
  bool hasKey(const std::string& fullpath ) const;

  const Item& operator[](const char* fullpath) const { return operator[](std::string(fullpath)); }  // "section/key"
  const Item& operator[](const std::string& fullpath) const;

  const Dictionary& getAll() const { return dict; };
};

#endif
