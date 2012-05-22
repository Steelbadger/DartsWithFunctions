//===========Game of Darts, 2 Computer Controlled Characters==========
//===========================By Ross Davies===========================
//
//=====This Program will prompt the user for the number of points=====
//======to play from, and will then simulate a game between two=======
//====players until one wins, with the requirement that, to win,======
//===============the player MUST finish with the bull=================
//====================================================================


#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;


//  Array of all outer board points, in clockwise order.
const int boardpoints[20] =  {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
const int distributionsize = 12;
int distributionarray[distributionsize] = {0};

//  Define some const values for easier referencing later
const bool ACCURACY = 0;
const bool POINTS = 1;
const short int THROWS = 2;
const short int WINS = 3;
const bool JOE = 0;
const bool SID = 1;


//  Function Prototypes
void TakeAShot(int player[], int flipflop, bool showtext);
int BullInaccuracy(int player[]);
int OuterInaccuracy(int player[], int target);

void main() {
	srand(time(NULL));
	
	//  Set up points and initialise player info variables
	int gamepoints = 301;
	bool flipflop = JOE;
	bool winner;
	short int gametype = 0;
	short int playernumber = 2;
	int games = 1;
	bool showgametext = 1;
	float averagethrows = 0;


	//  Ask user to define simulation parameters
	cout << "Play a 1 or 2 Player Game: ";
	cin >> playernumber;
	cout << endl;

	cout << "Points to play from: ";
	cin >> gamepoints;
	cout << endl;

	cout << "How many games to play: ";
	cin >> games;
	cout << endl;

	if (playernumber == 2) {
		cout << "Who goes first [0 = Joe, 1 = Sid, 2 = CoinToss then Loser First]: ";
		cin >> gametype;
		cout << endl;
	} else {
		cout << "Which Player to play [0 = Joe, 1 = Sid]: ";
		cin >> gametype;
		cout << endl;
	}
	cout << endl;


	//  If player is requesting multiple iterations then turn off full text output
	if (games > 1) {
		showgametext = 0;
	}
	//  Set up an array for each player with accuracy, points, throws and wins, in that order ( ex. joestats[4] = {accuracy, points, throws, wins} )
	int joestats[4] = {70, gamepoints, 0, 0};
	int sidstats[4] = {72, gamepoints, 0, 0};

	//  Loop for simulation
	for (int i = games; i > 0; i--) {

		//  Reset initial values for each simulation
		joestats[POINTS] = gamepoints;
		sidstats[POINTS] = gamepoints;
		joestats[THROWS] = 0;
		sidstats[THROWS] = 0;


		//  Ensure correct player begins each game
		if (gametype == 0) {
			flipflop = JOE;
		} else if (gametype == 1) {
			flipflop = SID;
		} else {
			if (i < games) {
				if (winner == JOE) {
					flipflop = SID;		
				} else {
					flipflop = JOE;
				}
			} else {
				flipflop = rand()%2;
			}
		}
		

		//  single game simulation loop flip-flops between first and second player shots
		while (joestats[POINTS] != 0 && sidstats[POINTS] != 0) {

			if (flipflop == JOE) {
				TakeAShot(joestats, flipflop, showgametext);
				joestats[THROWS] += 1;
			} else {
				TakeAShot(sidstats, flipflop, showgametext);
				sidstats[THROWS] +=1;
			}
			if (flipflop == JOE && playernumber == 2) {
				flipflop = SID;		
			} else if (playernumber == 2) {
				flipflop = JOE;
			}
		}


		//  Detect who won, add to their wins and remember who won for next game starter purposes
		if (joestats[POINTS] == 0) {
			if (showgametext == 1) {
				cout << "Joe wins, " << joestats[THROWS] << " throws to win the game" << endl << endl;
			}
			joestats[WINS] += 1;
			winner = JOE;
		} else {
			if (showgametext == 1) {
				cout << "Sid wins, " << sidstats[THROWS] << " throws to win the game" << endl << endl;
			}
			sidstats[WINS] += 1;
			winner = SID;
		}

		//  Assign throw numbers for each (single player) game to the distribution matrix and keep a running total of throws
		if (playernumber == 1) {
			if ((joestats[THROWS] + sidstats[THROWS]) > (distributionsize - 2 + 7)) {
				distributionarray[distributionsize-1] += 1;
			} else {
				distributionarray[joestats[THROWS]+sidstats[THROWS]-7] += 1;
			}

			averagethrows += joestats[THROWS]+sidstats[THROWS];
		}
		
	}
	
	//  Declare win frequency for each player
	cout << "Joe wins:\t" << joestats[WINS] << "\t[" << (float)joestats[WINS]/(joestats[WINS]+sidstats[WINS])*100 << "%]" << endl;
	cout << "Sid wins:\t" << sidstats[WINS] << "\t[" << (float)sidstats[WINS]/(joestats[WINS]+sidstats[WINS])*100 << "%]" << endl << endl;


	//  for single player games, return the frequency distribution table for throws
	if (playernumber == 1) {

		averagethrows = (float)averagethrows/games;
		cout << "Average Throws to Finish: " << averagethrows << endl << endl;

		for (int i = 0; i < distributionsize - 1; i++) {
			cout << "Finish in " << i+7 << "\t\t=\t" << distributionarray[i] << "\t[" << ((float)distributionarray[i]/games)*100 << "%]" << endl;
		}
		cout << "Finish in " << distributionsize+6 << " or more \t=\t" << distributionarray[distributionsize-1];
		cout << "\t[" << ((float)distributionarray[distributionsize-1]/games)*100 << "%]" << endl << endl;
	}

	while(true){}

}




void TakeAShot(int person[], int flipflop, bool showtext) {
//  First Selects a Target then checks for inaccuracy, modifies points based on what is hit

	//  Local variables for holding working values
	int target = 0;
	int hit = 0;


	//  Select what to aim for
	if (person[POINTS] > 100) {
		target = 50;
	} else if (person[POINTS] > 69) {
		target = 20;
	} else if (person[POINTS] > 50) {
		target = person[POINTS] - 50;
	} else if (person[POINTS] == 50) {
		target = 50;
	} else {
		cout << endl << "Illegal Score Ocurred, this message should never show" << endl;
	}
	
	//  Depending on target then run function that handles that kind of target
	if (target == 50) {
		hit = BullInaccuracy(person);
	} else {
		hit = OuterInaccuracy(person, target);
	}

	//  Display all the text associated with the target choise, number hit and points scored.
	//  only if specifically asked to do so (more than one game will not display this text)
	//  If no text is to be displayed then just display the values
	if (showtext == 1) {
		if (flipflop == JOE) {
			cout << "Joe";
		} else {
			cout << "Sid";
		}

		cout << " aimed for " << target << " and hit " << hit << endl;

		if ((person[POINTS]-hit) < 50 && (person[POINTS]-hit) != 0) {
			cout << person[POINTS] - hit << " is an illegal score, points reset to " << person[POINTS] << endl << endl;
		} else {
			if (flipflop == JOE) {
				cout << "Joe";
			} else {
				cout << "Sid";
			}
			person[POINTS] = person[POINTS] - hit;
			cout << " now has " << person[POINTS] << " points" << endl << endl;
		}
	} else {
		if ((person[POINTS]-hit) >= 50 || (person[POINTS]-hit) == 0) {
			person[POINTS] = person[POINTS] - hit;
		}
	}
}



int BullInaccuracy(int player[]) {
//  Handles checks for misses when aiming at the bull, if a miss occurs then it chooses a random number from
//  0 to 19 then uses that to reference the boardpoints array
//  If no miss happens then simply output that target

	int hit = 0;
	
	if (((rand()*100)/RAND_MAX) > player[ACCURACY]) {
		do {
			hit = rand();
		} while (hit > 32700);
		hit = hit%20;
		hit = boardpoints[hit];
	} else {
		hit = 50;
	}
	return hit;
}



int OuterInaccuracy(int player[], int target) {
//  Handles checks for misses when aiming at singles.  If a miss happens then increments up/down in a 50:50 ratio
//  Has a clause for handling array wraparound.  Then returns the new value hit.
//  If no miss happens then return the target.

	int hit = 0;
	int index = 0;


	//  if miss
	if (((rand()*100)/RAND_MAX) > 80) {
		//  find index location of target in array
		while (boardpoints[index] != target) {
			index++;
		}
		//  increment up/down (wraparound)
		if ((rand()*100)/RAND_MAX > 50) {
			if (index < 1) {	
				index = 19;
			} else {
				index -=1;
			}
		} else {
			if (index > 18) {
				index = 0;
			} else {
				index += 1;
			}
		}		

		if (index > 19) {
			index = 0;
		} else if (index < 0) {
			index = 19;
		}


		
		hit = boardpoints[index];
	} else {
		hit = target;
	}
	//  Output what gets hit
	return hit;
}



