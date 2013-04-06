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
	map.getNextEntityView();
	print_test("Iterador esta al final", map.iteratorAtEnd());

	Entity* entity = new Entity();
	EntityView* view = new EntityView(entity);
	map.positionEntityView(view, Coordinates(6,2));

	map.initIterator();
	print_test("Elemento insertado es correcto", map.getNextEntityView() == view);
	print_test("Iterador no esta al final", !map.iteratorAtEnd());
	map.getNextEntityView();
	print_test("Iterador esta al final", map.iteratorAtEnd());

	EntityView* view2 = new EntityView(entity);
	map.positionEntityView(view2, Coordinates(6,2));
	map.initIterator();

	print_test("Elemento insertado es correcto", map.getNextEntityView() == view);
	print_test("Elemento 2 insertado es correcto", map.getNextEntityView() == view2);
	print_test("Iterador no esta al final", !map.iteratorAtEnd());
	map.getNextEntityView();
	print_test("Iterador esta al final", map.iteratorAtEnd());



	EntityView* view3 = new EntityView(entity);
	map.positionEntityView(view3, Coordinates(5,3));
	map.initIterator();

	print_test("Elemento insertado en 5,3 es correcto", map.getNextEntityView() == view3);
	print_test("Elemento insertado es correcto", map.getNextEntityView() == view);
	print_test("Elemento 2 insertado es correcto", map.getNextEntityView() == view2);
	print_test("Iterador no esta al final", !map.iteratorAtEnd());
	map.getNextEntityView();
	print_test("Iterador esta al final", map.iteratorAtEnd());


	delete entity;
	delete view;
	delete view2;
	delete view3;

	return 0;
}
