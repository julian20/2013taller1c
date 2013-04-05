/*
 * ViewFactory.cpp
 *
 *  Created on: Apr 4, 2013
 *      Author: lucas
 */

#include <view/ViewFactory.h>
#include <view/entities/PersonajeVista.h>

namespace std {

template<typename T>
EntityView * createInstance(Entity entity) {
	return new T(&entity);
}

ViewFactory::ViewFactory() {
	//Acá agregamos todas las clases y sus vistas
	ViewNameMap["Entity"]=&createInstance<EntityView>;

}


EntityView* ViewFactory::getViewForEntity(Entity entity){
	string className = entity.getClassName();
	return ViewNameMap[className](entity);

}






ViewFactory::~ViewFactory() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
