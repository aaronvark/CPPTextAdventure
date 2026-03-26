#pragma once

#include <map>

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
	static inline std::map<std::string, ActionType> actionTypeFromString = std::map<std::string, ActionType>{
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

	static std::vector<Action> FromString(std::string actionList) {
		std::vector<Action> aList;

		// split ;
		std::vector<std::string> actionStrings = parseArguments(actionList, ';');

		// split :
		for (auto act : actionStrings) {
			std::vector<std::string> args = parseArguments(act, ':');
			if (args.size() <= 1) continue;	// each action should contain at least two parts...

			Action a;
			if (Action::TryParse(args, a)) {
				aList.push_back(a);
			}
		}

		return aList;
	}

	static bool TryParse(std::vector<std::string>& args, Action & action) {
		if (args.size() <= 1) 	// each action should contain at least two parts...
			return false;
		
		// first is type, rest is data
		if (actionTypeFromString.contains(args[0])) {
			action.type = actionTypeFromString[args[0]];
			args.erase(args.begin());
			action.data = args;
			return true;
		}
		return false;
	}
};