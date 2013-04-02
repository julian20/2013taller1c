/*
 * Coordinates.h
 *
 *  Created on: Apr 2, 2013
 *      Author: fede
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

class Coordinates {
public:
	Coordinates(int _row, int _col);
    void changeTo(int _row, int _col);
	int getRow() const;
	void setRow(int _row);
	int getCol() const;
	void setCol(int _col);
private:
	int row, col;
};


#endif /* COORDINATES_H_ */
