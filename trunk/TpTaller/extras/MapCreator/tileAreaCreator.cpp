/*
 * main.cpp
 *
 *  Created on: 14/04/2013
 *      Author: tomas
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>




void print_help2 (){
	std::cout << "parametro 1: que texture se va a insertar" << std::endl;
	std::cout << "parametro 2: la coordenada X en donde se ubicara el extremo superior del rectangulo" << std::endl;
	std::cout << "parametro 3: la coordenada Y en donde se ubicara el extremo superior del rectangulo" << std::endl;
	std::cout << "parametro 4: la cantidad de tiles del lado X" << std::endl;
	std::cout << "parametro 5: la cantidad de tiles del lado Y" << std::endl;
	std::cout << "parametro 6: la separacion entre tiles" << std::endl;	
}


// parametro 1: que texture se va a insertar
// parametro 2: la coordenada X en donde se ubicara el extremo superior del rectangulo
// parametro 3: la coordenada Y en donde se ubicara el extremo superior del rectangulo
// parametro 4: la cantidad de tiles del lado X
// parametro 5: la cantidad de tiles del lado Y
// parametro 6: la separacion entre tiles
int main(int argc, char** argv){

	char* header = argv[1];
	
	if (argc < 6 || strcmp(header,"-h") == 0){
		print_help2();
		return 0;
	}
	
	int centerX = atoi(argv[2]);
	int centerY = atoi(argv[3]);
	int sizeX = atoi(argv[4]);
	int sizeY = atoi(argv[5]);
	int step = atoi(argv[6]);
	for (int i = centerX ; i < centerX + sizeX ; i += step ){
		for (int j = centerY ; j < centerY + sizeY ; j += step){
			std::cout <<"  - position: [" << i << ", " << j << ", 0]" << std::endl;
			std::cout <<"    texture: "<< header << std::endl;

		}
	}






	return 0;
}

