/*
 * Power.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef POWER_H_
#define POWER_H_

#include <string>
#include <iostream>

using namespace std;

class Power {
public:
	Power();
	Power(std::string, int damage, int range);
	int getDamage() const;
	void setDamage(int damage);
	std::string getName();
	void setName(std::string name);
	int getRange();
	void setRange(int range);
	virtual ~Power();
	//Operator to transform the object into a stream.
	friend ostream& operator <<(std::ostream&, const Power&);

	//Operator to load an object from a stream
	friend istream& operator >>(std::istream&, Power&);
private:
	std::string name;
	int damage;
	int range;
};

#endif /* POWER_H_ */
