/*
 * Power.h
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#ifndef POWER_H_
#define POWER_H_

#include <string>

class Power {
public:
	Power(std::string, int damage, int range);
	int getDamage() const;
	void setDamage(int damage);
	std::string getName();
	void setName(std::string name);
	int getRange();
	void setRange(int range);
	virtual ~Power();
private:
	Power();
	std::string name;
	int damage;
	int range;
};

#endif /* POWER_H_ */
