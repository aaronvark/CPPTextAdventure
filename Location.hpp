#pragma once

#include <string>
#include <set>
#include <functional>

#include "Object.hpp"
#include "Action.hpp"
#include "Utility.hpp"

class Location {
public:
	Location() {}

	Location(std::string directions, std::vector<Object*> objects, std::string description, std::string requiredItem = "", std::string action = "") {
		std::vector<std::string> args = parseArguments(directions);
		for (auto arg : args) this->directions.insert(arg);
		this->objects = objects;
		this->description = description;
		this->requiredItem = requiredItem;
		this->actions = Action::FromString(action);
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

	void Update(std::string property, const str_iter start, const str_iter end) {
		if (propertyMap.contains(property)) {
			// TODO: replace with iterator implementation
			propertyMap[property](start, end);
		}
	}

	bool Remove(Object* obj)
	{
		objects.erase(find(objects.begin(), objects.end(), obj));
		return true;
	}

	void Place(Object* obj)
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

	std::vector<Object*> objects;
	std::set<std::string> directions;
	std::string description;
	std::string requiredItem;
	std::vector<Action> actions;

	std::map<std::string, std::function<void(const str_iter start, const str_iter end)>> propertyMap =
	{
		{ Strings.OBJECTS,			[this](const str_iter start, const str_iter end) { /* parse and push to objects */  } }, //unsupported
		{ Strings.DIRECTIONS,		[this](const str_iter start, const str_iter end) { 
			for (auto cur = start; cur != end; cur++) {
				directions.insert(*cur);
			}
		}},
		{ Strings.DESCRIPTION,		[this](const str_iter start, const str_iter end) { description = *start; } },	// only one supported
		{ Strings.REQUIRED_ITEM,	[this](const str_iter start, const str_iter end) { requiredItem = *start; } },	// only one supported
		// TODO: Implement action support using iterator implementation
		{ Strings.ACTIONS,			[this](const str_iter start, const str_iter end) { 
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
	};
};