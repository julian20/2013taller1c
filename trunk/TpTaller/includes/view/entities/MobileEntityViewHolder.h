/*
 * MobileEntityViewHolder.h
 *
 *  Created on: 10/06/2013
 *      Author: tomas
 */

#ifndef MOBILEENTITYVIEWHOLDER_H_
#define MOBILEENTITYVIEWHOLDER_H_

#include <string>
#include <map>

#include <view/entities/MobileEntityView.h>

namespace std {

class MobileEntityViewHolder {
public:
	MobileEntityViewHolder();
	virtual ~MobileEntityViewHolder();

	MobileEntityView* getMobileEntityView(string name);
	MobileEntityView* getMobileEntityViewByEntity(MobileEntity* entity);
	void loadMobileEntityView(string className, MobileEntityView* view);
private:
	MobileEntityView* duplicateView(MobileEntityView* view);
	map<string, MobileEntityView*> entityViewMap;
};

} /* namespace std */
#endif /* MOBILEENTITYVIEWHOLDER_H_ */
