#pragma once

class map;

class gold
{
protected:
	map* pworld;
private:
	int Ypos, Xpos;
public:
	int setPos(int YposC, int XposC);
	void writeEffectIntoArray(int y, int x);
	void writeEffect(int agentY, int agentX, bool gameOver);

	//Getter
	int xPos() { return Xpos; };
	int yPos() { return Ypos; };

	//Constructor
	gold(int YposC, int XposC, map* pworldC);
};

