/*
 * GlobalChanges.cpp
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#include <networking/GlobalChanges.h>

namespace std {

GlobalChanges::GlobalChanges() {
	// TODO Auto-generated constructor stub

}


void GlobalChanges::addChanges(int currentClient, PlayerInfo* info){

	changes.insert( pair<int,PlayerInfo*>(currentClient,info) );

}

vector<PlayerInfo*> GlobalChanges::getOthersChanges(int currentClient){

	vector<PlayerInfo*> vector;
	for (map<int,PlayerInfo*>::iterator it=changes.begin(); it!=changes.end(); ++it){
		if (currentClient != it->first){
			vector.push_back(it->second);
		}
	}

	return vector;


}

//Operator to transform the object into a stream.
ostream& operator <<(std::ostream& out, GlobalChanges& c){

	for (map<int,PlayerInfo*>::iterator it=c.changes.begin(); it!=c.changes.end(); ++it){
		out << it->first << " " << *(it->second) << " ";
	}

	return out;

}

//Operator to load an object from a stream
istream& operator >>(std::istream& in, GlobalChanges& c){

	while (!in.eof()){
		int current;
		PlayerInfo* pl = new PlayerInfo();
		in >> current;
		in >> *pl;
		c.addChanges(current,pl);
	}

	return in;

}


GlobalChanges::~GlobalChanges() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
