#include "sokoban.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
using namespace std;

static const int ROAD = 0, AGENT = 1, BOX = 2, WALL = 3, BLACK = 4, TARGET = 5,BOX_ON_TARGET=6, MAN_ON_TARGET=7;
static const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, RESTART = 4, LEAVE = 5;

sokoban::sokoban()
:move(0),push(0),curAction(0) {}

sokoban::~sokoban() {}

void sokoban::gameProgress( int ctr_source, int outside_ctr = -1 ) { // 0 from human, 1 from action list
	bool win_flag = false;
	int ctr_key, curX, curY;
	Point agentBackup = agentPos;
	showGame();
	if( !ctr_source ) {
		while( ctr_key = getControl() != LEAVE && !win_flag ) {
			curX = agentPos.getX();
			curY = agentPos.getY();
			curAction = ctr_key;
			switch(ctr_key) {
				case 0: // UP
					if( grid[curY-1][curX] == ROAD ) {
						agentPos.setY(curY-1); agentPos.setX(curX);
						grid[curY-1][curX] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY-1][curX] == BOX ) {
						if( grid[curY-1][curX] + grid[curY-2][curX] < 4 ) {
							agentPos.setY(curY-1); agentPos.setX(curX);
							grid[curY-1][curX] = AGENT;
							grid[curY-2][curX] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 1: // DOWN
					if( grid[curY+1][curX] == ROAD ) {
						agentPos.setY(curY+1); agentPos.setX(curX);
						grid[curY+1][curX] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY+1][curX] == BOX ) {
						if( grid[curY+1][curX] + grid[curY+2][curX] < 4 ) {
							agentPos.setY(curY+1); agentPos.setX(curX);
							grid[curY+1][curX] = AGENT;
							grid[curY+2][curX] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 2: // LEFT
					if( grid[curY][curX-1] == ROAD ) {
						agentPos.setY(curY); agentPos.setX(curX-1);
						grid[curY][curX-1] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY][curX-1] == BOX ) {
						if( grid[curY][curX-1] + grid[curY][curX-2] < 4 ) {
							agentPos.setY(curY); agentPos.setX(curX-1);
							grid[curY][curX-1] = AGENT;
							grid[curY][curX-2] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 3: // RIGHT
					if( grid[curY][curX+1] == ROAD ) {
						agentPos.setY(curY); agentPos.setX(curX+1);
						grid[curY][curX+1] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY][curX+1] == BOX ) {
						if( grid[curY][curX+1] + grid[curY][curX+2] < 4 ) {
							agentPos.setY(curY); agentPos.setX(curX+1);
							grid[curY][curX+1] = AGENT;
							grid[curY][curX+2] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 4: // RESTART
					win_flag = false;
					move = push = 0;
					grid = backup;
					agentPos= agentBackup;
					cout << "------!Game restart!------" << endl;
					showGame();
					break;
				default:
					break;
			}
			if (win_check()) {
				win_flag = true;
				break;
			}
			if( win_flag )
				cout << "You win" << endl;
			cout << "Game Stop" << endl;
		};
	} else {
		    curX = agentPos.getX();
			curY = agentPos.getY();
			curAction = outside_ctr;
			switch(outside_ctr) {
				case 0: // UP
					if( grid[curY-1][curX] == ROAD ) {
						agentPos.setY(curY-1); agentPos.setX(curX);
						grid[curY-1][curX] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY-1][curX] == BOX ) {
						if( grid[curY-1][curX] + grid[curY-2][curX] < 4 ) {
							agentPos.setY(curY-1); agentPos.setX(curX);
							grid[curY-1][curX] = AGENT;
							grid[curY-2][curX] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 1: // DOWN
					if( grid[curY+1][curX] == ROAD ) {
						agentPos.setY(curY+1); agentPos.setX(curX);
						grid[curY+1][curX] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY+1][curX] == BOX ) {
						if( grid[curY+1][curX] + grid[curY+2][curX] < 4 ) {
							agentPos.setY(curY+1); agentPos.setX(curX);
							grid[curY+1][curX] = AGENT;
							grid[curY+2][curX] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 2: // LEFT
					if( grid[curY][curX-1] == ROAD ) {
						agentPos.setY(curY); agentPos.setX(curX-1);
						grid[curY][curX-1] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY][curX-1] == BOX ) {
						if( grid[curY][curX-1] + grid[curY][curX-2] < 4 ) {
							agentPos.setY(curY); agentPos.setX(curX-1);
							grid[curY][curX-1] = AGENT;
							grid[curY][curX-2] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 3: // RIGHT
					if( grid[curY][curX+1] == ROAD ) {
						agentPos.setY(curY); agentPos.setX(curX+1);
						grid[curY][curX+1] = AGENT;
						grid[curY][curX] = ROAD;
						move++;
						showGame();
					} else if( grid[curY][curX+1] == BOX ) {
						if( grid[curY][curX+1] + grid[curY][curX+2] < 4 ) {
							agentPos.setY(curY); agentPos.setX(curX+1);
							grid[curY][curX+1] = AGENT;
							grid[curY][curX+2] = BOX;
							grid[curY][curX] = ROAD;
							push++; move++;
							showGame();
						}
					}
					break;
				case 4: // RESTART
					win_flag = false;
					move = push = 0;
					grid = backup;
					agentPos= agentBackup;
					cout << "------!Game restart!------" << endl;
					showGame();
					break;
				default:
					break;
			}
			if (win_check()) {
				win_flag = true;
			}
			if( win_flag )
				cout << "You win" << endl;
	}
}

bool sokoban::win_check() {
	int count = 0;
	for ( Point &p: target ) {
		if( grid[p.getY()][p.getX()] == 2 )
			count++;
	}
	
	if( count == target.size() )
		return true;
	else
		return false;
}

void sokoban::loadGame( char* filename ) {
	FILE* file;
	if( !(file = fopen(filename,"r"))) {
		std::cout << "No such file";
		return;
	}
	int x, y, target_number;
	fscanf(file,"%d %d", &x, &y); // get map size
	mapSize.setX(x); mapSize.setY(y);
	fscanf(file,"%d %d", &x, &y); // get agent position
	agentPos.setX(x); agentPos.setY(y);
	fscanf( file,"%d", &target_number );
	target.resize(target_number);
	for( Point& p: target ) {
		fscanf(file,"%d %d", &x, &y); // get target position
		p.setX(x); p.setY(y);
	}
	
	grid.resize(mapSize.getY());
	for ( vector<int> &temp : grid ) {
		temp.resize(mapSize.getX());
		for( int& number: temp ) {
			fscanf(file,"%d", &x);
			number = x;
		}
	}
	move = push = 0;
	backup = grid;
}

int sokoban::getControl() {
	char key;
	while((key = getch())) {
		if( key == -32 ) {
			key = getch();
			switch(key) {
				case 72:
					return UP;
					break;
				case 80:
					return DOWN;
					break;
				case 75:
					return LEFT;
					break;
				case 77:
					return RIGHT;
					break;
				default:
					continue;
			}	
		} else {
			switch(key) {
				case 114:
					return RESTART;
					break;
				case 113:
					return LEAVE;
					break;
				default:
					continue;
			}
		}
	}
	return LEAVE;
}

void sokoban::showGame(){
	system("cls");
	cout << "---------current state--------" << endl;
	for ( const Point &p: target ) {
		if(grid[p.getY()][p.getX()] != 2 && grid[p.getY()][p.getX()] != 1 )
			grid[p.getY()][p.getX()] = 5;
	}
	for ( vector<int> temp: grid ) {
		for ( int &number: temp ) {
			switch(number) {
				case 0: // ROAD
					cout << "  ";
					break;
				case 4: // BLACK
					cout << " .";
					break;
				case 1: // AGENT
					cout << " !";
					break;
				case 2: // BOX
					cout << " ^";
					break;
				case 3: // WALL
					cout << " #";
					break;
				case 5: // TARGET
					cout << " @";
					break;
				default:
					break;
			}
		}
		cout << endl;
	}
	cout << "Action: ";
	switch( curAction ) {
		case 2: // LEFT
			cout << "LEFT";
			break;
		case 0: // UP
			cout << "UP";
			break;
		case 1: // DOWN
			cout << "DOWN";
			break;
		case 3: // RIGHT
			cout << "RIGHT";
			break;
	}
	cout << " Move: " << move << " Push: " << push << endl;
	for ( const Point &p: target ) {
		if(grid[p.getY()][p.getX()] == 5 )
			grid[p.getY()][p.getX()] = 0;
	}
}





