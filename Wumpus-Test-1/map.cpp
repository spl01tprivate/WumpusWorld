#include "map.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "agentUser.h"
#include "agentKI.h"
#include "gold.h"
#include "pit.h"
#include "wumpus.h"

using namespace std;

//***** Array I/O Functions *****
char map::readGeneratedPath(int y, int x) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) return map::generatedPath[y][x]; //tests whether request is in ary boundaries
	else return '!';
}

void map::writeGeneratedPath(int y, int x, char data) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) map::generatedPath[y][x] = data; //tests whether request is in ary boundaries
}

char map::readFieldData(int y, int x) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) return map::fieldData[y][x]; //tests whether request is in ary boundaries
	else return '!';
}

void map::writeFieldData(int y, int x, char data) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) map::fieldData[y][x] = data; //tests whether request is in ary boundaries
}

string map::readFieldDataString(int y, int x) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) return map::fieldDataString[y][x]; //tests whether request is in ary boundaries
	else return "!";
}

void map::writeFieldDataString(int y, int x, string data) {
	if (y < arrayY && y > -1 && x < arrayX && x > -1) map::fieldDataString[y][x] = data; //tests whether request is in ary boundaries
}

//***** Render Engine + Symbol Functions *****

void map::drawMap(int fieldValue) {
  /* _____ _____ _____
	|     |     |     |
	|  P  |  W  |  G  |
	|_____|_____|_____|
	|  ~  |  $  |  *  |
	|     |     |     |
	|_____|_____|_____|
	|     |     |     |
	|     |     |     |
	|_____|_____|_____|
  */
	bool gameOver = false;

	if (fieldValue == 3 || fieldValue == 4 || fieldValue == 5 || fieldValue == 8) {
		gameOver = true;
	}

	//*** Object Effects ***
	if (!visibilityEffectSymbols) {
		for (int y = 0; y < arrayY; y++)
		{
			for (int x = 0; x < arrayX; x++)
			{
				this->writeFieldDataString(y, x, "."); //clean ary for new calculation bc agent is moving
			}
		}
	}
	//Let Objects write their effects into fieldDataString Ary
	for (int i = 0; i < goldNumber; i++)
	{
		goldVector.at(i).writeEffect(pagentUser->getY(), pagentUser->getX(), gameOver);
	}
	for (int i = 0; i < pitNumber; i++)
	{
		pitVector.at(i).writeEffect(pagentUser->getY(), pagentUser->getX(), gameOver);
	}
	for (int i = 0; i < wumpusNumber; i++)
	{
		wumpusVector.at(i).writeEffect(pagentUser->getY(), pagentUser->getX(), gameOver);
	}

	//*** "Render Engine :)" ***
	int reachedX = 0;
	int reachedY = 0;

	while (reachedY < arrayY) {
		//Top Part
		for (; reachedX < arrayX; reachedX++) //top layer, only at beginning
		{
			if (reachedY == 0) cout << " _____";
			if (reachedX == arrayX - 1 && reachedY == 0) cout << endl;
		}
		reachedX = 0;

		//Mid Part
		for (; reachedX < arrayX; reachedX++)
		{
			cout << "| " << getFieldDataString(reachedY, reachedX) << " "; //inserts effects symbols string
		}
		cout << "|" << endl;
		reachedX = 0;

		for (; reachedX < arrayX; reachedX++)
		{
			cout << "|" << getVisitedData(reachedY, reachedX, gameOver, true) << " " << getFieldDataSymbol(reachedY, reachedX, gameOver) << " " << getVisitedData(reachedY, reachedX, gameOver, false); //inserts visited . and , and Object Symbols (W,P,A,C,G)
		}
		cout << "|" << endl;
		reachedX = 0;

		//Bottom Part
		for (; reachedX < arrayX; reachedX++)
		{
			cout << "|_____";
		}
		cout << "|" << endl;
		reachedX = 0;
		reachedY++;
	}
}

char map::getFieldDataSymbol(int reachedY, int reachedX, bool gameOver) { //returns Object Symbols
	if (visiblityObjectSymbols || gameOver) {
		if (readFieldData(reachedY, reachedX) == '.') return ' ';
		else return readFieldData(reachedY, reachedX);
	}
	else {
		if (readFieldData(reachedY, reachedX) == 'A') return 'A';
		if (visibilityKI && readFieldData(reachedY, reachedX) == 'C') return 'C';
		else return ' ';
	}
}

string map::getFieldDataString(int y, int x) { //returns effects symbols
	if (readFieldDataString(y, x) == ".") { //. = no effect
		return "   ";
	}
	else {
		string retval;
		switch (map::fieldDataString[y][x].size()) //lenght of string -> seperation of chars?
		{
		case 1: 
			return " " + fieldDataString[y][x] + " ";
			break;
		case 2:
			retval.push_back(fieldDataString[y][x].at(0));
			retval.push_back(' ');
			retval.push_back(fieldDataString[y][x].at(1));
			return retval;
			break;
		case 3:
			return fieldDataString[y][x];
			break;
		default:
			break;
		}
	}
}

char map::getVisitedData(int y, int x, bool gameOver, bool userOrKI) {
	if (userOrKI) { //User
		if (pagentUser->getVisitedData(y, x) == '_') return ' ';
		else if (pagentUser->getVisitedData(y, x) == '.') return '.';
		else return pagentUser->getVisitedData(y, x);
	}
	else if (!userOrKI && gameOver) {
		if (pagentKI->getVisitedData(y, x) == '_') return ' ';
		else if (pagentKI->getVisitedData(y, x) == ',') return ',';
		else return pagentKI->getVisitedData(y, x);
	}
	else return ' ';
}

//***** Array Init Functions *****

void map::generatePath() {
	int maxYDistance = 0;
	int maxXDistance = 0;
	switch (yWidth)
	{
	case 1:
		maxYDistance = aryMidPoint[0];
		break;
	case 2:
		maxYDistance = aryMidPoint[1];
		break;
	case 3:
		maxYDistance = aryMidPoint[0] + 1;
		break;
	case 4:
		maxYDistance = aryMidPoint[1] + 1;
		break;
	default:
		break;
	}
	switch (xWidth)
	{
	case 1:
		maxXDistance = aryMidPoint[2];
		break;
	case 2:
		maxXDistance = aryMidPoint[3];
		break;
	case 3:
		maxXDistance = aryMidPoint[2] + 1;
		break;
	case 4:
		maxXDistance = aryMidPoint[3] + 1;
		break;
	default:
		break;
	}
	int maxSteps = maxYDistance + maxXDistance;

	//AgentUser Path
	bool onGold = false;
	while (!onGold) {
		for (int y = 0; y < arrayY; y++) //Clean generated path ary
		{
			for (int x = 0; x < arrayX; x++)
			{
				writeGeneratedPath(y, x, '.');
			}
		}

		int x = pagentUser->getX(); //start pos
		int y = pagentUser->getY();

		for (int i = 0; i < maxSteps; i++)
		{
			int randNum3 = (randomNumber(0, 2));
			int randNum4 = (randomNumber(0, 3));
			switch (pagentUser->getPosition(y, x)) //8 Faelle: 0 = LO, 1 = O, 2 = RO, 3 = R, 4 = RU, 5 = U, 6 = LU, 7 = L, 8 = Not at border
			{
			case 0:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y, x + 1) != 'A') x++;
				else if (readGeneratedPath(y + 1, x) != 'A') y++;
				break;
			case 1:
				if (randNum3 == 0 && readGeneratedPath(y, x + 1) != 'A') x++;
				else if (randNum3 == 1 && readGeneratedPath(y + 1, x) != 'A') y++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			case 2:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y + 1, x) != 'A') y++;
				else if (readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			case 3:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (randNum3 == 1 && readGeneratedPath(y + 1, x) != 'A') y++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			case 4:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			case 5:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (randNum3 == 1 && readGeneratedPath(y, x + 1) != 'A') x++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			case 6:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (readGeneratedPath(y, x + 1) != 'A') x++;
				break;
			case 7:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (randNum3 == 1 && readGeneratedPath(y, x + 1) != 'A') x++;
				else if (randNum3 == 2 && readGeneratedPath(y + 1, x) != 'A') y++;
				break;
			case 8:
				if (randNum4 == 0 && readGeneratedPath(y - 1, x) != 'A') y--;
				else if (randNum4 == 1 && readGeneratedPath(y, x + 1) != 'A') x++;
				else if (randNum4 == 2 && readGeneratedPath(y + 1, x) != 'A') y++;
				else if (randNum4 == 2 && readGeneratedPath(y, x - 1) != 'A') x--;
				break;
			}
			writeGeneratedPath(y, x, 'A');
			if (x == goldXKI && y == goldYKI) {
				onGold = true; //Abbruch Kriterium
				break;
			}
		}
	}

	//AgentKI Path
	onGold = false;
	while (!onGold) {
		for (int y = 0; y < arrayY; y++) //Clean generated path ary
		{
			for (int x = 0; x < arrayX; x++)
			{
				if (readGeneratedPath(y, x) == 'C') writeGeneratedPath(y, x, '.');
				else if (readGeneratedPath(y, x) == 'B') writeGeneratedPath(y, x, 'A');
			}
		}

		int x = pagentKI->getX(); //start pos
		int y = pagentKI->getY();

		for (int i = 0; i < maxSteps; i++)
		{
			int randNum3 = (randomNumber(0, 2));
			int randNum4 = (randomNumber(0, 3));
			switch (pagentKI->getPosition(y, x)) //8 Faelle: 0 = LO, 1 = O, 2 = RO, 3 = R, 4 = RU, 5 = U, 6 = LU, 7 = L, 8 = Not at border
			{
			case 0:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y, x + 1) != 'C') x++;
				else if (readGeneratedPath(y + 1, x) != 'C') y++;
				break;
			case 1:
				if (randNum3 == 0 && readGeneratedPath(y, x + 1) != 'C') x++;
				else if (randNum3 == 1 && readGeneratedPath(y + 1, x) != 'C') y++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			case 2:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y + 1, x) != 'C') y++;
				else if (readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			case 3:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (randNum3 == 1 && readGeneratedPath(y + 1, x) != 'C') y++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			case 4:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			case 5:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (randNum3 == 1 && readGeneratedPath(y, x + 1) != 'C') x++;
				else if (randNum3 == 2 && readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			case 6:
				if (randomNumber(0, 1) == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (readGeneratedPath(y, x + 1) != 'C') x++;
				break;
			case 7:
				if (randNum3 == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (randNum3 == 1 && readGeneratedPath(y, x + 1) != 'C') x++;
				else if (randNum3 == 2 && readGeneratedPath(y + 1, x) != 'C') y++;
				break;
			case 8:
				if (randNum4 == 0 && readGeneratedPath(y - 1, x) != 'C') y--;
				else if (randNum4 == 1 && readGeneratedPath(y, x + 1) != 'C') x++;
				else if (randNum4 == 2 && readGeneratedPath(y + 1, x) != 'C') y++;
				else if (randNum4 == 2 && readGeneratedPath(y, x - 1) != 'C') x--;
				break;
			}
			if (readGeneratedPath(y, x) == 'A') writeGeneratedPath(y, x, 'B');
			else writeGeneratedPath(y, x, 'C');

			if (x == goldXKI && y == goldYKI) {
				onGold = true; //Abbruch Kriterium
				break;
			}
		}
	}

	cout << endl << "Path to gold was found!" << endl;
	for (int y = 0; y < arrayY; y++)
	{
		for (int x = 0; x < arrayX; x++)
		{
			cout << readGeneratedPath(y, x) << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool map::initArray() { //once called at drawMap to fill ary
	//Fill Arrays with points = empty space
	for (int y = 0; y < arrayY; y++)
	{
		for (int x = 0; x < arrayX; x++)
		{
			writeFieldData(y, x, '.');
			writeFieldDataString(y, x, ".");
		}
	}

	//Agent Spawn at maxY and minX (0|0)
	map* pworld = this;
	pagentUser->initByMap(arrayY - 1, 0, pworld);
	pagentUser->setPos(0, 0);
	pagentKI->initByMap(0, arrayX - 1, pworld);
	pagentKI->setPos(arrayY - 1, arrayX - 1);

	generatePath();

	//Gold Spawn
	if (this->readFieldData(goldYKI, goldXKI) == '.' && readFieldData(goldYKI - 1, goldXKI) != 'A' && readFieldData(goldYKI, goldXKI - 1) != 'A' && readFieldData(goldYKI, goldXKI + 1) != 'A' && readFieldData(goldYKI + 1, goldXKI) != 'A' && readFieldData(goldYKI - 1, goldXKI) != 'C' && readFieldData(goldYKI, goldXKI - 1) != 'C' && readFieldData(goldYKI, goldXKI + 1) != 'C' && readFieldData(goldYKI + 1, goldXKI) != 'C') { //look whether free
		gold gold(goldYKI, goldXKI, pworld);
		goldVector.push_back(gold);
	}
	for (int i = 0; i < goldNumber - 1; i++) //repeats for the number of golds initialzied with map object
	{
		bool goldSet = false;
		while (!goldSet) //bruteforces localization of free array space
		{
			int goldX = this->randomNumber(0, arrayX - 1);
			int goldY = this->randomNumber(0, arrayY - 1);

			if (this->readFieldData(goldY, goldX) == '.' && readFieldData(goldY - 1, goldX) != 'A' && readFieldData(goldY, goldX - 1) != 'A' && readFieldData(goldY, goldX + 1) != 'A' && readFieldData(goldY + 1, goldX) != 'A' && readFieldData(goldY - 1, goldX) != 'C' && readFieldData(goldY, goldX - 1) != 'C' && readFieldData(goldY, goldX + 1) != 'C' && readFieldData(goldY + 1, goldX) != 'C') { //look whether free
				gold gold(goldY, goldX, pworld);
				goldVector.push_back(gold); //store object in vector for later use
				goldSet = true;
			}
		}
	}
	//Pit Spawn
	for (int i = 0; i < pitNumber; i++) //repeats for the number of pits initialzied with map object
	{
		bool pitSet = false;
		while (!pitSet) //bruteforces localization of free array space
		{
			int pitX = this->randomNumber(0, arrayX - 1);
			int pitY = this->randomNumber(0, arrayY - 1);

			if (this->readFieldData(pitY, pitX) == '.' && readFieldData(pitY - 1, pitX) != 'A' && readFieldData(pitY, pitX - 1) != 'A' && readFieldData(pitY, pitX + 1) != 'A' && readFieldData(pitY + 1, pitX) != 'A' && readFieldData(pitY - 1, pitX) != 'C' && readFieldData(pitY, pitX - 1) != 'C' && readFieldData(pitY, pitX + 1) != 'C' && readFieldData(pitY + 1, pitX) != 'C' && readGeneratedPath(pitY, pitX) == '.') { //look whether free
				pit pit(pitY, pitX, pworld);
				pitVector.push_back(pit); //store object in vector for later use
				pitSet = true;
			}
		}
	}
	//Wumpus Spawn
	for (int i = 0; i < wumpusNumber; i++) //repeats for the number of wumpus initialzied with map object
	{
		bool wumpusSet = false;
		while (!wumpusSet)
		{
			int wumpusX = this->randomNumber(0, arrayX - 1);
			int wumpusY = this->randomNumber(0, arrayY - 1);

			if (this->readFieldData(wumpusY, wumpusX) == '.' && readFieldData(wumpusY - 1, wumpusX) != 'A' && readFieldData(wumpusY, wumpusX - 1) != 'A' && readFieldData(wumpusY, wumpusX + 1) != 'A' && readFieldData(wumpusY + 1, wumpusX) != 'A' && readFieldData(wumpusY - 1, wumpusX) != 'C' && readFieldData(wumpusY, wumpusX - 1) != 'C' && readFieldData(wumpusY, wumpusX + 1) != 'C' && readFieldData(wumpusY + 1, wumpusX) != 'C' && readGeneratedPath(wumpusY, wumpusX) == '.') {
				wumpus wumpus(wumpusY, wumpusX, pworld);
				wumpusVector.push_back(wumpus); //store object in vector for later use
				wumpusSet = true;
			}
		}
	}

	return true;
}

void map::goldSpawnArea() {
	//Y-Coords
	if (arrayY % 2 == 0) {
		if ((arrayY - 4) >= 2) {
			//Border width = 4
			yWidth = 4;
			if (randomNumber(0, 1) == 0) {
				goldYKI = aryMidPoint[0] - randomNumber(0, 1);
			}
			else {
				goldYKI = aryMidPoint[1] + randomNumber(0, 1);
			}
		}
		else {
			//Border width = 2
			yWidth = 2;
			if (randomNumber(0, 1) == 0) goldYKI = aryMidPoint[0];
			else goldYKI = aryMidPoint[1];
		}
	}
	else {
		if ((arrayY - 3) >= 2) {
			//Border width = 3
			yWidth = 3;
			switch (randomNumber(0, 2))
			{
			case 0: goldYKI = aryMidPoint[0] - 1;
				break;
			case 1: goldYKI = aryMidPoint[0];
				break;
			case 2: goldYKI = aryMidPoint[0] + 1;
				break;
			}
		}
		else {
			//Border width = 1
			yWidth = 1;
			goldYKI = aryMidPoint[0];
		}
	}
	//X-Coords
	if (arrayX % 2 == 0) {
		if ((arrayX - 4) >= 2) {
			//Border width = 4
			xWidth = 4;
			if (randomNumber(0, 1) == 0) {
				goldXKI = aryMidPoint[2] - randomNumber(0, 1);
			}
			else {
				goldXKI = aryMidPoint[3] + randomNumber(0, 1);
			}
		}
		else {
			//Border width = 2
			xWidth = 2;
			if (randomNumber(0, 1) == 0) goldXKI = aryMidPoint[0];
			else goldXKI = aryMidPoint[3];
		}
	}
	else {
		if ((arrayX - 3) >= 2) {
			//Border width = 3
			xWidth = 3;
			switch (randomNumber(0, 2))
			{
			case 0: goldXKI = aryMidPoint[2] - 1;
				break;
			case 1: goldXKI = aryMidPoint[2];
				break;
			case 2: goldXKI = aryMidPoint[2] + 1;
				break;
			}
		}
		else {
			//Border width = 1
			xWidth = 1;
			goldXKI = aryMidPoint[2];
		}
	}
	cout << endl << "Gold Y | X : " << goldYKI << " | " << goldXKI << endl;
}

void map::getMidPoint() {
	//Y-Mid
	if (arrayY % 2 == 0) {
		aryMidPoint[1] = arrayY / 2;
		aryMidPoint[0] = (arrayY / 2) - 1;
	}
	else {
		aryMidPoint[0] = arrayY / 2;
		aryMidPoint[1] = -1;
	}
	//X-Mid
	if (arrayX % 2 == 0) {
		aryMidPoint[3] = arrayX / 2;
		aryMidPoint[2] = (arrayX / 2) - 1;
	}
	else {
		aryMidPoint[2] = arrayX / 2;
		aryMidPoint[3] = -1;
	}

	cout << "Mid-Point Y | X : " << aryMidPoint[0] << " " << aryMidPoint[1] << " | " << aryMidPoint[2] << " " << aryMidPoint[3] << endl;
}

void map::setArraySize(int aryY, int aryX) {
	if (!arraySizeSet) {
		arrayY = aryY; //Array Size
		arrayX = aryX;

		//Array itself
		fieldData = new char* [arrayY]; //Init first pointer array (rows - zeilen)
		for (int i = 0; i < arrayY; i++)
		{
			fieldData[i] = new char[arrayX]; //for every row init a pointer array of cols 
		}

		fieldDataString = new string * [arrayY];
		for (int i = 0; i < arrayY; i++)
		{
			fieldDataString[i] = new string[arrayX];
		}

		generatedPath = new char* [arrayY]; 
		for (int i = 0; i < arrayY; i++)
		{
			generatedPath[i] = new char[arrayX]; 
		}

		//confirm set and prevent reset
		arraySizeSet = true;

		getMidPoint();
		goldSpawnArea();

		if (!aryInitialised) {
			aryInitialised = initArray();
		}
	}
}

//***** Helper Functions *****

int map::randomNumber(int lowerBound, int upperBound) {
	random_device rd;
	mt19937 gen = mt19937(rd());
	uniform_int_distribution<> dis =
		uniform_int_distribution<>(lowerBound, upperBound);
	return dis(gen);
}

//***** Constructor *****

map::map(int pitNum, int wumpusNum, int goldNum, agentUser* pagentUser, agentKI* pagentKI, bool pvisiblityObjectSymbols, bool pvisibilityEffectSymbols, bool pvisibilityKI) {
	map::pitNumber = pitNum;
	map::wumpusNumber = wumpusNum;
	map::goldNumber = goldNum;
	map::pagentUser = pagentUser;
	map::pagentKI = pagentKI;
	map::visiblityObjectSymbols = pvisiblityObjectSymbols;
	map::visibilityEffectSymbols = pvisibilityEffectSymbols;
	map::visibilityKI = pvisibilityKI;
}