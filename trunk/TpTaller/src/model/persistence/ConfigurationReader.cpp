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
#define MOBILEENTITYVIEWS_POSITION 1
#define ENTITYVIEWS_POSITION 2
#define GAME_CONFIGURATION_POSITION 3
#define TILE_DEFINITION_POSITION 4
#define MAP_DIMENSION_POSITION 5
#define PLAYER_LOCATIONS_POSITION 6
#define MOBILEENTITY_LOCATIONS_POSITION 7
#define ITEM_LOCATIONS_POSITION 8
#define ENTITY_LOCATIONS_POSITION 9
#define MAP_TILES_POSITION 10

#define DEFAULT_ROWS 50
#define DEFAULT_COLS 50
#define DEFAULT_POSITION 0 // [0, 0, 0]
#define DEFAULT_DAMAGE 5
#define DEFAULT_RANGE 1
#define DEFAULT_SPEED 5
#define DEFAULT_VIEWRANGE 250
#define DEFAULT_NUMBER_CLIPS 0
#define DEFAULT_IMAGE_WIDTH 250
#define DEFAULT_IMAGE_HEIGHT 250
#define DEFAULT_FPS 30
#define DEFAULT_DELAY 3
#define DEFAULT_MOVEMENT_MARGIN 50
#define DEFAULT_REPEATS 2
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_BPP 1
#define DEFAULT_BASE_COLS 1
#define DEFAULT_BASE_ROWS 1
#define DEFAULT_TEXTURE "grass"
#define DEFAULT_GAME_MUSIC "resources/sound/stalemate.ogg"
#define DEFAULT_MENU_IMAGE "resources/wastelands.jpg"
#define DEFAULT_MENU_MUSIC "resources/sound/pirates.ogg"
#define DEFAULT_IMAGE_SRC "resources/questionMark.png"
#define DEFAULT_NAME "UndefinedName"
#define DEFAULT_MISSION "TeamFight"
#define DEFAULT_PORT 32001
#define DEFAULT_IP "192.168.190.136"
#define DEFAULT_TILE_WIDTH	70
#define DEFAULT_TILE_HEIGHT	50

// Excepcion que se lanza al recibir un nombre que no fue declarado previamente
class PlayerLocationNameExcepton: public std::exception {
public:
	const char* what() const throw () {
		return "playerLocations error. Name not found in declaration.";
	}
};

// Error Log.
Logs errorLog;
TextureHolder* textureHolder = NULL;
int rows;
int cols;

// Player name, for checking the player location.
map<string, int> playerName;

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
	outputFile << player->getSpeed()->getDirection()->getX() << ", ";
	outputFile << player->getSpeed()->getDirection()->getY() << "]";
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
		//outputFile << "  - imageSrc: " << playerView->getImagePath()
		//		<< std::endl;
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
		//outputFile << "  - imageSrc: " << entityView->getImagePath()
		//		<< std::endl;
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
		Logs::logErrorMessage(
				string("Error parsing X Position coordinate: ")
						+ yamlException.what());
		auxX = DEFAULT_POSITION;
	}
	try {
		yamlNode[1] >> auxY;

	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Y Position coordinate: ")
						+ yamlException.what());
		auxY = DEFAULT_POSITION;
	}
	try {
		yamlNode[2] >> auxZ;

	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
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
		Logs::logErrorMessage(
				string("Error parsing X Vector2 coordinate: ")
						+ yamlException.what());
		auxX = DEFAULT_POSITION;
	}
	try {
		yamlNode[1] >> auxY;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
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
		Logs::logErrorMessage(
				string("Error parsing Power name: ") + yamlException.what());
		auxDamage = DEFAULT_DAMAGE;
	}
	try {
		yamlNode["damage"] >> auxDamage;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing Power damage: ")+yamlException.what());
		auxRange = DEFAULT_RANGE;
	}
	try {
		yamlNode["range"] >> auxRange;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing Power range: ")+yamlException.what());
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
		Logs::logErrorMessage(
				string("Error parsing Speed magnitude: ")
						+ yamlException.what());
		auxMagnitude = DEFAULT_SPEED;
	}
	try {
		yamlNode["direction"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing Speed: ")+yamlException.what());
	}

	speed->setMagnitude(auxMagnitude);
	speed->setDirection(new Vector2(auxPosition->getX(), auxPosition->getY()));
	delete auxPosition;
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Player* personaje) {

	Position* auxPosition = new Position(0, 0, 0);
	Speed* auxSpeed = new Speed(0, new Vector2(0, 0));
	bool auxMainPlayer;
	int auxViewRange;
	std::string auxName;
	std::vector<Power*> auxPowers;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player name: ") + yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player position: ")
						+ yamlException.what());
	}
	try {
		yamlNode["speed"] >> auxSpeed;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player speed: ") + yamlException.what());
		auxSpeed->setMagnitude(DEFAULT_SPEED);
		auxSpeed->setDirection(new Vector2(0, 0));
	}
	try {
		const YAML::Node& powers = yamlNode["powers"];
		for (unsigned i = 0; i < powers.size(); i++) {
			Power* power = new Power("", 0, 0);
			powers[i] >> power;
			auxPowers.push_back(power);
		}

	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player powers: ") + yamlException.what());
	}
	try {
		yamlNode["mainPlayer"] >> auxMainPlayer;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player mainPlayer: ")
						+ yamlException.what());
		auxMainPlayer = false;
	}
	if (auxMainPlayer) {
		try {
			yamlNode["viewRange"] >> auxViewRange;
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
					string("Error parsing Player viewRange: ")
							+ yamlException.what());
			auxViewRange = DEFAULT_VIEWRANGE;
		}
	}

	int row = auxPosition->getX();
	int col = auxPosition->getY();
	stringstream sRow;
	stringstream sCol;
	stringstream sZ;

	sRow << row;
	sCol << col;
	sZ << auxPosition->getZ();

	if (row < 0 || row >= rows) {
		Logs::logErrorMessage(
				string(
						"Error parsing Player position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		auxPosition->setX(0);
	}
	if (col < 0 || col >= cols) {
		Logs::logErrorMessage(
				string(
						"Error parsing Player position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		auxPosition->setY(0);
	}

	personaje->setName(auxName);
	personaje->setPowers(auxPowers);
	personaje->setCoordinates(auxPosition->getX(), auxPosition->getY());
	Coordinates* playerCoords = new Coordinates(auxPosition->getX(),
			auxPosition->getY());
	Tile* newTile = new Tile(playerCoords);
	personaje->setTile(newTile);
	personaje->setSpeed(auxSpeed);
	if (auxMainPlayer) {
		personaje->setAsMainPlayer();
		personaje->setViewRange(auxViewRange);
	}
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Entity* entity) {

	Position* auxPosition = new Position(0, 0, 0);
	std::string auxName;
	//TOMI: Antes controlabamos excepciones. Lo saque, porque no tenia sentido ubicar una Entity que no existia.
	yamlNode["name"] >> auxName;

	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Entity position: ")
						+ yamlException.what());
	}

	int row = auxPosition->getX();
	int col = auxPosition->getY();
	//printf("row :%i col: %i\n ",row,col);

	stringstream sRow;
	stringstream sCol;
	stringstream sZ;

	sRow << row;
	sCol << col;
	sZ << auxPosition->getZ();

	if (row < 0 || row >= rows) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		entity = NULL;
		delete auxPosition;
		return;
	}
	if (col < 0 || col >= cols) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		entity = NULL;
		delete auxPosition;
		return;
	}

	entity->setName(auxName);
	entity->setCoordinates(auxPosition->getX(), auxPosition->getY());
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, Item* item) {

	Position* auxPosition = new Position(0, 0, 0);
	std::string auxName;

	yamlNode["name"] >> auxName;

	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Entity position: ")
						+ yamlException.what());
	}

	int row = auxPosition->getX();
	int col = auxPosition->getY();
	//printf("row :%i col: %i\n ",row,col);

	stringstream sRow;
	stringstream sCol;
	stringstream sZ;

	sRow << row;
	sCol << col;
	sZ << auxPosition->getZ();

	if (row < 0 || row >= rows) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		item = NULL;
		delete auxPosition;
		return;
	}
	if (col < 0 || col >= cols) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		item = NULL;
		delete auxPosition;
		return;
	}

	item->setName(auxName);
	item->setCoordinates(auxPosition->getX(), auxPosition->getY());
}

/**
 * Sobrecarga de operador >> para llenar los datos de un puntero
 * a Personaje.
 */
void operator >>(const YAML::Node& yamlNode, MobileEntity* mobileEntity) {

	Position* auxPosition = new Position(0, 0, 0);
	std::string auxName;
	Speed* auxSpeed = new Speed(0, new Vector2(0, 0));
	yamlNode["name"] >> auxName;

	try {
		yamlNode["position"] >> auxPosition;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Entity position: ")
						+ yamlException.what());
	}
	try {
		yamlNode["speed"] >> auxSpeed;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing Player speed: ") + yamlException.what());
		auxSpeed->setMagnitude(DEFAULT_SPEED);
		auxSpeed->setDirection(new Vector2(0, 0));
	}

	int row = auxPosition->getX();
	int col = auxPosition->getY();
	//printf("row :%i col: %i\n ",row,col);

	stringstream sRow;
	stringstream sCol;
	stringstream sZ;

	sRow << row;
	sCol << col;
	sZ << auxPosition->getZ();

	if (row < 0 || row >= rows) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		mobileEntity = NULL;
		delete auxPosition;
		return;
	}
	if (col < 0 || col >= cols) {
		Logs::logErrorMessage(
				string(
						"Error parsing Entity position >> Entity row out of bound: ")
						+ string("name: ") + auxName + string(" position: [")
						+ sRow.str() + string(", ") + sCol.str() + string(", ")
						+ sZ.str() + string("]"));
		mobileEntity = NULL;
		delete auxPosition;
		return;
	}

	mobileEntity->setName(auxName);
	mobileEntity->setSpeed(auxSpeed);

	Coordinates* mobileEntityCoords = new Coordinates(auxPosition->getX(),
			auxPosition->getY());
	Tile* newTile = new Tile(mobileEntityCoords);

	mobileEntity->setCoordinates(auxPosition->getX(), auxPosition->getY());
	mobileEntity->setTile(newTile);
}

/**
 * Sobrecarga de operador >> para llenar los campos de una Player View.
 */
void operator >>(const YAML::Node& yamlNode, PlayerView* playerView) {
	vector<Power*> auxPowers;
	Player* auxPlayer = NULL;
	string auxWalkingImageSrc, auxRunningImageSrc, auxIdleImageSrc,
			auxAttackImageSrc, auxIdleBlocking, auxBlockingAnim,
			auxWalkingBowImageSrc, auxIdleBowImageSrc, auxAttackBowImageSrc,
			auxHit, auxDie, auxCast, auxHitBow, auxDieBow, auxCastBow, auxName;
	Vector2* auxAnchorPixel = new Vector2(0, 0);
	int auxImageWidth, auxImageHeight, auxNumberOfClips, auxFps,
			auxAnimationNumberOfRepeats, auxBaseRows, auxBaseCols;

	float auxDelay;
	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView name: ")
						+ yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["walkingImageSrc"] >> auxWalkingImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView walking image source: ")
						+ yamlException.what());
		auxWalkingImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["idleImageSrc"] >> auxIdleImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle image source: ")
						+ yamlException.what());
		auxIdleImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["runningImageSrc"] >> auxRunningImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView running image source: ")
						+ yamlException.what());
		auxRunningImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["attackImageSrc"] >> auxAttackImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView attack image source: ")
						+ yamlException.what());
		auxAttackImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["idleBlockingImageSrc"] >> auxIdleBlocking;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxIdleBlocking = DEFAULT_IMAGE_SRC;
	}

	try {
		yamlNode["castSpellImageSrc"] >> auxCast;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxCast = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["hitImageSrc"] >> auxHit;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxHit = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["dieImageSrc"] >> auxDie;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxDie = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["walkingBowImageSrc"] >> auxWalkingBowImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView walking image source: ")
						+ yamlException.what());
		auxWalkingBowImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["idleBowImageSrc"] >> auxIdleBowImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle image source: ")
						+ yamlException.what());
		auxIdleBowImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["attackBowImageSrc"] >> auxAttackBowImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView attack image source: ")
						+ yamlException.what());
		auxAttackBowImageSrc = DEFAULT_IMAGE_SRC;
	}

	try {
		yamlNode["castSpellBowImageSrc"] >> auxCastBow;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxCastBow = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["hitBowImageSrc"] >> auxHitBow;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxHitBow = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["dieBowImageSrc"] >> auxDieBow;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxDieBow = DEFAULT_IMAGE_SRC;
	}

	try {
		yamlNode["anchorPixel"] >> auxAnchorPixel;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
	}
	try {
		yamlNode["imageWidth"] >> auxImageWidth;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView image width: ")
						+ yamlException.what());
		auxImageWidth = DEFAULT_IMAGE_WIDTH;
	}
	try {
		yamlNode["imageHeight"] >> auxImageHeight;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView image height: ")
						+ yamlException.what());
		auxImageHeight = DEFAULT_IMAGE_HEIGHT;
	}
	try {
		yamlNode["numberOfClips"] >> auxNumberOfClips;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxNumberOfClips = DEFAULT_NUMBER_CLIPS;
	}
	try {
		yamlNode["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		yamlNode["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView: ") + yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
	}
	try {
		yamlNode["baseRows"] >> auxBaseRows;

	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseRows = DEFAULT_BASE_ROWS;
	}
	try {
		yamlNode["baseCols"] >> auxBaseCols;

	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseCols = DEFAULT_BASE_COLS;
	}

	playerView->setName(auxName);
	playerView->setAnchorPixel(auxAnchorPixel);
//	playerView->setBaseHeight(auxBaseCols);
//	playerView->setBaseWidth(auxBaseRows);
	playerView->setImageHeight(auxImageHeight);
	playerView->setImageWidth(auxImageWidth);
	playerView->setNClips(auxNumberOfClips);
	playerView->setFps(auxFps);
	playerView->setDelay(auxDelay);
	playerView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
	playerView->setEntity(auxPlayer);
	TextureDefinition* walkingTexture = new TextureDefinition(
			auxName + string(WALKING_MODIFIER), auxWalkingImageSrc);
	textureHolder->addTexture(walkingTexture);
	TextureDefinition* idleTexture = new TextureDefinition(
			auxName + string(IDLE_MODIFIER), auxIdleImageSrc);
	textureHolder->addTexture(idleTexture);
	TextureDefinition* runningTexture = new TextureDefinition(
			auxName + string(RUNNING_MODIFIER), auxRunningImageSrc);
	textureHolder->addTexture(runningTexture);
	TextureDefinition* attackTexture = new TextureDefinition(
			auxName + string(ATTACK_MODIFIER), auxAttackImageSrc);
	textureHolder->addTexture(attackTexture);
	TextureDefinition* idleBlockTexture = new TextureDefinition(
			auxName + string(IDLE_BLOCKING_MODIFIER), auxIdleBlocking);
	textureHolder->addTexture(idleBlockTexture);

	TextureDefinition* castTexture = new TextureDefinition(
			auxName + string(CAST_SPELL_MODIFIER), auxCast);
	textureHolder->addTexture(castTexture);
	TextureDefinition* hitTexture = new TextureDefinition(
			auxName + string(HIT_MODIFIER), auxHit);
	textureHolder->addTexture(hitTexture);
	TextureDefinition* dieTexture = new TextureDefinition(
			auxName + string(DIE_MODIFIER), auxDie);
	textureHolder->addTexture(dieTexture);

	TextureDefinition* walkingBowTexture = new TextureDefinition(
			auxName + string(BOW_WALKING_MODIFIER), auxWalkingBowImageSrc);
	textureHolder->addTexture(walkingBowTexture);
	TextureDefinition* idleBowTexture = new TextureDefinition(
			auxName + string(BOW_IDLE_MODIFIER), auxIdleBowImageSrc);
	textureHolder->addTexture(idleBowTexture);
	TextureDefinition* attackBowTexture = new TextureDefinition(
			auxName + string(BOW_ATTACK_MODIFIER), auxAttackBowImageSrc);
	textureHolder->addTexture(attackBowTexture);

	TextureDefinition* castTextureBow = new TextureDefinition(
			auxName + string(BOW_CAST_SPELL_MODIFIER), auxCastBow);
	textureHolder->addTexture(castTextureBow);
	TextureDefinition* hitTextureBow = new TextureDefinition(
			auxName + string(BOW_HIT_MODIFIER), auxHitBow);
	textureHolder->addTexture(hitTextureBow);
	TextureDefinition* dieTextureBow = new TextureDefinition(
			auxName + string(BOW_DIE_MODIFIER), auxDieBow);
	textureHolder->addTexture(dieTextureBow);

	playerView->setTextureHolder(textureHolder);
}

/**
 * Sobrecarga de operador >> para llenar los campos de una PersonajeVista.
 */
void operator >>(const YAML::Node& yamlNode,
		MobileEntityView* mobileEntityView) {

	MobileEntity* auxPlayer = NULL;
	std::string auxWalkingImageSrc;
	std::string auxRunningImageSrc;
	std::string auxIdleImageSrc;
	std::string auxAttackImageSrc;
	std::string auxIdleBlocking;
	std::string auxBlockingAnim;
	std::string auxName;
	Vector2* auxAnchorPixel = new Vector2(0, 0);
	int auxImageWidth, auxImageHeight, auxNumberOfClips, auxFps,
			auxAnimationNumberOfRepeats, auxBaseRows, auxBaseCols;

	float auxDelay;
	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView name: ")
						+ yamlException.what());
		auxName = DEFAULT_NAME;
	}
	try {
		yamlNode["walkingImageSrc"] >> auxWalkingImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView walking image source: ")
						+ yamlException.what());
		auxWalkingImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["idleImageSrc"] >> auxIdleImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle image source: ")
						+ yamlException.what());
		auxIdleImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["runningImageSrc"] >> auxRunningImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView running image source: ")
						+ yamlException.what());
		auxRunningImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["attackImageSrc"] >> auxAttackImageSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView attack image source: ")
						+ yamlException.what());
		auxAttackImageSrc = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["idleBlockingImageSrc"] >> auxIdleBlocking;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView idle block image source: ")
						+ yamlException.what());
		auxIdleBlocking = DEFAULT_IMAGE_SRC;
	}
	try {
		yamlNode["anchorPixel"] >> auxAnchorPixel;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
	}
	try {
		yamlNode["imageWidth"] >> auxImageWidth;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView image width: ")
						+ yamlException.what());
		auxImageWidth = DEFAULT_IMAGE_WIDTH;
	}
	try {
		yamlNode["imageHeight"] >> auxImageHeight;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView image height: ")
						+ yamlException.what());
		auxImageHeight = DEFAULT_IMAGE_HEIGHT;
	}
	try {
		yamlNode["numberOfClips"] >> auxNumberOfClips;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxNumberOfClips = DEFAULT_NUMBER_CLIPS;
	}
	try {
		yamlNode["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		yamlNode["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing PlayerView: ") + yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
	}
	try {
		yamlNode["baseRows"] >> auxBaseRows;

	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseRows = DEFAULT_BASE_ROWS;
	}
	try {
		yamlNode["baseCols"] >> auxBaseCols;

	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing PlayerView: ") + yamlException.what());
		auxBaseCols = DEFAULT_BASE_COLS;
	}

	mobileEntityView->setName(auxName);
	mobileEntityView->setAnchorPixel(auxAnchorPixel);
//	playerView->setBaseHeight(auxBaseCols);
//	playerView->setBaseWidth(auxBaseRows);
	mobileEntityView->setImageHeight(auxImageHeight);
	mobileEntityView->setImageWidth(auxImageWidth);
	mobileEntityView->setNClips(auxNumberOfClips);
	mobileEntityView->setFps(auxFps);
	mobileEntityView->setDelay(auxDelay);
	mobileEntityView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
	mobileEntityView->setEntity(auxPlayer);

	TextureDefinition* walkingTexture = new TextureDefinition(
			auxName + string(WALKING_MODIFIER), auxWalkingImageSrc);
	textureHolder->addTexture(walkingTexture);
	TextureDefinition* idleTexture = new TextureDefinition(
			auxName + string(IDLE_MODIFIER), auxIdleImageSrc);
	textureHolder->addTexture(idleTexture);
	TextureDefinition* runningTexture = new TextureDefinition(
			auxName + string(RUNNING_MODIFIER), auxRunningImageSrc);
	textureHolder->addTexture(runningTexture);
	TextureDefinition* attackTexture = new TextureDefinition(
			auxName + string(ATTACK_MODIFIER), auxAttackImageSrc);
	textureHolder->addTexture(attackTexture);
	TextureDefinition* idleBlockTexture = new TextureDefinition(
			auxName + string(IDLE_BLOCKING_MODIFIER), auxIdleBlocking);
	textureHolder->addTexture(idleBlockTexture);

	mobileEntityView->setTextureHolder(textureHolder);
}

void operator >>(const YAML::Node& yamlNode,
		std::vector<MobileEntity*>& entityVector) {
	const YAML::Node& mobileEntityLocations = yamlNode["mobileEntityLocations"];
	for (unsigned i = 0; i < mobileEntityLocations.size(); i++) {
	//	MobileEntity* entity = new MobileEntity();
		Mob* entity = new Mob();
		mobileEntityLocations[i] >> entity;
		if (entity != NULL)
			entityVector.push_back(entity);
	}
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
			auxAnimationNumberOfRepeats, auxBaseCols, auxBaseRows;

	try {
		yamlNode["name"] >> auxName;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
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
		Logs::logErrorMessage(
				string("Error parsing EntityView: ") + yamlException.what());
		auxImageSrc = DEFAULT_IMAGE_SRC;
		auxImageWidth = DEFAULT_IMAGE_WIDTH;
		auxImageHeight = DEFAULT_IMAGE_HEIGHT;
		auxNumberOfClips = DEFAULT_NUMBER_CLIPS;
	}
	try {
		yamlNode["anchorPixel"] >> auxAnchorPixel;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing EntityView: ") + yamlException.what());
	}
	try {
		yamlNode["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		yamlNode["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing EntityView: ") + yamlException.what());
		auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
	}
	try {
		yamlNode["baseRows"] >> auxBaseRows;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxBaseRows = DEFAULT_BASE_ROWS;
	}
	try {
		yamlNode["baseCols"] >> auxBaseCols;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
		auxBaseCols = DEFAULT_BASE_COLS;
	}

	map<string, SDL_Surface*> images;

	entityView->setAnchorPixel(auxAnchorPixel);
	entityView->setBaseSizes(auxBaseRows, auxBaseCols);
	entityView->setName(auxName);
	entityView->setImageHeight(auxImageHeight);
	entityView->setImageWidth(auxImageWidth);
	entityView->setNClips(auxNumberOfClips);
	entityView->setFps(auxFps);
	entityView->setDelay(auxDelay);
	entityView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
	entityView->setEntity(auxEntity);

	TextureDefinition* textureDef = new TextureDefinition(auxName, auxImageSrc);
	textureHolder->addTexture(textureDef);

	entityView->setTextureHolder(textureHolder);
}
/*
 void operator >>(const YAML::Node& yamlNode, EntityViewData* entityView) {

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
 Logs::logErrorMessage(
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
 Logs::logErrorMessage(
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
 //		Logs::logErrorMessage(
 //				string("Error parsing EntityView: ") + yamlException.what());
 }
 try {
 yamlNode["fps"] >> auxFps;
 } catch (YAML::Exception& yamlException) {
 //		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
 auxFps = DEFAULT_FPS;
 }
 try {
 yamlNode["delay"] >> auxDelay;
 } catch (YAML::Exception& yamlException) {
 //		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
 auxDelay = DEFAULT_DELAY;
 }
 try {
 yamlNode["animationRepeats"] >> auxAnimationNumberOfRepeats;
 } catch (YAML::Exception& yamlException) {
 //		Logs::logErrorMessage(
 //				string("Error parsing EntityView: ") + yamlException.what());
 auxAnimationNumberOfRepeats = DEFAULT_REPEATS;
 }
 try {
 yamlNode["baseWidth"] >> auxBaseWidth;
 } catch (YAML::Exception& yamlException) {
 //		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
 auxBaseWidth = DEFAULT_BASE_WIDTH;
 }
 try {
 yamlNode["baseHeight"] >> auxBaseLength;
 } catch (YAML::Exception& yamlException) {
 //		Logs::logErrorMessage(string("Error parsing EntityView: ")+yamlException.what());
 auxBaseLength = DEFAULT_BASE_LENGTH;
 }

 entityView->setAnchorPixel(auxAnchorPixel);
 //	entityView->setBaseWidth(auxBaseWidth);
 //	entityView->setBaseHeight(auxBaseLength);
 entityView->setEntityId(auxName);
 entityView->setImageHeight(auxImageHeight);
 entityView->setImageWidth(auxImageWidth);
 entityView->setNClips(auxNumberOfClips);
 //entityView->setEntity(auxEntity);
 entityView->setEntityImage(auxImageSrc);
 entityView->setFps(auxFps);
 entityView->setDelay(auxDelay);
 entityView->setNumberOfRepeats(auxAnimationNumberOfRepeats);
 }*/

void operator >>(const YAML::Node& yamlNode,
		std::vector<Entity*>& entityVector) {
	const YAML::Node& entityLocations = yamlNode["entityLocations"];
	for (unsigned i = 0; i < entityLocations.size(); i++) {
		Entity* entity = new Entity();
		entityLocations[i] >> entity;
		if (entity != NULL)
			entityVector.push_back(entity);
	}
}

void operator >>(const YAML::Node& yamlNode, std::vector<Item*>& itemVector) {
	const YAML::Node& itemLocations = yamlNode["itemLocations"];
	for (unsigned i = 0; i < itemLocations.size(); i++) {
		Item* item = new Item();
		itemLocations[i] >> item;
		if (item != NULL)
			itemVector.push_back(item);
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
			if (playerName.count(player->getName()) == 0) {
				throw PlayerLocationNameExcepton();
			}
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
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
			playerName[entityView->getName()] = 0;
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
					string("Error parsing PlayerView List: ")
							+ yamlException.what());
		}
	}

}

/**
 * Sobrecarga de operador >> para llenar los datos de una lista de entidades.
 */
void operator >>(const YAML::Node& yamlNode,
		std::vector<MobileEntityView*>& entityList) {
	const YAML::Node& mobileEntityViews = yamlNode["mobileEntityViews"];
	for (unsigned i = 0; i < mobileEntityViews.size(); i++) {
		MobileEntityView* mobileEntityView = new MobileEntityView();
		try {
			mobileEntityViews[i] >> mobileEntityView;
			entityList.push_back(mobileEntityView);
			playerName[mobileEntityView->getName()] = 0;
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
					string("Error parsing MobileEntityView List: ")
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
			Logs::logErrorMessage(
					string("Error parsing EntityView List: ")
							+ yamlException.what());
		}
	}
}/*
 void operator >>(const YAML::Node& yamlNode,
 EntityHolder* entityList) {
 const YAML::Node& entityViews = yamlNode["entityViews"];
 for (unsigned i = 0; i < entityViews.size(); i++) {
 EntityViewData* entityView = new EntityViewData();
 try {
 entityViews[i] >> entityView;
 entityList->addEntityViewData(entityView);
 } catch (YAML::Exception& yamlException) {
 Logs::logErrorMessage(
 string("Error parsing EntityView List: ")
 + yamlException.what());
 }
 }
 }
 */
/* ******************************************************* *
 * *********** ANIMATION CONFIGURATION PARSING *********** *
 * ******************************************************* */

void checkValues(int* value, string valueName, int default_value) {
	if (*value < 0) {
		*value = default_value;
		Logs::logErrorMessage(
				string("Error parsing ") + valueName
						+ string(", negative values are not allowed."));
	}
}

/**
 * Sobrecarga de operador >> para parsear un AnimationConfiguration.
 */
void operator >>(const YAML::Node& yamlNode,
		GameConfiguration* animationConfig) {
	const YAML::Node& configuration = yamlNode["gameConfiguration"];
	int auxFps, auxDelay, auxHeight, auxWidth, auxBPP, auxMovementMargin;
	int auxTileWidth, auxTileHeight, auxPort;
	bool auxFullscreen, auxAutoConfig;
	std::string auxGameMusicSrc, auxMenuImage, auxMenuMusic, auxIP, auxMissionType;
	try {
		configuration["serverIP"] >> auxIP;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing GameConfiguration Server IP Address: ")
						+ yamlException.what());
		auxIP = DEFAULT_IP;
	}
	try {
		configuration["serverPort"] >> auxPort;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing GameConfiguration Server Port: ")
						+ yamlException.what());
		auxPort = DEFAULT_PORT;
	}
	try {
		configuration["missionType"] >> auxMissionType;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing GameConfiguration Mission Type: ")
						+ yamlException.what());
		auxMissionType = DEFAULT_MISSION;
	}
	try {
		configuration["fps"] >> auxFps;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing GameConfiguration FPS: ")
						+ yamlException.what());
		auxFps = DEFAULT_FPS;
	}
	try {
		configuration["delay"] >> auxDelay;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxDelay = DEFAULT_DELAY;
	}
	try {
		configuration["movementMargin"] >> auxMovementMargin;
	} catch (YAML::Exception& yamlException) {
		// No logeo, porque si no esta se usa el standard.
		auxMovementMargin = DEFAULT_MOVEMENT_MARGIN;
	}
	try {
		configuration["gameMusic"] >> auxGameMusicSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing GameConfiguration game music: ")
						+ yamlException.what());
		auxGameMusicSrc = DEFAULT_GAME_MUSIC;
	}
	try {
		configuration["fullscreen"] >> auxFullscreen;
	} catch (YAML::Exception& yamlException) {
		auxFullscreen = false;
	}
	try {
		configuration["screenAutoConfig"] >> auxAutoConfig;
	} catch (YAML::Exception& yamlException) {
		auxAutoConfig = false;
	}
	try {
		configuration["defaultScreenHeight"] >> auxHeight;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxHeight = DEFAULT_SCREEN_HEIGHT;
	}
	try {
		configuration["defaultScreenWidth"] >> auxWidth;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxWidth = DEFAULT_SCREEN_WIDTH;
	}
	try {
		configuration["defaultBPP"] >> auxBPP;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxBPP = DEFAULT_BPP;
	}
	try {
		configuration["menuBackImage"] >> auxMenuImage;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxMenuImage = DEFAULT_MENU_IMAGE;
	}
	try {
		configuration["menuBackMusic"] >> auxMenuMusic;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxMenuMusic = DEFAULT_MENU_MUSIC;
	}
	try {
		configuration["tileWidth"] >> auxTileWidth;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxTileWidth = DEFAULT_TILE_WIDTH;
	}
	try {
		configuration["tileHeight"] >> auxTileHeight;
	} catch (YAML::Exception& yamlException) {
//		Logs::logErrorMessage(
//				string("Error parsing GameConfiguration: ")
//						+ yamlException.what());
		auxTileHeight = DEFAULT_TILE_HEIGHT;
	}

	checkValues(&auxFps, string("fps"), DEFAULT_FPS);
	checkValues(&auxDelay, string("delay"), DEFAULT_DELAY);
	checkValues(&auxMovementMargin, string("movementMargin"),
			DEFAULT_MOVEMENT_MARGIN);
	checkValues(&auxHeight, string("defaultScreenHeight"),
			DEFAULT_SCREEN_HEIGHT);
	checkValues(&auxWidth, string("defaultScreenWidth"), DEFAULT_SCREEN_WIDTH);
	checkValues(&auxBPP, string("defaultBPP"), DEFAULT_BPP);
	checkValues(&auxTileWidth, string("TileWidth"), DEFAULT_TILE_WIDTH);
	checkValues(&auxTileHeight, string("TileHeight"), DEFAULT_TILE_HEIGHT);

	animationConfig->setServerIP(auxIP);
	animationConfig->setServerPort((unsigned int) auxPort);
	animationConfig->setDelay((unsigned int) auxDelay);
	animationConfig->setFps((unsigned int) auxFps);
	animationConfig->setMovementMargin((unsigned int) auxMovementMargin);
	animationConfig->setGameMusicSrc(auxGameMusicSrc);
	animationConfig->setDefaultScreenHeight((unsigned int) auxHeight);
	animationConfig->setDefaultScreenWidth((unsigned int) auxWidth);
	animationConfig->setDefaultBPP((unsigned int) auxBPP);
	animationConfig->setMenuBackImageSrc(auxMenuImage);
	animationConfig->setMenuBackMusicSrc(auxMenuMusic);
	animationConfig->setTileWidth((unsigned int) auxTileWidth);
	animationConfig->setTileHeight((unsigned int) auxTileHeight);
	animationConfig->setFullscreen(auxFullscreen);
	animationConfig->setScreenAutoConfig(auxAutoConfig);
	animationConfig->setMission(auxMissionType);
}

/* *********************************************** *
 * *********** TILE DEFINITION PARSING *********** *
 * *********************************************** */

/**
 * Extraction operator for tile definition.
 */
void operator >>(const YAML::Node& yamlNode,
		TextureDefinition* tileDefinition) {

	std::string auxId, auxSrc;
	try {
		yamlNode["identifier"] >> auxId;
		yamlNode["imageSrc"] >> auxSrc;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing TileDefinition: ")
						+ yamlException.what());
	}
	tileDefinition->setTextureId(auxId);
	tileDefinition->setTextureImageSrc(auxSrc);
}

/**
 * Extraction of all configured entities.
 */
void operator >>(const YAML::Node& yamlNode, TextureHolder* textureHolder) {
	const YAML::Node& yamlTileDefinitions = yamlNode["tiles"];
	for (unsigned i = 0; i < yamlTileDefinitions.size(); i++) {
		TextureDefinition* tileDef = new TextureDefinition("", "");
		try {
			yamlTileDefinitions[i] >> tileDef;
			textureHolder->addTexture(tileDef);
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
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
		Logs::logErrorMessage(
				string("Error parsing Tile position: ") + yamlException.what());
	}
	try {
		yamlNode["texture"] >> auxId;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
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
		Tile* tile = new Tile();
		try {
			tileList[i] >> tile;
			stringstream sRow;
			stringstream sCol;
			sRow << tile->getPosition()->getX();
			sCol << tile->getPosition()->getY();
			if (tile->getPosition()->getX() >= rows
					|| tile->getPosition()->getX() < 0) {
				Logs::logErrorMessage(
						string(
								"Error parsing Tile >> row index out of bounds: ")
								+ string(" position: [") + sRow.str()
								+ string(", ") + sCol.str() + string("]"));
				delete tile;
				continue;
			}
			if (tile->getPosition()->getY() >= cols
					|| tile->getPosition()->getY() < 0) {
				Logs::logErrorMessage(
						string(
								"Error parsing Tile >> column index out of bounds: ")
								+ string(" position: [") + sRow.str()
								+ string(", ") + sCol.str() + string("]"));
				delete tile;
				continue;
			}
			destMap.tileList.push_back(tile);
		} catch (YAML::Exception& yamlException) {
			Logs::logErrorMessage(
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
		int auxRow = auxTile->getPosition()->getX();
		int auxCol = auxTile->getPosition()->getY();
		destMap->setTileType(auxTile->getTextureIdentifier(), auxRow, auxCol);
	}
}

/**
 * Extraction operator of a map.
 */
void operator >>(const YAML::Node& yamlNode, AuxMapDimension& dimension) {
	const YAML::Node& yamlDimensions = yamlNode["mapDimensions"];
	try {
		yamlDimensions[0] >> dimension.nrows;
		rows = dimension.nrows;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing map dimension rows: ")
						+ yamlException.what());
		dimension.nrows = DEFAULT_ROWS;
		rows = DEFAULT_ROWS;
	}
	try {
		yamlDimensions[1] >> dimension.ncols;
		cols = dimension.ncols;
	} catch (YAML::Exception& yamlException) {
		Logs::logErrorMessage(
				string("Error parsing map dimension columns: ")
						+ yamlException.what());
		dimension.ncols = DEFAULT_COLS;
		cols = dimension.ncols;
	}

	stringstream sRow;
	stringstream sCol;

	sRow << rows;
	sCol << cols;

	if (rows < 0) {
		Logs::logErrorMessage(
				string(
						"Error parsing Map dimensions >> row index out of bounds: ")
						+ string(" [") + sRow.str() + string(", ") + sCol.str()
						+ string("]"));
		rows = DEFAULT_ROWS;
		dimension.nrows = rows;
	}

	if (cols < 0) {
		Logs::logErrorMessage(
				string(
						"Error parsing Map dimensions >> column index out of bounds: ")
						+ string(" [") + sRow.str() + string(", ") + sCol.str()
						+ string("]"));
		cols = DEFAULT_COLS;
		dimension.ncols = cols;
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
				personaje->getCoordinates());
	}

}

void loadEntityViewMap(EntityViewMap* entityViewMap,
		std::vector<EntityView*> entityViewVector) {

	for (unsigned int j = 0; j < entityViewVector.size(); j++) {
		EntityView* parsedEntityView = entityViewVector[j];
		Entity* entity = parsedEntityView->getEntity();

		Coordinates coordinates = Coordinates(entity->getCoordinates().getRow(),
				entity->getCoordinates().getCol());

		entityViewMap->positionEntityView(parsedEntityView, coordinates);
	}

}

void loadEntityViewMap(EntityViewMap* entityViewMap,
		std::vector<MobileEntityView*> entityViewVector) {

	for (unsigned int j = 0; j < entityViewVector.size(); j++) {
		MobileEntityView* parsedEntityView = entityViewVector[j];
		MobileEntity* entity = parsedEntityView->getEntity();

		Coordinates coordinates = Coordinates(entity->getCoordinates().getRow(),
				entity->getCoordinates().getCol());

		entityViewMap->positionEntityView(parsedEntityView, coordinates);
	}

}

void assignEntities(MapData* mapData, std::vector<Entity*> entities) {
	for (unsigned i = 0; i < entities.size(); i++) {
		Entity* currentEntity = entities[i];
		Coordinates coor = currentEntity->getCoordinates();
		mapData->addEntity(coor.getRow(), coor.getCol(), currentEntity);
	}
}

void assignItems(MapData* mapData, std::vector<Item*> items) {
	for (unsigned i = 0; i < items.size(); i++) {
		Item* currentItem = items[i];
		Coordinates coor = currentItem->getCoordinates();
		mapData->addItem(coor.getRow(), coor.getCol(), currentItem);
	}
}

void assignMobileEntities(MapData* mapData,
		std::vector<MobileEntity*> mobileEntities) {
	for (unsigned i = 0; i < mobileEntities.size(); i++) {
		MobileEntity* currentMobileEntity = mobileEntities[i];
		Coordinates coor = currentMobileEntity->getCoordinates();
		mapData->addMobileEntity(coor.getRow(), coor.getCol(),
				currentMobileEntity);
	}
}

std::vector<EntityView*> assignEntities(std::vector<EntityView*> entityViews,
		std::vector<Entity*> entities) {
	std::vector<EntityView*> completeViews;
	for (unsigned i = 0; i < entities.size(); i++) {
		Entity* actualEntity = entities[i];
		std::string entityName = actualEntity->getName();
		EntityView* duplicate = NULL;
		for (unsigned j = 0; j < entityViews.size(); j++) {
			EntityView* actualView = entityViews[j];
			if ((actualView->getName()).compare(entityName) == 0) {
				duplicate = new EntityView(actualView);
				duplicate->setEntity(actualEntity);
				completeViews.push_back(duplicate);
			}
		}
		// Si no se le asigno ninguna entidad a la vista.
		if (duplicate == NULL) {
			Logs::logErrorMessage(
					string(
							"Could not find entity view with the name "
									+ entityName));
		}
	}
	return completeViews;
}

std::vector<EntityView*> assignEntities(std::vector<EntityView*> itemViews,
		std::vector<Item*> items) {
	std::vector<EntityView*> completeViews;
	for (unsigned i = 0; i < items.size(); i++) {
		Entity* actualEntity = items[i];
		std::string entityName = actualEntity->getName();
		EntityView* duplicate = NULL;
		for (unsigned j = 0; j < itemViews.size(); j++) {
			EntityView* actualView = itemViews[j];
			if ((actualView->getName()).compare(entityName) == 0) {
				duplicate = new EntityView(actualView);
				duplicate->setEntity(actualEntity);
				completeViews.push_back(duplicate);
			}
		}
		// Si no se le asigno ninguna entidad a la vista.
		if (duplicate == NULL) {
			Logs::logErrorMessage(
					string(
							"Could not find entity view with the name "
									+ entityName));
		}
	}
	return completeViews;
}

std::vector<MobileEntityView*> assignMobileEntities(
		std::vector<MobileEntityView*> entityViews,
		std::vector<MobileEntity*> entities) {
	std::vector<MobileEntityView*> completeViews;
	for (unsigned i = 0; i < entities.size(); i++) {
		MobileEntity* actualEntity = entities[i];
		std::string entityName = actualEntity->getName();
		MobileEntityView* duplicate = NULL;
		for (unsigned j = 0; j < entityViews.size(); j++) {
			MobileEntityView* actualView = entityViews[j];
			if ((actualView->getName()).compare(entityName) == 0) {
				duplicate = new MobileEntityView(actualView);
				duplicate->setEntity(actualEntity);
				completeViews.push_back(duplicate);
			}
		}
		// Si no se le asigno ninguna entidad a la vista.
		if (duplicate == NULL) {
			Logs::logErrorMessage(
					string(
							"Could not find mobile entity view with the name "
									+ entityName));
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
				PlayerView* duplicate = new PlayerView(actualView);
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

void cleanUnusedViews(std::vector<MobileEntityView*> viewVector) {
	std::vector<MobileEntityView*> cleanVector;
	for (unsigned i = 0; i < viewVector.size(); i++) {
		if (viewVector[i]->getEntity() == NULL) {
			delete viewVector[i];
		} else {
			cleanVector.push_back(viewVector[i]);
		}
	}
}

void setDefaultPlayerView() {

}

std::vector<Item*> parseItems(std::vector<Item*> itemVector) {

	std::vector<Item*> parsedItems;

	Item* actualItem;

	for (unsigned i = 0; i < itemVector.size(); i++) {

		actualItem = itemVector[i];

		if (actualItem->getName() == "lifeheart") {

			LifeHeart* lifeHeart = new LifeHeart();

			lifeHeart->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			lifeHeart->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(lifeHeart);

		} else if (actualItem->getName() == "magicbottle") {

			MagicBottle* magicbottle = new MagicBottle();

			magicbottle->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			magicbottle->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(magicbottle);

		} else if (actualItem->getName() == "speedboots") {

			SpeedBoots* speedBoots = new SpeedBoots();

			speedBoots->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			speedBoots->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(speedBoots);

		} else if (actualItem->getName() == "lantern") {

			Lantern* lantern = new Lantern();

			lantern->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			lantern->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(lantern);

		} else if (actualItem->getName() == "shield") {

			Shield* shield = new Shield();

			shield->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			shield->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(shield);

		} else if (actualItem->getName() == "earthquakeitem") {

			EarthquakeItem* earthquakeItem = new EarthquakeItem();

			earthquakeItem->setName(actualItem->getName());
			Coordinates coord = actualItem->getCoordinates();
			earthquakeItem->setCoordinates(coord.getRow(), coord.getCol());

			parsedItems.push_back(earthquakeItem);

		}

	}

	return parsedItems;

}

void setDefaultGameConfiguration(GameConfiguration* gameConf) {
	gameConf->setFps(30);
	gameConf->setDelay(10);
	gameConf->setMovementMargin(50);
	gameConf->setGameMusicSrc("resources/sound/stalemate.ogg");
	gameConf->setFullscreen(true);
	gameConf->setScreenAutoConfig(true);
	gameConf->setDefaultScreenHeight(690);
	gameConf->setDefaultScreenWidth(1280);
	gameConf->setDefaultBPP(0);
	gameConf->setMenuBackImageSrc("resources/wastelands.jpg");
	gameConf->setMenuBackMusicSrc("resources/sound/pirates.ogg");
	gameConf->setTileWidth(70);
	gameConf->setTileHeight(50);
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

	if (textureHolder != NULL) {
		delete textureHolder;
	}
	textureHolder = new TextureHolder();

	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);
	std::ofstream outputFile(outputFilename.c_str());

// Error Check
	if (!inputFile) {
		//cout << "No se encontro el archivo de configuracion" << std::endl;
		Logs::logErrorMessage(
				string("No se encontro el archivo de configuracion: "));
		exit(1);
	}

// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

// Parsing PlayerViews.
	std::vector<PlayerView*> playerViewVector;
	yamlNode[PLAYERVIEWS_POSITION] >> playerViewVector;

	std::vector<MobileEntityView*> mobileEntityViewVector;
	yamlNode[MOBILEENTITYVIEWS_POSITION] >> mobileEntityViewVector;

// Parsing EntityViews.
	std::vector<EntityView*> entityViewVector;
	yamlNode[ENTITYVIEWS_POSITION] >> entityViewVector;

// Parsing animation configuration.
	GameConfiguration* animationConfig = new GameConfiguration();
	yamlNode[GAME_CONFIGURATION_POSITION] >> animationConfig;

// Parsing tile definition.
// La declaracion del textureHolder la hice global
	yamlNode[TILE_DEFINITION_POSITION] >> textureHolder;

// Parsing map dimensions.
	AuxMap mapConfiguration;
	AuxMapDimension mapDimension;
	yamlNode[MAP_DIMENSION_POSITION] >> mapDimension;
	mapConfiguration.dimension = mapDimension;

	MapData* mapData = new MapData(mapConfiguration.dimension.nrows,
			mapConfiguration.dimension.ncols);

// Parsing player locations.
	std::vector<Player*> playerVector;
	yamlNode[PLAYER_LOCATIONS_POSITION] >> playerVector;

// Parsing mobile entity locations.
	std::vector<MobileEntity*> mobileEntityVector;
	yamlNode[MOBILEENTITY_LOCATIONS_POSITION] >> mobileEntityVector;

// Parsing mobile entity locations.
	std::vector<Item*> itemVector;
	yamlNode[ITEM_LOCATIONS_POSITION] >> itemVector;

	std::vector<Item*> parsedItems = parseItems(itemVector);

// Parsing player locations.
	std::vector<Entity*> entityVector;
	yamlNode[ENTITY_LOCATIONS_POSITION] >> entityVector;

	std::vector<EntityView*> cleanEntityViews = assignEntities(entityViewVector,
			entityVector);
	std::vector<EntityView*> cleanItemViews = assignEntities(entityViewVector,
			parsedItems);
	std::vector<PlayerView*> cleanPlayerViews = assignPlayers(playerViewVector,
			playerVector);
	std::vector<MobileEntityView*> cleanMobileEntityViews =
			assignMobileEntities(mobileEntityViewVector, mobileEntityVector);

	assignEntities(mapData, entityVector);
	assignItems(mapData, parsedItems);
	assignMobileEntities(mapData, mobileEntityVector);
	cleanUnusedViews(entityViewVector);
	cleanUnusedViews(playerViewVector);
	cleanUnusedViews(mobileEntityViewVector);

// Parsing map tile locations.
	yamlNode[MAP_TILES_POSITION] >> mapConfiguration;
	mapConfiguration >> mapData;

// Create entityViewMap:
	EntityViewMap* entityViewMap = new EntityViewMap(mapData);
	loadEntityViewMap(entityViewMap, cleanPlayerViews);
	loadEntityViewMap(entityViewMap, cleanEntityViews);
	loadEntityViewMap(entityViewMap, cleanItemViews);
	//loadEntityViewMap(entityViewMap, cleanMobileEntityViews);

// Packing parser results.
	PersistentConfiguration configuration = PersistentConfiguration();
	configuration.setEntityViewMap(entityViewMap);
	configuration.setTextureHolder(textureHolder);
	configuration.setMapData(mapData);
	configuration.setAnimationConfiguration(animationConfig);
	configuration.setViewList(cleanPlayerViews);
	configuration.setMobileEntityViewList(cleanMobileEntityViews);
	configuration.setMobileEntities(mobileEntityVector);
	configuration.setItemViews(cleanItemViews);

	return configuration;
}

ServerMapPersistentConfiguration ConfigurationReader::loadServerMapConfiguration(
		std::string configurationFile) {

	int entityViewsPosition = 0;
	int gameConfigurationPosition = 1;
	int tileDefinitionPosition = 2;
	int mapDimensionPosition = 3;
	int entityLocationPosition = 4;
	int tileLocationPosition = 5;

	if (textureHolder != NULL) {
		delete textureHolder;
	}
	textureHolder = new TextureHolder();

	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);
	std::ofstream outputFile(
			"./configuration/serverMapConfigurationOutput.yaml");

// Error Check
	if (!inputFile) {
		//cout << "No se encontro el archivo de configuracion" << std::endl;
		Logs::logErrorMessage(
				string("No se encontro el archivo de configuracion: "));
		exit(1);
	}

// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

// Parsing EntityViews.
	std::vector<EntityView*> entityViewVector;
	yamlNode[entityViewsPosition] >> entityViewVector;

// Parsing animation configuration.
	GameConfiguration* animationConfig = new GameConfiguration();
	yamlNode[gameConfigurationPosition] >> animationConfig;

// Parsing tile definition.
	yamlNode[tileDefinitionPosition] >> textureHolder;

// Parsing map dimensions.
	AuxMap mapConfiguration;
	AuxMapDimension mapDimension;
	yamlNode[mapDimensionPosition] >> mapDimension;
	mapConfiguration.dimension = mapDimension;

	MapData* mapData = new MapData(mapConfiguration.dimension.nrows,
			mapConfiguration.dimension.ncols);

// Parsing player locations.
	std::vector<Entity*> entityVector;
	yamlNode[entityLocationPosition] >> entityVector;

// Assigning Entities to EntityViews.
	std::vector<EntityView*> cleanEntityViews = assignEntities(entityViewVector,
			entityVector);
	assignEntities(mapData, entityVector);
	cleanUnusedViews(entityViewVector);

// Parsing map tile locations.
	yamlNode[tileLocationPosition] >> mapConfiguration;
	mapConfiguration >> mapData;

// Create entityViewMap:
	EntityViewMap* entityViewMap = new EntityViewMap(mapData);
	loadEntityViewMap(entityViewMap, cleanEntityViews);

	ServerMapPersistentConfiguration configuration =
			ServerMapPersistentConfiguration();
	configuration.setEntityViewMap(entityViewMap);
	configuration.setTextureHolder(textureHolder);
	configuration.setMapData(mapData);
	configuration.setAnimationConfiguration(animationConfig);

	return configuration;
}

ClientPlayerPersistentConfiguration ConfigurationReader::loadClientPlayerConfiguration(
		std::string configurationFile) {

	int playerViewsPosition = 0;
	int gameConfigurationPosition = 1;
	int playerLocationPosition = 2;

	if (textureHolder != NULL) {
		delete textureHolder;
	}
	textureHolder = new TextureHolder();

	std::ifstream inputFile(configurationFile.c_str(), std::ifstream::in);

// Error Check
	if (!inputFile) {
		//cout << "No se encontro el archivo de configuracion" << std::endl;
		Logs::logErrorMessage(
				string("No se encontro el archivo de configuracion: "));
		exit(1);
	}

// Parser initialization.
	YAML::Parser parser(inputFile);
	YAML::Node yamlNode;
	parser.GetNextDocument(yamlNode);

// Parsing PlayerViews.
	std::vector<PlayerView*> playerViewVector;
	yamlNode[playerViewsPosition] >> playerViewVector;

// Parsing animation configuration.
	GameConfiguration* animationConfig = new GameConfiguration();
	yamlNode[gameConfigurationPosition] >> animationConfig;

// Parsing player locations.
	std::vector<Player*> playerVector;
	yamlNode[playerLocationPosition] >> playerVector;

// Clean not assigned player views.
	std::vector<PlayerView*> cleanPlayerViews = assignPlayers(playerViewVector,
			playerVector);
	cleanUnusedViews(playerViewVector);

	ClientPlayerPersistentConfiguration configuration =
			ClientPlayerPersistentConfiguration();
	configuration.setGameConfiguration(animationConfig);
	configuration.setPlayerViewList(cleanPlayerViews);

	return configuration;
}

ConfigurationReader::ConfigurationReader() {
}

ConfigurationReader::ConfigurationReader(const ConfigurationReader& orig) {
}

ConfigurationReader::~ConfigurationReader() {
}
