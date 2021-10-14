#include "networkwidget.hpp"
#include "ui_networkwidget.h"

NetworkWidget::NetworkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkWidget), _opMode{OperationMode::Client}, _gameMode{GameMode::ClientFirst},
    _connected{false}, _addr{""}, _localPort{0}, _netStat{NetStat::Ready}
{
    ui->setupUi(this);
    _stream.setDevice(&_socket);
    switchState();
    connect(ui->buttonStartGame, SIGNAL(clicked()), this, SLOT(onStartGame()));
    connect(ui->radioButtonClientOperation, SIGNAL(clicked()), this, SLOT(onToggleOperationMode()));
    connect(ui->radioButtonServerOperation, SIGNAL(clicked()), this, SLOT(onToggleOperationMode()));
    connect(ui->radioButtonClientFirst, SIGNAL(clicked()), this, SLOT(onToggleGameMode()));
    connect(ui->radioButtonServerFirst, SIGNAL(clicked()), this, SLOT(onToggleGameMode()));
    connect(ui->radioButtonRandom, SIGNAL(clicked()), this, SLOT(onToggleGameMode()));
    connect(ui->buttonConnect, SIGNAL(clicked()), this, SLOT(connectDisconnect()));
    connect(&_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
    connect(&_socket, SIGNAL(readyRead()), this, SLOT(response()));    
    connect(ui->lineEditLocalPort, SIGNAL(returnPressed()), this, SLOT(connectDisconnect()));    
}

NetworkWidget::~NetworkWidget()
{
    delete ui;
}

void NetworkWidget::testStart() {
    emit startGame(true);
}

bool NetworkWidget::setAddrPort()
{
    bool okPort;    
    _addr = "localhost";
    _localPort = ui->lineEditLocalPort->text().toUShort(&okPort);
    return okPort;
}

void NetworkWidget::send(QByteArray data)
{
    _stream.writeRawData(data, data.size());    
}

QByteArray NetworkWidget::receive()
{
    return _socket.readAll();
}

QString NetworkWidget::receiveQStr()
{
    QByteArray received = receive();
    return QString::fromUtf8(received.constData(), received.length());
}

void NetworkWidget::switchState()
{
    ui->labelLocalPort->setDisabled(_connected);    
    ui->lineEditLocalPort->setDisabled(_connected);
    ui->groupBoxOperationMode->setDisabled(_connected);

    if ( _opMode == OperationMode::Client ) {
        ui->buttonStartGame->setEnabled(_connected);
    }

    if ( _connected ) {
        ui->buttonConnect->setText("Disconnect");
    }
    else {
        ui->buttonConnect->setText("Connect");
    }
}

void NetworkWidget::setToServer()
{
    _opMode = OperationMode::Server;     
    ui->groupBoxGameMode->setEnabled(true);
    ui->buttonStartGame->setText("Start Game");
#ifdef DEBUG
    // for test
    ui->lineEditLocalPort->setText(SERVER_PORT);
#endif
}

void NetworkWidget::setToClient()
{
    _opMode = OperationMode::Client;    
    ui->groupBoxGameMode->setEnabled(false);
    ui->buttonStartGame->setText("Ask Game");
#ifdef DEBUG
    // for test
    ui->lineEditLocalPort->setText(CLIENT_PORT);
#endif
}

void NetworkWidget::resNetServer(QByteArray msg)
{
    QByteArray res;
    quint8 comByte = msg.at(0);
    quint8 lenByte = msg.at(1);    
    bool meFirst = false;

    if ( _netStat == NetStat::Ready && comByte == HEX_ASK_CON ) {
        if ( lenByte == HEX_LEN_ASK_CON ) {
            ui->buttonStartGame->setEnabled(true);
        }
    }
    if ( _netStat == NetStat::GameBeginSent && comByte == HEX_OK) {
        if ( lenByte == HEX_LEN_OK ) {
            _netStat = NetStat::InGame;
            switch(_gameMode) {
                case GameMode::ClientFirst:
                    meFirst = false;
                    break;
                case GameMode::ServerFirst:
                    meFirst = true;
                    break;
                case GameMode::Random:
                    meFirst = false;
                    break;
            }
            emit startGame(meFirst);
        }
    }
    ui->groupBoxGameMode->setEnabled(true);
}

void NetworkWidget::resNetClient(QByteArray msg)
{
    QByteArray res;
    quint8 comByte = msg.at(0);
    quint8 lenByte = msg.at(1);
    bool meFirst = false;

    if ( _netStat == NetStat::AskGameSent && comByte == HEX_GAME ) {
        if ( lenByte == HEX_LEN_GAME ) {
            quint8 gameModeByte = msg.at(2);
            switch (gameModeByte) {
                case HEX_GAME_C:
                    _gameMode = GameMode::ClientFirst;
                    meFirst = true;
                    break;
                case HEX_GAME_S:
                    _gameMode = GameMode::ServerFirst;
                    meFirst = false;
                    break;
                case HEX_GAME_R:
                    _gameMode = GameMode::Random;                    
                    meFirst = true; // test
                    break;
            }
            quint8 groupByte = msg.at(3);
            res.append(HEX_OK);
            res.append(HEX_LEN_OK);
            res.append(groupByte);
            send(res);
            _netStat = NetStat::InGame;
            emit startGame(meFirst);
        }
    }    
}

void NetworkWidget::resGame(QByteArray msg)
{
    emit gameMsg(msg);
}

void NetworkWidget::onStartGame()
{    
    QByteArray msg;
    switch(_opMode) {
        case OperationMode::Client:            
            msg.append(HEX_ASK_CON);
            msg.append(HEX_LEN_ASK_CON);
            _netStat = NetStat::AskGameSent;
            break;
        case OperationMode::Server:
            msg.append(HEX_GAME);
            msg.append(HEX_LEN_GAME);
            switch(_gameMode) {
                case GameMode::ClientFirst:
                    msg.append(HEX_GAME_C);
                    break;
                case GameMode::ServerFirst:
                    msg.append(HEX_GAME_S);                    
                    break;
                case GameMode::Random:
                    msg.append(HEX_GAME_R);
                    break;
                default:
                    msg.append(HEX_GAME_C);
            }
            msg.append(HEX_GROUP);
            _netStat = NetStat::GameBeginSent;
            break;
    }
    send(msg);
    ui->groupBoxGameMode->setEnabled(false);
}

void NetworkWidget::connectDisconnect()
{
    qDebug() << "Starting connection...";
    setAddrPort();
    ui->buttonConnect->setEnabled(false);
    if ( _connected ) {
        _socket.disconnectFromHost();
    }
    else {
        qDebug() << "Connecting...";
        if ( setAddrPort() ) {
            _socket.connectToHost(_addr, _localPort);
            if ( _socket.waitForConnected(3000000) ) {
                ui->buttonConnect->setEnabled(true);
            }
            else {
                qDebug() << "Connection failed.";
                ui->buttonConnect->setEnabled(true);
            }
        }

        else {
            qDebug() << "Invalid port number.";
            ui->buttonConnect->setEnabled(true);
        }
    }
}

void NetworkWidget::onToggleGameMode()
{
    if ( ui->radioButtonClientFirst->isChecked() ) {
        _gameMode = GameMode::ClientFirst;
    }

    else if ( ui->radioButtonServerFirst->isChecked() ) {
        _gameMode = GameMode::ServerFirst;
    }

    else if ( ui->radioButtonRandom->isChecked() ) {
        _gameMode = GameMode::Random;
    }
}

void NetworkWidget::onToggleOperationMode()
{
    if ( ui->radioButtonClientOperation->isChecked() ) {        
        setToClient();
    }
    else if ( ui->radioButtonServerOperation->isChecked() ) {        
        setToServer();
    }
}

void NetworkWidget::onConnected()
{
    qDebug() << "Connected.";
    _connected = true;
    switchState();
    ui->buttonConnect->setEnabled(true);    
    if ( _opMode == OperationMode::Client ) {
        ui->buttonStartGame->setEnabled(true);
    }
    emit portNotice(_localPort);
}

void NetworkWidget::onDisconnected()
{
    qDebug() << "Disconnected.";
    _connected = false;
    switchState();
    ui->buttonConnect->setEnabled(true);
}

void NetworkWidget::onError()
{
    qDebug() << "Connecting error or interrupt.";
    _connected = false;
    switchState();
    ui->buttonConnect->setEnabled(true);
}

void NetworkWidget::response()
{
    QByteArray received = receive();    
    if (_netStat == NetStat::InGame ) {
        resGame(received);
    }
    else {
        switch (_opMode) {
        case OperationMode::Client:
            resNetClient(received);
            break;

        case OperationMode::Server:
            resNetServer(received);
            break;
        }
    }
}

void NetworkWidget::sendMsg(QByteArray msg) {
    send(msg);
}
