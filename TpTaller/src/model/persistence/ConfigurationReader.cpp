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
#include <view/configuration/AnimationConfiguration.h>

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
#define CONFIGURATION_ANIMATIONPARAMETERS_DEFINITION 1
#define CONFIGURATION_TILES_DEFINITION 2
#define CONFIGURATION_MAPDIMENSION_DEFINITION 3
#define CONFIGURATION_TILELOCATION_DEFINITION 4

/* ************************************** *
 * *********** AUX STRUCTURES *********** *
 * ************************************** */

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
	std::cout << tile->getTextureIdentifier() << std::endl;
}

/**
 * Prints an personaje to check if it was parsed correctly.
 */
void printPersonajes(std::vector<Personaje*> entities) {
	for (unsigned j = 0; j < entities.size(); j++) {
		Personaje* parsedPersonaje = entities[j];
		std::cout << "Name: ";
		std::cout << parsedPersonaje->getName() << std::endl;
		std::cout << "Position: (";
		std::cout << parsedPersonaje->getPosition()->getX() << ", ";
		std::cout << parsedPersonaje->getPosition()->getY() << ", ";
		std::cout << parsedPersonaje->getPosition()->getZ() << ")\n";
		std::cout << "Speed:\n";
		std::cout << "      " << "Magnitude: ";
		std::cout << parsedPersonaje->getSpeed()->getMagnitude() << std::endl;
		std::cout << "      " << "Direction: (";
		std::cout << parsedPersonaje->getSpeed()->getDirection().GetX() << ", ";
		std::cout << parsedPersonaje->getSpeed()->getDirection().GetY()
				<< ")\n";
		std::cout << "Powers:\n";
		for (unsigned i = 0; i < parsedPersonaje->getPowers().size(); i++) {
			std::cout << "       " << "- Name: "
					<< parsedPersonaje->getPowers()[i]->getName() << std::endl;
			std::cout << "       " << "  Damage: "
					<< parsedPersonaje->getPowers()[i]->getDamage()
					<< std::endl;
			std::cout << "       " << "  Range: "
					<< parsedPersonaje->getPowers()[i]->getRange() << std::endl;
		}
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
		std::cout << textureIds[i] << std::endl;
		std::cout << "Image Source: ";
		std::cout << parsedTileDefinition->getTextureSrc(textureIds[i]);
		std::cout << std::endl;
	}

}

/**
 * Prints the dimensions of a map.
 */
void printMapDimensions(AuxMap &mapConfiguration) {

	std::cout << "Map Dimensions: " << std::endl;
	std::cout << "     Rows: " << mapConfiguration.dimension.nrows << std::endl;
	std::cout << "     Columns: " << mapConfiguration.dimension.ncols
			<< std::endl;

}

/**
 * Prints the configured tiles of a map.
 */
void printConfiguredTiles(AuxMap &mapConfiguration) {
	for (unsigned int i = 0; i < mapConfiguration.tileList.size(); i++) {
		printTile(mapConfiguration.tileList[i]);
	}
}

/**
 * Prints the map configuration.
 */
void printMapConfiguration(AuxMap &mapConfiguration) {
	printMapDimensions(mapConfiguration);
	printConfiguredTiles(mapConfiguration);
}

/**
 * Prints the animated configuration.
 */
void printAnimationConfiguration(AnimationConfiguration* aConfig) {

	std::cout << "Animation Configuration: " << std::endl;
	std::cout << "     FPS: " << aConfig->getFps() << std::endl;
	std::cout << "     Delay: " << aConfig->getDelay() << std::endl;

}

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
void operator >>(const YAML::Node& yamlNode,
		std::vector<Personaje*>& entityList) {
	const YAML::Node& entities = yamlNode["players"];
	for (unsigned i = 0; i < entities.size(); i++) {
		std::vector<Power*> auxPowers;
		Personaje* personaje = new Personaje("", NULL, NULL, auxPowers);
		entities[i] >> personaje;
		entityList.push_back(personaje);
	}
}

/* ******************************************************* *
 * *********** ANIMATION CONFIGURATION PARSING *********** *
 * ******************************************************* */

/**
 * Sobrecarga de operador >> para parsear un AnimationConfiguration.
 */
void operator >>(const YAML::Node& yamlNode,
		AnimationConfiguration* animationConfig) {
	const YAML::Node& configuration = yamlNode["animation"];
	unsigned int auxFps, auxDelay;

	configuration["fps"] >> auxFps;
	configuration["delay"] >> auxDelay;

	animationConfig->setDelay(auxDelay);
	animationConfig->setFps(auxFps);
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
void operator >>(AuxMap& originConfig, MapData* destMap) {
	for (unsigned int i = 0; i < originConfig.tileList.size(); i++) {
		Tile* auxTile = originConfig.tileList[i];
		int auxCol = auxTile->getPosition()->getX();
		int auxRow = auxTile->getPosition()->getY();
		destMap->SetTileType(auxTile->getTextureIdentifier(), auxRow, auxCol);
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

/* ********************************************* *
 * *********** CONFIGURATION LOADING *********** *
 * ********************************************* */

/**
 * Loads the configuration, prints its output and returns
 * a persistent configuration object.
 */
PersistentConfiguration ConfigurationReader::loadConfiguration(
		std::string configurationFile) {

	/**
	 * TODO: hacer free de los structs auxiliares luego
	 * de parsear.
	 */
	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);

	// Error Check
	if (!inputFile) {
		cout << "No se encontro el archivo de configuracion" << std::endl;
		exit(1);
	}

	// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

	// Parsing entities.
	std::vector<Personaje*> entities;
	yamlNode[CONFIGURATION_ENTITIES_DEFINITION] >> entities;

	// Parsing animation configuration.
	AnimationConfiguration* animationConfig = new AnimationConfiguration();
	yamlNode[CONFIGURATION_ANIMATIONPARAMETERS_DEFINITION] >> animationConfig;

	// Parsing tile definition.
	TextureHolder* textureHolder = new TextureHolder();
	yamlNode[CONFIGURATION_TILES_DEFINITION] >> textureHolder;

	// Parsing map dimensions.
	AuxMap mapConfiguration;
	AuxMapDimension mapDimension;
	yamlNode[CONFIGURATION_MAPDIMENSION_DEFINITION] >> mapDimension;
	mapConfiguration.dimension = mapDimension;
	MapData* mapData = new MapData(mapConfiguration.dimension.nrows,
			mapConfiguration.dimension.ncols);

	// Parsing map tile locations.
	yamlNode[CONFIGURATION_TILELOCATION_DEFINITION] >> mapConfiguration;
	mapConfiguration >> mapData;

	// Packing parser results.
	PersistentConfiguration configuration = PersistentConfiguration();
	configuration.setPersonajeList(entities);
	configuration.setTextureHolder(textureHolder);
	configuration.setMapData(mapData);
	configuration.setAnimationConfiguration(animationConfig);

	// Print parsed elements.
	printPersonajes(entities);
	printAnimationConfiguration(animationConfig);
	printTextureHolder(textureHolder);
	printMapConfiguration(mapConfiguration);

	return configuration;
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}

