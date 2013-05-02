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
	void resetChanges();

	map<string, list<PlayerEvent*> > getChanges();

	virtual ~Changes();
private:
	map<string, list<PlayerEvent*> > changes;
};

} /* namespace std */
#endif /* GLOBALCHANGES_H_ */
