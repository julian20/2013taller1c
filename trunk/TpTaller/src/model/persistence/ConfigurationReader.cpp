/* 
 * File:   ConfigurationReader.cpp
 * Author: gonchub (el niño parser)
 * 
 * Created on March 21, 2013, 9:25 PM
 */

#include <yaml-cpp/yaml.h>
#include <model/Vector2.h>
#include <model/entityProperties/Speed.h>
#include <model/entityProperties/Position.h>
#include <model/entityProperties/Power.h>
#include <model/persistence/ConfigurationReader.h>
#include <model/entities/Entity.h>
#include <model/entities/personaje/Personaje.h>
#include <model/map/Tile.h>
#include <model/map/TileDefinition.h>
#include <model/map/TextureHolder.h>
#include <model/persistence/PersistentConfiguration.h>

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
#define CONFIGURATION_MAPDIMENSION_DEFINITION 2
#define CONFIGURATION_TILELOCATION_DEFINITION 3

/* ************************************** *
 * *********** AUX STRUCTURES *********** *
 * ************************************** */

/**
 * Structure to represent an personaje.
 */
struct AuxPersonajeList {
	std::vector<Personaje*> entities;
};

/**
 * List of all tile texture definitions.
 */
struct AuxTileDefinitionList {
	std::vector<TileDefinition*> tileDefinitionList;
};

/**
 * Dimensions of a map.
 */
struct AuxMapDimension {
	int nrows, ncols;
};

/**
 * Map: list of tiles.
 */
struct AuxMap {
	std::vector<Tile*> tileList;
	AuxMapDimension dimension;
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
 * a Position.
 */
void operator >>(const YAML::Node& yamlNode, Vector2* vector) {
	int auxX, auxY, auxZ;

	yamlNode[0] >> auxX;
	yamlNode[1] >> auxY;
	yamlNode[2] >> auxZ;

	vector->SetValues(auxX, auxY);
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

	Vector2* auxPosition = new Vector2(0, 0);
	yamlNode["direction"] >> auxPosition;
	speed->setDirection(Vector2(auxPosition->GetX(), auxPosition->GetY()));
	delete auxPosition;
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Personaje* personaje) {

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

	personaje->setName(auxName);
	personaje->setPosition(auxPosition);
	personaje->setSpeed(auxSpeed);
	personaje->setPowers(auxPowers);
}

/**
 * Sobrecarga de operador >> para llenar los datos de una lista de entidades.
 */
void operator >>(const YAML::Node& yamlNode, AuxPersonajeList& entityList) {
	const YAML::Node& entities = yamlNode["players"];
	for (unsigned i = 0; i < entities.size(); i++) {
		std::vector<Power*> auxPowers;
		Personaje* personaje = new Personaje("", NULL, NULL, auxPowers);
		entities[i] >> personaje;
		entityList.entities.push_back(personaje);
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
void operator >>(const YAML::Node& yamlNode, TextureHolder* textureHolder) {
	const YAML::Node& yamlTileDefinitions = yamlNode["tiles"];
	for (unsigned i = 0; i < yamlTileDefinitions.size(); i++) {
		TileDefinition* tileDef = new TileDefinition("", "");
		yamlTileDefinitions[i] >> tileDef;
		textureHolder->addTexture(tileDef);
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
void operator >>(const YAML::Node& yamlNode, AuxMap& destMap) {
	const YAML::Node& tileList = yamlNode["tileLocations"];
	for (unsigned i = 0; i < tileList.size(); i++) {
		Tile* tile = new Tile(NULL, "");
		tileList[i] >> tile;
		destMap.tileList.push_back(tile);
	}
}

/**
 * Extraction operator of a map.
 */
void operator >>(const YAML::Node& yamlNode, AuxMapDimension& dimension) {
	const YAML::Node& yamlDimensions = yamlNode["mapDimensions"];

	yamlDimensions[0] >> dimension.nrows;
	yamlDimensions[1] >> dimension.ncols;
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
 * Prints an personaje to check if it was parsed correctly.
 */

void printPersonaje(Personaje* parsedPersonaje) {

	std::cout << "Name: ";
	std::cout << parsedPersonaje->getName() << "\n";
	std::cout << "Position: (";
	std::cout << parsedPersonaje->getPosition()->getX() << ", ";
	std::cout << parsedPersonaje->getPosition()->getY() << ", ";
	std::cout << parsedPersonaje->getPosition()->getZ() << ")\n";
	std::cout << "Speed:\n";
	std::cout << "      " << "Magnitude: ";
	std::cout << parsedPersonaje->getSpeed()->getMagnitude() << "\n";
	std::cout << "      " << "Direction: (";
	std::cout << parsedPersonaje->getSpeed()->getDirection().GetX() << ", ";
	std::cout << parsedPersonaje->getSpeed()->getDirection().GetY() << ")\n";
	std::cout << "Powers:\n";
	for (unsigned i = 0; i < parsedPersonaje->getPowers().size(); i++) {
		std::cout << "       " << "- Name: "
				<< parsedPersonaje->getPowers()[i]->getName() << "\n";
		std::cout << "       " << "  Damage: "
				<< parsedPersonaje->getPowers()[i]->getDamage() << "\n";
		std::cout << "       " << "  Range: "
				<< parsedPersonaje->getPowers()[i]->getRange() << "\n";
	}

}
/**
 * Prints an personaje to check if it was parsed correctly.
 */
void printTextureHolder(TextureHolder* parsedTileDefinition) {

	std::vector<std::string> textureIds;

	textureIds.push_back("soil");
	textureIds.push_back("neutral");
	textureIds.push_back("water");
	textureIds.push_back("tree");

	for (unsigned int i = 0; i < textureIds.size(); i++) {
		std::cout << "Identifier: ";
		std::cout << textureIds[i] << "\n";
		std::cout << "Image Source: ";
		std::cout << parsedTileDefinition->getTextureSrc(textureIds[i]);
		std::cout << "\n";
	}

}

void printMapDimensions(AuxMap &mapConfiguration) {

	std::cout << "Map Dimensions: " << "\n";
	std::cout << "     Rows: " << mapConfiguration.dimension.nrows << "\n";
	std::cout << "     Columns: " << mapConfiguration.dimension.ncols << "\n";

}

/**
 * Loads the configuration, prints its output and returns
 * a persistent configuration object.
 */
PersistentConfiguration* ConfigurationReader::loadConfiguration(
		std::string configurationFile) {

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
	AuxPersonajeList entities;
	yamlNode[CONFIGURATION_ENTITIES_DEFINITION] >> entities;
	for (unsigned j = 0; j < entities.entities.size(); j++) {
		printPersonaje(entities.entities[j]);
	}

	// Parsing tile definition.
	TextureHolder* textureHolder = new TextureHolder();
	yamlNode[CONFIGURATION_TILES_DEFINITION] >> textureHolder;
	printTextureHolder(textureHolder);

	// Parsing map dimensions.
	AuxMap mapConfiguration;
	AuxMapDimension mapDimension;
	yamlNode[CONFIGURATION_MAPDIMENSION_DEFINITION] >> mapDimension;
	mapConfiguration.dimension = mapDimension;
	printMapDimensions(mapConfiguration);

	MapData* mapData = new MapData(mapConfiguration.dimension.nrows,
			mapConfiguration.dimension.ncols);

	// Parsing map tile locations.
	yamlNode[CONFIGURATION_TILELOCATION_DEFINITION] >> mapConfiguration;

	for (unsigned int i = 0; i < mapConfiguration.tileList.size(); i++) {
		Tile* auxTile = mapConfiguration.tileList[i];
		int auxCol = auxTile->getPosition()->getX();
		int auxRow = auxTile->getPosition()->getY();
		mapData->SetTileType(auxTile->getTextureIdentifier(), auxRow, auxCol);
		printTile(auxTile);
	}

	// Packing parser results.
	PersistentConfiguration* configuration = new PersistentConfiguration();
	configuration->setPersonajeList(entities.entities);
	configuration->setTextureHolder(textureHolder);
	configuration->setMapData(mapData);

	std::cout << configuration->getMapData()->GetTileType(1, 1) << "\n";

	return configuration;
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}

