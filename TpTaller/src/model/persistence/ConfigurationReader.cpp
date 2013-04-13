/* 
 * File:   ConfigurationReader.cpp
 * Author: gonchub (el niño parser)
 * 
 * Created on March 21, 2013, 9:25 PM
 */

#include <model/persistence/ConfigurationReader.h>
#include <model/Logs/Logs.h>
using namespace std;

/**
 * YAML Configuration file position.
 */
#define PLAYERVIEWS_POSITION 0
#define ENTITYVIEWS_POSITION 1
#define GAME_CONFIGURATION_POSITION 2
#define TILE_DEFINITION_POSITION 3
#define MAP_DIMENSION_POSITION 4
#define PLAYER_LOCATIONS_POSITION 5
#define ENTITY_LOCATIONS_POSITION 6
#define MAP_TILES_POSITION 7

#define DEFAULT_ROWS 20
#define DEFAULT_COLS 20
#define DEFAULT_POSITION 0 // [0, 0, 0]
#define DEFAULT_DAMAGE 5
#define DEFAULT_RANGE 1
#define DEFAULT_SPEED 5
#define DEFAULT_NUMBER_CLIPS 0
#define DEFAULT_IMAGE_WIDTH 250
#define DEFAULT_IMAGE_HEIGHT 250
#define DEFAULT_FPS 30
#define DEFAULT_DELAY 3
#define DEFAULT_REPEATS 2
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_BPP 1
#define DEFAULT_BASE_LENGTH 1
#define DEFAULT_BASE_WIDTH 1
#define DEFAULT_TEXTURE "grass"
#define DEFAULT_GAME_MUSIC "resources/sound/NamiLogin.ogg"
#define DEFAULT_MENU_IMAGE "resources/lich.jpg"
#define DEFAULT_MENU_MUSIC "resources/sound/pirates.ogg"
#define DEFAULT_IMAGE_SRC "resources/questionMark.png"
#define DEFAULT_NAME "UndefinedName"

// Error Log.
Logs errorLog;

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

void printHeader(std::string title) {
	unsigned int titleLength = title.size() + 2;
	std::string output;
	output.append("*****");
	for (unsigned j = 0; j < titleLength; j++) {
		output.append("*");
	}
	output.append("*****");
	output.append("\n");

	output.append("***** ");
	output.append(title);
	output.append(" *****");
	output.append("\n");

	output.append("*****");
	for (unsigned j = 0; j < titleLength; j++) {
		output.append("*");
	}
	output.append("*****");
	output.append("\n");

	std::cout << output;
}

/**
 * Prints a tile to check for parsing integrity.
 */
void printTile(Tile* tile, std::ofstream& outputFile) {
	outputFile << "  - position: [" << tile->getPosition()->getX() << ", "
			<< tile->getPosition()->getY() << ", "
			<< tile->getPosition()->getZ() << "]" << std::endl;
	outputFile << "    texture: " << tile->getTextureIdentifier() << std::endl;
}

/**
 * Prints an personaje to check if it was parsed correctly.
 */
void printPlayer(Player* player, std::ofstream& outputFile) {
	outputFile << "    player:\n";
	outputFile << "      name: ";
	outputFile << player->getName() << std::endl;
	outputFile << "      position: [";
	outputFile << player->getCurrentPos()->getX() << ", ";
	outputFile << player->getCurrentPos()->getY() << ", ";
	outputFile << player->getCurrentPos()->getZ() << "]\n";
	outputFile << "      speed:\n";
	outputFile << "        magnitude: ";
	outputFile << player->getSpeed()->getMagnitude() << std::endl;
	outputFile << "        direction: [";
	outputFile << player->getSpeed()->getDirection().getX() << ", ";
	outputFile << player->getSpeed()->getDirection().getY() << "]";
	outputFile << std::endl;
	outputFile << "      powers:\n";
	for (unsigned i = 0; i < player->getPowers().size(); i++) {
		outputFile << "        - name: " << player->getPowers()[i]->getName()
				<< std::endl;
		outputFile << "          damage: "
				<< player->getPowers()[i]->getDamage() << std::endl;
		outputFile << "          range: " << player->getPowers()[i]->getRange()
				<< std::endl;
	}

}

/**
 * Prints an personaje to check if it was parsed correctly.
 */
void printEntity(Entity* parsedPersonaje, std::ofstream& outputFile) {
	outputFile << "    entity:\n";
	outputFile << "      name: ";
	outputFile << parsedPersonaje->getName() << std::endl;
	outputFile << "      position: [";
	outputFile << parsedPersonaje->getCurrentPos()->getX() << ", ";
	outputFile << parsedPersonaje->getCurrentPos()->getY() << ", ";
	outputFile << "0" << "]\n";
}

/**
 * Prints a list of player views.
 */
void printPlayerViews(std::vector<PlayerView*> entityViews,
		std::ofstream& outputFile) {

	outputFile << "- playerViews:" << std::endl;
	for (unsigned int j = 0; j < entityViews.size(); j++) {
		PlayerView* playerView = entityViews[j];
		outputFile << "  - imageSrc: " << playerView->getImagePath()
				<< std::endl;
		outputFile << "    fps: " << playerView->getFps() << std::endl;
		outputFile << "    delay: " << playerView->getDelay() << std::endl;
		printPlayer((Player*) playerView->getEntity(), outputFile);
	}

}

/**
 * Prints a list of player views.
 */
void printEntityViews(std::vector<EntityView*> entityViews,
		std::ofstream& outputFile) {

	outputFile << "- entityViews:" << std::endl;
	for (unsigned int j = 0; j < entityViews.size(); j++) {
		EntityView* entityView = entityViews[j];
		outputFile << "  - imageSrc: " << entityView->getImagePath()
				<< std::endl;
		outputFile << "    fps: " << entityView->getFps() << std::endl;
		outputFile << "    delay: " << entityView->getDelay() << std::endl;
		printEntity(entityView->getEntity(), outputFile);
	}

}

/**
 * Prints an personaje to check if it was parsed correctly.
 */
void printTextureHolder(TextureHolder* parsedTileDefinition,
		std::ofstream& outputFile) {

	std::vector<std::string> textureIds;

	textureIds.push_back("soil");
	textureIds.push_back("neutral");
	textureIds.push_back("water");
	textureIds.push_back("tree");

	outputFile << "- tiles:" << std::endl;

	for (unsigned int i = 0; i < textureIds.size(); i++) {
		outputFile << "  - identifier: ";
		outputFile << textureIds[i] << std::endl;
		outputFile << "    imageSrc: ";
		outputFile << parsedTileDefinition->getTextureSrc(textureIds[i]);
		outputFile << std::endl;
	}

}

/**
 * Prints the dimensions of a map.
 */
void printMapDimensions(AuxMap &mapConfiguration, std::ofstream& outputFile) {

	outputFile << "- mapDimensions: ";
	outputFile << "[" << mapConfiguration.dimension.nrows;
	outputFile << ", " << mapConfiguration.dimension.ncols << "]" << std::endl;

}

/**
 * Prints the configured tiles of a map.
 */
void printConfiguredTiles(AuxMap &mapConfiguration, std::ofstream& outputFile) {

	outputFile << "- tileLocations:" << std::endl;

	for (unsigned int i = 0; i < mapConfiguration.tileList.size(); i++) {
		printTile(mapConfiguration.tileList[i], outputFile);
	}
}

/**
 * Prints the map configuration.
 */
void printMapConfiguration(AuxMap &mapConfiguration,
		std::ofstream& outputFile) {

	printMapDimensions(mapConfiguration, outputFile);
	printConfiguredTiles(mapConfiguration, outputFile);

}

/**
 * Prints the animated configuration.
 */
void printGameConfiguration(GameConfiguration* aConfig,
		std::ofstream& outputFile) {

	outputFile << "- gameConfiguration:" << std::endl;
	outputFile << "    fps: " << aConfig->getFps() << std::endl;
	outputFile << "    delay: " << aConfig->getDelay() << std::endl;
	outputFile << "    gameMusic: " << aConfig->getGameMusicSrc() << std::endl;
	outputFile << "    defaultScreenHeight: "
			<< aConfig->getDefaultScreenHeight() << std::endl;
	outputFile << "    defaultScreenWidth: " << aConfig->getDefaultScreenWidth()
			<< std::endl;
	outputFile << "    defaultBPP: " << aConfig->getDefaultBPP() << std::endl;
	outputFile << "    menuBackImage: " << aConfig->getMenuBackImageSrc()
			<< std::endl;
	outputFile << "    menuBackMusic: " << aConfig->getMenuBackMusicSrc()
			<< std::endl;

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

	try {
		yamlNode[0] >> auxX;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing X Position coordinate: ")
						+ yamlException.what());
		auxX = DEFAULT_POSITION;
	}
	try {
		yamlNode[1] >> auxY;

	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Y Position coordinate: ")
						+ yamlException.what());
		auxY = DEFAULT_POSITION;
	}
	try {
		yamlNode[2] >> auxZ;

	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Z Position coordinate: ")
						+ yamlException.what());
		auxZ = DEFAULT_POSITION;
	}

	vector->setX(auxX);
	vector->setY(auxY);
	vector->setZ(auxZ);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Position.
 */
void operator >>(const YAML::Node& yamlNode, Vector2* vector) {
	int auxX, auxY;

	try {
		yamlNode[0] >> auxX;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing X Vector2 coordinate: ")
						+ yamlException.what());
		auxX = DEFAULT_POSITION;
	}
	try {
		yamlNode[1] >> auxY;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Y Vector2 coordinate: ")
						+ yamlException.what());
		auxY = DEFAULT_POSITION;
	}

	vector->setValues(auxX, auxY);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Power.
 */
void operator >>(const YAML::Node& yamlNode, Power* power) {
	std::string auxName;
	int auxDamage, auxRange;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Power name: ") + yamlException.what());
		auxDamage = DEFAULT_DAMAGE;
	}
	try {
		yamlNode["damage"] >> auxDamage;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing Power damage: ")+yamlException.what());
		auxRange = DEFAULT_RANGE;
	}
	try {
		yamlNode["range"] >> auxRange;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing Power range: ")+yamlException.what());
		auxRange = DEFAULT_RANGE;
	}

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
	Vector2* auxPosition = new Vector2(0, 0);

	try {
		yamlNode["magnitude"] >> auxMagnitude;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Speed magnitude: ")
						+ yamlException.what());
		auxMagnitude = DEFAULT_SPEED;
	}
	try {
		yamlNode["direction"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing Speed: ")+yamlException.what());
	}

	speed->setMagnitude(auxMagnitude);
	speed->setDirection(Vector2(auxPosition->getX(), auxPosition->getY()));
	delete auxPosition;
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Player* personaje) {

	Position* auxPosition = new Position(0, 0, 0);
	Speed* auxSpeed = new Speed(0, Vector2(0, 0));
	std::string auxName;
	std::vector<Power*> auxPowers;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Player name: ") + yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Player position: ")
						+ yamlException.what());
	}
	try {
		yamlNode["speed"] >> auxSpeed;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Player speed: ") + yamlException.what());
		auxSpeed->setMagnitude(DEFAULT_SPEED);
		auxSpeed->setDirection(Vector2(0, 0));
	}
	try {
		const YAML::Node& powers = yamlNode["powers"];
		for (unsigned i = 0; i < powers.size(); i++) {
			Power* power = new Power("", 0, 0);
			powers[i] >> power;
			auxPowers.push_back(power);
		}

	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Player powers: ") + yamlException.what());
	}

	personaje->setName(auxName);
	personaje->setPowers(auxPowers);
	personaje->setCoordinates(auxPosition->getX(), auxPosition->getY());
	//Tile* newTile = new Tile(personaje->getCoordinates());
	//personaje->setTile(newTile);
	personaje->setSpeed(auxSpeed);
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Entity* entity) {

	Position* auxPosition = new Position(0, 0, 0);
	std::string auxName;
	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Entity name: ") + yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Entity position: ")
						+ yamlException.what());
	}

	entity->setName(auxName);
	entity->setPos(auxPosition->getX(), auxPosition->getY());
}

/**
 * Sobrecarga de operador >> para llenar los campos de una PersonajeVista.
 */
void operator >>(const YAML::Node& yamlNode, PlayerView* playerView) {

	std::vector<Power*> auxPowers;
	Player* auxPlayer = NULL;
	std::string auxImageSrc;
	std::string auxName;
	Vector2* auxAnchorPixel = new Vector2(0, 0);
	int auxImageWidth, auxImageHeight, auxNumberOfClips, auxFps, auxDelay,
			auxAnimationNumberOfRepeats, auxBaseWidth, auxBaseLength;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing PlayerView name: ")
						+ yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["imageSrc"] >> auxImageSrc;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing PlayerView image source: ")
						+ yamlException.what());
		auxImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["anchorPixel"] >> auxAnchorPixel;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
	}
	try {
		yamlNode["imageWidth"] >> auxImageWidth;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing PlayerView image width: ")
						+ yamlException.what());
		auxImageWidth = DEFAULT_IMAGE_WIDTH;
	}
	try {
		yamlNode["imageHeight"] >> auxImageHeight;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing PlayerView image height: ")
						+ yamlException.what());
		auxImageHeight = DEFAULT_IMAGE_HEIGHT;
	}
	try {
		yamlNode["numberOfClips"] >> auxNumberOfClips;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxNumberOfClips = DEFAULT_NUMBER_CLIPS;
	}
	try {
		yamlNode["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		yamlNode["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
	}
	try {
		yamlNode["baseWidth"] >> auxBaseWidth;

	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseWidth = DEFAULT_BASE_WIDTH;
	}
	try {
		yamlNode["baseHeight"] >> auxBaseLength;

	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseLength = DEFAULT_BASE_LENGTH;
	}

	playerView->setName(auxName);
	playerView->setAnchorPixel(auxAnchorPixel);
//	playerView->setBaseHeight(auxBaseLength);
//	playerView->setBaseWidth(auxBaseWidth);
	playerView->cargarImagen(auxImageSrc);
	playerView->setImageHeight(auxImageHeight);
	playerView->setImageWidth(auxImageWidth);
	playerView->setImagePath(auxImageSrc);
	playerView->setNClips(auxNumberOfClips);
	playerView->setFps(auxFps);
	playerView->setDelay(auxDelay);
	playerView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
	playerView->setEntity(auxPlayer);

}

/**
 * Sobrecarga de operador >> para llenar los campos de una EntityView.
 */
void operator >>(const YAML::Node& yamlNode, EntityView* entityView) {

	Entity* auxEntity = NULL;
	Vector2* auxAnchorPixel = new Vector2(0, 0);
	std::vector<Power*> auxPowers;
	std::string auxImageSrc;
	std::string auxName;
	int auxImageWidth, auxImageHeight, auxNumberOfClips, auxFps, auxDelay,
			auxAnimationNumberOfRepeats, auxBaseLength, auxBaseWidth;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing EntityView name: ")
						+ yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["imageSrc"] >> auxImageSrc;
		yamlNode["imageWidth"] >> auxImageWidth;
		yamlNode["imageHeight"] >> auxImageHeight;
		yamlNode["numberOfClips"] >> auxNumberOfClips;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing EntityView: ")
						+ yamlException.what());
		auxImageSrc = DEFAULT_IMAGE_SRC;
		auxImageWidth = DEFAULT_IMAGE_WIDTH;
		auxImageHeight = DEFAULT_IMAGE_HEIGHT;
		auxNumberOfClips = DEFAULT_NUMBER_CLIPS;
	}
	try {
		yamlNode["anchorPixel"] >> auxAnchorPixel;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing EntityView: ") + yamlException.what());
	}
	try {
		yamlNode["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		yamlNode["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing EntityView: ") + yamlException.what());
		auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
	}
	try {
		yamlNode["baseWidth"] >> auxBaseWidth;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxBaseWidth = DEFAULT_BASE_WIDTH;
	}
	try {
		yamlNode["baseHeight"] >> auxBaseLength;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxBaseLength = DEFAULT_BASE_LENGTH;
	}

	entityView->setAnchorPixel(auxAnchorPixel);
//	entityView->setBaseWidth(auxBaseWidth);
//	entityView->setBaseHeight(auxBaseLength);
	entityView->setName(auxName);
	entityView->setImageHeight(auxImageHeight);
	entityView->setImageWidth(auxImageWidth);
	entityView->setNClips(auxNumberOfClips);
	entityView->setEntity(auxEntity);
	entityView->setImagePath(auxImageSrc);
	entityView->setFps(auxFps);
	entityView->setDelay(auxDelay);
	entityView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
}

void operator >>(const YAML::Node& yamlNode,
		std::vector<Entity*>& entityVector) {
	const YAML::Node& entityLocations = yamlNode["entityLocations"];
	for (unsigned i = 0; i < entityLocations.size(); i++) {
		Entity* entity = new Entity();
		try {
			entityLocations[i] >> entity;
			entityVector.push_back(entity);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing Entity Location List: ")
							+ yamlException.what());
		}
	}
}

void operator >>(const YAML::Node& yamlNode,
		std::vector<Player*>& playerVector) {
	const YAML::Node& playerLocations = yamlNode["playerLocations"];
	for (unsigned i = 0; i < playerLocations.size(); i++) {
		Player* player = new Player();
		try {
			playerLocations[i] >> player;
			playerVector.push_back(player);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing Player Location List: ")
							+ yamlException.what());
		}
	}
}

/**
 * Sobrecarga de operador >> para llenar los datos de una lista de entidades.
 */
void operator >>(const YAML::Node& yamlNode,
		std::vector<PlayerView*>& entityList) {
	const YAML::Node& playerViews = yamlNode["playerViews"];
	for (unsigned i = 0; i < playerViews.size(); i++) {
		PlayerView* entityView = new PlayerView();
		try {
			playerViews[i] >> entityView;
			entityList.push_back(entityView);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing PlayerView List: ")
							+ yamlException.what());
		}
	}
}

/**
 * Sobrecarga de operador >> para llenar los datos de una lista de entidades.
 */
void operator >>(const YAML::Node& yamlNode,
		std::vector<EntityView*>& entityList) {
	const YAML::Node& entityViews = yamlNode["entityViews"];
	for (unsigned i = 0; i < entityViews.size(); i++) {
		EntityView* entityView = new EntityView();
		try {
			entityViews[i] >> entityView;
			entityList.push_back(entityView);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing EntityView List: ")
							+ yamlException.what());
		}
	}
}

/* ******************************************************* *
 * *********** ANIMATION CONFIGURATION PARSING *********** *
 * ******************************************************* */

/**
 * Sobrecarga de operador >> para parsear un AnimationConfiguration.
 */
void operator >>(const YAML::Node& yamlNode,
		GameConfiguration* animationConfig) {
	const YAML::Node& configuration = yamlNode["gameConfiguration"];
	unsigned int auxFps, auxDelay, auxHeight, auxWidth, auxBPP;
	std::string auxGameMusicSrc, auxMenuImage, auxMenuMusic;
	try {
		configuration["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing GameConfiguration FPS: ")
						+ yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		configuration["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		configuration["gameMusic"] >> auxGameMusicSrc;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing GameConfiguration game music: ")
						+ yamlException.what());
		auxGameMusicSrc = DEFAULT_GAME_MUSIC;
	}
	try {
		configuration["defaultScreenHeight"] >> auxHeight;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxHeight = DEFAULT_SCREEN_HEIGHT;
	}
	try {
		configuration["defaultScreenWidth"] >> auxWidth;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxWidth = DEFAULT_SCREEN_WIDTH;
	}
	try {
		configuration["defaultBPP"] >> auxBPP;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxBPP = DEFAULT_BPP;
	}
	try {
		configuration["menuBackImage"] >> auxMenuImage;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxMenuImage = DEFAULT_MENU_IMAGE;
	}
	try {
		configuration["menuBackMusic"] >> auxMenuMusic;
	} catch (YAML::Exception& yamlException) {
//		errorLog.logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxMenuMusic = DEFAULT_MENU_MUSIC;
	}

	animationConfig->setDelay(auxDelay);
	animationConfig->setFps(auxFps);
	animationConfig->setGameMusicSrc(auxGameMusicSrc);
	animationConfig->setDefaultScreenHeight(auxHeight);
	animationConfig->setDefaultScreenWidth(auxWidth);
	animationConfig->setDefaultBPP(auxBPP);
	animationConfig->setMenuBackImageSrc(auxMenuImage);
	animationConfig->setMenuBackMusicSrc(auxMenuMusic);
}

/* *********************************************** *
 * *********** TILE DEFINITION PARSING *********** *
 * *********************************************** */

/**
 * Extraction operator for tile definition.
 */
void operator >>(const YAML::Node& yamlNode, TileDefinition* tileDefinition) {

	std::string auxId, auxSrc;
	try {
		yamlNode["identifier"] >> auxId;
		yamlNode["imageSrc"] >> auxSrc;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing TileDefinition: ")
						+ yamlException.what());
	}
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
		try {
			yamlTileDefinitions[i] >> tileDef;
			textureHolder->addTexture(tileDef);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing TextureHolder: ")
							+ yamlException.what());
		}
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
	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Tile position: ") + yamlException.what());
	}
	try {
		yamlNode["texture"] >> auxId;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing Tile id: ") + yamlException.what());
		auxId = DEFAULT_TEXTURE;
	}

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
		try {
			tileList[i] >> tile;
			destMap.tileList.push_back(tile);
		} catch (YAML::Exception& yamlException) {
			errorLog.logErrorMessage(
					string("Error parsing tile locations: ")
							+ yamlException.what());
		}
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
	try {
		yamlDimensions[0] >> dimension.nrows;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing map dimension rows: ")
						+ yamlException.what());
		dimension.nrows = DEFAULT_ROWS;
	}
	try {
		yamlDimensions[1] >> dimension.ncols;
	} catch (YAML::Exception& yamlException) {
		errorLog.logErrorMessage(
				string("Error parsing map dimension columns: ")
						+ yamlException.what());
		dimension.ncols = DEFAULT_COLS;
	}
}

/* ************************************************ *
 * *********** ENTITY VIEW MAP CREATION *********** *
 * ************************************************ */

void loadEntityViewMap(EntityViewMap* entityViewMap,
		std::vector<PlayerView*> entityViewVector) {

	for (unsigned int j = 0; j < entityViewVector.size(); j++) {
		PlayerView* parsedEntityView = entityViewVector[j];
		Player* personaje = (Player*) parsedEntityView->getEntity();

		entityViewMap->positionEntityView(parsedEntityView,
				*personaje->getCoordinates());
	}

}

void loadEntityViewMap(EntityViewMap* entityViewMap,
		std::vector<EntityView*> entityViewVector) {

	for (unsigned int j = 0; j < entityViewVector.size(); j++) {
		EntityView* parsedEntityView = entityViewVector[j];
		Entity* personaje = parsedEntityView->getEntity();

		/**
		 * TODO: hacer conversion entre Posicion y Coordinates.
		 */
		Coordinates coordinates = Coordinates(
				personaje->getCurrentPos()->getX(),
				personaje->getCurrentPos()->getY());

		entityViewMap->positionEntityView(parsedEntityView, coordinates);
	}

}

void duplicateView(EntityView* sourceView, EntityView* destView) {

//Duplico el anchor
	Vector2* anchor = sourceView->getAnchorPixel();
	Vector2* anchorCopy = new Vector2(anchor->getX(), anchor->getY());

	destView->setAnchorPixel(anchorCopy);
	destView->setName(sourceView->getName());
	destView->setImageHeight(sourceView->getImageHeight());
	destView->setImageWidth(sourceView->getImageWidth());
	destView->setNClips(sourceView->getNClips());
	destView->setImagePath(sourceView->getImagePath());
	destView->setFps(sourceView->getFps());
	destView->setDelay(sourceView->getDelay());
	destView->setNumberOfRepeats(sourceView->getNumberOfRepeats());
	destView->setEntity(NULL);

}

void duplicateView(PlayerView* sourceView, PlayerView* destView) {

//Duplico el anchor
	Vector2* anchor = sourceView->getAnchorPixel();
	Vector2* anchorCopy = new Vector2(anchor->getX(), anchor->getY());

	destView->setAnchorPixel(anchorCopy);
	destView->setName(sourceView->getName());
	destView->setImageHeight(sourceView->getImageHeight());
	destView->setImageWidth(sourceView->getImageWidth());
	destView->setNClips(sourceView->getNClips());
	destView->setImagePath(sourceView->getImagePath());
	destView->setFps(sourceView->getFps());
	destView->setDelay(sourceView->getDelay());
	destView->setNumberOfRepeats(sourceView->getNumberOfRepeats());
	destView->cargarImagen(sourceView->getImagePath());
	destView->setEntity(NULL);

}

std::vector<EntityView*> assignEntities(std::vector<EntityView*> entityViews,
		std::vector<Entity*> entities) {
	std::vector<EntityView*> completeViews;
	for (unsigned i = 0; i < entities.size(); i++) {
		Entity* actualEntity = entities[i];
		std::string entityName = actualEntity->getName();
		for (unsigned j = 0; j < entityViews.size(); j++) {
			EntityView* actualView = entityViews[j];
			if ((actualView->getName()).compare(entityName) == 0) {
				EntityView* duplicate = new EntityView();
				duplicateView(actualView, duplicate);
				duplicate->setEntity(actualEntity);
				completeViews.push_back(duplicate);
			}
		}
	}
	return completeViews;
}

std::vector<PlayerView*> assignPlayers(std::vector<PlayerView*> playerViews,
		std::vector<Player*> players) {
	std::vector<PlayerView*> completeViews;
	for (unsigned i = 0; i < players.size(); i++) {
		Player* actualPlayer = players[i];
		std::string entityName = actualPlayer->getName();
		for (unsigned j = 0; j < playerViews.size(); j++) {
			PlayerView* actualView = playerViews[j];
			if ((actualView->getName()).compare(entityName) == 0) {
				PlayerView* duplicate = new PlayerView();
				duplicateView(actualView, duplicate);
				duplicate->setEntity(actualPlayer);
				duplicate->setPersonaje(actualPlayer);
				completeViews.push_back(duplicate);
			}
		}
	}
	return completeViews;
}

void cleanUnusedViews(std::vector<PlayerView*> viewVector) {
	std::vector<PlayerView*> cleanVector;
	for (unsigned i = 0; i < viewVector.size(); i++) {
		if (viewVector[i]->getEntity() == NULL) {
			delete viewVector[i];
		} else {
			cleanVector.push_back(viewVector[i]);
		}
	}
}

void cleanUnusedViews(std::vector<EntityView*> viewVector) {
	std::vector<EntityView*> cleanVector;
	for (unsigned i = 0; i < viewVector.size(); i++) {
		if (viewVector[i]->getEntity() == NULL) {
			delete viewVector[i];
		} else {
			cleanVector.push_back(viewVector[i]);
		}
	}
}

/* ********************************************* *
 * *********** CONFIGURATION LOADING *********** *
 * ********************************************* */

/**
 * Loads the configuration, prints its output and returns
 * a persistent configuration object.
 */
PersistentConfiguration ConfigurationReader::loadConfiguration(
		std::string configurationFile, std::string outputFilename) {

	/**
	 * TODO: hacer free de los structs auxiliares luego
	 * de parsear.
	 */
	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);
	std::ofstream outputFile(outputFilename.c_str());

// Error Check
	if (!inputFile) {
		//cout << "No se encontro el archivo de configuracion" << std::endl;
		errorLog.logErrorMessage(
				string("No se encontro el archivo de configuracion: "));
		exit(1);
	}

// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

// Parsing PersonajeVista.
	std::vector<PlayerView*> playerViewVector;
	try {
		yamlNode[PLAYERVIEWS_POSITION] >> playerViewVector;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

// Parsing EntityViews.
	std::vector<EntityView*> entityViewVector;
	try {
		yamlNode[ENTITYVIEWS_POSITION] >> entityViewVector;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

// Parsing animation configuration.
	GameConfiguration* animationConfig = new GameConfiguration();
	try {
		yamlNode[GAME_CONFIGURATION_POSITION] >> animationConfig;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

// Parsing tile definition.
	TextureHolder* textureHolder = new TextureHolder();
	try {
		yamlNode[TILE_DEFINITION_POSITION] >> textureHolder;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

// Parsing map dimensions.
	AuxMap mapConfiguration;
	AuxMapDimension mapDimension;
	try {
		yamlNode[MAP_DIMENSION_POSITION] >> mapDimension;
		mapConfiguration.dimension = mapDimension;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

	MapData* mapData = new MapData(mapConfiguration.dimension.nrows,
			mapConfiguration.dimension.ncols);

// Parsing player locations.
	std::vector<Player*> playerVector;
	try {
		yamlNode[PLAYER_LOCATIONS_POSITION] >> playerVector;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

// Parsing player locations.
	std::vector<Entity*> entityVector;
	try {
		yamlNode[ENTITY_LOCATIONS_POSITION] >> entityVector;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}

	std::vector<EntityView*> cleanEntityViews = assignEntities(entityViewVector,
			entityVector);
	std::vector<PlayerView*> cleanPlayerViews = assignPlayers(playerViewVector,
			playerVector);

	cleanUnusedViews(entityViewVector);
	cleanUnusedViews(playerViewVector);

// Parsing map tile locations.
	try {
		yamlNode[MAP_TILES_POSITION] >> mapConfiguration;
	} catch (YAML::Exception& yamlException) {
		//std::cout << yamlException.what() << "\n";
		errorLog.logErrorMessage(yamlException.what());
	}
	mapConfiguration >> mapData;

// Create entityViewMap:
	EntityViewMap* entityViewMap = new EntityViewMap(mapData->GetNRows(),
			mapData->GetNCols());
	loadEntityViewMap(entityViewMap, cleanPlayerViews);
	loadEntityViewMap(entityViewMap, cleanEntityViews);

// Packing parser results.
	PersistentConfiguration configuration = PersistentConfiguration();
	configuration.setEntityViewMap(entityViewMap);
	configuration.setTextureHolder(textureHolder);
	configuration.setMapData(mapData);
	configuration.setAnimationConfiguration(animationConfig);

// Print parsed elements.
//	printPlayerViews(playerViewVector, outputFile);
//	printEntityViews(entityViewVector, outputFile);
//	printGameConfiguration(animationConfig, outputFile);
//	printTextureHolder(textureHolder, outputFile);
//	printMapConfiguration(mapConfiguration, outputFile);
	printHeader("END OF PARSER");

	return configuration;
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}
