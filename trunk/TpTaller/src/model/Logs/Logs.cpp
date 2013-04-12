/*
 * Logs.cpp
 *
 *  Created on: 01/04/2013
 *      Author: damian
 */

#include <model/Logs/Logs.h>

Logs::Logs() {
	this->archivo = fopen(RUTA_LOG, "a");
	if (!this->archivo) {
		printf(
				"Error al crear archivo de logs\n");
	}

}

void Logs::logErrorMessage(string msg) {

	string hora = this->Date();
	string errorLog = hora + string("--------") + msg + string("\n");
	fprintf(archivo,"%s",errorLog.c_str());
}

string Logs::Date() {

	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	char salida[128];
	strftime(salida, 128, "%d/%m/%y %H:%M:%S", tlocal);
	return string(salida);
}
Logs::~Logs() {
	fclose(archivo);
}

