/*
 * IRepresentable.h
 *
 *  Created on: Mar 26, 2013
 *      Author: fede
 */

#ifndef IREPRESENTABLE_H_
#define IREPRESENTABLE_H_

class IRepresentable {
public:
	virtual ~IRepresentable();

	virtual bool Transitable();
};

#endif /* IREPRESENTABLE_H_ */
