/*
 * PlayerInfo.h
 *
 *  Created on: 23/04/2013
 *      Author: tomas
 */

#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_

#include <string>
#include <networking/PlayerUpdate.h>
#include <model/Vector2.h>
#include <model/Vector3.h>
#include <model/entities/player/Player.h>
#include <view/entities/PlayerView.h>

using namespace std;

class PlayerInfo {
public:
	PlayerInfo();

	void setName(string name);
	string getName();

	void setOriginalName(string name);
	string getOriginalName();

	void setImages(map<string,string> images);
	map<string,string> getImages();

	void setImageDimentions(int width, int height);
	int getImageWidth();
	int getImageHeight();

	void setAnchorPixel(float x, float y);
	void setAnchorPixel(Vector2* anchorPixel);
	Vector2* getAnchorPixel();

	void setInitCoordinates(Coordinates* coords);
	Coordinates* getInitCoordinates();

	void setPlayer(Player* player);
	Player* getPlayer();

	void setDelay(float delay);
	float getDelay();

	void setFPS(int fps);
	int getFPS();

	PlayerView* createPlayerView();
	void updatePlayer(PlayerUpdate* update);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const PlayerInfo&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, PlayerInfo&);

	virtual ~PlayerInfo();
private:
	string name;
	string originalName;

	map<string,string> images;

	int imageWidth, imageHeight;
	Vector2* anchorPixel;
	int fps;
	float delay;
	Coordinates* initCoords;

	Player* player;
};

#endif /* PLAYERINFO_H_ */
