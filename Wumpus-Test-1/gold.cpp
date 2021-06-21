#include "gold.h"
#include "map.h"

using namespace std;

void gold::writeEffectIntoArray(int y, int x) {
	string arrayContent = pworld->readFieldDataString(y, x); //get content at array position
	if (arrayContent == ".") arrayContent = "*"; //if content is empty
	else if (arrayContent.find("*") != string::npos); //already in there -> no entry
	else arrayContent = arrayContent + "*"; //else add * at the end
	pworld->writeFieldDataString(y, x, arrayContent);
}

void gold::writeEffect(int agentY, int agentX, bool gameOver) {
	if (agentY == Ypos && agentX == Xpos && !gameOver) {
		writeEffectIntoArray(Ypos, Xpos);
	}
	else if (gameOver) {
		writeEffectIntoArray(Ypos, Xpos);
	}
}

int gold::setPos(int YposC, int XposC) {
	if (YposC < pworld->getAryY() && YposC > -1 && XposC < pworld->getAryX() && XposC > -1) { //checks whether given coordinates are in ary boundaries
		if (pworld->readFieldData(YposC, XposC) == '.') {
			pworld->writeFieldData(YposC, XposC, 'G');
			pworld->writeFieldData(Ypos, Xpos, '.'); //reset field from which agent is coming with point again
			gold::Ypos = YposC;
			gold::Xpos = XposC;
			return 0;
		}
		else return 1; //Retval = 1 --> Other Object
	}
	else return 2; //Retval = 2 --> Wall
}

//***** Constructor *****
gold::gold(int YposC, int XposC, map* pworldC) { //Inits Gold with start coordinates and world Pointer for ary access, called in initArry() map class
	gold::Ypos = YposC;
	gold::Xpos = XposC;
	pworld = pworldC;
	pworld->writeFieldData(Ypos, Xpos, 'G');
}