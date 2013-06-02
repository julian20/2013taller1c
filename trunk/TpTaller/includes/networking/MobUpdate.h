/*
 * MobUpdate.h
 *
 *  Created on: 02/06/2013
 *      Author: tomas
 */

#ifndef MOBUPDATE_H_
#define MOBUPDATE_H_

#include <networking/MobileEntityUpdate.h>

namespace std {

class MobUpdate : public MobileEntityUpdate {
public:
	MobUpdate();
	virtual ~MobUpdate();
};

} /* namespace std */
#endif /* MOBUPDATE_H_ */
