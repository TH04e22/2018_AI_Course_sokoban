#ifndef SOKOBAN_H
#define SOKOBAN_H
#include <QObject>
#include <QVector>
#include <QPixmap>
#include <QGraphicsPixmapItem>
class sokoban:public QObject {
    Q_OBJECT
public:
	sokoban();
	~sokoban();
    int getWidth();
    int getHeight();
    void gameProgress( int );
	bool win_check();
    void loadGame(char* filename);
    int getControl();
    void showGame();
private:
	class Point{
	public:
		Point( int x = 0, int y = 0 ) {
			this->x = x;
			this->y = y;
		}
	~Point() {}
		inline void setX( int x ) { this->x = x; }
		inline void setY( int y ) { this->y = y; }
		inline int getX() const { return x; }
		inline int getY() const { return y; }
	private:
		int x;
		int y;
	};
	const int ROAD = 0, AGENT = 1, BOX = 2, WALL = 3, BLACK = 4, TARGET = 5,BOX_ON_TARGET=6, MAN_ON_TARGET=7;
	const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, RESTART = 4, LEAVE = 5;
	Point mapSize;
	Point agentPos;
	int curAction;
	int move;
	int push;
    QVector<QVector<int>> grid;
    QVector<QVector<int>> backup;
    QVector<QVector<QPixmap*>> graph;
    QVector<Point> target;
};
#endif
