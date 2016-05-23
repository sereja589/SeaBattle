#ifndef GAME_H
#define GAME_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QGroupBox>
#include <QString>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QTextEdit>

#include "menu.h"
#include "mygraphicsscene.h"
#include "mygraphicsview.h"
#include "getip.h"
#include "field.h"
#include "ship.h"

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    ~Game();

private:
    enum Condition {MAINMENU, GETIP, CONNECTED, BEFOREGAME, GAME};
    Condition condition;

    const static int NPORT = 60009;

    MyGraphicsView *view;
    MyGraphicsScene *scene;
    QTcpServer *server;
    QTcpSocket *socket;
    Menu *menu;
    GetIp *getip;
    QGraphicsProxyWidget *pgrp;

    QString currStr;
    qint16 blockSize;

    bool isConnected;

    Field *field1, *field2;
    QGraphicsProxyWidget *chatWidget;
    QLineEdit *chatLine;
    QTextEdit *chatText;
    QVector<Ship*> ships;
    QGraphicsProxyWidget *buttonsWidget;
    bool iReady, opponentReady;
    int isMyTurn;

    void showGetIp();
    void showMenu();
    void hideMenu();
    void hideGetIp();
    void sendText(QString);
    void readText(QString);
    void showGameInterface();
    void hideGameInterface();
    void showField();
    void hideField();
    void showChat();
    void hideChat();
    void showShips();
    void hideShips();
    void showButtons();
    void hideButtons();
    void runGame();
    void processOpponentShoot(int x, int y);
    void processWeDamage(int x, int y);
    void processWePass(int x, int y);
    void processWeKill(int x, int y);

signals:
    void signalEnterPress();

private slots:
    void startServer();
    void connectToServer(QString);
    void getHostServer();
    void slotNewConnection();
    void slotReadyRead();
    void sendMessage();
    void enterPress();
    void slotRun();
    void slotMenu();
    void slotClickedOnCell(int x, int y);
    void slotRandomShuffle();
};

#endif // GAME_H
