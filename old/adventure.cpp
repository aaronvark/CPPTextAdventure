#include <iostream>

using namespace std;

void display();
int find(string secondword);
string getword(string input);
string process(string command);
int go(string direction);
string getfirstword(string input);
string getsecondword(string input);
void showinventory();
int inventoryplace(string secondword);
int inventoryremove(string secondword);
string drop(string command);
string pickup(string command);
void initialize();
void place(string secondword);
int remove(string secondword);
void examine(string command);
void action(string secondword);
int checkdirections(string direction);
void showdirections();
int useobject(string secondword);
int findinv(string secondword);
int findloc(string secondword);
int endgame();

class object {
    public:
        object();
        ~object();
        string name;
        string examine;
        bool pickup;
        string pickupdescr;
        int destiny;
};

object::object() {

}

object::~object() {

}

class square {
    public:
        square();
        ~square();
        int remove();
        void place();
        string objects[5];
        string directions[4];
        string description;
};

square::square() {

}

square::~square() {

}

int location;
string inventory[5];
square loc[17];
object object[20];
string directions[4];
bool plankused = false;

int main() 
{
	char direction[256];
	location = 2;
	string playing = "1";
	int newloc;
	string input;
	initialize();
	while (playing == "1") {
		system("cls");	
		display();
		showdirections();
		getline(cin, input, '\n');
		playing = process(input);
	}
	return(0);
}


void initialize() {

    //--LOCATIONS--//

    //-Loc1-//
    loc[1].directions[0] = "north";
    loc[1].description = "As you walk down the dark forest path, you see a boarded up door to the west. At the bottom there is a board missing, which lets through a chilling draft.\n\n";

    //-Loc2-//
    loc[2].objects[0] = "bed";
    loc[2].objects[1] = "closet";
    loc[2].objects[2] = "man";
    loc[2].objects[3] = "key";
    loc[2].description = "A bed to your left seems to have been slept in. To your right stands a closet. In front of you stands a fuzzy man that seems to keep talking incessantly.\n\n";

    //-Loc3-//

    //-Loc4-//

    //-Loc5-//
    loc[5].objects[0] = "trees";
    loc[5].objects[1] = "tree";
    loc[5].description = "Strange sounds surround you as you wander deeply into a forest.\n\n";
    loc[5].directions[0] = "north";
    loc[5].directions[1] = "east";
    loc[5].directions[2] = "south";

    //-Loc6-//
    loc[6].description = "You find yourself outside of your old room, happy to be away from the mumbling man you crept away from through the hole he made. There's a door to the east. North and west of you are two paths that appear to lead into a forest, what is this place?\n\n";
    loc[6].directions[0] = "";
    loc[6].directions[1] = "north";
    loc[6].directions[2] = "west";
    loc[6].directions[3] = "south";

    //-Loc7-//
    loc[7].directions[0] = "west";
    loc[7].objects[0] = "plank";
    loc[7].objects[1] = "bag";
    loc[7].description = "It's very dark in what appears to be a toolshed. There's a small bag and a wooden plank on the ground.\n\n";

    //-Loc8-//
    loc[8].description = "Down the hall from the elevator there is a window to the east. It's dark outside and you can not see anything. A strong draft pushes you away from the window. There's an old letter on the ground.\n\n";
    loc[8].objects[0] = "letter";
    loc[8].directions[0] = "";
    loc[8].directions[1] = "north";

    //-Loc9-//
    loc[9].objects[0] = "trees";
    loc[9].objects[1] = "tree";
    loc[9].description = "The forest starts to creep you out. Trees surround you and the creaking noises make you want to leave.\n\n";
    loc[9].directions[0] = "north";
    loc[9].directions[1] = "east";
    loc[9].directions[2] = "south";

    //-Loc10-//
    loc[10].objects[0] = "fence";
    loc[10].description = "There appears to be some kind of fence to the east of the road, preventing you from going any further. The forest to the west looks daunting. To the north there is light.\n\n";
    loc[10].directions[0] = "south";
    loc[10].directions[1] = "north";
    loc[10].directions[2] = "west";

    //-Loc11-//
    loc[11].description = "The hallway is a lime green, there's nobody around, but it feels, not to mention the smell, like there were people here before. \n\n";
    loc[11].directions[0] = "east";
    loc[11].directions[1] = "north";

    //-Loc12-//
    loc[12].description = "Wandering in the hallways you wonder if they'll ever end. To your north is an elevator with a rusty door. To the south the corridor goes on and on and on and... Hey look, there's some coins on the ground!\n\n";
    loc[12].objects[0] = "coins";
    loc[12].directions[0] = "south";
    loc[12].directions[1] = "north";
    loc[12].directions[2] = "west";

    //-Loc13-//
    loc[13].objects[0] = "tree";
    loc[13].objects[1] = "trees";
    loc[13].objects[2] = "fence";
    loc[13].description = "The end of the forest! At last! But what's this? North and west of the edge is a fence, blocking your exit! To the west a feint light can be seen.\n\n";
    loc[13].directions[0] = "south";
    loc[13].directions[1] = "east";

    //-Loc14-//
    loc[14].objects[0] = "nameplate";
    loc[14].objects[1] = "door";
    loc[14].description = "You're glad to finally be away from that scary place. To the east there is a door, this must have been where the light you saw was coming from. It appears to go into some kind of building, but you can't read the nameplate any more.\n\n";
    loc[14].directions[0] = "";
    loc[14].directions[1] = "south";
    loc[14].directions[2] = "west";

    //-Loc15-//
    loc[15].description = "Having entered the building, you're surprised to find it to be a complete mess. The hallways wind to the south. What the hell could have happened here?\n\n";
    loc[15].directions[0] = "south";
    loc[15].directions[1] = "west";

    //-Loc16-//
    loc[16].description = "There's a man sitting in front of you. His glasses are dark, but not as dark as his blank stare. There's an empty hat beside him.\n\n";
    loc[16].objects[0] = "beggar";
    loc[16].directions[0] = "south";

    //--OBJECTS--//

    object[0].examine = "\nWhatever you're looking to examine is entirely inside your imagination";

    object[1].name = "plank";
    object[1].examine = "\nA nasty looking slab of dead wood. From the stench of things it appears to have been lying here for quite some time.";
    object[1].pickup = true;
    object[1].pickupdescr = "";

    object[2].name = "bed";
    object[2].examine = "\nThe bed sheets are sweaty and dirty. You wonder what could have happened the previous night, but are interrupted by the weird man talking into your right ear very loudly.";
    object[2].pickup = false;
    object[2].pickupdescr = "\nWhat the hell are you trying to do picking up a bed superman?";

    object[3].name = "closet";
    object[3].examine = "\nAn old brown closet with a fine tuned finish. You ordered it yourself after you heard the slightly breathing wood type would keep your clothes in better shape.";
    object[3].pickup = false;
    object[3].pickupdescr = "\nNormally this closet would be no match for your mighty self, but your wardrobe has so significantly increased since your last visit to the mall, it's simply too heavy!";

    object[4].name = "man";
    object[4].examine = "\nA fuzzy man that keeps talking to you about things you're not comprehending properly. He suddenly notices you're not paying attention and kicks a hole in the wall behind him.\n\nNew exit: north";
    object[4].pickup = false;
    object[4].pickupdescr = "\nThe man refuses to let you pick him up and proceeds to talk incoherently into your face.";

    object[5].name = "key";
    object[5].examine = "\nA rusty old key that probably hasn't been used in ages.";
    object[5].pickup = true;
    object[5].pickupdescr = "";

    object[6].name = "coins";
    object[6].examine = "\Some money left behind on the floor. You have no idea who could have left them there, but you might be able to find out.";
    object[6].pickup = true;
    object[6].pickupdescr = "";

    object[7].name = "letter";
    object[7].examine = "\nAn old letter is lying on the floor. It looks very frail, as if it might disintegrate if anything were to touch it. It reads:\n\n'Dear tom,\n\nI'll see you soon, I know it! The've been treating us so harshly. I've decided to end the pain and leave this place forever. I hope you'll understand,\n\nLove, Mary.'";
    object[7].pickup = false;
    object[7].pickupdescr = "\nAs you try to pick up the letter it falls into hundreds of pieces, making it impossible to ever read it again.";

    object[8].name = "beggar";
    object[8].examine = "\nAn old man in an abandoned elevator that doesn't work any longer. The man probably doesn't work anymore either, since it appears he is blind and has quit his day job.";
    object[8].pickup = false;
    object[8].pickupdescr = "\nWhat on earth would you want to do picking up an old blind beggar? It's not like he's sitting on a pot of gold!";

    object[9].name = "trees";
    object[9].examine = "\nThe trees are scary and big and dark and... GO AWAY! AWAY I SAY!";
    object[9].pickup = false;
    object[9].pickupdescr = "\nWhat the hell would you do with a bunch of trees in your backpack? nevermind why, HOW?";

    object[10].name = "tree";
    object[10].examine = "\nThe tree is scary and big and dark and... GO AWAY! AWAY I SAY!";
    object[10].pickup = false;
    object[10].pickupdescr = "\nWhat are you, a botanist? Is this a botanist convention?";

    object[11].name = "fence";
    object[11].examine = "\nA metal wire fence. You grab onto it and it zaps you. You wonder why there wasn't a warning sign for this voltage flowing through the fence.";
    object[11].pickup = false;
    object[11].pickupdescr = "ZZZZZZZAAAAAAAPPPPPPPP!";

    object[12].name = "nameplate";
    object[12].examine = "\nYou try to read what it says, but can't make it all out: 'As... um... thor... el... nly...'";
    object[12].pickup = false;
    object[12].pickupdescr = "\You pick up the nameplate and try to read it a little better. Realizing it is completely useless, you put it back.";

    object[13].name = "bag";
    object[13].examine = "\nA bag of tools containing a hammer, some nails, a screwdriver and some rusty screws. It's made of a nice suede leather and could be worn around the waist, had it not been torn.";
    object[13].pickup = true;
    object[13].pickupdescr = "";
}

void display() {
	string descr;
	descr = loc[location].description;
	cout<<descr;
	cout<<"Current Location: Grid "<<location<<"\n";
}

int go(string direction) {

	direction = getsecondword(direction);
	int i = checkdirections(direction);
	if (i == 0) {
	if (direction == "north" || direction == "west" || direction == "south" || direction == "east") { 
		cout<<"\nYou start moving toward the "<< direction<< " but your nose disagrees with you.";
		cin.get();
		return(location); 
	}
	else {
		cout<<"\nA mysterious force has kept you in place. It might have been the dictionary.";
		cin.get();
		return(location);
}
}
	if (direction == "quit") {
		location = -1;
		return(location);
	}

	if (direction == "north") {
		if (location >= 13 && location <= 16) { return(location); }
		location += 4;
		return(location);
	}

	else if (direction == "south") {
		if (location >= 1 && location <= 4) { return(location); }
		location -= 4;
		return(location);
	}
	
	else if (direction == "east") {
		if (location == 8 && plankused == true) { 
			location = endgame(); 
			return(location);
		}
		if (location == 4 || location == 8 || location == 12 || location == 16) { return(location); }
		location += 1;
		return(location);
	}

	else if (direction == "west") {
		if (location == 1 || location == 5 || location == 9 || location == 13) { return(location); }
		location -= 1;
		return(location);
	}

	else {
		cout<<"\nA mysterious force has kept you in place. It might have been the dictionary.";
		cin.get();
		return(location);
	}
}

string getfirstword(string input) {
	unsigned int i = 0;
	string output;
	while (i <= input.length()) {
		if (input.substr(i, 1) == " ") {
		return(output);
		}
		else {
			output = output + input.substr(i, 1);
			i++;
		}
	}
	return(output);
}

string getsecondword(string input) {
	unsigned int i = 0;
	string output;
	int spacefound = 0;
	while (i <= input.length() && spacefound == 0) {
		if (input.substr(i, 1) == " ") {
			spacefound = 1;
			i++;
		}
		else {
			i++;
		}
	}

	spacefound = 0;

	while (i <= input.length() && spacefound == 0) {
		if (input.substr(i, 1) == " ") {
			return(output);
		}
		else {
			output = output + input.substr(i, 1);
			i++;
		}
	}
	return(output);
}

string process(string command) {
	string firstword = getfirstword(command);
	if (firstword == "go") {
		int oldloc = location;
		int newloc = go(command);
		if (newloc > 16 || newloc < 1) {
			if (newloc == -10) { 
				firstword = "0";
				return(firstword);
			}
			cout<<"\nYou attempt to move but as soon as your skull crashes into a wall, you stop trying.";
			cin.get();
			return("1");
		}

		else if (newloc == oldloc) {
			firstword = "1";
			return(firstword);
		} 
		else {
			location = newloc; 
			firstword = "1";
			return(firstword);
		}
	}
	else if (firstword == "use") {
		string secondword = getsecondword(command);
		useobject(secondword);
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "pickup") {
		string reaction = pickup(command);
		cout<< reaction;		
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "drop") {
		string reaction = drop(command);
		cout<< reaction;		
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "examine") {
		examine(command);
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "inventory") {
		showinventory();
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "help") {
		cout<<"\nLIST OF COMMANDS\n\n";
		cout<<"* go north/south/east/west - Head in these respective directions on the board\n";
		cout<<"* pickup [objectname]      - Will pick up the selected object is this is possible.\n";
		cout<<"* inventory		  - Will show the current contents of your inventory.\n";
		cout<<"* examine [objectname]	  - Will bring up any relevant (or irrelevant) information about the object and will place it under selection until you leave the room or select another object.\n";
		cout<<"* use [objectname]	  - Will use the selected object.";
		cin.get();
		firstword = "1";
		return(firstword);
	}
	else if (firstword == "die") {
		cout<<"\n*SECRET COMMAND*\n\nYou decide to see how well the human neck competes with modern day pogo-sticks, with very damaging result.\n\nYou are now dead.\n";
		cin.get();
		firstword = "0";
		return(firstword);
	}
	else if (firstword == "quit") {
		cout<<"Press any key to quit...\n";
		cin.get();
		firstword = "0";
		return(firstword);
	}
	cout<<"unknown command!\n\n";
	firstword = "1";
	return(firstword);
}

void showinventory() {
	cout<<"\nYou open up your backpack and look inside to find 5 neatly divided areas.\n\n";
	cout<<"1. "<< inventory[0] <<".\n";
	cout<<"2. "<< inventory[1] <<".\n";
	cout<<"3. "<< inventory[2] <<".\n";
	cout<<"4. "<< inventory[3] <<".\n";
	cout<<"5. "<< inventory[4] <<".\n\n";
}

string pickup(string command) {
	string secondword = getsecondword(command);
	int achieved = 0;
	string reaction;
	for (int i=0; i<=4; i++) {
		if (secondword == loc[location].objects[i]) {
			achieved = 1;
			int z = find(secondword);
			if (object[z].pickup == true) {
			
				int success = inventoryplace(secondword);
				if (success == 0) { 
					reaction = "Could not place " + secondword + " in your inventory. It is full!";
					return(reaction);
				}
				else if (success == 1) {
					reaction = "Placed " + secondword + " in your inventory.";
					loc[location].objects[i] = "";
					if (location == 7 && secondword == "bag") {
						int find = findloc("plank");
						if (find == 1) {
							loc[7].description = "It's very dark in what appears to be a toolshed. There's a wooden plank on the ground.\n\n";
						}
						else { loc[7].description = "It's very dark in what appears to be a toolshed.\n\n"; }
					}
					else if (location == 7 && secondword == "plank") {
						int find = findloc("bag");
						if (find == 1) {
							loc[7].description = "It's very dark in what appears to be a toolshed. There's a small bag on the ground.\n\n";
						}
						else { loc[7].description = "It's very dark in what appears to be a toolshed.\n\n"; }
					}
					else if (location == 12 && secondword == "coins") {
						loc[12].description = "Wandering in the hallways you wonder if they'll ever end. To your north is an elevator with a rusty door. To the south the corridor goes on and on and on and...\n\n";
					}

					return(reaction);
				}
			}
			else { cout<<object[z].pickupdescr; }
		}
	}
	if (achieved == 0) {
		reaction = "Invalid object!";
	}
	return(reaction);
}

int inventoryplace(string secondword) {
	for (int i=0; i<=4; i++) {
		if (inventory[i] == "") {
			inventory[i] = secondword;
			return(1);
		}
	}
	return(0);
}

string drop(string command) {
	string secondword = getsecondword(command);
	int success = inventoryremove(secondword);
	string reaction;
	if (success == 1) {
		reaction = "Removed (an instance of) " + secondword + " from your inventory";
		place(secondword);
		return(reaction);
	}
	else {
		reaction = "Could not find " + secondword + " in your inventory.";
		return(reaction);
	}
}

int inventoryremove(string secondword) {
	for (int i=0; i<=4; i++) {
		if (inventory[i] == secondword) {
			inventory[i] = "";
			return(1);
		}
	}
	return(0);
}

int remove(string secondword) {
	for (int i=0; i<=4; i++) {
		if (loc[location].objects[i] == secondword) {
			loc[location].objects[i] = "";
			return(1);
		}
	}
	return(0);
}

int findloc(string secondword) {
	for (int i=0; i<=4; i++) {
		if (loc[location].objects[i] == secondword) {
			return(i);
		}
	}
	return(10);
}

int findinv(string secondword) {
	for (int i=0; i<=4; i++) {
		if (inventory[i] == secondword) {
			return(1);
		}
	}
	return(0);
}

int find(string secondword) {
	for (int i=0; i<=20; i++) {
		if (object[i].name == secondword) {
			return(i);
		}
	}
	return(0);
}

void examine(string command) {
	string secondword = getsecondword(command);
	int find = findloc(secondword);
	if (find == 10) {
		cout<<"\nNow you're really getting things mixed up here...";
	}
	else {
		int i = find(secondword);
		cout<<object[i].examine<<"\n\n";
		if (object[i].name == "letter") { object[i].name = "somethingyoullnevereverevereverevereverguess"; }
		action(secondword);
	}
}

void action(string secondword) {
	if (secondword == "man" && location == 2) {
		loc[2].directions[0] = "north";
		loc[2].description = "A bed to your left seems to have been slept in. To your right stands a closet. In front of you stands a fuzzy man that seems to keep talking incessantly. There's a key on the floor in front of him and behind him is a hole, perhaps you could fit through!\n\n";
		place(secondword);
	}
}

int useobject(string secondword) {

	if (secondword == "plank" && location == 1) {
		int find = findinv(secondword);
		int find2 = findinv("bag");
		if (find == 1 && find2 == 1) {
			cout<<"\nYou board up the final part of the door with the wooden plank using the tools from the shed. The draft you felt earlier has gone.";
			loc[location].description = "As you walk down the dark forest path, you see the boarded up door to the west. It now contains the wooden plank you have added and the draft is gone.\n\n";
			int x = find(secondword);
			object[x].examine = "\nA wooden plank you found in the toolshed. It's not firmly boarded up against the doorposts.";
			object[x].pickup = false;
			object[x].pickupdescr = "\nIt's boarded it up to the doorpost, you can't get it back off.";
			plankused = true;
			loc[8].directions[0] = "east";
			place(secondword);
			inventoryremove(secondword);
			return(0);
		}
		else if (find == 1 || find2 == 1) {
			cout<<"\nIt feels right, but you think you're missing something...";
			return(0);
		}
		else {  
			cout<<"\nWhat plank?"; 
			return(0);
		}
	}

	if (secondword == "coins" && location == 16) {
		int find = findinv(secondword);
		if (find == 1) {
			cout<<"\nThe man suddenly starts talking to you: 'Oh, thank you. Thanks for the money. You know, I haven't heard anybody give me money for a long time. I used to be able to make a good living back here, but since a few weeks, everything's gotten really quiet. I sure do miss the little lady that always gave me right at the break of dawn, at least I think it was the break of dawn. I can't see, you see. I wonder what happened to her.'\n\nYou turn to leave the man now that he's stopped talking thinking: 'What is going on...'";
			loc[location].description = "There's a man sitting in front of you. His glasses are dark, but not as dark as his blank stare. The hat aside him contains the coins you dropped in.\n\n";
			int x = findloc(secondword);
			loc[location].objects[x] = "";
			place(secondword);
			inventoryremove(secondword);
			x = find(secondword);
			object[x].pickup = false;
			object[x].pickupdescr = "\nStealing from a BLIND BEGGAR! Shame on you! I won't allow it!";
			return(0);
		}
		else { 
			cout<<"\nWhat coins?";
			return(0);
 		}
	}

	if (secondword == "key" && location == 6) {
		int find = findinv(secondword);
		if (find == 1) {
			cout<<"\nYou use the key you found on the door. It opens.\n\nNew Exit: East";
			loc[location].directions[0] = "east";
			loc[location].description = "You're glad to finally be away from that scary place. To the east there is the door you've opened, this must have been where the light you saw was coming from. It appears to go into some kind of building, but you can't read the nameplate any more.\n\n";
			return(0);
		}
		else { 
			cout<<"\nWhat key?";
			return(0);
 		}
	}

	if (secondword == "key" && location == 14) {
		int find = findinv(secondword);
		if (find == 1) {
			cout<<"\nYou use the key you found on the door. It opens.\n\nNew Exit: East";
			loc[location].directions[0] = "east";
			loc[location].description = "You find yourself outside of your old room, happy to be away from the mumbling man you crept away from through the hole he made. The door you've opened is to the east. North and west of you are two paths that appear to lead into a forest, what is this place?\n\n";
			return(0);
		}
		else { 
			cout<<"\nWhat key?";
			return(0);
		}
	}

	cout<<"\nYou must be delusional";
	return(0);
}

int checkdirections(string direction) {
	for (int i = 0; i <= 4; i++) {
		if (loc[location].directions[i] == direction) { return(1); }
	}
	return(0);
}

void showdirections() {
	cout<<"Possible Exits: ";
	for (int i = 0; i <= 3; i++) {
		cout<<loc[location].directions[i]<<" ";
	}
	cout<<"\n";
}

int endgame() {
cout<<"\nYou proceed to open the window, now freed from the draft. You hesitantly climb out and jump, expecting to die. When you fall into the shrubbery, you open your eyes.\n\n'Tom? Oh my god, tom, it's you! I've finally found you!', a voice shouts from beyond the shrubbery.\n\n'Who are you?'\n\n'Don't worry tom, you probably don't remember, but I'm Mary. I left you a letter hoping you'd understand. Everything will be ok now.'\n\nTHE END\n\nPress any key to quit";
cin.get();
location = -10;
return(location);
}
