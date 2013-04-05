/*
 * ViewFactory.h
 *
 *  Created on: Apr 4, 2013
 *      Author: lucas
 */

#ifndef VIEWFACTORY_H_
#define VIEWFACTORY_H_

#include <model/entities/Entity.h>
#include <view/entities/EntityView.h>
#include <string>
#include <map>


namespace std {

class ViewFactory {
public:
	ViewFactory();
	virtual ~ViewFactory();
	EntityView* getViewForEntity(Entity entity);
private:
	//Map key: entity class name; Map value: the constructor to the entity's view
	map<string,EntityView*(*)(Entity entity)> ViewNameMap;

};

} /* namespace std */
#endif /* VIEWFACTORY_H_ */
