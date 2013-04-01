/* 
 * File:   ConfigurationReader.cpp
 * Author: gonchub (el niño parser)
 * 
 * Created on March 21, 2013, 9:25 PM
 */

#include <yaml-cpp/yaml.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Power.h>
#include <model/ConfigurationReader.h>
#include <model/entities/Entity.h>
#include <model/map/Tile.h>
#include <model/map/TileDefinition.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cstdlib>
#include <stdlib.h>

using namespace std;
/**
 * YAML Configuration file position.
 */
#define CONFIGURATION_ENTITIES_DEFINITION 0
#define CONFIGURATION_TILES_DEFINITION 1
#define CONFIGURATION_MAP_DEFINITION 2

/* ************************************** *
 * *********** AUX STRUCTURES *********** *
 * ************************************** */

/**
 * Structure to represent an entity.
 */
struct AuxEntityList {
	std::vector<Entity*> entities;
};

/**
 * List of all tile texture definitions.
 */
struct AuxTileDefinitionList {
	std::vector<TileDefinition*> tileDefinitionList;
};

/**
 * Map: list of tiles.
 */
struct AuxMap {
	std::vector<Tile*> tileList;
};

/* ************************************** *
 * *********** ENTITY PARSING *********** *
 * ************************************** */

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Position.
 */
void operator >>(const YAML::Node& yamlNode, Position* vector) {
	int auxX, auxY, auxZ;

	yamlNode[0] >> auxX;
	yamlNode[1] >> auxY;
	yamlNode[2] >> auxZ;

	vector->setX(auxX);
	vector->setY(auxY);
	vector->setZ(auxZ);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Power.
 */
void operator >>(const YAML::Node& yamlNode, Power* power) {
	std::string auxName;
	int auxDamage, auxRange;

	yamlNode["name"] >> auxName;
	yamlNode["damage"] >> auxDamage;
	yamlNode["range"] >> auxRange;

	power->setName(auxName);
	power->setDamage(auxDamage);
	power->setRange(auxRange);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Speed.
 */
void operator >>(const YAML::Node& yamlNode, Speed* speed) {
	int auxMagnitude;
	yamlNode["magnitude"] >> auxMagnitude;
	speed->setMagnitude(auxMagnitude);

	Position* auxPosition = new Position(0, 0, 0);
	yamlNode["direction"] >> auxPosition;
	speed->setDirection(auxPosition);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Entity.
 */
void operator >>(const YAML::Node& yamlNode, Entity* entity) {

	Position* auxPosition = new Position(0, 0, 0);
	Speed* auxSpeed = new Speed(0, NULL);
	std::string auxName;
	std::vector<Power*> auxPowers;

	yamlNode["name"] >> auxName;
	yamlNode["position"] >> auxPosition;
	yamlNode["speed"] >> auxSpeed;

	const YAML::Node& powers = yamlNode["powers"];
	for (unsigned i = 0; i < powers.size(); i++) {
		Power* power = new Power("", 0, 0);
		powers[i] >> power;
		auxPowers.push_back(power);
	}

	entity->setName(auxName);
	entity->setPosition(auxPosition);
	entity->setSpeed(auxSpeed);
	entity->setPowers(auxPowers);

}

/**
 * Sobrecarga de operador >> para llenar los datos de una lista de entidades.
 */
void operator >>(const YAML::Node& yamlNode, AuxEntityList& entityList) {
	const YAML::Node& entities = yamlNode["entities"];
	for (unsigned i = 0; i < entities.size(); i++) {
		std::vector<Power*> auxPowers;
		Entity* entity = new Entity("", NULL, NULL, auxPowers);
		entities[i] >> entity;
		entityList.entities.push_back(entity);
	}
}

/* *********************************************** *
 * *********** TILE DEFINITION PARSING *********** *
 * *********************************************** */

/**
 * Extraction operator for tile definition.
 */
void operator >>(const YAML::Node& yamlNode, TileDefinition* tileDefinition) {

	std::string auxId, auxSrc;

	yamlNode["identifier"] >> auxId;
	yamlNode["imageSrc"] >> auxSrc;

	tileDefinition->setTileId(auxId);
	tileDefinition->setTileImageSrc(auxSrc);
}

/**
 * Extraction of all configured entities.
 */
void operator >>(const YAML::Node& yamlNode,
		AuxTileDefinitionList& tileDefinitionList) {
	const YAML::Node& tileDefinitions = yamlNode["tiles"];
	for (unsigned i = 0; i < tileDefinitions.size(); i++) {
		TileDefinition* tileDef = new TileDefinition("", "");
		tileDefinitions[i] >> tileDef;
		tileDefinitionList.tileDefinitionList.push_back(tileDef);
	}
}

/* *********************************** *
 * *********** MAP PARSING *********** *
 * *********************************** */

/**
 * Extraction operator for map tile.
 */
void operator >>(const YAML::Node& yamlNode, Tile* tile) {
	std::string auxId;
	Position* auxPosition = new Position(0, 0, 0);

	yamlNode["position"] >> auxPosition;
	yamlNode["texture"] >> auxId;

	tile->setPosition(auxPosition);
	tile->setTextureIdentifier(auxId);
}

/**
 * Extraction operator of a map.
 */
void operator >>(const YAML::Node& yamlNode, AuxMap& entity) {
	const YAML::Node& tileList = yamlNode["map"];
	for (unsigned i = 0; i < tileList.size(); i++) {
		Tile* tile = new Tile(NULL, "");
		tileList[i] >> tile;
		entity.tileList.push_back(tile);
	}
}

/* *************************************************** *
 * *********** PRINTING OF PARSED ELEMENTS *********** *
 * *************************************************** */

/**
 * Prints a tile to check for parsing integrity.
 */
void printTile(Tile* tile) {
	std::cout << "Position: ";
	std::cout << "(" << tile->getPosition()->getX() << ", "
			<< tile->getPosition()->getY() << ", "
			<< tile->getPosition()->getZ() << ")\n";
	std::cout << "Texture: ";
	std::cout << tile->getTextureIdentifier() << "\n";
}

/**
 * Prints an entity to check if it was parsed correctly.
 */
void printEntity(Entity* parsedEntity) {

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
		std::cout << "       " << "- Name: "
				<< parsedEntity->getPowers()[i]->getName() << "\n";
		std::cout << "       " << "  Damage: "
				<< parsedEntity->getPowers()[i]->getDamage() << "\n";
		std::cout << "       " << "  Range: "
				<< parsedEntity->getPowers()[i]->getRange() << "\n";
	}

}

/**
 * Prints an entity to check if it was parsed correctly.
 */
void printTileDefinition(TileDefinition* parsedTileDefinition) {

	std::cout << "Identifier: ";
	std::cout << parsedTileDefinition->getTileId() << "\n";
	std::cout << "Image Source: ";
	std::cout << "\n";

}

/**
 * Loads the configuration and prints its output.
 */

void ConfigurationReader::loadConfiguration(std::string configurationFile) {
	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);

	// Error Check
	if (!inputFile) {
		cout << "No se encontro el archivo de conf\n";
		exit(1);
	}

	// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

	// Parsing entities.
	AuxEntityList entities;
	yamlNode[CONFIGURATION_ENTITIES_DEFINITION] >> entities;
	for (unsigned j = 0; j < entities.entities.size(); j++) {
		printEntity(entities.entities[j]);
	}

	// Parsing tile definition.
	AuxTileDefinitionList tileDefinitionList;
	yamlNode[CONFIGURATION_TILES_DEFINITION] >> tileDefinitionList;
	for (unsigned j = 0; j < tileDefinitionList.tileDefinitionList.size();
			j++) {
		printTileDefinition(tileDefinitionList.tileDefinitionList[j]);
	}

	// Parsing map tile locations.
	AuxMap mapConfiguration;
	yamlNode[CONFIGURATION_MAP_DEFINITION] >> mapConfiguration;
	for (unsigned j = 0; j < mapConfiguration.tileList.size(); j++) {
		printTile(mapConfiguration.tileList[j]);
	}

}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}

