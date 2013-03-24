/* 
 * File:   ConfigurationReader.cpp
 * Author: gonchub
 * 
 * Created on March 21, 2013, 9:25 PM
 */


#include <yaml-cpp/yaml.h>
#include <model/ConfigurationReader.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

/**
 * Structure to represent a two dimensional vector.
 */
struct TwoDimensionalVector {
    int x, y;
};

/**
 * Structure to represent the power of an entity.
 */
struct Power {
    std::string name;
    int damage;
};

/**
 * Structure to represent the power of an entity.
 */
struct Speed {
    int magnitude;
};

/**
 * Structure to represent an entity.
 */
struct Entity {
    std::string name;
    TwoDimensionalVector position;
    Speed speed;
    std::vector <Power> powers;
};

/**
 * Extraction operator for two dimensional vectors.
 * @param yamlNode node that holds the information.
 * @param vector output vector.
 */
void operator >>(const YAML::Node& yamlNode, TwoDimensionalVector& vector) {
    yamlNode[0] >> vector.x;
    yamlNode[1] >> vector.y;
}

/**
 * Extraction operator for power.
 * @param yamlNode node that holds the information.
 * @param power output power.
 */
void operator >>(const YAML::Node& yamlNode, Power& power) {
    yamlNode["name"] >> power.name;
    yamlNode["damage"] >> power.damage;
}

/**
 * Extraction of an entity.
 * @param yamlNode node that holds the information.
 * @param entity output entity.
 */
void operator >>(const YAML::Node& yamlNode, Entity& entity) {
    yamlNode["name"] >> entity.name;
    yamlNode["position"] >> entity.position;
    yamlNode["speed"] >> entity.speed.magnitude;
    const YAML::Node& powers = yamlNode["powers"];
    for (unsigned i = 0; i < powers.size(); i++) {
        Power power;
        powers[i] >> power;
        entity.powers.push_back(power);
    }
}

/**
 * Loads the configuration and prints its output.
 * @param configurationFile yaml configuration file.
 */
void ConfigurationReader::loadConfiguration(std::string configurationFile) {
    std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);
    //Error Check
    if (!inputFile){
    	cout << "No se encontro el archivo de conf\n";
    	exit(1);
    }
    YAML::Parser parser(inputFile);
    YAML::Node yamlNode;
    parser.GetNextDocument(yamlNode);
    for (unsigned i = 0; i < yamlNode.size(); i++) {
        Entity entity;
        yamlNode[i] >> entity;
        std::cout << entity.name << "\n";
    }
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}

