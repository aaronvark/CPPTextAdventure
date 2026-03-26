# CPP Text Adventure (engine?)

This is an old student assignment from my first forays into C++ (see original code in the "old" folder... here be dragons).
As an exercise in more modern C++ practices, I decided to refactor it and update it with some cool new features.

The core upgrade is that all of the game data & interactions are defined in INI files using the mINI single-head library. I've rather haphazardly created a syntax based on ; and : splits to define all kinds of actions and data. This still really needs a naming pass to get things to be consistent (e.g. actions used to be singular, now multiple, and sometimes there are different actions for different situations in the same places or for the same objects).

Other quirks:
 - INI files for objects & locations parse based on index, and start counting at 1 
 - Locations are mapped bottom left to top right
 - Some additional data is stored in events, which isn't a very clear name
 - Only a single item can be the "requiredItem" in a location, which means you can "use" it (again, a terrible name)
 - Due to parsing restrictions, using the editobject / editlocation action to edit the actions of an object/location only allows you to define a single (or no) action.

You can peruse the example INI files to figure out how it all works. The core idea is that you need to reach the targetLocation to trigger the ending, and this will only happen if all the requiredEvents (space separated) have been executed.

See the Action.hpp file for info on the types of actions you might try.
