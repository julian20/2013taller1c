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

namespace std {

class PlayerInfo {
public:
	PlayerInfo();

	void setName(string name);
	string getName();

	void setWalkingImageSrc(string img);
	string getWalkingImageSrc();

	void setRunningImageSrc(string img);
	string getRunningImageSrc();

	void setIdleImageSrc(string img);
	string getIdleImageSrc();

	void setAttackImageSrc(string img);
	string getAttackImageSrc();

	void setIdleBlockingImageSrc(string img);
	string getIdleBlockingImageSrc();

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

	void setDelay(int delay);
	int getDelay();

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
	string walkingImageSrc,runningImageSrc,idleImageSrc,attackImageSrc, idleBlockingImageScr;
	int imageWidth, imageHeight;
	Vector2* anchorPixel;
	int fps,delay;
	Coordinates* initCoords;

	Player* player;

};

} /* namespace std */
#endif /* PLAYERINFO_H_ */
