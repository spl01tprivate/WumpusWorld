#include "wumpus.h"
#include "map.h"

using namespace std;

void wumpus::writeEffectIntoArray(int y, int x) {
	if (pworld->getFieldDataSymbol(y, x, true) != 'P' && pworld->getFieldDataSymbol(y, x, true) != 'W') {
		string arrayContent = pworld->readFieldDataString(y, x); //get content at array position
		if (arrayContent == ".") arrayContent = "$"; //if content is empty
		else if (arrayContent.find("$") != string::npos); //already in there -> no entry
		else arrayContent += "$"; //else add $ at the end
		pworld->writeFieldDataString(y, x, arrayContent);
	}
}

void wumpus::writeEffect(int agentY, int agentX, bool gameOver) {
	if (agentY == Ypos - 1 && agentX == Xpos && !gameOver || agentY == Ypos && agentX == Xpos - 1 && !gameOver || agentY == Ypos && agentX == Xpos + 1 && !gameOver || agentY == Ypos + 1 && agentX == Xpos && !gameOver) {
		writeEffectIntoArray(agentY, agentX);
	}
	else if (gameOver) {
		if (Ypos - 1 >= 0) writeEffectIntoArray(Ypos - 1, Xpos);
		if (Xpos - 1 >= 0) writeEffectIntoArray(Ypos, Xpos - 1);
		if (Xpos + 1 < pworld->getAryX()) writeEffectIntoArray(Ypos, Xpos + 1);
		if (Ypos + 1 < pworld->getAryY()) writeEffectIntoArray(Ypos + 1, Xpos);
	}
}

int wumpus::setPos(int YposC, int XposC) {
	if (YposC < pworld->getAryY() && YposC > -1 && XposC < pworld->getAryX() && XposC > -1) { //checks whether given coordinates are in ary boundaries
		if (pworld->readFieldData(YposC, XposC) == '.') {
			pworld->writeFieldData(YposC, XposC, 'W');
			pworld->writeFieldData(Ypos, Xpos, '.'); //reset field from which agent is coming with point again
			wumpus::Ypos = YposC;
			wumpus::Xpos = XposC;
			return 0;
		}
		else return 1; //Retval = 1 --> Other Object
	}
	else return 2; //Retval = 2 --> Wall
}

//***** Constructor *****
wumpus::wumpus(int YposC, int XposC, map* pworldC) { //Inits Gold with start coordinates and world Pointer for ary access, called in initArry() map class
	wumpus::Ypos = YposC;
	wumpus::Xpos = XposC;
	pworld = pworldC;
	pworld->writeFieldData(Ypos, Xpos, 'W');
}