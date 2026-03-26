#pragma once

#include <functional>
#include <string>
#include <map>

#include "Strings.hpp"
#include "Action.hpp"

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

	void Update(const std::string& property, const str_iter start, const str_iter end) {
		if (propertyMap.contains(property)) {
			propertyMap[property](start, end);
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

	std::map<std::string, std::function<void(const str_iter, const str_iter)>> propertyMap =
	{
		{ Strings.NAME,						[this](const str_iter start, const str_iter end) { name = *start; } },
		{ Strings.EXAMINE,					[this](const str_iter start, const str_iter end) { examine = *start; } },
		{ Strings.PICKUP,					[this](const str_iter start, const str_iter end) { pickup = (*start == Strings.TRUE || *start == Strings.ONE); } },
		{ Strings.PICKUP_DESCRIPTION,		[this](const str_iter start, const str_iter end) { pickupdescr = *start; } },
		{ Strings.ACTIONS,					[this](const str_iter start, const str_iter end) {
			// TODO: this is exactly the same as the Location implementation, can we share this somehow?
			// unique case: no args = wipe actions
			if (start == end) {
				actions.clear();
				return;
			}

			// build list
			std::vector<std::string> actionList;
			for (auto cur = start; cur != end; cur++) {
				actionList.push_back(*cur);
			}

			// apply list
			Action a;
			if (Action::TryParse(actionList, a)) {
				actions.clear();
				// TODO: Decide if we need to support multiple actions to be injected in this way (not sure we can with the current syntax)
				actions.push_back(a);
			}
		} },
		{ Strings.LOCATION_DESCRIPTION,		[this](const str_iter start, const str_iter end) { locationdescr = *start; } },
		{ Strings.PICKUP_ACTIONS,			[this](const str_iter start, const str_iter end) { /* parse and push to actions */ } }	// unsupported atm... but could be fascinating!
	};
};