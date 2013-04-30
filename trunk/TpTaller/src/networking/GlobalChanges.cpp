/*
 * GlobalChanges.cpp
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#include <networking/GlobalChanges.h>
#include <iostream>

namespace std {

GlobalChanges::GlobalChanges() {
	// TODO Auto-generated constructor stub

}


void GlobalChanges::addChanges(PlayerInfo* info){


	if (changes.count(info->getPlayer()->getName()) == 0){
		changes.insert( pair<string,PlayerInfo*>(info->getPlayer()->getName(),info) );
	} else {
		delete (changes[info->getPlayer()->getName()]);
		changes[info->getPlayer()->getName()] = info;
	}

}

vector<PlayerInfo*> GlobalChanges::getOthersChanges(string clientName){

	vector<PlayerInfo*> vector;
	for (map<string,PlayerInfo*>::iterator it=changes.begin(); it!=changes.end(); ++it){
		if (clientName != it->first){
			vector.push_back(it->second);
		}
	}

	return vector;


}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, GlobalChanges& c){

	int size = c.changes.size();

	out << size << " ";

	for (map<string,PlayerInfo*>::iterator it=c.changes.begin(); it!=c.changes.end(); ++it){
		out << *(it->second) << " ";
	}


	return out;

}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, GlobalChanges& c){

	int size;
	in >> size;

	for (int i = 0 ; i < size ; i++){
		PlayerInfo* pl = new PlayerInfo();
		in >> *pl;
		c.addChanges(pl);
	}

	return in;

}


GlobalChanges::~GlobalChanges() {
	for (map<string,PlayerInfo*>::iterator it=changes.begin(); it!=changes.end(); ++it){
			delete (it->second);
	}

}

} /* namespace std */
