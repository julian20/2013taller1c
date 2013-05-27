/*
 * SoundEffectHandler.h
 *
 *  Created on: May 27, 2013
 *      Author: mint
 */

#ifndef SOUNDEFFECTHANDLER_H_
#define SOUNDEFFECTHANDLER_H_
#include <SDL/SDL_mixer.h>
#include <map>

#include <string>

using namespace std;

class soundPack{
public:
	Mix_Chunk* sound;
	int channel;
};

class SoundEffectHandler {
public:
	SoundEffectHandler();
	static void loadSound(string soundID, string soundSrc);
	static void playSound(string soundID);
	static void stopSound(string soundID);
	static bool isSoundPlaying(string soundID);

	static void initialize();
	virtual ~SoundEffectHandler();

private:
	static map<string,soundPack> sounds;
	static int nextAvailableChannel;
	static int getNextChannel();
};




#endif /* SOUNDEFFECTHANDLER_H_ */
