/*
 * SoundEffectHandler.cpp
 *
 *  Created on: May 27, 2013
 *      Author: mint
 */

#include <view/sound/SoundEffectHandler.h>
#include <SDL/SDL_mixer.h>
#include <model/Logs/Logs.h>
#define DEFAULT_NUMBER_OF_CHANNELS 20
int SoundEffectHandler::nextAvailableChannel = 0;
map<string,soundPack> SoundEffectHandler::sounds;
SoundEffectHandler::SoundEffectHandler() {
	// TODO Auto-generated constructor stub

}

void SoundEffectHandler::loadSound(string soundID, string soundSrc) {
	// Cargamos un sonido
	Mix_Chunk* sound = Mix_LoadWAV(soundSrc.c_str());
	if (sound == NULL) {
		Logs::logErrorMessage(
				"No se puede cargar el sonido: " + string(SDL_GetError()));
		return;
	}

	int volume = 1000;
	Mix_VolumeChunk(sound, volume);
	soundPack pack;
	pack.sound = sound;
	pack.channel = SoundEffectHandler::getNextChannel();
	sounds[soundID.c_str()] = pack;

}

int SoundEffectHandler::getNextChannel() {
	nextAvailableChannel++;
	return nextAvailableChannel - 1;

}

void SoundEffectHandler::initialize() {

	Mix_AllocateChannels(DEFAULT_NUMBER_OF_CHANNELS);
	nextAvailableChannel = 0;

}

bool SoundEffectHandler::isSoundPlaying(string soundID) {
	bool exists = ( sounds.find(soundID.c_str()) != sounds.end() );
	if (!exists)
		return false;
	soundPack pack = sounds[soundID];
	bool val = (Mix_Playing(pack.channel)==1);
	return val;

}

void SoundEffectHandler::playSound(string soundID) {
	bool exists = ( sounds.find(soundID.c_str()) != sounds.end() );
	if (!exists)
		return;
	soundPack pack = sounds[soundID];
	Mix_PlayChannel(pack.channel, pack.sound, 0);
}

void SoundEffectHandler::stopSound(string soundID) {
	bool exists = ( sounds.find(soundID.c_str()) != sounds.end() );
	if (!exists)
		return;
	soundPack pack = sounds[soundID];
	Mix_HaltChannel(pack.channel);
}

void SoundEffectHandler::close(){
	for (map<string,soundPack>::iterator it = sounds.begin();it!=sounds.end();++it){
		soundPack pack = it->second;
		Mix_FreeChunk(pack.sound);
	}

}

SoundEffectHandler::~SoundEffectHandler() {
}

