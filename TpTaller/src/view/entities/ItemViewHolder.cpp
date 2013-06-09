/*
 * ItemViewHolder.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: gonchub
 */

#include <view/entities/ItemViewHolder.h>

ItemViewHolder::ItemViewHolder() {
}

EntityView* ItemViewHolder::getEntityView(string className) {
	if (entityViewMap.find(className) == entityViewMap.end()) {
		return NULL;
	} else {
		EntityView* duplicate = duplicateView(entityViewMap[className]);
		return duplicate;
	}
}

void ItemViewHolder::loadEntityView(string className, EntityView* view) {
	if (entityViewMap.find(className) == entityViewMap.end()) {
		entityViewMap[className] = view;
	} else {
		// Si ya existia, borro la vista anterior y le cargo la nueva.
		EntityView* existingView = entityViewMap[className];
		delete existingView;
		entityViewMap[className] = view;
	}
}

EntityView* ItemViewHolder::duplicateView(EntityView* view) {
	EntityView* duplicateView = new EntityView(view);
	return duplicateView;
}

ItemViewHolder::~ItemViewHolder() {
	// Itero el map y borro las entity views.
	EntityView* actualView;

	map<string, EntityView*>::iterator iter;
	for (iter = entityViewMap.begin();
			iter != entityViewMap.end(); iter++) {
		actualView = iter->second;
		delete actualView;
	}
}

