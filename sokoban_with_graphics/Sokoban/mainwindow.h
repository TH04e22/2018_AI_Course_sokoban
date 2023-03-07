#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "agent.h"
#include <QDir>
#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <string>
#include <QImage>
#include <QSize>
#include <QPixmap>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
namespace Ui {
class MainWindow;
}

class MainWindow;

class sokoban:public QObject {
    Q_OBJECT
public:
    sokoban();
    ~sokoban();
    int getWidth();
    int getHeight();
    int getMove() { return move; };
    int getPush() { return push; };
    QVector<QPoint>& getHistory();
    QVector<QPoint>& getTargetPos();
    const QVector<QVector<int>>& getMaze();
    QPoint getAgentPos();
    void restore();
    int getData(int, int);
    bool gameProgress( int );
    bool win_check();
    void loadGame(std::string filename);
private:
    const int ROAD = 0, AGENT = 1, BOX = 2, WALL = 3, BLACK = 4, TARGET = 5,BOX_ON_TARGET=6, MAN_ON_TARGET=7;
    const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, RESTART = 4, LEAVE = 5;
    QPoint mapSize;
    QPoint agentPos;
    int curAction;
    int move;
    int push;
    QVector<QVector<int>> grid;
    QVector<QVector<int>> backup;
    QVector<QPoint> target;
    QVector<QPoint> history;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setGameView();
    void gameStart();
    void gameStop();
    void aiControl();
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
signals:

private:
    Ui::MainWindow *ui;
    void setLevelComboBox();
    void loadImage();
    QString cwd;
    QSize originSize;
    QTime *timer = nullptr;
    QTimer aiTimer;
    QVector<QVector<QGraphicsPixmapItem*>> graph;
    QVector<QPixmap> *imageVector = nullptr;
    QGraphicsScene *scene;
    sokoban *game;
    agent *man;
    bool game_start = false;
};


#endif // MAINWINDOW_H
