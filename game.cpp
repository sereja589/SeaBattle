#include "game.h"
#include <QPushButton>
#include <QFont>
#include <QPalette>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QDataStream>
#include <QtAlgorithms>
#include <QTextEdit>
#include <QGroupBox>

#include "cell.h"

Game::Game()
{
    view = new MyGraphicsView;
    view->setMinimumSize(1000, 500);
    scene = new MyGraphicsScene(QRectF(-500, -250, 1000, 500));
    view->setScene(scene);
    view->show();
    connect(view, SIGNAL(enterPress()), this, SLOT(enterPress()));
    //showGameInterface();
    server = 0;
    socket = 0;
    showMenu();
}

void Game::showMenu()
{
    menu = new Menu();

    QFont f("Times", 30, -1, true);
    QPalette p;
    p.setColor(QPalette::Button, Qt::blue);

    QPushButton *b1 = new QPushButton("start server");
    b1->setMaximumHeight(2000);
    b1->setFont(f);
    b1->setAutoFillBackground(true);
    b1->setPalette(p);

    QPushButton *b2 = new QPushButton("connect to server");
    b2->setMaximumHeight(2000);
    b2->setFont(f);
    b2->setAutoFillBackground(true);
    b2->setPalette(p);

    connect(b1, SIGNAL(clicked(bool)), this, SLOT(startServer()));
    connect(b2, SIGNAL(clicked(bool)), this, SLOT(getHostServer()));

    menu->addButton(b1);
    menu->addButton(b2);
    scene->setMenu(menu, 30, 20);
    condition = MAINMENU;
}

void Game::hideMenu()
{
    scene->removeMenu();
    menu = 0;
}

void Game::showGetIp()
{
    getip = new GetIp;
    QGroupBox *grp = getip->getWidget();
    pgrp = scene->addWidget(grp);
    pgrp->setGeometry(scene->sceneRect());
    connect(this, SIGNAL(signalEnterPress()), getip, SLOT(enterPress()));
    condition = GETIP;
}

void Game::hideGetIp()
{
    scene->removeItem(pgrp);
    pgrp = 0;
    getip = 0;
}

void Game::getHostServer()
{
    hideMenu();
    showGetIp();
    connect(getip, SIGNAL(clickButton(QString)), this, SLOT(connectToServer(QString)));
}

void Game::startServer()
{
    hideMenu();
    server = 0;
    socket = 0;
    isConnected = false;
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, NPORT);
    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    blockSize = 0;
    condition = CONNECTED;
}

void Game::connectToServer(QString strIp)
{
    server = 0;
    socket = 0;
    isConnected = false;
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(strIp), NPORT);
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    hideGetIp();
    blockSize = 0;
}

void Game::sendText(QString str)
{
    if (isConnected)
    {
        QByteArray arr;
        QDataStream out(&arr, QIODevice::WriteOnly);
        out << qint16(0);
        out << str;
        out.device()->seek(0);
        out << qint16(arr.size() - sizeof(qint16));
        socket->write(arr);
    }
}

void Game::readText(QString str)
{
    if (str.length() >= 2 && str[0] == '/' && str[1] == '/')
    {
        qDebug() << str;
    }
    else if (str[0] == '/')
    {
        QString s;
        for (int i = 1; i < str.length(); ++i)
            s.append(str[i]);
        chatText->append(s);
    }
    else
    {
        if (str.size() == 1)
        {
            if (str[0] == 'R')
            {
                opponentReady = true;
                if (iReady && opponentReady)
                    runGame();
            }
            if (str[0] == '0' || str[0] == '1')
            {
                if (str[0] == '0')
                    isMyTurn = 0;
                else
                    isMyTurn = 1;
            }
        }
        if (str.size() == 3 && str[0] == 'S')
        {
            int x = str[1].toLatin1() - '0';
            int y = str[2].toLatin1() - '0';
            processOpponentShoot(x, y);
        }
        if (str.size() == 4 && str[0] == 'R')
        {
            int x = str[2].toLatin1() - '0';
            int y = str[3].toLatin1() - '0';
            if (str[1] == 'P')
                processWePass(x, y);
            if (str[1] == 'D')
                processWeDamage(x, y);
            if (str[1] == 'K')
                processWeKill(x, y);
        }
    }
}

void Game::slotNewConnection()
{
    qDebug() << 1223;
    if (!isConnected)
    {
        qDebug() << "connected";
        isConnected = true;
        socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
        sendText("//connected");
        showGameInterface();
    }
    else
        server->nextPendingConnection();
}

void Game::slotReadyRead()
{
    if (!isConnected)
        showGameInterface();
    isConnected = true;
    QDataStream in(socket);
    while (true)
    {
        if (blockSize == 0)
        {
            if (socket->bytesAvailable() >= sizeof(qint16))
                in >> blockSize;
            else
                break;
        }
        if (socket->bytesAvailable() >= blockSize)
        {
            QString str;
            in >> str;
            readText(str);
            blockSize = 0;
        }
        else
        {
            break;
        }
    }
}

void Game::showGameInterface()
{
    showField();
    showChat();
    showShips();
    showButtons();
    condition = BEFOREGAME;
    iReady = false;
    opponentReady = false;
}

void Game::hideGameInterface()
{
    hideField();
    hideChat();
    hideShips();
    hideButtons();
}

void Game::showField()
{
    qreal w = scene->width();
    qreal h = scene->height();
    qreal _size = h / 2;
    field1 = new Field(_size);
    scene->addItem(field1);
    qreal x1 = scene->sceneRect().left();
    qreal y1 = scene->sceneRect().top();
    field1->setPos(x1 + _size / 2 + 30, y1 + _size / 2 + 30);

    qreal x2 = scene->sceneRect().right();
    field2 = new Field(_size);
    scene->addItem(field2);
    field2->setPos(x2 - _size / 2 - 30, y1 + _size / 2 + 30);
}

void Game::hideField()
{
    scene->removeItem(field1);
    scene->removeItem(field2);
}

void Game::showShips()
{
    qreal w = scene->width();
    qreal h = scene->height();
    qreal _size = h / 2;
    qreal x = _size / 8;
    qreal xBegin = scene->sceneRect().left() + _size + 30 + 100;
    qreal currY = scene->sceneRect().top() + 30;
    qreal currX;

    Ship *ship;
    int n;

    n = 4;
    ship = new Ship(n, x, field1);
    ships.push_back(ship);
    scene->addItem(ship);
    ship->setPos(xBegin + n * x / 2, currY + x / 2);

    n = 3;
    currY += 2 * x;
    ship = new Ship(n, x, field1);
    ships.push_back(ship);
    scene->addItem(ship);
    ship->setPos(xBegin + n * x / 2, currY + x / 2);

    currY += 2 * x;
    n = 2;
    currX = xBegin;
    for (int i = 0; i < 3; ++i)
    {
        ship = new Ship(n, x, field1);
        ships.push_back(ship);
        scene->addItem(ship);
        ship->setPos(currX + n * x / 2, currY + x / 2);
        currX += n * x + x;
    }

    currY += 2 * x;
    n = 1;
    currX = xBegin;
    for (int i = 0; i < 4; ++i)
    {
        ship = new Ship(n, x, field1);
        ships.push_back(ship);
        scene->addItem(ship);
        ship->setPos(currX + n * x / 2, currY + x / 2);
        currX += n * x + x;
    }
}

void Game::hideShips()
{
    for (int i = 0; i < ships.size(); ++i)
        scene->removeItem(ships[i]);
    ships.clear();
}

void Game::showButtons()
{
    QGroupBox *grp = new QGroupBox;
    QHBoxLayout *hl = new QHBoxLayout;
    QPushButton *b0 = new QPushButton("random");
    QPushButton *b1 = new QPushButton("run");
    QPushButton *b2 = new QPushButton("menu");
    hl->addWidget(b0);
    hl->addWidget(b1);
    hl->addWidget(b2);
    grp->setLayout(hl);
    buttonsWidget = scene->addWidget(grp);
    QRectF rect = scene->sceneRect();
    buttonsWidget->resize(rect.width() / 10, rect.height() / 10);
    buttonsWidget->setPos(rect.right() - 30 - buttonsWidget->size().width(),
                          rect.bottom() - 30 - buttonsWidget->size().height());
    connect(b0, SIGNAL(clicked(bool)), this, SLOT(slotRandomShuffle()));
    connect(b1, SIGNAL(clicked(bool)), this, SLOT(slotRun()));
    connect(b2, SIGNAL(clicked(bool)), this, SLOT(slotMenu()));
}

void Game::hideButtons()
{
    scene->removeItem(buttonsWidget);
    buttonsWidget = 0;
}

void Game::sendMessage()
{
    QString str = chatLine->text();
    chatLine->clear();
    chatText->append("My: " + str);
    sendText('/' + str);
}

void Game::showChat()
{
    chatLine = new QLineEdit;
    chatText = new QTextEdit;
    chatText->setReadOnly(true);
    QPushButton *btn = new QPushButton("send");
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(sendMessage()));

    QGroupBox *grpBox = new QGroupBox;
    QVBoxLayout *vbxl = new QVBoxLayout;
    vbxl->addWidget(chatText);
    vbxl->addWidget(chatLine);
    vbxl->addWidget(btn);
    grpBox->setLayout(vbxl);

    qreal w = scene->width();
    qreal h = scene->height();
    qreal x1 = scene->sceneRect().left();
    qreal y1 = scene->sceneRect().top();
    qreal y2 = scene->sceneRect().bottom();
    qreal _size = h / 2;

    chatWidget = scene->addWidget(grpBox);
    chatWidget->resize(_size, h / 3);
    chatWidget->setPos(x1 + 10, y2 - 10 - h / 3);
}

void Game::hideChat()
{
    scene->removeItem(chatWidget);
    chatText = 0;
    chatLine = 0;
}

void Game::enterPress()
{
    if ((condition == BEFOREGAME || condition == GAME) && chatLine->hasFocus())
    {
        sendMessage();
    }
    if (condition == GETIP)
    {
        emit signalEnterPress();
    }
}

void Game::slotMenu()
{
    hideGameInterface();
    if (server)
        delete server;
    else
        delete socket;
    showMenu();
}

void Game::runGame()
{
    condition = GAME;
    isMyTurn = -1;
    connect(field2, SIGNAL(clicked(int,int)), this, SLOT(slotClickedOnCell(int,int)));
    if (server)
    {
        int x = qrand() % 2;
        if (x == 0)
        {
            sendText(tr("1"));
            isMyTurn = 0;
        }
        else
        {
            sendText(tr("0"));
            isMyTurn = 1;
        }
    }
}

void Game::slotClickedOnCell(int x, int y)
{
    if (isMyTurn == 1)
    {
        if (field2->getIsDamage(x, y))
            return;
        QString str = "S";
        str.append(x + '0');
        str.append(y + '0');
        sendText(str);
        isMyTurn = false;
    }
}

void Game::processOpponentShoot(int x, int y)
{
    int r = field1->damage(x, y);
    QString ans = "R";
    if (r == 0)
        ans.append('P');
    if (r == 1)
        ans.append('D');
    if (r == 2)
        ans.append('K');
    ans.append(x + '0');
    ans.append(y + '0');
    sendText(ans);
    if (r == 0)
        isMyTurn = true;
}

void Game::processWePass(int x, int y)
{
    field2->setIsPass(x, y);
}

void Game::processWeDamage(int x, int y)
{
    field2->setIsDamage(x, y);
    isMyTurn = true;
}

void Game::processWeKill(int x, int y)
{
    field2->setIsDamage(x, y);
    QRect r = field2->getOpponentShip(x, y);
    field2->addShip(r);
    isMyTurn = true;
}

void Game::slotRun()
{
    if (iReady)
        return;
    for (int i = 0; i < (int)ships.size(); ++i)
    {
        if (!ships[i]->onField())
            return;
    }
    for (int i = 0; i < (int)ships.size(); ++i)
    {
        ships[i]->setFixed();
    }
    sendText("R");
    iReady = true;
    if (iReady && opponentReady)
    {
        runGame();
    }
}

void Game::slotRandomShuffle()
{
    field1->setShipsRandom(&ships);
}

Game::~Game()
{

}
