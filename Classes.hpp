#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

#include "Strings.hpp"

enum ActionType
{
	AT_NO_ACTION,
	AT_ADD_DIRECTION,
	AT_OUTPUT,
	AT_EDIT_OBJECT,
	AT_EVENT,
	AT_EDIT_INVENTORY,
	AT_PLACE,
	AT_INVENTORY_CONTAINS,
	AT_EDIT_LOCATION,
	AT_DESTROY_OBJECT,
	AT_NUM_TYPES
};


class Action {
public:
	static inline std::map<std::string, ActionType> actionTypeFromString = std::map<std::string, ActionType> {
		{ Strings.EXIT, AT_ADD_DIRECTION },
		{ Strings.OUTPUT, AT_OUTPUT},
		{ Strings.EDIT_OBJECT, AT_EDIT_OBJECT},
		{ Strings.EVENT, AT_EVENT},
		{ Strings.EDIT_INVENTORY, AT_EDIT_INVENTORY},
		{ Strings.PLACE, AT_PLACE},
		{ Strings.INVENTORY_CONTAINS, AT_INVENTORY_CONTAINS },
		{ Strings.EDIT_LOCATION, AT_EDIT_LOCATION },
		{ Strings.DESTROY_OBJECT, AT_DESTROY_OBJECT }
	};

	ActionType type = AT_ADD_DIRECTION;
	std::vector<std::string> data;

	static std::vector<Action> Parse(std::string actionList) {
		std::vector<Action> aList;

		// split ;
		std::vector<std::string> actionStrings = parseArguments(actionList, ';');

		// split :
		for (auto act : actionStrings) {
			std::vector<std::string> args = parseArguments(act, ':');
			if (args.size() <= 1) continue;	// each action should contain at least two parts...

			// first is type, rest is data
			if (actionTypeFromString.contains(args[0])) {	
				Action a;
				a.type = actionTypeFromString[args[0]];
				args.erase(args.begin());
				a.data = args;
				aList.push_back(a);
			}
		}

		return aList;
	}
};

class Object {
public:
	Object() {}
	Object(int id, std::string name, std::string examine, bool pickup, std::string pickupDescr, std::string locationDescr, std::vector<Action> actions, std::vector<Action> pickupActions)
	{
		this->id = id;
		this->name = name;
		this->examine = examine;
		this->pickup = pickup;
		this->pickupdescr = pickupDescr;
		this->locationdescr = locationDescr;
		this->actions = actions;
		this->pickupActions = pickupActions;
	}
	
	~Object() {}
	
	void Update(std::string property, std::string value) {
		if (propertyMap.contains(property)) {
			propertyMap[property](value);
		}
	}

	int id = -1;
	std::string name = "";
	std::string examine = "";
	std::vector<Action> actions;
	std::vector<Action> pickupActions;
	bool pickup = false;
	std::string pickupdescr = "";
	std::string locationdescr = "";

	std::map<std::string, std::function<void(const std::string&)>> propertyMap =
	{
		{ Strings.NAME,						[this](const std::string& v) { name = v; } },
		{ Strings.EXAMINE,					[this](const std::string& v) { examine = v; } },
		{ Strings.PICKUP,					[this](const std::string& v) { pickup = (v == Strings.TRUE || v == Strings.ONE); } },
		{ Strings.PICKUP_DESCRIPTION,		[this](const std::string& v) { pickupdescr = v; } },
		{ Strings.ACTIONS,					[this](const std::string& v) { /* parse and push to actions */ } },	// unsupported atm... but could be fascinating!
		{ Strings.LOCATION_DESCRIPTION,		[this](const std::string& v) { locationdescr = v; } },
		{ Strings.PICKUP_ACTIONS,			[this](const std::string& v) { /* parse and push to actions */ } }	// unsupported atm... but could be fascinating!
	};
};

class Location {
public:
	Location() {}
	
	Location(std::string directions, std::vector<Object*> objects, std::string description, std::string requiredItem = "", std::string action = "") {
		std::vector<std::string> args = parseArguments(directions);
		for (auto arg : args) this->directions.insert(arg);
		this->objects = objects;
		this->description = description;
		this->requiredItem = requiredItem;
		this->actions = Action::Parse(action);
	}

	~Location() {}

	std::string Describe() {
		std::string out = description;
		for (auto obj : objects) {
			if (obj->pickup && obj->locationdescr.length() > 0) {
				out += " " + obj->locationdescr;
			}
		}
		return out;
	}

	void Update(std::string property, std::string value) {
		if (propertyMap.contains(property)) {
			propertyMap[property](value);
		}
	}

	bool Remove(Object * obj)
	{
		objects.erase(find(objects.begin(), objects.end(), obj));
		return true;
	}
	
	void Place(Object * obj)
	{
		if (obj == nullptr) {
			int x = 0;
		}
		objects.push_back(obj);
	}

	bool Contains(std::string objStr)
	{
		for (auto obj : objects) {
			if (obj->name == objStr) return true;
		}
		return false;
	}
	
	std::vector<Object *> objects;
	std::set<std::string> directions;
	std::string description;
	std::string requiredItem;
	std::vector<Action> actions;

	std::map<std::string, std::function<void(const std::string&)>> propertyMap =
	{
		{ Strings.OBJECTS,			[this](const std::string& v) { /* parse and push to objects */  } }, //unsupported
		{ Strings.DIRECTIONS,		[this](const std::string& v) { directions.insert(v); }},
		{ Strings.DESCRIPTION,		[this](const std::string& v) { description = v; } },
		{ Strings.REQUIRED_ITEM,	[this](const std::string& v) { requiredItem = v; } },
		{ Strings.ACTIONS,			[this](const std::string& v) { /* parse and push to actions */ } }, //unsupported
	};
};

class Inventory {
public:
	Inventory() {}
	~Inventory() {}

	std::map<std::string, std::function<bool(Object* object)>> actionMap =
	{
		{ Strings.REMOVE,	[this](Object* object) { return Remove(object); } },
		{ Strings.ADD,		[this](Object* object) { return Place(object); } }
	};

	bool Parse(std::string action, Object * object) {
		if (actionMap.contains(action)) {
			return actionMap[action](object);
		}

		return false;
	}
	
	bool Remove(Object * obj) {
		bool found = false;
		for (auto localObj : objects) {
			if (localObj->name == obj->name) {
				found = true;
				break;
			}
		}
		objects.erase(std::find(objects.begin(), objects.end(), obj));
		return found;	// assuming the removal doesn't cause errors here...
	}

	bool Place(Object * obj) {
		objects.push_back(obj);
		return true;
	}

	void Display() {
		std::cout << Strings.INVENTORY_OPEN;
		for (auto obj : objects) {
			std::cout << Strings.INVENTORY_LIST << obj->name << Strings.DOT << std::endl;
		}
		std::cout << std::endl;
	}
	
	bool Contains(std::string objStr)
	{
		for (auto obj : objects) {
			if (obj->name == objStr) return true;
		}
		return false;
	}

private:
	std::vector<Object *> objects;
};