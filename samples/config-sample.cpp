#include <iostream>
#include "Configuration.hpp"
#include "FileIODevice.hpp"
#include "StringIODevice.hpp"

void dumpConfig(const Configuration& c);

int main(int argc, const char** argv)
{
  Configuration c;

  c.set("key1","3");
  c.set("secondkey","baa");
  c.set("thirdkey","s2k");

  int v = c["key1"].toInteger();
  std::cout << "Config setting key1 is int of value "<<v<<std::endl;

    // to read config data from a file, use this:
  //  FileIODevice configfile("config-sample.conf");
  //  configfile.open();    // open the file; mergeFile wants an open device
  //  c.mergeFile(configfile);
  //  configfile.close();
    
    // However, To ensure that this example will always run, 
    // we'll read the new config out of a string instead.
  const char* configData="key1=4\n"
                         "keyfour=vier";
  StringIODevice overlay(configData);
    // string IO devices are always "open", so no need to call overlay.open()
  c.mergeFile(overlay);

    // key1 is now 4, so display that.
  v = c["key1"].toInteger();
  std::cout << "Config setting key1 now is int of value "<<v<<std::endl;

  std::cout<<"Final configuration:"<<std::endl;
  dumpConfig(c);
}

void dumpConfig(const Configuration& c)
{
  Configuration::Dictionary items = c.getAll();
  for (Configuration::Dictionary::const_iterator i = items.begin(); i!=items.end(); ++i )
  {
    std::cout<<"key: '"<<i->first<<"', value='"<<i->second.toString()<<"'"<<std::endl;
  }
}