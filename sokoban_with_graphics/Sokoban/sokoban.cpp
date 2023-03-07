#include "mainwindow.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

sokoban::sokoban()
:move(0),push(0),curAction(0){}

sokoban::~sokoban() {}

const QVector<QVector<int>>& sokoban::getMaze() {
    return grid;
}

int sokoban::getWidth() {
    return mapSize.x();
}

int sokoban::getHeight() {
    return mapSize.y();
}

int sokoban::getData( int x, int y ) {
    return grid[y][x];
}

QVector<QPoint>& sokoban::getHistory() {
    return history;
}

QVector<QPoint>& sokoban::getTargetPos() {
    return target;
}

QPoint sokoban::getAgentPos() {
    return QPoint(agentPos.x(),agentPos.y());
}

bool sokoban::gameProgress( int ctr_key ) {
	bool win_flag = false;
    int curX, curY;
    QPoint agentBackup = agentPos;
    curX = agentPos.x();
    curY = agentPos.y();
    curAction = ctr_key;
    switch(ctr_key) {
        case 0: // UP
            if( grid[curY-1][curX] == ROAD ) {
                agentPos.setY(curY-1); agentPos.setX(curX);
                history.append(QPoint(curX,curY));
                history.append(QPoint(curX,curY-1));
                move++;
            } else if( grid[curY-1][curX] == BOX ) {
                if( grid[curY-1][curX] + grid[curY-2][curX] < 4 ) {
                    agentPos.setY(curY-1); agentPos.setX(curX);
                    history.append(QPoint(curX,curY));
                    history.append(QPoint(curX,curY-1));
                    history.append(QPoint(curX,curY-2));
                    push++; move++;
                }
            }
            break;
        case 1: // DOWN
            if( grid[curY+1][curX] == ROAD ) {
                agentPos.setY(curY+1); agentPos.setX(curX);
                history.append(QPoint(curX,curY));
                history.append(QPoint(curX,curY+1));
                move++;
            } else if( grid[curY+1][curX] == BOX ) {
                if( grid[curY+1][curX] + grid[curY+2][curX] < 4 ) {
                    agentPos.setY(curY+1); agentPos.setX(curX);
                    history.append(QPoint(curX,curY));
                    history.append(QPoint(curX,curY+1));
                    history.append(QPoint(curX,curY+2));
                    push++; move++;
                }
            }
            break;
        case 2: // LEFT
            if( grid[curY][curX-1] == ROAD ) {
                agentPos.setY(curY); agentPos.setX(curX-1);
                history.append(QPoint(curX,curY));
                history.append(QPoint(curX-1,curY));
                move++;
            } else if( grid[curY][curX-1] == BOX ) {
                if( grid[curY][curX-1] + grid[curY][curX-2] < 4 ) {
                    agentPos.setY(curY); agentPos.setX(curX-1);
                    history.append(QPoint(curX,curY));
                    history.append(QPoint(curX-1,curY));
                    history.append(QPoint(curX-2,curY));
                    push++; move++;
                }
            }
            break;
        case 3: // RIGHT
            if( grid[curY][curX+1] == ROAD ) {
                agentPos.setY(curY); agentPos.setX(curX+1);
                history.append(QPoint(curX,curY));
                history.append(QPoint(curX+1,curY));
                move++;
            } else if( grid[curY][curX+1] == BOX ) {
                if( grid[curY][curX+1] + grid[curY][curX+2] < 4 ) {
                    agentPos.setY(curY); agentPos.setX(curX+1);
                    history.append(QPoint(curX,curY));
                    history.append(QPoint(curX+1,curY));
                    history.append(QPoint(curX+2,curY));
                    push++; move++;
                }
            }
            break;
        case 4: // RESTART
            win_flag = false;
            move = push = 0;
            grid = backup;
            agentPos= agentBackup;
            break;
        default:
            break;
        }

    for( int i = 0; i < history.size(); i++ ) {
         grid[history[i].y()][history[i].x()] = i;
    }
    if (win_check()) {
        win_flag = true;
    }
    return win_flag;
}


bool sokoban::win_check() {
	int count = 0;
    for ( QPoint &p: target ) {
        if( grid[p.y()][p.x()] == 2 )
			count++;
	}
	
	if( count == target.size() )
		return true;
	else
		return false;
}

void sokoban::loadGame( std::string filename ) {
	FILE* file;
    if( !(file = fopen(const_cast<char*>(filename.c_str()),"r"))) {
		return;
	}
	int x, y, target_number;
	fscanf(file,"%d %d", &x, &y); // get map size
	mapSize.setX(x); mapSize.setY(y);
	fscanf(file,"%d %d", &x, &y); // get agent position
	agentPos.setX(x); agentPos.setY(y);
	fscanf( file,"%d", &target_number );
	target.resize(target_number);
    for( QPoint& p: target ) {
		fscanf(file,"%d %d", &x, &y); // get target position
        p.setX(x); p.setY(y);
	}
	
    grid.resize(mapSize.y());
    for ( QVector<int> &temp : grid ) {
        temp.resize(mapSize.x());
        for( int& number: temp ) {
            fscanf(file,"%d", &x);
            number = x;
        }
    }
	move = push = 0;
	backup = grid;
}


void sokoban::restore() {
    grid = backup;
}




