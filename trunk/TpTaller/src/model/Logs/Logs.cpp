/*
 * Logs.cpp
 *
 *  Created on: 01/04/2013
 *      Author: damian
 */

#include <model/Logs/Logs.h>

Logs::Logs(string msj) {

	this->msj = msj;
	this->archivo = NULL;
	this->GenerateLogMsj();
	this->~Logs();
}

void Logs::GenerateLogMsj() {
	this->archivo = fopen(ruta, "a");
	if (!this->archivo) {
		printf(
				"No se puede continuar con la ejecucion ya que no se pueden manejar los errores\n");
	}
	string hora = this->Date();
	string errorLog = hora + string("-----") + this->msj;
	fwrite(errorLog.c_str(), 1, errorLog.size(), this->archivo);

}

string Logs::Date() {

	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	char salida[128];
	strftime(salida, 128, "%d/%m/%y %H:%M:%S", tlocal);
	//printf("%s\n",output);

	return string(salida);
}
Logs::~Logs() {
	// TODO Auto-generated destructor stub
}

