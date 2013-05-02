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

void Changes::addChanges(string name ,list<PlayerEvent*> events){
	changes.insert(pair<string , list<PlayerEvent*> >(name,events));
}

map<string, list<PlayerEvent*> > Changes::getChanges(){

	return changes;

}

void Changes::resetChanges(){
	for (map<string, list<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		while(!it->second.empty()){
			delete it->second.front();
			it->second.pop_front();
		}
	}
}

list<PlayerEvent*> Changes::getPlayerEvents(string player){

	if (changes.count(player) <= 0){
		list<PlayerEvent*> empty;
		return empty;
	}

	return changes[player];

}


Changes::~Changes() {

	for (map<string, list<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		for (list<PlayerEvent*>::iterator ti = it->second.begin() ; ti != it->second.end() ; ++ti)
			delete *ti;
	}

}


} /* namespace std */
