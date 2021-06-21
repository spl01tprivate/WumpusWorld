#pragma once
#include "agent.h"

class agentKI :
    public agent
{
private:
    bool died = false;
public:
    int setPos(int y, int x);
    int move(char direction);
    int decideMove();
};
