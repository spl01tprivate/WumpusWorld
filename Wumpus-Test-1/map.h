#pragma once

#include <vector>
#include <string>
#include "gold.h"
#include "pit.h"
#include "wumpus.h"

class agentUser;
class agentKI;

class map
{
protected: 
	agentUser* pagentUser;
	agentKI* pagentKI;

private:
	int arrayY, arrayX, pitNumber, wumpusNumber, goldNumber, goldYKI, goldXKI, xWidth, yWidth;
	int aryMidPoint[4];
	bool aryInitialised = false;
	bool arraySizeSet = false;
	bool visiblityObjectSymbols, visibilityEffectSymbols, visibilityKI;
	char** generatedPath; //stores objects positions
	char** fieldData; //stores objects positions
	std::string** fieldDataString; //stores objects effects positions
	//Vectors storing spawned objects
	std::vector<pit> pitVector;
	std::vector<wumpus> wumpusVector;
	std::vector<gold> goldVector;

public:
	void drawMap(int fieldValue);
	char getFieldDataSymbol(int y, int x, bool gameOver);
	std::string getFieldDataString(int y, int x);
	char getVisitedData(int y, int x, bool gameOver, bool userOrKI);
	
	char readGeneratedPath(int y, int x);
	void writeGeneratedPath(int y, int x, char data);
	char readFieldData(int y, int x);
	void writeFieldData(int y, int x, char data); 
	std::string readFieldDataString(int y, int x);
	void writeFieldDataString(int y, int x, std::string data);
	int getAryY() { return arrayY; };
	int getAryX() { return arrayX; };

	bool initArray();
	void setArraySize(int aryY, int aryX);
	void getMidPoint();
	void goldSpawnArea();
	void generatePath();

	int randomNumber(int lowerBound, int upperBound);

	map(int pitNum, int wumpusNum, int goldNum, agentUser*, agentKI*, bool visiblityObjectSymbols, bool visibilityEffectSymbols, bool visibilityKI);
};
