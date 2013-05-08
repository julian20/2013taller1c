/*
 * GlobalChanges.cpp
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#include <networking/Changes.h>
#include <iostream>

namespace std {

Changes::Changes() {
	// TODO Auto-generated constructor stub

}

void Changes::addChanges(string name ,vector<PlayerEvent*> events){
	changes.insert(pair<string , vector<PlayerEvent*> >(name,events));
}

map<string, vector<PlayerEvent*> > Changes::getChanges(){

	return changes;

}

void Changes::resetChanges(){

	for (map<string, vector<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		for (int i = 0 ; i < it->second.size() ; i++ ){
			delete it->second[i];
		}
		it->second.clear();
	}

	changes.clear();
}

vector<PlayerEvent*> Changes::getPlayerEvents(string player){

	if (changes.count(player) <= 0){
		vector<PlayerEvent*> empty;
		return empty;
	}

	return changes[player];

}


Changes::~Changes() {

	for (map<string, vector<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		for (int i = 0 ; i < it->second.size() ; i++ ){
			delete it->second[i];
		}
		it->second.clear();
	}

	changes.clear();

}


} /* namespace std */
