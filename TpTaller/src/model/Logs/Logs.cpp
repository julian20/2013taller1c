/*
 * Logs.cpp
 *
 *  Created on: 01/04/2013
 *      Author: damian
 */

#include <model/Logs/Logs.h>

Logs::Logs(string msj) {

//	printf("entra al log\n");
	this->msj=msj;
//	printf("msj = %s",msj.c_str());
	this->archivo=NULL;
	this->GenerateLogMsj();
//	this->~Logs();
}

void Logs::GenerateLogMsj()
{
	this->archivo=fopen(ruta,"a");
	if(!this->archivo)
	{
		printf("No se puede continuar con la ejecucion ya que no se pueden manejar los errores\n");
	}
	string hora=this->Date();
	string errorLog= hora+string("--------")+this->msj+string("\n");
	fwrite(errorLog.c_str(),1,errorLog.size(),this->archivo);
//	delete[] hora;
//	delete[] errorLog;
	fclose(this->archivo);
}


string Logs::Date() {

        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char salida[128];
        strftime(salida,128,"%d/%m/%y %H:%M:%S",tlocal);
        return string(salida);
}
Logs::~Logs() {
	delete this;
}

