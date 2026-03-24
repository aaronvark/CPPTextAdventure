#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

#include "Utility.hpp"
#include "Classes.hpp"
#include "Strings.hpp"

using namespace std;

// Core loop functions
void	initialize();
void	display();

// Global gameplay functions
bool	process(string command);
int		go(string direction);
string	drop(string command);
string	pickup(string command);
void	examine(string command);
void	action(string secondword);
bool	isDirectionAvailable(string direction);
void	showdirections();
bool	performAction(Action act);
void	fireEvent(string evtName);
void	useobject(string secondword);
int		endgame();

// Persistent game data
map<int, unique_ptr<Location>>		locMap;
map<string, unique_ptr<Object>>		objMap;
set<string>							events;	// that have occurred
map<string, vector<Action>>			eventActions;

set<string>							validDirections{ Strings.NORTH, Strings.EAST, Strings.SOUTH, Strings.WEST };
map<string, int>					directionOffset; // calculated based on mapWidth & mapHeight

// Function map for more readable / extensible code down the line
bool handleGo( vector<string> args );
bool handleUse(vector<string> args );
bool handlePickup(vector<string> args);
bool handleDrop(vector<string> args);
bool handleExamine(vector<string> args);
bool handleInventory(vector<string> args);
bool handleHelp(vector<string> args);
bool handleDie(vector<string> args);
bool handleQuit(vector<string> args);
map < string, function<bool(vector<string>&)>> featureFuncs {
	{ Strings.GO,			handleGo },
	{ Strings.USE,			handleUse },
	{ Strings.PICKUP,		handlePickup },
	{ Strings.DROP,			handleDrop },
	{ Strings.EXAMINE,		handleExamine },
	{ Strings.INVENTORY,	handleInventory },
	{ Strings.HELP,			handleHelp },
	{ Strings.DIE,			handleDie },
	{ Strings.QUIT,			handleQuit }
};

// Gameplay variables
int				location;
int				targetLocation;
Inventory		inventory;
string			introduction;
string			ending;
unsigned int	minLoc = 0;
unsigned int	maxLoc = 16; // default, gets set when you load events.ini
unsigned int	mapWidth, mapHeight;

int main()
{	
	// loop variables
	bool playing = true;
	string input;

	initialize();

	// print the introduction
	cout << introduction;
	cin.get();

	// core loop
	while (playing) {
		system("cls");
		display();
		showdirections();
		getline(cin, input);
		playing = process(input);
	}

	return 0;
}

void parseMap(string url, int maxLocations = 64) {
	auto callback = [](int locID, string locIDStr, mINI::INIStructure ini) {
		string dirs = ini[locIDStr][Strings.DIRECTIONS];
		string desc = ini[locIDStr][Strings.DESCRIPTION];
		string acts = ini[locIDStr][Strings.ACTIONS];
		string req = ini[locIDStr][Strings.REQUIRED_ITEM];

		vector<string> objStrings = parseArguments(ini[locIDStr][Strings.OBJECTS]);
		vector<Object*> objects;
		for (auto objStr : objStrings) {
			if (objMap.contains(objStr))
				objects.push_back(objMap[objStr].get());
		}

		// make_unique is used here to be able to reference the locations consistently throughout the game session
		locMap.insert_or_assign(locID, make_unique<Location>(dirs, objects, desc, req, acts));
	};

	parseIni(url, callback, maxLocations);
}

void parseObjects(string url, int maxObjects = 64) {
	auto callback = [](int objID, string objString, mINI::INIStructure ini) {
		string name = ini[objString][Strings.NAME];
		string examine = ini[objString][Strings.EXAMINE];
		string action = ini[objString][Strings.ACTIONS];
		bool pickup = ini[objString][Strings.PICKUP] == Strings.TRUE;
		string pickupDescription = ini[objString][Strings.PICKUP_DESCRIPTION];
		string locationDescription = ini[objString][Strings.LOCATION_DESCRIPTION];
		string pickupActions = ini[objString][Strings.PICKUP_ACTIONS];

		// These might be multi-line
		replace(examine.begin(), examine.end(), '_', '\n');
		replace(pickupDescription.begin(), pickupDescription.end(), '_', '\n');

		std::vector<Action> examineActionList = action != "" ? Action::Parse(action) : Action::Parse("");
		std::vector<Action> pickupActionList = pickupActions != "" ? Action::Parse(pickupActions) : Action::Parse("");

		// make_unique is used here to be able to reference the objects consistently throughout the game session
		objMap.insert_or_assign(name, make_unique<Object>(objID, name, examine, pickup, pickupDescription, locationDescription, examineActionList, pickupActionList));
	};

	parseIni(url, callback, maxObjects);
}

void parseEvents(string url) {
	mINI::INIFile file(url);
	mINI::INIStructure ini;
	file.read(ini);

	mapWidth = stoi(ini[Strings.MAP_DATA][Strings.WIDTH]);
	mapHeight = stoi(ini[Strings.MAP_DATA][Strings.HEIGHT]);
	maxLoc = mapWidth * mapHeight;
	location = stoi(ini[Strings.MAP_DATA][Strings.STARTING_LOCATION]);
	
	vector<string> startingInventory = parseArguments(ini[Strings.MAP_DATA][Strings.STARTING_LOCATION]);
	for (auto item : startingInventory) {
		if (objMap.contains(item))
			inventory.Place(objMap[item].get());
	}

	introduction = ini[Strings.INTRODUCTION][Strings.OUTPUT];
	targetLocation = stoi(ini[Strings.ENDING][Strings.TARGET_LOCATION]);
	ending = ini[Strings.ENDING][Strings.OUTPUT];

	// These might be multi-line
	replace(introduction.begin(), introduction.end(), '_', '\n');
	replace(ending.begin(), ending.end(), '_', '\n');

	int ID = 1;
	while (ID < 64) {
		string IDString = to_string(ID);
		if (ini.has(IDString)) {
			eventActions.insert_or_assign(ini[IDString][Strings.NAME], Action::Parse(ini[IDString][Strings.ACTIONS]));
		}
		ID++;
	}
}

void initialize() {
	LoadStringsFromIni("english.ini");

	parseObjects("objects.ini");
	parseMap("map.ini");
	parseEvents("events.ini");

	directionOffset = std::map<string, int>
	{
		{ Strings.NORTH, mapWidth },
		{ Strings.SOUTH, - (int)mapWidth},
		{ Strings.EAST, 1},
		{ Strings.WEST, -1}
	};
}

void display() {
	cout << locMap[location]->Describe() << std::endl << std::endl;
	cout << Strings.CURRENT_LOCATION << location << std::endl;
}

int go(string direction) {
	bool dirAvailable = isDirectionAvailable(direction);
	bool isValidDirection = validDirections.contains(direction);

	if (!isValidDirection) {
		cout << Strings.DICTIONARY_SNARK;
		cin.get();
		return location;
	}
	else if (!dirAvailable) {
		cout << Strings.START_MOVING << direction << Strings.MOVE_FAILURE_JOKE;
		cin.get();
		return location;
	}
	else
	{
		// valid direction input
		location += directionOffset[direction];
		return location;
	}
}

bool process(string command) {
	vector<string> args = parseArguments(command);

	if (args.size() == 0) return true;
	
	string firstword = args[0];
	if (featureFuncs.contains(firstword)) {
		return featureFuncs[firstword](args);
	}
	cout << Strings.UNKNOWN_COMMAND << std::endl << std::endl;
	return true;
}

bool handleGo(vector<string> args)
{
	if (args.size() < 2) return false;

	int oldloc = location;
	int newloc = go(args[1]);

	// check if we reach the target location
	if (newloc == targetLocation) {
		endgame();
		newloc = -10;
		return false;
	}

	if (newloc > maxLoc || newloc < minLoc) {
		cout << Strings.INVALID_GO_SNARK;
		cin.get();
		return true;
	}
	else if (newloc == oldloc)
		return true;
	else {
		location = newloc;
		return true;
	}
}

bool handleUse(vector<string> args)
{
	if (args.size() < 2) return true;
	useobject(args[1]);
	cin.get();
	return true;
}

bool handleDrop(vector<string> args)
{
	if (args.size() < 2) return true;
	string reaction = drop(args[1]);
	cout << reaction << std::endl << std::endl;
	cin.get();
	return true;
}

bool handlePickup(vector<string> args)
{
	if (args.size() < 2) return true;
	string reaction = pickup(args[1]);
	cout << reaction << std::endl << std::endl;
	cin.get();
	return true;
}

bool handleExamine(vector<string> args)
{
	if (args.size() < 2) return true;
	examine(args[1]);
	cin.get();
	return true;
}

bool handleInventory(vector<string> args)
{
	inventory.Display();
	cin.get();
	return true;
}

bool handleHelp(vector<string> args)
{
	cout << Strings.HELP_TEXT;
	cin.get();
	return true;
}

bool handleDie(vector<string> args)
{
	cout << Strings.DIE_RESPONSE;
	cin.get();
	return false;
}

bool handleQuit(vector<string> args)
{
	cout << Strings.ANY_KEY_QUIT;
	cin.get();
	return false;
}

string pickup(string objectString) {
	int achieved = 0;
	bool success = false;
	string reaction;

	// Loop through objects in current location
	for (int i = 0; i < locMap[location]->objects.size(); i++) {
		if (objectString == locMap[location]->objects[i]->name) {
			achieved = 1;

			if (objMap[objectString]->pickup) {
				success = inventory.Place(objMap[objectString].get());
				if (success) {
					locMap[location]->Remove(objMap[objectString].get());
				}
			}
			else { cout << objMap[objectString]->pickupdescr; }
		}
	}

	// Parse reaction
	if (achieved == 0) {
		reaction = Strings.INVALID_OBJECT;
	}
	else {
		if (!success) {
			reaction = Strings.FULL_FAIL_PRE + objectString + Strings.FULL_FAIL_POST;
		}
		else {
			reaction = Strings.ADD_SUCCESS_PRE + objectString + Strings.ADD_SUCCESS_POST;
		}

		// Pickup actions are triggered regardless of whether or not we can (successfully) pick it up
		for (auto act : objMap[objectString]->pickupActions) {
			performAction(act);
		}
	}

	return reaction;
}

string drop(string objectString) {
	int success = inventory.Remove(objMap[objectString].get());
	string reaction;
	if (success == 1) {
		reaction = Strings.REM_SUCCESS_PRE + objectString + Strings.REM_SUCCESS_POST;
		locMap[location]->Place(objMap[objectString].get());
		return(reaction);
	}
	else {
		reaction = Strings.REM_FAIL_PRE + objectString + Strings.REM_FAIL_POST;
		return(reaction);
	}
}

void examine(string objectString) {
	bool objectPresent = locMap[location]->Contains(objectString);
	if (!objectPresent) {
		cout << Strings.EXAMINE_FAIL;
	}
	else {
		cout << objMap[objectString]->examine << std::endl << std::endl;
		action(objectString);
	}
}

void action(string secondword) {
	for (auto action : objMap[secondword]->actions)
	{
		if ( !performAction(action) ) break;
	}
}

bool performAction( Action act )
{
	switch (act.type)
	{
	case AT_ADD_DIRECTION:
		if (!locMap[location]->directions.contains(act.data[0])) {
			locMap[location]->directions.insert(act.data[0]);
			cout << Strings.NEW_EXIT + act.data[0];
		}
		return true;
	case AT_EVENT:
		fireEvent(act.data[0]);
		return true;
	case AT_EDIT_OBJECT:
		objMap[act.data[0]]->Update(act.data[1], act.data[2]);
		return true;
	case AT_EDIT_INVENTORY:
		if (objMap.contains(act.data[1])) {
			bool res = inventory.Parse(act.data[0], objMap[act.data[1]].get());
			// if (!res) cout << "DEBUG: Invalid inventory action: " << act.data[0] << " , " << act.data[1] << std::endl << std::endl;
		}
		return true;
	case AT_OUTPUT:
		// print to cout
		{
			string toOutput = act.data[0];
			replace(toOutput.begin(), toOutput.end(), '_', '\n');
			cout << act.data[0] << std::endl << std::endl;
		}
		return true;
	case AT_PLACE:
		locMap[location]->objects.push_back(objMap[act.data[0]].get());
		return true;
	case AT_INVENTORY_CONTAINS:
		return inventory.Contains(act.data[0]);	// this is the only action that can fail currently
	case AT_EDIT_LOCATION:
		// if there is an additional datapoint, then we're editing another location
		if (act.data.size() > 2) {
			int locID = stoi(act.data[0]);
			locMap[locID]->Update(act.data[1], act.data[2]);
		}
		else 
			locMap[location]->Update(act.data[0], act.data[1]);
		return true;
	case AT_DESTROY_OBJECT:
		// Remove from inventory & current location
		if ( inventory.Contains(act.data[0]) )
			inventory.Remove(objMap[act.data[0]].get());
		if ( locMap[location]->Contains(act.data[0]) )
			locMap[location]->Remove(objMap[act.data[0]].get());

		// Destroy from objMap
		objMap.erase(act.data[0]);
		break;
	}

	return false;
}

void useobject(string secondword) {
	bool isPresent = locMap[location]->Contains(secondword);
	bool hasItem = inventory.Contains(secondword);

	// Hardcoded responses
	if (hasItem) {
		if (locMap[location]->requiredItem == secondword) {
			bool success = true;
			for (auto act : locMap[location]->actions) {
				if (!performAction(act)) {
					success = false;
					break;
				}
			}
			if (!success) {
				cout << Strings.FEELS_RIGHT_BUT;
			}
			return;
		}
	}

	// Generic responses
	if (isPresent && !hasItem) {
		cout << Strings.NOT_USEFUL;
		return;
	}
	if (objMap.contains(secondword) && !hasItem) {
		cout << Strings.WHAT_PRE << secondword << Strings.QMARK;
		return;
	}
	else {
		// Fallback snark
		cout << Strings.DELUSIONAL;
	}
}

bool isDirectionAvailable(string direction) {
	return locMap[location]->directions.contains(direction);
}

void showdirections() {
	if (locMap[location]->directions.size() == 0) {
		cout << Strings.NO_EXITS << std::endl << std::endl;
		return;
	}

	cout << Strings.POSSIBLE_EXITS;
	for (auto dir : locMap[location]->directions) {
		cout << dir << " ";
	}
	cout << std::endl;
}

void fireEvent(string evtName) {
	events.insert(evtName);
	if (eventActions.contains(evtName)) {
		for (auto act : eventActions[evtName]) {
			performAction(act);
		}
	}
}

int endgame() {
	replace(ending.begin(), ending.end(), '_', '\n');

	cout << ending;
	cin.get();
	location = -10;
	return location;
}