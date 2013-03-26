/* 
 * File:   ConfigurationReader.h
 * Author: gonchub
 *
 * Created on March 21, 2013, 9:25 PM
 */

#ifndef CONFIGURATIONREADER_H
#define	CONFIGURATIONREADER_H

#include <string>
#include <model/entities/Entity.h>

class ConfigurationReader {
public:
    ConfigurationReader();
    ConfigurationReader(const ConfigurationReader& orig);
    void loadConfiguration(std::string configurationFile);
    virtual ~ConfigurationReader();

private:
};

#endif	/* CONFIGURATIONREADER_H */

