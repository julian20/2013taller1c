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

#define ruta "/Logs/errores.log"
using namespace std;

class Logs {
public:
	Logs(string msj);
	virtual ~Logs();
private:
	void GenerateLogMsj();
	string Date();
	string msj;
	FILE* archivo;
};

#endif /* LOGS_H_ */
