/*
 * ItemViewHolder.h
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#ifndef ITEMVIEWHOLDER_H_
#define ITEMVIEWHOLDER_H_

#include <string>
#include <map>

#include <view/entities/EntityView.h>

using namespace std;

class ItemViewHolder {
public:
	ItemViewHolder();
	virtual ~ItemViewHolder();

	EntityView* getEntityView(string className);
	EntityView* getEntityViewByItem(Entity* item);
	void loadEntityView(string className, EntityView* view);
private:
	EntityView* duplicateView(EntityView* view);
	map<string, EntityView*> entityViewMap;
};

#endif /* ITEMVIEWHOLDER_H_ */
