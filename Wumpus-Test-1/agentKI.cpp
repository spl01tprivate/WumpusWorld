#include <iostream>
#include "agentKI.h"
#include "map.h"

using namespace std;

int agentKI::setPos(int YposC, int XposC) {
	if (YposC < pworld->getAryY() && YposC > -1 && XposC < pworld->getAryX() && XposC > -1) { //checks whether given coordinates are in ary boundaries
		//Going on empty Field
		if (pworld->readFieldData(YposC, XposC) == '.') { //checks which object is selected field
			pworld->writeFieldData(YposC, XposC, 'C');
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G'); //reset field from which agent is coming with Gold
			else pworld->writeFieldData(Ypos, Xpos, '.'); //reset field from which agent is coming with point again
			onGoldField = false;
		}
		//Going on Gold Field
		else if (pworld->readFieldData(YposC, XposC) == 'G') {
			pworld->writeFieldData(YposC, XposC, 'C');
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = true;
			visitedFieldData[YposC][XposC] = 'w';
			return 5; //Retval = 5 --> Picked up gold
		}
		//Going on Pit Field
		else if (pworld->readFieldData(YposC, XposC) == 'P') {
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = false;
			agent::Ypos = YposC;
			agent::Xpos = XposC;
			died = true;
			visitedFieldData[YposC][XposC] = 'd';
			return 3;
		}
		//Going on Wumpus Field
		else if (pworld->readFieldData(YposC, XposC) == 'W') {
			if (onGoldField) pworld->writeFieldData(Ypos, Xpos, 'G');
			else pworld->writeFieldData(Ypos, Xpos, '.');
			onGoldField = false;
			agent::Ypos = YposC;
			agent::Xpos = XposC;
			died = true;
			visitedFieldData[YposC][XposC] = 'd';
			return 4;
		}
		else return 1; //Retval = 1 --> Other object on field
		agent::Ypos = YposC;
		agent::Xpos = XposC;
		visitedFieldData[Ypos][Xpos] = ',';
		return 0; //Retval = 0 --> Nothing
	}
	else return 2; //Retval = 2 --> Wall
}

int agentKI::move(char direction) {
	switch (direction) {
	case 'u': if (Ypos > 0) return setPos(Ypos - 1, Xpos);
		break;
	case 'd': if (Ypos < pworld->getAryY() - 1) return setPos(Ypos + 1, Xpos);
		break;
	case 'r': if (Xpos < pworld->getAryX() - 1) return setPos(Ypos, Xpos + 1);
		break;
	case 'l': if (Xpos > 0) return setPos(Ypos, Xpos - 1);
		break;
	default: cout << endl << "Error in KI Player - move()" << endl;
		return -1;
		break;
	}
}

int agentKI::decideMove() {
	if (died) return 6;

	//8 Faelle: 0 = LO, 1 = O, 2 = RO, 3 = R, 4 = RU, 5 = U, 6 = LU, 7 = L, 8 = Not at border
	int position = getPosition(Ypos, Xpos);

	int upperBound; //decide how many decisions should be made
	if (position == 0 || position == 2 || position == 4 || position == 6) upperBound = 1; //2 possible directions to move
	else if (position == 1 || position == 3 || position == 5 || position == 7) upperBound = 2; //3 possible directions to move
	else upperBound = 3; //4 possible directions to move

	int decision = randomNumber(0, upperBound); //get random decision
	
	switch (position)
	{
		//Even cases -> in corner
	case 0: 
		if (decision == 0) {
			return move('r');
	    }
		else return move('d');
		break;
	case 2: 
		if (decision == 0) {
			return move('d');
		}
		else return move('l');
		break;
	case 4: 
		if (decision == 0) {
			return move('l');
		}
		else return move('u');
		break;
	case 6: 
		if (decision == 0) {
			return move('u');
		}
		else return move('r');
		break;
		//Odd cases -> at edge
	case 1: 
		if (decision == 0) return move('r');
		else if (decision == 1) return move('d');
		else if (decision == 2) return move('l');
		break;
	case 3:
		if (decision == 0) return move('u');
		else if (decision == 1) return move('d');
		else if (decision == 2) return move('l');
		break;
	case 5:
		if (decision == 0) return move('u');
		else if (decision == 1) return move('r');
		else if (decision == 2) return move('l');
		break;
	case 7:
		if (decision == 0) return move('u');
		else if (decision == 1) return move('d');
		else if (decision == 2) return move('r');
		break;
	case 8:
		if (decision == 0) return move('u');
		else if (decision == 1) return move('d');
		else if (decision == 2) return move('l');
		else if (decision == 3) return move('r');
		break;
	default:
		cout << endl << "Error in KI Player - decideMove()" << endl;
		return -2;
		break;
	}
	return 0;
}