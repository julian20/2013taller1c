/* 
 * File:   ConfigurationReader.cpp
 * Author: gonchub
 * 
 * Created on March 21, 2013, 9:25 PM
 */

#include <yaml-cpp/yaml.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Power.h>
#include <model/ConfigurationReader.h>
#include <model/entities/Entity.h>

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
struct ThreeDimensionalVector {
	int x, y, z;
};

void operator >>(const YAML::Node& yamlNode, ThreeDimensionalVector& vector) {
	yamlNode[0] >> vector.x;
	yamlNode[1] >> vector.y;
	yamlNode[2] >> vector.z;
}
/**
 * Structure to represent the power of an entity.
 */
struct AuxPower {
	std::string name;
	int damage;
	int range;
};

/**
 * Structure to represent the power of an entity.
 */
struct AuxSpeed {
	int magnitude;
	ThreeDimensionalVector direction;
};

/**
 * Structure to represent an entity.
 */
struct AuxEntity {
	std::string name;
	ThreeDimensionalVector position;
	AuxSpeed speed;
	std::vector<AuxPower> powers;
};

/**
 * Extraction operator for two dimensional vectors.
 */
/**
 * Extraction operator for power.
 */
void operator >>(const YAML::Node& yamlNode, AuxPower& power) {
	yamlNode["name"] >> power.name;
	yamlNode["damage"] >> power.damage;
	yamlNode["range"] >> power.range;
}

/**
 * Extraction operator for power.
 */
void operator >>(const YAML::Node& yamlNode, AuxSpeed& speed) {
	yamlNode["magnitude"] >> speed.magnitude;
	yamlNode["direction"] >> speed.direction;
}

/**
 * Extraction of an entity.
 */
void operator >>(const YAML::Node& yamlNode, AuxEntity& entity) {
	yamlNode["name"] >> entity.name;
	yamlNode["position"] >> entity.position;
	yamlNode["speed"] >> entity.speed;
	const YAML::Node& powers = yamlNode["powers"];
	for (unsigned i = 0; i < powers.size(); i++) {
		AuxPower power;
		powers[i] >> power;
		entity.powers.push_back(power);
	}
}

void ConfigurationReader::printEntity(Entity* parsedEntity) {

	std::cout << "Name: ";
	std::cout << parsedEntity->getName() << "\n";
	std::cout << "Position: (";
	std::cout << parsedEntity->getPosition()->getX() << ", ";
	std::cout << parsedEntity->getPosition()->getY() << ", ";
	std::cout << parsedEntity->getPosition()->getZ() << ")\n";
	std::cout << "Speed:\n";
	std::cout << "      " << "Magnitude: ";
	std::cout << parsedEntity->getSpeed()->getMagnitude() << "\n";
	std::cout << "      " << "Direction: (";
	std::cout << parsedEntity->getSpeed()->getDirection()->getX() << ", ";
	std::cout << parsedEntity->getSpeed()->getDirection()->getY() << ", ";
	std::cout << parsedEntity->getSpeed()->getDirection()->getZ() << ")\n";
	std::cout << "Powers:\n";
	for (unsigned i = 0; i < parsedEntity->getPowers().size(); i++) {
		std::cout << "       " << "- Name: " << parsedEntity->getPowers()[i]->getName() << "\n";
		std::cout << "       " << "  Damage: " << parsedEntity->getPowers()[i]->getDamage() << "\n";
		std::cout << "       " << "  Range: " << parsedEntity->getPowers()[i]->getRange() << "\n";
	}

}

/**
 * Loads the configuration and prints its output.
 */
void ConfigurationReader::loadConfiguration(std::string configurationFile) {
	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);
//Error Check
	if (!inputFile) {
		cout << "No se encontro el archivo de conf\n";
		exit(1);
	}
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);
	for (unsigned i = 0; i < yamlNode.size(); i++) {

		AuxEntity entity;
		yamlNode[i] >> entity;

		Position* parsedPosition = new Position(entity.position.x,
				entity.position.y, entity.position.z);

		Speed* parsedSpeed = new Speed(entity.speed.magnitude,
				new Position(entity.speed.direction.x, entity.speed.direction.y,
						entity.speed.direction.z));

		std::vector<Power*> parsedPowers;
		for (unsigned i = 0; i < entity.powers.size(); i++) {
			Power* parsedPower = new Power(entity.powers[i].name,
					entity.powers[i].damage, entity.powers[i].range);
			parsedPowers.push_back(parsedPower);
		}

		Entity* parsedEntity = new Entity(entity.name, parsedPosition,
				parsedSpeed, parsedPowers);

		printEntity(parsedEntity);

	}
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}

