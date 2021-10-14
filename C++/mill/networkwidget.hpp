#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QThread>
#include "netconfig.hpp"
#include "comconfig.hpp"

namespace Ui {
class NetworkWidget;
}

enum class OperationMode {Client, Server};
enum class NetStat {Ready, AskGameSent, GameBeginSent, OkSent, InGame};

/// main window for network/game settings and connection
class NetworkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkWidget(QWidget *parent = nullptr);
    ~NetworkWidget();

    /// start game for debugging
    void testStart();

private:
    Ui::NetworkWidget *ui;
    OperationMode _opMode;
    GameMode _gameMode;
    QString _addr;
    quint16 _localPort;    
    bool _connected;
    QTcpSocket _socket;
    QDataStream _stream;
    NetStat _netStat;

    /// acquire ip address and port number from line edits
    bool setAddrPort();

    /// send data bytes through connection
    /// @param data to send in QByteArray
    void send(QByteArray data);

    /// receive data bytes through connection
    /// @returns data in QByteArray
    QByteArray receive();

    /// receive data bytes through connection and return QString
    /// @returns QString
    QString receiveQStr();

    /// switch state between connected and disconnected
    void switchState();

    /// set gui elements to server operation mode
    void setToServer();

    /// set gui elements to client operation mode
    void setToClient();

    /// response in network settings for server, part method of response
    /// @param msg to send in QByteArray
    void resNetServer(QByteArray msg);

    /// response in network settings for client, part method of response
    /// @param msg to send in QByteArray
    void resNetClient(QByteArray msg);

    /// response in game, part method of response
    /// @param msg to send in QByteArray
    void resGame(QByteArray msg);

private slots:
    /// function called when Start Game pressed
    void onStartGame();

    /// connect to the proxy server, called when Connect button clicked
    void connectDisconnect();

    /// function called when Operation Mode(server or client) toggled
    void onToggleGameMode();

    /// function called when Game Mode toggled
    void onToggleOperationMode();

    /// set state connected
    void onConnected();

    /// set state disconnected
    void onDisconnected();

    /// set state eror
    void onError();

    /// receive data and interpret to play game
    void response();

    /// wrapper function for send() for signal from logic
    /// @param msg to send in QByteArray
    void sendMsg(QByteArray msg);

signals:
    /// signal to start game and open GameWidget
    void startGame(bool meFirst);

    /// signal for game message during the game between NetworkWidget and GameWidget
    /// @param msg to send in QByteArray
    void gameMsg(QByteArray msg);

    /// signal to send port number to GameWidget for identifying
    /// @param port
    void portNotice(quint16 port);
};

#endif // NEWORKWIDGET_H
