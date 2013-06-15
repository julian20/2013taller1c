/*
 * MobileEntityViewHolder.cpp
 *
 *  Created on: 10/06/2013
 *      Author: tomas
 */

#include <view/entities/MobileEntityViewHolder.h>

namespace std {

MobileEntityViewHolder::MobileEntityViewHolder(){

}

MobileEntityView* MobileEntityViewHolder::getMobileEntityViewByEntity(MobileEntity* entity) {

	string className = (*entity).getName();

	MobileEntityView* newView = getMobileEntityView(className);

	if (newView != NULL)
		newView->setEntity(entity);

	return newView;

}

MobileEntityView* MobileEntityViewHolder::getMobileEntityView(string name) {
	if (entityViewMap.find(name) == entityViewMap.end()) {
		return NULL;
	} else {
		MobileEntityView* duplicate = duplicateView(entityViewMap[name]);
		return duplicate;
	}
}

void MobileEntityViewHolder::loadMobileEntityView(string className, MobileEntityView* view) {
	if (entityViewMap.find(className) == entityViewMap.end()) {
		entityViewMap[className] = view;
	} else {
		// Si ya existia, borro la vista anterior y le cargo la nueva.
		EntityView* existingView = entityViewMap[className];
		delete existingView;
		entityViewMap[className] = view;
	}
}


MobileEntityView* MobileEntityViewHolder::duplicateView(MobileEntityView* view) {
	MobileEntityView* duplicateView = new MobileEntityView(view);
	return duplicateView;
}

MobileEntityViewHolder::~MobileEntityViewHolder() {
	// Itero el map y borro las entity views.
	MobileEntityView* actualView;

	map<string, MobileEntityView*>::iterator iter;
	for (iter = entityViewMap.begin(); iter != entityViewMap.end(); iter++) {
		actualView = iter->second;
		delete actualView;
	}
}

} /* namespace std */
