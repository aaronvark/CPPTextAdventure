#pragma once

#include <string>
#include <map>
#include <iostream>

#include "Object.hpp"

class Inventory {
public:
	Inventory() {}
	~Inventory() {}

	std::map<std::string, std::function<bool(Object* object)>> actionMap =
	{
		{ Strings.REMOVE,	[this](Object* object) { return Remove(object); } },
		{ Strings.ADD,		[this](Object* object) { return Place(object); } }
	};

	bool Parse(std::string action, Object* object) {
		if (actionMap.contains(action)) {
			return actionMap[action](object);
		}

		return false;
	}

	bool Remove(Object* obj) {
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

	bool Place(Object* obj) {
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
	std::vector<Object*> objects;
};