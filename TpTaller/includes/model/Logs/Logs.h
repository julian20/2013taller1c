/*
 * Logs.h
 *
 *  Created on: 01/04/2013
 *      Author: damian
 */

#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>

#define RUTA_LOG "errores.log"
using namespace std;

class Logs {
public:
	Logs();
	virtual ~Logs();
	static void openFile();
	static void logErrorMessage(string msg);
	static void closeFile();
private:

	static string Date();
	string msj;
	static FILE* archivo;
};

#endif /* LOGS_H_ */
