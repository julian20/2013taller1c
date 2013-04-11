/*
 * Entity.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: gonchub
 */

#include <model/entities/Entity.h>
#include <model/map/Tile.h>

#include <vector>
#include <stdio.h>
#include <vector>

using namespace std;

Entity::Entity() {
        this->currentPos = new Vector2(0, 0);
        this->coord = new Coordinates(0,0);
        this->base = new Base();
        this->name = "";
}

void Entity::setPos(float x, float y) {
        currentPos->setValues(x,y);
        coord->changeTo((int) x/Tile::computePosition(0,0).w , (int) y/Tile::computePosition(0,0).h);
}

Vector2* Entity::getCurrentPos() {
        return currentPos;
}

Coordinates* Entity::getCoordinates() {
        return coord;
}

void Entity::setCoordinates(int x, int y){
        coord->changeTo(x,y);
}

string Entity::getClassName() {
        return "Entity";
}

Base* Entity::getBase() {
        return this->base;
}

void Entity::setBase(Base* base) {
        this->base = base;
}

std::string Entity::getName() {
        return this->name;
}

void Entity::setName(std::string name) {
        this->name = name;
}

Entity::~Entity() {
        delete this->coord;
        delete this->currentPos;
        delete this->base;
}
