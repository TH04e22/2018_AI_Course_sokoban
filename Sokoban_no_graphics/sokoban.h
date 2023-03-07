#ifndef SOKOBAN_H
#define SOKOBAN_H
#include "point.h"
#include <vector>
#include <string>


class sokoban {
public:
	sokoban();
	~sokoban();
	void gameProgress( int, int );
	bool win_check();
	Point getAgentPos() const { return agentPos; };
	std::vector<Point> getTarget() const { return target; };
	std::vector<std::vector<int>> getMaze() const { return grid; };
	virtual void loadGame(char* filename);
	virtual int getControl();
	virtual void showGame();
private:
	Point mapSize;
	Point agentPos;
	int curAction;
	int move;
	int push;
	std::vector<std::vector<int>> grid;
	std::vector<std::vector<int>> backup;
	std::vector<Point> target;
};
#endif
