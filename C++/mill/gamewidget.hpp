#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QGraphicsScene>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <memory>
#include "gestone.hpp"
#include "gesegment.hpp"
#include "networkwidget.hpp"
#include "netconfig.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

/// game widget window
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
    QGraphicsScene *board;    
    std::shared_ptr<GESignalHandler> handler;
    std::vector<std::shared_ptr<GEStone>> myStones;
    std::vector<std::shared_ptr<GEStone>> oppStones;
    std::vector<std::shared_ptr<GESegment>> segments;
    std::map<hqcoord_t, bcoord_t> hqToBMap;
    std::map<bcoord_t, hqcoord_t> bToHqMap;    
    const hqcoord_t nullCoord = std::make_pair(0, 0);
    std::shared_ptr<GEStone> _selectedStone;
    GamePhase _gamePhase;    
    bsize_t _myStoneCount;
    bsize_t _oppStoneCount;
    bool _myTurn;    
    bool _removeActive;

    /// disable stone
    /// @param stone reference to a Stone object
    /// @returns boolean if the operation is successful
    bool lockStone(bcoord_t bcoord);

    /// enable stone
    /// @param stone reference to a Stone object
    /// @returns boolean if the operation is successful
    bool unlockStone(bcoord_t bcoord);

    /// disable segment
    /// @param bcoord board coordinate in bcoord_t type
    /// @returns boolean if the operation is successful
    bool lockSegment(bcoord_t bcoord);

    /// disable segment
    /// @param x x_coordinate in bsize_t type
    /// @param y y_coordinate in bsize_t type
    /// @returns boolean if the operation is successful
    bool lockSegment(bsize_t x, bsize_t y);

    /// enable segment
    /// @param bcoord board coordinate in bcoord_t type
    /// @returns boolean if the operation is successful
    bool unlockSegment(bcoord_t bcoord);

    /// enable segment
    /// @param x x_coordinate in bsize_t type
    /// @param y y_coordinate in bsize_t type
    /// @returns boolean if the operation is successful
    bool unlockSegment(bsize_t x, bsize_t y);

    /// disable all graphic elements
    void lockAll();

    /// enable all graphic elements
    void unlockAll();

    /// list up all existing stones dead or alive in console
    void listUpStones();

    /// show massage in msgLabel
    /// @param msg reference to std::string
    void showMsg(const std::string& msg);

    /// verify the coordinate whethe it is valid
    /// @param bcoord coordinate to verify in bcoord_t
    /// @returns true if valid
    bool verifyBCoord(bcoord_t bcoord);

    /// convert coordinate from Cartesian to Himmelsrichtung-Quadrat
    /// @param bcoord in bcoord_t type
    /// @returns coordinate in hqcoord_t type
    hqcoord_t bToHqCoord(bcoord_t bcoord);

    /// convert coordinate from Cartesian to Himmelsrichtung-Quadrat
    /// @param x x_coordinate in bsize_t type
    /// @param y y_coordinate in bsize_t type
    /// @returns coordinate in hqcoord_t type
    hqcoord_t bToHqCoord(bsize_t x, bsize_t y);

    /// convert coordinate from Himmelsrichtung-Quadrat to Cartesian
    /// @param hqcoord in hqcoord_t type
    /// @returns coordinate in bcoord_t type
    bcoord_t hqToBCoord(hqcoord_t hqcoord);

    /// convert coordinate from Himmelsrichtung-Quadrat to Cartesian
    /// @param h Himmelsrichtung coordinate in bsize_t type
    /// @param y Quadrat coordinate in bsize_t type
    /// @returns coordinate in bcoord_t type
    bcoord_t hqToBCoord(bsize_t h, bsize_t q);

    /// generate a map of HQ, XY coordinate pair with given coordinate values
    /// @param h Himmelsrichting
    /// @param q Quadrat
    /// @param x x_coordinate in Cartesian
    /// @param y y_coordinate in Cartesian
    /// @returns nothing, save results in hqToBMap and bToHqMap
    void mapCoord(bsize_t h, bsize_t q, bsize_t x, bsize_t y);

    /// gererate the entire mapping data of HQ, XY coordinate
    void generateCoordMap();

    /// create a segment on the game board
    /// @param bcoord in bcoord_t type
    void createSegment(bcoord_t bcoord);

    /// draw all segments ont the game board
    void drawSegments();

    /// draw the game board
    void drawBoard();

    /// fit the size of game board(QGraphicsScene) to GGraphicsView obejct
    void fitBoard();

    /// draw all lines on the game board
    void drawBoardLines();

    /// draw all points on the game board
    void drawBoardPoints();

    /// add pair of two points into lines vector in Cartesian
    /// @param lines a reference to a vector of pairs of two points
    /// @param x1 x_coordinate of the first point in bsize_t type
    /// @param y1 y_coordinate of the first point in bsize_t type
    /// @param x2 x_coordinate of the second point in bsize_t type
    /// @param y2 y_coordinate of the second point in bsize_t type
    void addLine(std::vector<bcpair_t>& lines, bsize_t x1, bsize_t y1, bsize_t x2, bsize_t y2);

    /// add point into points vector in Cartesian
    /// @param points a reference to a vector of points
    /// @param x
    /// @param y
    void addPoint(std::vector<bcoord_t>& points, bsize_t x, bsize_t y);

    /// find graphic element stone with stone ID
    /// @param stone reference to a Stone object
    /// @returns a pointer to a graphic element stone
    std::shared_ptr<GEStone> findStone(bcoord_t bcoord);

    /// print information of a graphic element stone in console
    /// @param gestone a pointer to a graphic element stone
    void infoStone(std::shared_ptr<GEStone> gestone);

private slots:
    /// function to call to emit stoneClicked signal
    /// @param bcoord the coordinate of clicked stone
    void onStoneClicked(bsize_t x, bsize_t y);

    /// function to call to emit segmentClicked signal
    /// @param bcoord the coordinate of clicked segment
    void onSegmentClicked(bsize_t x, bsize_t y);

    /// function to call to emit show network settings signal
    void onShowNet();

    /// create my stone on the game board in placing phase
    /// @param bcoord the coordinate of a new stone
    /// @returns boolean if the operation is successful
    bool placeMyStone(hqcoord_t hqcoord);

    /// create rival's stone on the game board in placing phase
    /// @param bcoord the coordinate of a new stone
    /// @returns boolean if the operation is successful
    bool placeOppStone(hqcoord_t hqcoord);

    /// change the coordinate of my stone
    /// @param bcoordFrom the current coordinate of the stone
    /// @param bcoordTo new coordinate of the stone
    /// @returns boolean if the operation is successful
    bool moveStone(hqcoord_t hqcoordFrom, hqcoord_t hqcoordTo);

    /// remove stone from the game board
    /// @param stone reference to a Stone object
    /// @returns boolean if the operation is successful
    bool removeStone(hqcoord_t hqcoord);

    /// function called if the request rejected by logic
    /// @param msg reference to std::string
    void onRejected(std::string &msg);

    /// enquire phase to logic
    void enquirePhase();

    /// set game phase
    /// @param phase GamePhase to set
    void setPhase(GamePhase phase);

    /// activate remove mode
    void actRemove();

    /// set as beginner
    void setBeginner(bool meFirst);

    /// set port number
    void setPort(quint16 port);

    /// change turn
    void changeTurn(bool myTurn);

    /// unselect stone
    void unselectStone();

    /// end game
    /// @param youWin
    void gameOver(bool youWin);

//    /// start a new game
//    void newGame();

signals:    
    /// signal to show network settings
    void showNetSignal();

    /// signal to enquire current phase to logic
    void phaseInquiry();

    /// signal to request to place my stone
    /// @param hqcoord the coordinate to place in hqcoord_t
    void placeMyStoneRequest(hqcoord_t hqcoord);

    /// signal to request to move my stone
    /// @param hqcoordFrom the current coordinate of the stone in hqcoord_t
    /// @param hqcoordTo new coordinate of the stone in hqcoord_t
    void moveMyStoneRequest(hqcoord_t hqcoordFrom, hqcoord_t hqcoordTo);

    /// signal to request to remove my stone
    /// @param hqcoord the current coordinate of the stone to remove in hqcoord_t
    void removeStoneRequest(hqcoord_t hqcoord);    

    /// inform logic that turn is over
    void turnChangeNotice(bool myTurn);
};
#endif // GAMEWIDGET_H
