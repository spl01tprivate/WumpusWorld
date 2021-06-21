#include <iostream>
#include <random>
#include "map.h"
#include "agentUser.h"
#include "agentKI.h"

using namespace std;

//TODO: KI Agent

void randomNumber() {
	int lowerBound, upperBound;

	cout << endl;
	cout << "Lower-Bound: ";
	cin >> lowerBound;
	cout << "Upper-Bound: ";
	cin >> upperBound;

	random_device rd;
	mt19937 gen = mt19937(rd());

	uniform_int_distribution<> dis =
		uniform_int_distribution<>(lowerBound, upperBound);

	cout << endl << "Random Number: " << dis(gen) << endl;
}

//*** Game Setting Functions ***
int calcMaxSteps(int* arrayX, int* arrayY) {
	int maxSteps = 0;
	int aryMidPointMain[4];
	//Y-Mid
	if (*arrayY % 2 == 0) {
		aryMidPointMain[1] = *arrayY / 2;
		aryMidPointMain[0] = (*arrayY / 2) - 1;
	}
	else {
		aryMidPointMain[0] = *arrayY / 2;
		aryMidPointMain[1] = -1;
	}
	//X-Mid
	if (*arrayX % 2 == 0) {
		aryMidPointMain[3] = *arrayX / 2;
		aryMidPointMain[2] = (*arrayX / 2) - 1;
	}
	else {
		aryMidPointMain[2] = *arrayX / 2;
		aryMidPointMain[3] = -1;
	}

	int yWidth, xWidth;
	//Y-Coords
	if (*arrayY % 2 == 0) {
		if ((*arrayY - 4) >= 2) {
			//Border width = 4
			yWidth = 4;
		}
		else {
			//Border width = 2
			yWidth = 2;
		}
	}
	else {
		if ((*arrayY - 3) >= 2) {
			//Border width = 3
			yWidth = 3;
		}
		else {
			//Border width = 1
			yWidth = 1;
		}
	}
	//X-Coords
	if (*arrayX % 2 == 0) {
		if ((*arrayX - 4) >= 2) {
			//Border width = 4
			xWidth = 4;
		}
		else {
			//Border width = 2
			xWidth = 2;
		}
	}
	else {
		if ((*arrayX - 3) >= 2) {
			//Border width = 3
			xWidth = 3;
		}
		else {
			//Border width = 1
			xWidth = 1;
		}
	}

	int maxYDistance = 0;
	int maxXDistance = 0;
	switch (yWidth)
	{
	case 1:
		maxYDistance = aryMidPointMain[0];
		break;
	case 2:
		maxYDistance = aryMidPointMain[1];
		break;
	case 3:
		maxYDistance = aryMidPointMain[0] + 1;
		break;
	case 4:
		maxYDistance = aryMidPointMain[1] + 1;
		break;
	default:
		break;
	}
	switch (xWidth)
	{
	case 1:
		maxXDistance = aryMidPointMain[2];
		break;
	case 2:
		maxXDistance = aryMidPointMain[3];
		break;
	case 3:
		maxXDistance = aryMidPointMain[2] + 1;
		break;
	case 4:
		maxXDistance = aryMidPointMain[3] + 1;
		break;
	default:
		break;
	}
	return maxSteps = maxYDistance + maxXDistance;
}

//Set number of spawned objects
void setObjectNumbers(int* pitNumber, int* wumpusNumber, int* goldNumber, int* arrayX, int* arrayY) {
	char option = '0';
	int number = 0;

	int possibleObjects = (*arrayX * *arrayY) - (2 * (calcMaxSteps(arrayX, arrayY) + 2));
	//cout << endl << "MaxSteps from main(): " << calcMaxSteps(arrayX, arrayY) << endl;

	cout << endl << "--- Set number of spawned objects ---" << endl
		<< "1: Pits   - " << *pitNumber << endl
		<< "2: Wumpus - " << *wumpusNumber << endl
		<< "3: Gold   - " << *goldNumber << endl
		<< "4: Exit     " << endl
		<< "Current   - " << (*pitNumber + *wumpusNumber + *goldNumber) << " / " << possibleObjects << endl
		<< "Choice: ";
	cin >> option;
	cout << endl;


	switch (option)
	{
	case '1':
		cout << "Number of Pits: ";
		while (true)
		{
			cin >> number;
			if (!cin)
			{
				cout << "Invalid input - input-data-type: integer!" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else break;
		}
		if (number < 0) {
			cout << "Invalid number, boundaries starting at 0" << endl;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else if ((number + *wumpusNumber + *goldNumber) > possibleObjects) {
			cout << endl << "Too many objects on this size of field. Please use maximal " << possibleObjects << " objects." << endl;
			*pitNumber = number;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else *pitNumber = number;
		break;
	case '2':
		cout << "Number of Wumpus: ";
		while (true)
		{
			cin >> number;
			if (!cin)
			{
				cout << "Invalid input - input-data-type: integer!" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else break;
		}
		if (number < 0) {
			cout << "Invalid number, boundaries starting at 0" << endl;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else if ((number + *pitNumber + *goldNumber) > possibleObjects) {
			cout << endl << "Too many objects on this size of field. Please use maximal " << possibleObjects << " objects." << endl;
			*wumpusNumber = number;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else *wumpusNumber = number;
		break;
	case '3':
		cout << "Number of Gold: ";
		while (true)
		{
			cin >> number;
			if (!cin)
			{
				cout << "Invalid input - input-data-type: integer!" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else break;
		}
		if (number < 0) {
			cout << "Invalid number, boundaries starting at 0" << endl;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else if ((number + *wumpusNumber + *pitNumber) > possibleObjects) {
			cout << endl << "Too many objects on this size of field. Please use maximal " << possibleObjects << " objects." << endl;
			*goldNumber = number;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		else *goldNumber = number;
		break;
	case '4': 
		if ((number + *wumpusNumber + *pitNumber) > possibleObjects) {
			cout << endl << "Too many objects on this size of field. Please use maximal " << possibleObjects << " objects." << endl;
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		}
		break;
	default:
		cout << "Please select a valid option!" << endl;
		setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
		break;
	}
}
//Set visibility of obj symbols
bool setVisObjSym() {
	int option = 0;

	cout << endl << "--- Set visibility of object symbols ---" << endl
		<< "1: Show" << endl
		<< "2: Hide" << endl
		<< "Choice: ";

	while (true)
	{
		cin >> option;
		if (!cin)
		{
			cout << "Invalid input - input-data-type: integer!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else break;
	}

	cout << endl;

	switch (option)
	{
	case 1: return true;
		break;
	case 2: return false;
		break;
	default:
		cout << "Please select a valid option!" << endl;
		setVisObjSym();
		break;
	}
}
//Set visibility of obj symbols
bool setVisEftSym() {
	int option = 0;

	cout << endl << "--- Set visibility of effect symbols ---" << endl
		<< "1: Show" << endl
		<< "2: Hide" << endl
		<< "Choice: "; 

	while (true)
	{
		cin >> option;
		if (!cin)
		{
			cout << "Invalid input - input-data-type: integer!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else break;
	}

	cout << endl;

	switch (option)
	{
	case 1: return true;
		break;
	case 2: return false;
		break;
	default:
		cout << "Please select a valid option!" << endl;
		setVisEftSym();
		break;
	}
}
//Set visibility of obj symbols
bool setVisKI() {
	int option = 0;

	cout << endl << "--- Set visibility of KI player ---" << endl
		<< "1: Show" << endl
		<< "2: Hide" << endl
		<< "Choice: ";

	while (true)
	{
		cin >> option;
		if (!cin)
		{
			cout << "Invalid input - input-data-type: integer!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else break;
	}

	cout << endl;

	switch (option)
	{
	case 1: return true;
		break;
	case 2: return false;
		break;
	default:
		cout << "Please select a valid option!" << endl;
		setVisKI();
		break;
	}
}
//Set field size
void setFieldSize(int* pitNumber, int* wumpusNumber, int* goldNumber, int* parrayX, int* parrayY) {
	int arrayX = 5; //default 5x5
	int arrayY = 5;

	cout << endl;
	cout << "Array Size X: ";
	while (true)
	{
		cin >> arrayX;
		if (!cin)
		{
			cout << "Invalid input - Please select a valid option (4-...): ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else break;
	}
	if (arrayX > 3) {
		cout << "Array Size Y: ";
		while (true)
		{
			cin >> arrayY;
			if (!cin)
			{
				cout << "Invalid input - Please select a valid option (4-...): ";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else break;
		}
		if (arrayY <= 3) {
			cout << "Value must be greater than 3" << endl;
			setFieldSize(pitNumber, wumpusNumber, goldNumber, parrayX, parrayY);
		}
		else {
			if (*parrayY > arrayY || *parrayX > arrayX) {
				*parrayX = arrayX;
				*parrayY = arrayY;
				cout << endl << "Because you set a smaller field size, you have to adjust the number of spawned objects!" << endl;
				setObjectNumbers(pitNumber, wumpusNumber, goldNumber, parrayX, parrayY);
			}
			else {
				*parrayX = arrayX;
				*parrayY = arrayY;
			}
			cout << endl;
		}
	}
	else {
		cout << "Value must be greater than 3" << endl;
		setFieldSize(pitNumber, wumpusNumber, goldNumber, parrayX, parrayY);
	}
}
//Game Settings Menu
void gameSettings(int* pitNumber, int* wumpusNumber, int* goldNumber, bool* visiblityObjectSymbols, bool* visiblityEffectSymbols, bool* visiblityKI, int* arrayX, int* arrayY) {
	bool inMenu = true;

	while (inMenu) {
		int intoption = 0;

		cout << endl << "--- Game settings ---" << endl
			<< "1: Number of spawned objects" << endl
			<< "2: Hide / Show object symbols (def. hide)" << endl
			<< "3: Show / Hide effect symbols (def. show)" << endl
			<< "4: Hide / Show KI player (def. hide)" << endl
			<< "5: Set field size" << endl
			<< "6: Exit settings menu" << endl
			<< "Choice: ";

		while (true)
		{
			cin >> intoption;
			if (!cin || intoption < 1 || intoption > 6)
			{
				cout << endl << "Invalid input - Please select a valid option (1-6): ";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			else break;
		}

		switch (intoption)
		{
		case 1:
			setObjectNumbers(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
			break;
		case 2:
			*visiblityObjectSymbols = setVisObjSym();
			break;
		case 3:
			*visiblityEffectSymbols = setVisEftSym();
			break;
		case 4:
			*visiblityKI = setVisKI();
			break;
		case 5:
			setFieldSize(pitNumber, wumpusNumber, goldNumber, arrayX, arrayY);
			break;
		case 6:
			inMenu = false;
			break;
		default: cout << "Please select a valid option!" << endl;
			gameSettings(pitNumber, wumpusNumber, goldNumber, visiblityObjectSymbols, visiblityEffectSymbols, visiblityKI, arrayX, arrayY);
			break;
		}
	}
}


//*** Game Functions ***
//call map and give it ary size
void initGame(map* pworld, int arrayX, int arrayY) {
	pworld->setArraySize(arrayY, arrayX); //ary size set and generation of ary in map.cpp
    pworld->drawMap(0); //Cast map out
}
//cast out info messages below map
void castMessage(int message) {
	cout << endl;
	cout << "... * --- * ... * --- * INFO * --- * ... * --- * ..." << endl;
		
	switch (message)
	{
	case 1:
		cout << "                Other object in your way!" << endl;
		break;
	case 2:
		cout << "                 Wall in your way!" << endl;
		break;
	case 6:
	    cout << "            You are not on a gold field!" << endl;
		break;
	case 7:
		cout << "      Your opponent died - that's your chance!" << endl;
		break;
	case 8:
		cout << "      Your opponent found gold - you loose!" << endl;
	break;
	}

	cout << "... * --- * ... * --- * INFO * --- * ... * --- * ..." << endl;
}
//cast out game over screen
void gameOver(int* pwonReason) {
	if (*pwonReason == 5) {
		cout << endl
			<< " - * - * - Congratulations - * - * - " << endl
			<< " - * - * -     You won!    - * - * - " << endl
			<< " - * - * - * - * - - * - * - * - * - " << endl;
	}
	else if (*pwonReason == 3) {
		cout << endl
			<< " - * - * - * - * Game  Over * - * - * - * - " << endl
			<< " - * - * -  You fell into a pit!  - * - * - " << endl
			<< " - * - * - * - * - * -- * - * - * - * - * - " << endl;
	}
	else if (*pwonReason == 4) {
		cout << endl
			<< " - * - * - * - * - *  Game Over  * - * - * - * - * - " << endl
			<< " - * - * -  You were eaten by the Wumpus!  - * - * - " << endl
			<< " - * - * - * - * - * - * * - * - * - * - * - * - * - " << endl;
	}
	else if (*pwonReason == 6) {
		cout << endl
			<< " - * - * - * - * - *  Game  Over  * - * - * - * - * - " << endl
			<< " - * - * - * - * The KI won the game! * - * - * - * - " << endl
			<< " - * - * - * - * - * - * -  - * - * - * - * - * - * - " << endl;
	}
}
//asks to play again
bool playAgain() {
	char option;

	cout << endl
		<< "--- Play again? ---" << endl
		<< "---    y / n    ---" << endl
		<< "Choice: ";
	cin >> option;

	if (option == 'y' || option == 'Y') {
		return true;
	}
	else if (option == 'n' || option == 'N') {
		return false;
	}
	else {
		cout << endl << "Invalid option!" << endl;
		playAgain();
	}
}

//*** Agent Functions ***
int agentMove(agentUser* pagentUser, agentKI* pagentKI, map* pworld, char direction, int* wonReason) {
	int xPos = pagentUser->getX(); //get agents coordinates
	int yPos = pagentUser->getY();
	int setPosRetVal = 5;

	switch (direction)
	{
	case 'l': setPosRetVal = pagentUser->setPos(yPos, xPos - 1); //send agent new coordinates and let him reset its position in ary in map class
		break;
	case 'r': setPosRetVal = pagentUser->setPos(yPos, xPos + 1);
		break;
	case 'u': setPosRetVal = pagentUser->setPos(yPos - 1, xPos);
		break;
	case 'd': setPosRetVal = pagentUser->setPos(yPos + 1, xPos);
		break;
	case 'e': setPosRetVal = pagentUser->pickUpGold();
		break;
	default:
		break;
	}

	if (setPosRetVal == 0) { //if User did not die or win, move KI
		int KIReturn = -1;
		KIReturn = pagentKI->decideMove(); 
		switch (KIReturn)
		{
		case 3:
			setPosRetVal = 7; //KI fell into Pit
			break;
		case 4:
			setPosRetVal = 7; //KI killed by Wumpus
			break;
		case 5:
			setPosRetVal = 8; //KI won
			break;
		}
	}
	
	pworld->drawMap(setPosRetVal); //update map

	switch (setPosRetVal)
	{
	case 0: //Nothing or Gold on field
		break;
	case 1: //Obeject on field
		castMessage(1);
		return 0;
	case 2: //At Border
		castMessage(2); 
		return 0;
	case 3: //Pit on Field
		*wonReason = 3;
		return 1;
	case 4: //Wumpus on Field
		*wonReason = 4;
		return 1;
	case 5: //Gold picked up
		*wonReason = 5;
		return 1;
	case 6: //Tried gold pickup, but not on gold field
		castMessage(6);
		return 0;
	case 7: //KI died
		castMessage(7);
		return 0;
	case 8: //KI won
		castMessage(8);
		*wonReason = 6;
		return 1;
	default:
		cout << endl << "agentMove() - other error!";
		return 0;
	}
}

//*** Main Loop ***
bool programLoop() {
	//***** Variables *****
	char option, preoption;
	int preintoption = 0;
	//Game running? - Vars
	bool ongoing = true;
	int wonReason = 0;
	int* pwonReason = &wonReason;
	//Game Settings
	int arrayX = 5;
	int* parrayX = &arrayX;
	int arrayY = 5;
	int* parrayY = &arrayY;
	int pitNumber = 4;
	int* ppitNum = &pitNumber;
	int wumpusNumber = 1;
	int* pwumpusNum = &wumpusNumber;
	int goldNumber = 1;
	int* pgoldNum = &goldNumber;
	bool visiblityObjectSymbols = false;
	bool* pvisObjSym = &visiblityObjectSymbols;
	bool visiblityEffectSymbols = true;
	bool* pvisEftSym = &visiblityEffectSymbols;
	bool visiblityKI = false;
	bool* pvisKI = &visiblityKI;

	//Menu to modify game options before map is generated
	cout << endl << endl
		<< "--- Modify game settings ---" << endl
		<< "            y / n           " << endl
		<< "Choice: ";

	while (true)
	{
		cin >> option;
		if (option == 'y' || option == 'n' || option == 'Y' || option == 'N')
		{
			if (option == 'y' || option == 'Y') gameSettings(ppitNum, pwumpusNum, pgoldNum, pvisObjSym, pvisEftSym, pvisKI, parrayX, parrayY);
			break;
		}
		else {
			cout << endl << "Please select a valid option (y/n): ";
			continue;
		}
	}

	//***** Objects *****
	//Agent
	agentUser agentUser1; //Generate player agent
	agentUser* pagentUser = &agentUser1;
	agentKI agentKI1; //Generate player agent
	agentKI* pagentKI = &agentKI1;
	//Map
	map world(pitNumber, wumpusNumber, goldNumber, pagentUser, pagentKI, *pvisObjSym, *pvisEftSym, *pvisKI); //Generate new map
	map* pworld = &world;

	//***** Code *****
	initGame(pworld, arrayX, arrayY); //set ary size and first cast out

	while (ongoing) {
		cout << endl
			 << "~~~ Wumpus - by Sploit ~~~" << endl
			 << "     1: Exit program" << endl
		     << "     a: <-- Agent" << endl
			 << "     d: --> Agent" << endl
		     << "     w: UP Agent" << endl
			 << "     s: DOWN Agent" << endl
			 << "     e: Pick up Gold" << endl;

		cin >> option;

		switch (option)
		{
		case '1': ongoing = false;
			break;
		case 'a': if (agentMove(pagentUser, pagentKI, pworld, 'l', pwonReason) == 1) ongoing = false;
			break;
		case 'd': if (agentMove(pagentUser, pagentKI, pworld, 'r', pwonReason) == 1) ongoing = false;
			break;
		case 'w': if (agentMove(pagentUser, pagentKI, pworld, 'u', pwonReason) == 1) ongoing = false;
			break;
		case 's': if (agentMove(pagentUser, pagentKI, pworld, 'd', pwonReason) == 1) ongoing = false;
			break;
		case 'e': if (agentMove(pagentUser, pagentKI, pworld, 'e', pwonReason) == 1) ongoing = false;
			break;
		default:
			pworld->drawMap(0);
			cout << endl << "Please select a valid option!";
			break;
		}

		cout << endl;

		if (!ongoing) { //Check wheter game is over and if so exec end screen with reason parameter
			gameOver(pwonReason);
		}
	}

	if (playAgain()) return true;
	else return false;
}

int main()
{
	bool playAgain = true;

	while (playAgain) {
		playAgain = programLoop();
	}

	return 0;
}