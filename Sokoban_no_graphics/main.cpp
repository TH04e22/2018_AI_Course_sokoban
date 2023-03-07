#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include "sokoban.h"
#include "agent.h"
using namespace std;
int main ()
{
	string filename;
  	sokoban game;
	cout << "Which level you want to play? ";
	getline(cin,filename);
  	game.loadGame(const_cast<char*>(filename.c_str()));
	//game.gameProgress( 0 ); // 0 is human, 1 is AI
  	agent man( game.getMaze(),game.getTarget(),game.getAgentPos());
	bool sol = man.planAction();
	int action = man.getAction();
	if( sol ) {
		while( action != -1 ) {
			Sleep( 300 );
			game.gameProgress( 1, action );
			action = man.getAction();
		}
	} else {
		cout << "The maze has no solution!" << endl;
	}
  	return 0;
} 
