/*
 * EntityViewMap.h
 *
 *  Created on: 05/04/2013
 *      Author: tomas
 */

#ifndef ENTITYVIEWMAP_H_
#define ENTITYVIEWMAP_H_

#include <view/entities/EntityView.h>
#include <model/entityProperties/Coordinates.h>

#include <vector>
#include <list>

namespace std {

class EntityViewMap;

/* Iterador de una coordenada del mapa. Se itera sobre la lista de entities en una posicion dada.
 * Si se tiene activo un iterador en la posicion x,y y se agrega o se quita una entity de esa posicion, el iterador
 * queda en un estado invalido, por lo que hay que reinicializarlo para esa posicion.
 */

class ViewMapIterator {
public:
	ViewMapIterator(const EntityViewMap* viewMap,Coordinates coordinates);
	bool isAtEnd();
	EntityView* getCurrent();
	void next();
	virtual ~ViewMapIterator();
private:
	list<EntityView*> entityList;
	list<EntityView*>::iterator it;
};

class EntityViewMap {
public:
	EntityViewMap(int rows, int cols);
	int getNCols();
	int getNRows();
	void positionEntityView (EntityView* entity, Coordinates coordinates);

	/*
	 * Inicializa el iterador en la posicion 0,0 del ViewMap
	 */
	void initIterator();

	/*
	 * Devuelve la proxima entidad que encuentre en el mapa recorriendo por filas. En caso de llegar al final y
	 * no encontrar otra entidad, devuelve NULL
	 */
	EntityView* getNextEntityView();

	/*
	 * Devuelve true si el itereador ha llegado al final del mapa y false en otro caso.
	 */
	bool iteratorAtEnd();


	list<EntityView*> getListAtRowAndCol(int row, int col);

	virtual ~EntityViewMap();
private:
	int rows, cols;
	int currentRow, currentCol;
	ViewMapIterator* it;
	vector< vector< list<EntityView*> > > entities;

	friend class ViewMapIterator;

	ViewMapIterator* getViewMapIterator(Coordinates coordinates);
};

} /* namespace std */
#endif /* ENTITYVIEWMAP_H_ */
