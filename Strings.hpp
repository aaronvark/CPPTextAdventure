#pragma once

#include <string>
#include "ini.h"

// English values are loaded by default
// TODO: Find out how to properly support referencing strings from here to the object & location descriptions
//          Right now those don't translate if you translate this stuff, and there's some things we need to remain the same (for the INI formatting)
struct Strings
{
    std::string TRUE = "true";
    std::string ONE = "1";
    std::string QMARK = "?";

    // TODO: Parse this from file, so you can translate it!

    // Help
    std::string HELP_TEXT =
        "\nLIST OF COMMANDS\n\n\
* go north/south/east/west - Head in these respective directions on the board\n\
* pickup [objectname]      - Will pick up the selected object is this is possible.\n\
* inventory                - Will show the current contents of your inventory.\n\
* examine [objectname]     - Will bring up any relevant (or irrelevant) information about the object and will place it under selection until you leave the room or select another object.\n\
* use [objectname]         - Will use the selected object.";

    // Gameplay Text
    std::string DICTIONARY_SNARK = "\nA mysterious force has kept you in place. It might have been the dictionary.";
    std::string START_MOVING = "\nYou start moving toward the ";
    std::string MOVE_FAILURE_JOKE = " but your nose disagrees with you.";
    std::string CURRENT_LOCATION = "Current Location: Grid ";
    std::string UNKNOWN_COMMAND = "Unknown command";
    std::string INVALID_GO_SNARK = "\nYou attempt to move but as soon as your skull crashes into a wall, you stop trying.";
    std::string DIE_RESPONSE = "\n*SECRET COMMAND*\n\nYou decide to see how well the human neck competes with modern day pogo-sticks, with very damaging result.\n\nYou are now dead.\n";
    std::string ANY_KEY_QUIT = "Press any key to quit...\n";
    std::string INVALID_OBJECT = "Invalid object!";
    std::string EXAMINE_FAIL = "\nNow you're really getting things mixed up here...";
    std::string NEW_EXIT = "\nNew Exit: ";
    std::string FEELS_RIGHT_BUT = "\nIt feels right, but you think you're missing something...";
    std::string NOT_USEFUL = "\nIt doesn't look particularly useful...";
    std::string WHAT_PRE = "What ";
    std::string DELUSIONAL = "\nYou must be delusional";
    std::string NO_EXITS = "No Exits";
    std::string POSSIBLE_EXITS = "Possible Exits: ";

    // Inventory
    std::string INVENTORY_OPEN = "\nYou open up your backpack and look inside to find neatly divided areas.\n\n";
    std::string INVENTORY_LIST = ". ";
    std::string DOT = ".";
    std::string ADD = "add";
    std::string REMOVE = "remove";
    std::string FULL_FAIL_PRE = "Could not place ";
    std::string FULL_FAIL_POST = " in your inventory. It is full!";
    std::string ADD_SUCCESS_PRE = "Placed ";
    std::string ADD_SUCCESS_POST = " in your inventory.";
    std::string REM_FAIL_PRE = "Could not find ";
    std::string REM_FAIL_POST = " in your inventory.";
    std::string REM_SUCCESS_PRE = "Removed (an instance of) ";
    std::string REM_SUCCESS_POST = " from your inventory.";

    // Commands
    std::string GO = "go";
    std::string USE = "use";
    std::string PICKUP = "pickup";
    std::string DROP = "drop";
    std::string EXAMINE = "examine";
    std::string INVENTORY = "inventory";
    std::string HELP = "help";
    std::string DIE = "die";
    std::string QUIT = "quit";

    // Directions
    std::string NORTH = "north";
    std::string EAST = "east";
    std::string SOUTH = "south";
    std::string WEST = "west";

    // Map Data
    std::string DIRECTIONS = "directions";
    std::string DESCRIPTION = "description";
    std::string ACTIONS = "actions";
    std::string REQUIRED_ITEM = "requireditem";
    std::string OBJECTS = "objects";

    // Object Data (duplicates left for reference)
    std::string NAME = "name";
    // const std::string EXAMINE			= "examine";	// duplicates
    // const std::string ACTIONS			= "actions";	
    // const std::string PICKUP				= "pickup";
    std::string PICKUP_DESCRIPTION = "pickupdescr";
    std::string PICKUP_ACTIONS = "pickupactions";
    std::string LOCATION_DESCRIPTION = "locationdescr";

    // Actions
    std::string EXIT = "exit";
    std::string OUTPUT = "output";
    std::string EDIT_OBJECT = "editobject";
    std::string EVENT = "event";
    std::string EDIT_INVENTORY = "editinventory";
    std::string PLACE = "place";
    std::string INVENTORY_CONTAINS = "inventorycontains";
    std::string EDIT_LOCATION = "editlocation";
    std::string DESTROY_OBJECT = "destroy";

    // Events Data
    std::string MAP_DATA = "mapdata";
    std::string WIDTH = "width";
    std::string HEIGHT = "height";
    std::string STARTING_LOCATION = "startinglocation";
    std::string TARGET_LOCATION = "targetLocation";
    std::string INTRODUCTION = "introduction";
    std::string ENDING = "ending";
} Strings;

void LoadStringsFromIni(const std::string& filePath)
{
    mINI::INIFile file(filePath);
    mINI::INIStructure ini;

    if (!file.read(ini))
    {
        // Handle read failure however suits your error strategy
        return;
    }

    auto& s = ini["STRINGS"];

    auto load = [&](const std::string& key, std::string& target)
        {
            if (s.has(key))
            {
                target = s[key];
                // Replace underscore-encoded newlines back to actual newlines
                size_t pos = 0;
                replace(target.begin(), target.end(), '_', '\n');
            }
        };

    load("TRUE", Strings.TRUE);
    load("ONE", Strings.ONE);
    load("QMARK", Strings.QMARK);
    load("HELP_TEXT", Strings.HELP_TEXT);
    load("DICTIONARY_SNARK", Strings.DICTIONARY_SNARK);
    load("START_MOVING", Strings.START_MOVING);
    load("MOVE_FAILURE_JOKE", Strings.MOVE_FAILURE_JOKE);
    load("CURRENT_LOCATION", Strings.CURRENT_LOCATION);
    load("UNKNOWN_COMMAND", Strings.UNKNOWN_COMMAND);
    load("INVALID_GO_SNARK", Strings.INVALID_GO_SNARK);
    load("DIE_RESPONSE", Strings.DIE_RESPONSE);
    load("ANY_KEY_QUIT", Strings.ANY_KEY_QUIT);
    load("INVALID_OBJECT", Strings.INVALID_OBJECT);
    load("EXAMINE_FAIL", Strings.EXAMINE_FAIL);
    load("NEW_EXIT", Strings.NEW_EXIT);
    load("FEELS_RIGHT_BUT", Strings.FEELS_RIGHT_BUT);
    load("NOT_USEFUL", Strings.NOT_USEFUL);
    load("WHAT_PRE", Strings.WHAT_PRE);
    load("DELUSIONAL", Strings.DELUSIONAL);
    load("NO_EXITS", Strings.NO_EXITS);
    load("POSSIBLE_EXITS", Strings.POSSIBLE_EXITS);
    load("INVENTORY_OPEN", Strings.INVENTORY_OPEN);
    load("INVENTORY_LIST", Strings.INVENTORY_LIST);
    load("DOT", Strings.DOT);
    load("ADD", Strings.ADD);
    load("REMOVE", Strings.REMOVE);
    load("FULL_FAIL_PRE", Strings.FULL_FAIL_PRE);
    load("FULL_FAIL_POST", Strings.FULL_FAIL_POST);
    load("ADD_SUCCESS_PRE", Strings.ADD_SUCCESS_PRE);
    load("ADD_SUCCESS_POST", Strings.ADD_SUCCESS_POST);
    load("REM_FAIL_PRE", Strings.REM_FAIL_PRE);
    load("REM_FAIL_POST", Strings.REM_FAIL_POST);
    load("REM_SUCCESS_PRE", Strings.REM_SUCCESS_PRE);
    load("REM_SUCCESS_POST", Strings.REM_SUCCESS_POST);
    load("GO", Strings.GO);
    load("USE", Strings.USE);
    load("PICKUP", Strings.PICKUP);
    load("DROP", Strings.DROP);
    load("EXAMINE", Strings.EXAMINE);
    load("INVENTORY", Strings.INVENTORY);
    load("HELP", Strings.HELP);
    load("DIE", Strings.DIE);
    load("QUIT", Strings.QUIT);
    load("NORTH", Strings.NORTH);
    load("EAST", Strings.EAST);
    load("SOUTH", Strings.SOUTH);
    load("WEST", Strings.WEST);
    load("DIRECTIONS", Strings.DIRECTIONS);
    load("DESCRIPTION", Strings.DESCRIPTION);
    load("ACTIONS", Strings.ACTIONS);
    load("REQUIRED_ITEM", Strings.REQUIRED_ITEM);
    load("OBJECTS", Strings.OBJECTS);
    load("NAME", Strings.NAME);
    load("PICKUP_DESCRIPTION", Strings.PICKUP_DESCRIPTION);
    load("PICKUP_ACTIONS", Strings.PICKUP_ACTIONS);
    load("LOCATION_DESCRIPTION", Strings.LOCATION_DESCRIPTION);
    load("EXIT", Strings.EXIT);
    load("OUTPUT", Strings.OUTPUT);
    load("EDIT_OBJECT", Strings.EDIT_OBJECT);
    load("EVENT", Strings.EVENT);
    load("EDIT_INVENTORY", Strings.EDIT_INVENTORY);
    load("PLACE", Strings.PLACE);
    load("INVENTORY_CONTAINS", Strings.INVENTORY_CONTAINS);
    load("EDIT_LOCATION", Strings.EDIT_LOCATION);
    load("DESTROY_OBJECT", Strings.DESTROY_OBJECT);
    load("MAP_DATA", Strings.MAP_DATA);
    load("WIDTH", Strings.WIDTH);
    load("HEIGHT", Strings.HEIGHT);
    load("STARTING_LOCATION", Strings.STARTING_LOCATION);
    load("TARGET_LOCATION", Strings.TARGET_LOCATION);
    load("INTRODUCTION", Strings.INTRODUCTION);
    load("ENDING", Strings.ENDING);
}