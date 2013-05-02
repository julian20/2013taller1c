/*
 * GlobalChanges.h
 *
 *  Created on: 24/04/2013
 *      Author: tomas
 */

#ifndef GLOBALCHANGES_H_
#define GLOBALCHANGES_H_

#include <map>
#include <list>
#include <iostream>

#include <networking/PlayerEvent.h>

namespace std {

class Changes {
public:
	Changes();

	void addChanges(string,list<PlayerEvent*>);
	list<PlayerEvent*> getPlayerEvents(string player);

	map<string, list<PlayerEvent*> > getOthersChanges(string playerName);

	virtual ~Changes();
private:
	map<string, list<PlayerEvent*> > changes;
};

} /* namespace std */
#endif /* GLOBALCHANGES_H_ */
