#include <random>
#include "agent.h"
#include "map.h"

using namespace std;

int agent::randomNumber(int lowerBound, int upperBound) {
	random_device rd;
	mt19937 gen = mt19937(rd());
	uniform_int_distribution<> dis =
		uniform_int_distribution<>(lowerBound, upperBound);
	return dis(gen);
}

int agent::getPosition(int y, int x) {
	int position; //8 Faelle: 0 = LO, 1 = O, 2 = RO, 3 = R, 4 = RU, 5 = U, 6 = LU, 7 = L, 8 = Not at border
	if (Ypos == 0 && Xpos == 0) position = 0;
	else if (Ypos == 0 && Xpos != 0 && Xpos != pworld->getAryX() - 1) position = 1;
	else if (Ypos == 0 && Xpos == pworld->getAryX() - 1) position = 2;
	else if (Ypos != 0 && Ypos != pworld->getAryY() - 1 && Xpos == pworld->getAryX() - 1) position = 3;
	else if (Ypos == pworld->getAryY() - 1 && Xpos == pworld->getAryX() - 1) position = 4;
	else if (Ypos == pworld->getAryY() - 1 && Xpos != 0 && Xpos != pworld->getAryX() - 1) position = 5;
	else if (Ypos == pworld->getAryY() - 1 && Xpos == 0) position = 6;
	else if (Ypos != 0 && Ypos != pworld->getAryY() - 1 && Xpos == 0) position = 7;
	else position = 8;
	return position;
}

int agent::setPos(int YposC, int XposC) {
	if (YposC < pworld->getAryY() && YposC > -1 && XposC < pworld->getAryX() && XposC > -1) { //checks whether given coordinates are in ary boundaries
		//Going on empty Field
		if (pworld->readFieldData(YposC, XposC) == '.') { //checks which object is selected field
			pworld->writeFieldData(YposC, XposC, 'A');
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G'); //reset field from which agent is coming with Gold
			else pworld->writeFieldData(Ypos, Xpos, '.'); //reset field from which agent is coming with point again
			onGoldField = false;
			visitedFieldData[YposC][XposC] = '.';
		}
		//Going on Gold Field
		else if (pworld->readFieldData(YposC, XposC) == 'G') {
			pworld->writeFieldData(YposC, XposC, 'A');
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = true;
			visitedFieldData[YposC][XposC] = '.';
		}
		//Going on Pit Field
		else if (pworld->readFieldData(YposC, XposC) == 'P') {
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = false;
			agent::Ypos = YposC;
			agent::Xpos = XposC;
			visitedFieldData[Ypos][Xpos] = 'd';
			return 3;
		}
		//Going on Wumpus Field
		else if (pworld->readFieldData(YposC, XposC) == 'W') {
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = false;
			agent::Ypos = YposC;
			agent::Xpos = XposC;
			visitedFieldData[Ypos][Xpos] = 'd';
			return 4;
		}
		else return 1; //Retval = 1 --> Other object on field
		agent::Ypos = YposC;
		agent::Xpos = XposC;
		visitedFieldData[Ypos][Xpos] = '.';
		return 0; //Retval = 0 --> Nothing or Gold on field
	}
	else return 2; //Retval = 2 --> Wall
}

int agent::pickUpGold() {
	if (onGoldField) {
		visitedFieldData[Ypos][Xpos] = 'w';
		return 5;
	}
	else return 6;
}

char agent::getVisitedData(int y, int x) {
	return visitedFieldData[y][x];
}

void agent::initByMap(int YposC, int XposC, map* pworldC) { //Inits Agent with start coordinates and world Pointer for ary access, once called in initArry() map class
	//not in constructor, bc agent is created in main and x,y is inited in map
	agent::Ypos = YposC;
	agent::Xpos = XposC;
	pworld = pworldC;

	visitedFieldData = new int* [pworldC->getAryY()]; //Init first pointer array (rows - zeilen)
	for (int i = 0; i < pworldC->getAryY(); i++)
	{
		visitedFieldData[i] = new int[pworldC->getAryX()]; //for every row init a pointer array of cols 
	}

	for (int i = 0; i < pworldC->getAryY(); i++)
	{
		for (int j = 0; j < pworldC->getAryX(); j++)
		{
			visitedFieldData[i][j] = '_';
		}
	}
}

//***** Constructor *****
agent::agent() {
}