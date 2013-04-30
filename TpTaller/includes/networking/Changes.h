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

	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, Changes&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Changes&);

	virtual ~Changes();
private:
	map<string, PlayerEvent*> changes;
};

} /* namespace std */
#endif /* GLOBALCHANGES_H_ */
