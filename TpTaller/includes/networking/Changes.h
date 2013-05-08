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
#include <iostream>

#include <networking/PlayerEvent.h>

namespace std {

class Changes {
public:
	Changes();

	void addChanges(string,vector<PlayerEvent*>);
	vector<PlayerEvent*> getPlayerEvents(string player);
	void resetChanges();

	map<string, vector<PlayerEvent*> > getChanges();

	virtual ~Changes();
private:
	map<string, vector<PlayerEvent*> > changes;
};

} /* namespace std */
#endif /* GLOBALCHANGES_H_ */
