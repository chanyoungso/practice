#include <QApplication>
#include <QDebug>
#include "gamewidget.hpp"
#include "networkwidget.hpp"
#include "logic.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkWidget net;
    GameWidget gui;
    Logic logic;

    // Initial connection
    QObject::connect(&net, SIGNAL(startGame(bool)), &gui, SLOT(setBeginner(bool)));
    QObject::connect(&net, SIGNAL(startGame(bool)), &logic, SLOT(setBeginner(bool)));
    QObject::connect(&net, SIGNAL(startGame(bool)), &net, SLOT(hide()));
    QObject::connect(&net, SIGNAL(startGame(bool)), &logic, SLOT(startGame()));
    QObject::connect(&net, SIGNAL(portNotice(quint16)), &gui, SLOT(setPort(quint16)));

    // Disconnection
    QObject::connect(&gui, SIGNAL(showNetSignal()), &net, SLOT(show()));

    // logic <> gui
    QObject::connect(&logic, SIGNAL(placeOppStoneSig(hqcoord_t)), &gui, SLOT(placeOppStone(hqcoord_t)));
    QObject::connect(&logic, SIGNAL(placeMyStoneSig(hqcoord_t)), &gui, SLOT(placeMyStone(hqcoord_t)));
    QObject::connect(&logic, SIGNAL(moveStoneSig(hqcoord_t, hqcoord_t)), &gui, SLOT(moveStone(hqcoord_t, hqcoord_t)));
    QObject::connect(&logic, SIGNAL(userCanRemoveStone()), &gui, SLOT(actRemove()));
    QObject::connect(&logic, SIGNAL(removeStoneSig(hqcoord_t)), &gui, SLOT(removeStone(hqcoord_t)));
    QObject::connect(&logic, SIGNAL(guiErrorMsg(std::string&)), &gui, SLOT(onRejected(std::string&)));
    QObject::connect(&logic, SIGNAL(phaseNotice(GamePhase)), &gui, SLOT(setPhase(GamePhase)));
    QObject::connect(&logic, SIGNAL(changeTurn(bool)), &gui, SLOT(changeTurn(bool)));
    QObject::connect(&logic, SIGNAL(gameOver(bool)), &gui, SLOT(gameOver(bool)));
    QObject::connect(&gui, SIGNAL(phaseInquiry()), &logic, SLOT(getPhase()));
    QObject::connect(&gui, SIGNAL(placeMyStoneRequest(hqcoord_t)), &logic, SLOT(placeMyStoneReq(hqcoord_t)));
    QObject::connect(&gui, SIGNAL(moveMyStoneRequest(hqcoord_t,hqcoord_t)), &logic, SLOT(moveMyStoneReq(hqcoord_t,hqcoord_t)));
    QObject::connect(&gui, SIGNAL(removeStoneRequest(hqcoord_t)), &logic, SLOT(removeOppStoneReq(hqcoord_t)));

    // net <> logic
    QObject::connect(&net, SIGNAL(gameMsg(QByteArray)), &logic, SLOT(handleNetworkCommand(QByteArray)));
    QObject::connect(&logic, SIGNAL(sendToNetwork(QByteArray)), &net, SLOT(sendMsg(QByteArray)));

    // for debugging and test
//    gui.show();

    // program start
    net.show();
    return a.exec();
}
