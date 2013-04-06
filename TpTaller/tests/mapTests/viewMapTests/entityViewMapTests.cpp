#include <view/EntityViewMap.h>
#include <model/entities/Entity.h>
#include <model/entityProperties/Coordinates.h>
#include <view/entities/EntityView.h>
#include <iostream>

using namespace std;

void print_test(std::string test_msj, bool result){
	std::string result_txt;
	if (result) result_txt = "Ok";
	else result_txt = "ERROR";
	cout << test_msj << ": " << result_txt << endl;
}


// Change to main and run the makefile in the terminal.
int main(){
	EntityViewMap map = EntityViewMap(7,5);
	print_test("Cantidad de filas es 7", map.getNRows() == 7);
	print_test("Cantidad de columnas es 5", map.getNCols() == 5);

	Entity* entity = new Entity();
	EntityView* view = new EntityView(entity);
	map.positionEntityView(view, Coordinates(6,2));

	ViewMapIterator* it = map.getViewMapIterator(Coordinates(6,2));
	print_test("Elemento insertado es correcto", it->getActual() == view);
	print_test("Iterador no esta al final", !(it->isAtEnd()));
	it->next();
	print_test("Iterador esta al final", it->isAtEnd());

	EntityView* view2 = new EntityView(entity);
	map.positionEntityView(view2, Coordinates(6,2));
	delete it;

	it = map.getViewMapIterator(Coordinates(6,2));
	//print_test("Iterador no esta al final", !(it.isAtEnd()));
	print_test("Elemento 1 insertado no es el 2", it->getActual() != view2);
	it->next();
	print_test("Elemento 2 insertado es correcto", it->getActual() == view2);
	print_test("Iterador no esta al final", !(it->isAtEnd()));
	it->next();
	print_test("Iterador esta al final", it->isAtEnd());
	delete it;

	it = map.getViewMapIterator(Coordinates(5,3));
	print_test("Iterador esta al final en 5,3", it->isAtEnd());
	EntityView* view3 = new EntityView(entity);
	map.positionEntityView(view3, Coordinates(5,3));
	print_test("Iterador esta al final en 5,3 despues de haber insertado uno sin actualizar el iterador", it->isAtEnd());
	delete it;
	it = map.getViewMapIterator(Coordinates(5,3));
	print_test("Iterador no esta al final en 5,3 despues de actualizar el iterador", !(it->isAtEnd()));
	print_test("Elemento insertado en 5,3 es correcto", it->getActual() == view3);
	it->next();
	print_test("Iterador esta al final despues de ir al siguiente en 5,3", it->isAtEnd());

	delete it;
	delete entity;
	delete view;
	delete view2;
	delete view3;

	return 0;
}
