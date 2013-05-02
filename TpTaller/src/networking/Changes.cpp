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

map<string, list<PlayerEvent*> > Changes::getOthersChanges(string playerName){

	map<string, list<PlayerEvent*> > others;

	for (map<string, list<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){

		if (it->first != playerName && !it->second.empty())
			others.insert(pair<string, list<PlayerEvent*> >(it->first,it->second));
	}

	return others;

}

list<PlayerEvent*> Changes::getPlayerEvents(string player){

	return changes[player];

}


Changes::~Changes() {

	for (map<string, list<PlayerEvent*> >::iterator it = changes.begin() ; it != changes.end() ; ++it){
		for (list<PlayerEvent*>::iterator ti = it->second.begin() ; ti != it->second.end() ; ++ti)
			delete *ti;
	}

}


} /* namespace std */
