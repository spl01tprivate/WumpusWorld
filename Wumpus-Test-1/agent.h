#pragma once

class map;

class agent
{
protected:
	map* pworld;
private:
	int Ypos, Xpos;
	bool onGoldField = false;
	int** visitedFieldData; //stores objects visited fields
public:
	friend class agentUser;
	friend class agentKI;
	virtual int setPos(int Ypos, int Xpos);
	void initByMap(int YposC, int XposC, map* pworldC);
	int pickUpGold();

	int getPosition(int y, int x);
	int randomNumber(int lowerBound, int upperBound);

	//Getter
	int getX() { return Xpos; };
	int getY() { return Ypos; };
	char getVisitedData(int y, int x);

	agent();
};
