#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new sokoban();
    scene = new QGraphicsScene();
    originSize.setWidth(width());
    originSize.setHeight(height());
    cwd = QDir::currentPath();
    cwd.replace("/","\\");
    #ifdef QT_DEBUG
        cwd+="\\debug";
    #endif
    ui->stopButton->setEnabled(false);
    timer = new QTime();
    setLevelComboBox();
    loadImage();
    setGameView();
    connect(ui->levelComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setGameView()));
    connect(ui->startButton,SIGNAL(clicked()),
            this,SLOT(gameStart()));
    connect(ui->stopButton,SIGNAL(clicked()),
            this,SLOT(gameStop()));

}

void MainWindow::setLevelComboBox() { 
    QFile file( cwd + "\\level.txt");
    if(!file.open(QIODevice::ReadOnly)){
        ui->statusTextBrowser->append("NO such file!"+ cwd + "\\level.txt");
        return;
    }
    QString temp;
    QStringList list;
    while( !file.atEnd() ) {
        temp = file.readLine();
        temp = temp.left(temp.length() - 2);
        list.append(temp);
    }
    ui->levelComboBox->addItems(list);
    file.close();
}

void MainWindow::loadImage() {
    QFile file(cwd+"\\image.txt");
    if(!file.open(QIODevice::ReadOnly)){
        ui->statusTextBrowser->append("NO such file!"+ cwd + "\\image.txt");
        return;
    }
    QString temp;
    QPixmap image;
    imageVector = new QVector<QPixmap>();
    while( !file.atEnd() ) {
        temp = file.readLine();
        temp = temp.left(temp.length()-2);
        if( !image.load(cwd+"\\image\\"+temp,"PNG")) {
            ui->statusTextBrowser->append("Load image fail");
            return;
        }
        ui->statusTextBrowser->append("Load   " + temp);
        imageVector->append(image);
    }
    file.close();
}

MainWindow::~MainWindow()
{
    delete game;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    bool win_flag = false;
    QString game_status;
    if ( game_start ) {
        switch ( event->key() ) {
            case Qt::Key_W:
                win_flag = game->gameProgress(0);
                game_status += "Action: ";
                game_status += "Up ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case Qt::Key_S:
                win_flag = game->gameProgress(1);
                game_status += "Action: ";
                game_status += "Down ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case Qt::Key_A:
                win_flag = game->gameProgress(2);
                game_status += "Action: ";
                game_status += "Left ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case Qt::Key_D:
                win_flag = game->gameProgress(3);
                game_status += "Action: ";
                game_status += "Right ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            default:
                break;
        }
        QVector<QPoint> &history = game->getHistory();
        QVector<QPoint> target = game->getTargetPos();
        for( int i = 0; i < history.size(); i++ )
             graph[history[i].y()][history[i].x()]->setPixmap((*imageVector)[i]);
        history.clear();
        int data;
        for( QPoint &p: target ) {
            data = game->getData(p.x(),p.y());
            switch( data ) {
                case 0:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[5]);
                    break;
                case 1:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[7]);
                    break;
                case 2:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[6]);
                    break;
                default:
                    break;
            }
        }
        if( win_flag ) {
            ui->statusTextBrowser->append("You win!");
            gameStop();
        }
    }
}

void MainWindow::aiControl() {
    bool win_flag = false;
    int action = man->getAction();
    QString game_status;
        switch ( action ) {
            case 0:
                win_flag = game->gameProgress(0);
                game_status += "Action: ";
                game_status += "Up ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case 1:
                win_flag = game->gameProgress(1);
                game_status += "Action: ";
                game_status += "Down ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case 2:
                win_flag = game->gameProgress(2);
                game_status += "Action: ";
                game_status += "Left ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            case 3:
                win_flag = game->gameProgress(3);
                game_status += "Action: ";
                game_status += "Right ";
                game_status += "Agent: (" + QString::number(game->getAgentPos().x()) + "," +
                        QString::number(game->getAgentPos().y()) + ")  Move: " + QString::number(game->getMove()) +
                        " Push: " + QString::number(game->getPush());
                ui->statusTextBrowser->append(game_status);
                break;
            default:
                break;
        }
        QVector<QPoint> &history = game->getHistory();
        QVector<QPoint> target = game->getTargetPos();
        for( int i = 0; i < history.size(); i++ )
             graph[history[i].y()][history[i].x()]->setPixmap((*imageVector)[i]);
        history.clear();
        int data;
        for( QPoint &p: target ) {
            data = game->getData(p.x(),p.y());
            switch( data ) {
                case 0:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[5]);
                    break;
                case 1:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[7]);
                    break;
                case 2:
                    graph[p.y()][p.x()]->setPixmap((*imageVector)[6]);
                    break;
                default:
                    break;
            }
        }

        if( win_flag ) {
            ui->statusTextBrowser->append("You win!");
            gameStop();
        }

}

void MainWindow::setGameView()
{
    QString path = cwd + "\\level\\" + ui->levelComboBox->currentText() + ".txt";
    game->loadGame( path.toStdString());
    graph.resize(game->getHeight());
    for ( QVector<QGraphicsPixmapItem*> &temp: graph )
        temp.resize(game->getWidth());
    scene->clear();
    scene->setSceneRect(0,0,game->getWidth()*50,game->getHeight()*50);

    for ( int i = 0; i < game->getHeight(); i++ ) {
        for( int j = 0; j < game->getWidth(); j++ ) {
            graph[i][j] = scene->addPixmap((*imageVector)[game->getData(j,i)]);
            graph[i][j]->setPos(j*50,i*50);
        }
    }

    int data;
    for( QPoint &p: game->getTargetPos() ) {
        data = game->getData(p.x(),p.y());
        switch( data ) {
            case 0:
                graph[p.y()][p.x()]->setPixmap((*imageVector)[5]);
                break;
            case 1:
                graph[p.y()][p.x()]->setPixmap((*imageVector)[7]);
                break;
            case 2:
                graph[p.y()][p.x()]->setPixmap((*imageVector)[6]);
                break;
            default:
                break;
        }
    }

    QSize changeSize( game->getWidth()*50 + 50,game->getHeight()*50 + 50 );
    QSize viewSize(ui->graphicsView->width(),ui->graphicsView->height());
    resize(originSize.width()+changeSize.width()-viewSize.width(),originSize.height()+changeSize.height()-viewSize.height());
    originSize.setWidth(width());
    originSize.setHeight(height());
    ui->statusTextBrowser->resize(ui->statusTextBrowser->width(),changeSize.height()-ui->statusTextBrowser->y());
    ui->graphicsView->resize(changeSize.width(),changeSize.height());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::gameStart() {
    ui->stopButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->levelComboBox->setEnabled(false);
    ui->agentComboBox->setEnabled(false);
    *timer = QTime::currentTime();
    setGameView();
    QString curAgent = ui->agentComboBox->currentText();
    QString curLevel = ui->levelComboBox->currentText();
    QString message = "Game start\nAgent:  " + curAgent +
            "\nLevel:  " + curLevel+"\nStartTime:  "+
            timer->toString();
    ui->statusTextBrowser->setText(message);
    if( ui->agentComboBox->currentText() == "Human" ) {
        game_start = true;
    } else {
        setGameView();
        man = new agent( game->getMaze(), game->getTargetPos(), game->getAgentPos() );
        connect( &aiTimer, SIGNAL(timeout()), this, SLOT(aiControl()));
        bool has_sol = man->planAction();
        if( has_sol ) {
            aiTimer.start(250);
        } else {
            QString s = "The maze has no solution!\n";
            ui->statusTextBrowser->append(s);
            gameStop();
        }
    }
}

void MainWindow::gameStop() {
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    int cost_second = timer->secsTo(QTime::currentTime()) - timer->secsTo(*timer);
    QString time_string;
    time_string.sprintf("%02d:%02d",cost_second/60,cost_second%60);
    QString message = "Game stop\nStop time:  " + QTime::currentTime().toString()+
            "\nCost Time: " + time_string;
    ui->statusTextBrowser->append(message);
    ui->levelComboBox->setEnabled(true);
    ui->agentComboBox->setEnabled(true);
    game_start = false;
    if( ui->agentComboBox->currentText() == "AI" ) {
        disconnect( &aiTimer, SIGNAL(timeout()), this, SLOT(aiControl()));
        aiTimer.stop();
        delete man;
    }
}
