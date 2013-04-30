/*
 * GlobalChanges.h
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#ifndef GLOBALCHANGES_H_
#define GLOBALCHANGES_H_

#include <map>
#include <vector>

#include <networking/PlayerInfo.h>

namespace std {

class GlobalChanges {
public:
	GlobalChanges();
	void addChanges(PlayerInfo* info);
	vector<PlayerInfo*> getOthersChanges(string clientName);

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, GlobalChanges&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, GlobalChanges&);

	virtual ~GlobalChanges();
private:
	map<string, PlayerInfo*> changes;
};

} /* namespace std */
#endif /* GLOBALCHANGES_H_ */
