#include "gamewidget.hpp"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget),
      _gamePhase{GamePhase::Place}, _myTurn{false}, _removeActive{false},
      _myStoneCount{0}, _oppStoneCount{0}, _selectedStone{nullptr}
{
    ui->setupUi(this);    
    handler = std::make_shared<GESignalHandler>();    
    drawBoard();
    drawSegments();
    generateCoordMap();
    connect(handler.get(), SIGNAL(segmentClicked(bsize_t, bsize_t)), this, SLOT(onSegmentClicked(bsize_t, bsize_t)));
    connect(handler.get(), SIGNAL(stoneClicked(bsize_t, bsize_t)), this, SLOT(onStoneClicked(bsize_t, bsize_t)));
    connect(ui->buttonShowNet, SIGNAL(clicked()), this, SLOT(onShowNet()));
}

GameWidget::~GameWidget()
{
    delete ui;
}

bool GameWidget::lockStone(bcoord_t bcoord)
{
    std::shared_ptr<GEStone> gestone = findStone(bcoord);
    if ( gestone == nullptr ) {
        qDebug() << "Stone lock failed.";
        return false;
    }
    else if ( gestone->isEnabled() ) {
        gestone->lock();
        qDebug() << "Stone successfully locked.";
        return true;
    }
    else {
        qDebug() << "Stone is already locked.";
        return false;
    }
}

bool GameWidget::unlockStone(bcoord_t bcoord)
{
    std::shared_ptr<GEStone> gestone = findStone(bcoord);
    if ( gestone == nullptr ) {
        qDebug() << "Unlock failed.";
        return false;
    }
    else if ( !( gestone->isEnabled() ) ) {
        gestone->unlock();
        qDebug() << "Stone successfully unlocked.";
        return true;
    }
    else {
        qDebug() << "Stone is aleady unlocked.";
        return false;
    }
}

bool GameWidget::lockSegment(bcoord_t bcoord)
{
    for ( auto iter : segments ) {
        if ( iter->getBCoord() == bcoord ) {
            if ( iter->isEnabled() ) {
                iter->lock();
                qDebug() << "Segment successfully locked.";
                return true;
            }
            else {
                qDebug() << "Segment already locked.";
                return false;
            }
        }
    qDebug() << "Segment lock failed.";
    return false;
    }
}

bool GameWidget::lockSegment(bsize_t x, bsize_t y)
{
    bcoord_t bcoord = std::make_pair(x, y);
    return lockSegment(bcoord);
}

bool GameWidget::unlockSegment(bcoord_t bcoord)
{
    for ( auto iter : segments ) {
        if ( iter->getBCoord() == bcoord ) {
            if ( !(iter->isEnabled()) ) {
                iter->unlock();
                qDebug() << "Segment successfully unlocked.";
                return true;
            }
            else {
                qDebug() << "Segment already unlocked.";
                return false;
            }
        }
    }
    qDebug() << "Segment unlock failed.";
    return false;
}

bool GameWidget::unlockSegment(bsize_t x, bsize_t y)
{
    bcoord_t bcoord = std::make_pair(x, y);
    return unlockSegment(bcoord);
}

void GameWidget::lockAll()
{
    for(auto iter : myStones) {
        iter->lock();
    }
    for(auto iter : oppStones) {
        iter->lock();
    }
    for(auto iter : segments) {
        iter->lock();
    }
}

void GameWidget::unlockAll()
{
    for(auto iter : myStones) {
        iter->unlock();
    }
    for(auto iter : oppStones) {
        iter->unlock();
    }
    for(auto iter : segments) {
        iter->unlock();
    }
}

void GameWidget::listUpStones()
{
    qDebug() << "===== My Stones =====";
    for (auto iter : myStones) {
        infoStone(iter);
        qDebug() << "";
    }
    qDebug() << "";
    qDebug() << "===== Rival's Stones =====";
    for (auto iter : oppStones) {
        infoStone(iter);
        qDebug() << "";
    }
}

void GameWidget::showMsg(const std::string& msg)
{
    ui->labelMsg->setText(QString::fromStdString(msg));
}

bool GameWidget::verifyBCoord(bcoord_t bcoord)
{
    return bToHqCoord(bcoord) != nullCoord;
}

hqcoord_t GameWidget::bToHqCoord(bcoord_t bcoord)
{
    if ( bToHqMap.find(bcoord) == bToHqMap.end() ){
        qDebug() << "Invalid board coordinate input. Set to (1, 1).";
        return std::make_pair(0, 0);
    }
    else {
        return bToHqMap[bcoord];
    }
}

hqcoord_t GameWidget::bToHqCoord(bsize_t x, bsize_t y)
{
    return bToHqCoord(std::make_pair(x, y));
}

bcoord_t GameWidget::hqToBCoord(hqcoord_t hqcoord)
{
    if ( hqToBMap.find(hqcoord) == hqToBMap.end() ) {
        qDebug() << "Invalid hq coordinate input. Set to (0, 0)";
        return std::make_pair(0, 0);
    }
    else {
        return hqToBMap[hqcoord];
    }
}

bcoord_t GameWidget::hqToBCoord(bsize_t h, bsize_t q)
{
    return hqToBCoord(std::make_pair(h, q));
}

void GameWidget::mapCoord(bsize_t h, bsize_t q, bsize_t x, bsize_t y)
{
    hqToBMap[std::make_pair(h, q)] = std::make_pair(x, y);
    bToHqMap[std::make_pair(x, y)] = std::make_pair(h, q);
}

void GameWidget::generateCoordMap()
{
    // h, q, x, y
    mapCoord(1, 1, 4, 1);
    mapCoord(2, 1, 7, 1);
    mapCoord(3, 1, 7, 4);
    mapCoord(4, 1, 7, 7);
    mapCoord(5, 1, 4, 7);
    mapCoord(6, 1, 1, 7);
    mapCoord(7, 1, 1, 4);
    mapCoord(8, 1, 1, 1);
    mapCoord(1, 2, 4, 2);
    mapCoord(2, 2, 6, 2);
    mapCoord(3, 2, 6, 4);
    mapCoord(4, 2, 6, 6);
    mapCoord(5, 2, 4, 6);
    mapCoord(6, 2, 2, 6);
    mapCoord(7, 2, 2, 4);
    mapCoord(8, 2, 2, 2);
    mapCoord(1, 3, 4, 3);
    mapCoord(2, 3, 5, 3);
    mapCoord(3, 3, 5, 4);
    mapCoord(4, 3, 5, 5);
    mapCoord(5, 3, 4, 5);
    mapCoord(6, 3, 3, 5);
    mapCoord(7, 3, 3, 4);
    mapCoord(8, 3, 3, 3);
}

void GameWidget::createSegment(bcoord_t bcoord)
{
    std::shared_ptr<GESegment> segment = std::make_shared<GESegment>(bcoord, handler);
    segments.push_back(segment);
    board->addItem(segment.get());
}

void GameWidget::drawSegments()
{
    for (bsize_t i = 0; i < NUM_SEG; i++) {
        for (bsize_t j = 0; j < NUM_SEG; j++) {
            createSegment( std::make_pair(i, j) );
        }
    }
}

void GameWidget::drawBoard()
{
    board = new QGraphicsScene(this);
    ui->view->setScene(board);
    board->setBackgroundBrush(QColor(244, 164, 96, 255));
    drawBoardLines();
    drawBoardPoints();
}

void GameWidget::fitBoard()
{
    board->setSceneRect(0,0, ui->view->frameSize().width(), ui->view->frameSize().height());
}

void GameWidget::drawBoardLines()
{
    std::vector<bcpair_t> lines;
    qreal xSegLen = VIEW_LEN / NUM_SEG;
    qreal ySegLen = VIEW_LEN / NUM_SEG;

    QPen pen(Qt::black, 3);
    addLine(lines, 1, 1, 7, 1);
    addLine(lines, 2, 2, 6, 2);
    addLine(lines, 3, 3, 5, 3);
    addLine(lines, 1, 4, 3, 4);
    addLine(lines, 5, 4, 7, 4);
    addLine(lines, 3, 5, 5, 5);
    addLine(lines, 2, 6, 6, 6);
    addLine(lines, 1, 7, 7, 7);
    addLine(lines, 1, 1, 1, 7);
    addLine(lines, 2, 2, 2, 6);
    addLine(lines, 3, 3, 3, 5);
    addLine(lines, 4, 1, 4, 3);
    addLine(lines, 4, 5, 4, 7);
    addLine(lines, 5, 3, 5, 5);
    addLine(lines, 6, 2, 6, 6);
    addLine(lines, 7, 1, 7, 7);

    for (auto& iter : lines) {
        qreal x1FromLeftTop = ( iter.first.first * xSegLen + (xSegLen/2) );
        qreal y1FromLeftTop = ( iter.first.second * ySegLen + (ySegLen/2) );
        qreal x2FromLeftTop = ( iter.second.first * xSegLen + (xSegLen/2) );
        qreal y2FromLeftTop = ( iter.second.second * ySegLen + (ySegLen/2) );
        board->addLine(x1FromLeftTop, y1FromLeftTop, x2FromLeftTop, y2FromLeftTop, pen);
    }
}

void GameWidget::drawBoardPoints()
{
    std::vector<bcoord_t> points;
    qreal xSegLen = VIEW_LEN / NUM_SEG;
    qreal ySegLen = VIEW_LEN / NUM_SEG;

    QPen pen(Qt::black, 3);
    QBrush brush(Qt::black);
    addPoint(points, 1, 1);
    addPoint(points, 4, 1);
    addPoint(points, 7, 1);
    addPoint(points, 2, 2);
    addPoint(points, 4, 2);
    addPoint(points, 6, 2);
    addPoint(points, 3, 3);
    addPoint(points, 4, 3);
    addPoint(points, 5, 3);
    addPoint(points, 1, 4);
    addPoint(points, 2, 4);
    addPoint(points, 3, 4);
    addPoint(points, 5, 4);
    addPoint(points, 6, 4);
    addPoint(points, 7, 4);
    addPoint(points, 3, 5);
    addPoint(points, 4, 5);
    addPoint(points, 5, 5);
    addPoint(points, 2, 6);
    addPoint(points, 4, 6);
    addPoint(points, 6, 6);
    addPoint(points, 1, 7);
    addPoint(points, 4, 7);
    addPoint(points, 7, 7);

    for(auto& iter : points) {
        qreal xFromLeftTop = ( iter.first * xSegLen + (xSegLen/2) - POINT_OFFSET );
        qreal yFromLeftTop = ( iter.second * ySegLen + (ySegLen/2) - POINT_OFFSET );
        board->addEllipse(xFromLeftTop, yFromLeftTop, POINT_DIAM, POINT_DIAM, pen, brush);
    }

}

void GameWidget::addLine(std::vector<bcpair_t>& lines, bsize_t x1, bsize_t y1, bsize_t x2, bsize_t y2)
{
    bcpair_t bcpair = std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2));
    lines.push_back(bcpair);
}

void GameWidget::addPoint(std::vector<bcoord_t>& points, bsize_t x, bsize_t y)
{
    points.push_back(std::make_pair(x, y));
}

std::shared_ptr<GEStone> GameWidget::findStone(bcoord_t bcoord)
{
    for (auto iter : myStones) {
        if ( iter->getBCoord() == bcoord )
        {
            qDebug() << "Stone found in my stones.";
            return iter;
        }
    }
    for (auto iter : oppStones) {
        if ( iter->getBCoord() == bcoord )
        {
            qDebug() << "Stone found in opp's stones.";
            return iter;
        }
    }
    qDebug() << "Stone not found.";
    return nullptr;
}

void GameWidget::infoStone(std::shared_ptr<GEStone> gestone)
{
    qDebug() << "Stone ID: " << gestone->getId();
    qDebug() << "Board Coord: " << gestone->getBCoord();
    switch ( gestone->getColor() ) {
        case StoneColor::Red:
            qDebug() << "Color: Red";
            break;
        case StoneColor::Blue:
            qDebug() << "Color: Blue";
            break;
        default:
            break;
    }
    switch ( gestone->getOwner() ) {
        case Owner::Me:
            qDebug() << "Owner: Me";
            break;
        case Owner::Rival:
            qDebug() << "Owner: Rival";
            break;
        default:
            break;
    }
    qDebug() << "Alive: " << gestone->isAlive();
    qDebug() << "Unlocked: " << gestone->isEnabled();
}

void GameWidget::onStoneClicked(bsize_t x, bsize_t y)
{
    bcoord_t bcoord = std::make_pair(x, y);
    if ( _removeActive ) {
        lockAll();
        if ( verifyBCoord(bcoord) ) {
            unselectStone();
            _selectedStone = findStone(bcoord);
            if ( _selectedStone != nullptr ) {
                if ( !(_selectedStone->isMyStone()) ) {
                    _selectedStone->highlight(true);
                    qDebug() << "Rival's stone selected.";
                    emit removeStoneRequest(bToHqCoord(bcoord));
                }
                else {
                    _selectedStone = nullptr;
                    qDebug() << "You can't remove your stone.";
                }
            }
        }
        unlockAll();
        return;
    }

    switch( _gamePhase ) {
    case GamePhase::Place:
        qDebug() << "The place is already occupied.";
        break;

    case GamePhase::Move:
    case GamePhase::Fly:
        lockAll();
        if ( verifyBCoord(bcoord) ) {
            unselectStone();
            _selectedStone = findStone(bcoord);
            if ( _selectedStone != nullptr ) {
                if ( _selectedStone->isMyStone() ) {
                    _selectedStone->highlight(true);
                    qDebug() << "Stone selected.";
                }
                else {
                    _selectedStone = nullptr;
                    qDebug() << "The place is occupied by rival's stone.";
                }
            }
        }
        unlockAll();
        break;
    }
}

void GameWidget::onSegmentClicked(bsize_t x, bsize_t y)
{
    bcoord_t bcoord = std::make_pair(x, y);    
    switch(_gamePhase) {
    case GamePhase::Place:
        lockAll();
        if ( verifyBCoord(bcoord) ) {            
            emit placeMyStoneRequest(bToHqCoord(bcoord));
        }
        unlockAll();
        break;

    case GamePhase::Move:
    case GamePhase::Fly:
        lockAll();
        if ( verifyBCoord(bcoord) ) {
            if ( _selectedStone != nullptr ){
                emit moveMyStoneRequest(bToHqCoord(_selectedStone->getBCoord()), bToHqCoord(bcoord));
                unselectStone();
            }
        }        
        unlockAll();
        break;
    }
}

void GameWidget::onShowNet()
{
    emit showNetSignal();
}

bool GameWidget::placeMyStone(hqcoord_t hqcoord)
{
    bcoord_t bcoord = hqToBCoord(hqcoord);    
    std::shared_ptr<GEStone> gestone = findStone(bcoord);
    if ( gestone == nullptr ) {
        gestone = std::make_shared<GEStone>(handler);
        gestone->setBCoord(bcoord);
        gestone->setId(_myStoneCount);
        _myStoneCount++;
        gestone->setActive(true);
        gestone->setColor(StoneColor::Red);
        gestone->setOwner(Owner::Me);
        oppStones.push_back(gestone);
        board->addItem(gestone.get());
        qDebug() << "My stone successfully placed.";
        return true;
    }
    else {
        qDebug() << "The place is occupied.";
        return false;
    }
}

bool GameWidget::placeOppStone(hqcoord_t hqcoord)
{
    bcoord_t bcoord = hqToBCoord(hqcoord);
    std::shared_ptr<GEStone> gestone = findStone(bcoord);
    if ( gestone == nullptr ) {
        gestone = std::make_shared<GEStone>(handler);
        gestone->setBCoord(bcoord);
        gestone->setId(_oppStoneCount + 10);
        _oppStoneCount++;
        gestone->setActive(true);
        gestone->setColor(StoneColor::Blue);
        gestone->setOwner(Owner::Rival);
        oppStones.push_back(gestone);
        board->addItem(gestone.get());
        qDebug() << "Rival's stone successfully placed.";
        return true;
    }
    else {
        qDebug() << "The place is occupied.";
        return false;
    }
}

bool GameWidget::moveStone(hqcoord_t hqcoordFrom, hqcoord_t hqcoordTo)
{
    bcoord_t bcoordFrom = hqToBCoord(hqcoordFrom);
    bcoord_t bcoordTo = hqToBCoord(hqcoordTo);    
    std::shared_ptr<GEStone> gestone = findStone(bcoordFrom);
    if ( gestone == nullptr ) {
        qDebug() << "Stone move failed.";
        return false;
    }
    else if ( gestone->isAlive() ) {
        gestone->setBCoord(bcoordTo);
        qDebug() << "Stone successfully updated.";
        return true;
    }
    else {
        qDebug() << "Stone is not alive.";
        return false;
    }
}

bool GameWidget::removeStone(hqcoord_t hqcoord)
{
    bcoord_t bcoord = hqToBCoord(hqcoord);
    std::shared_ptr<GEStone> gestone = findStone(bcoord);
    if ( gestone == nullptr ) {
        qDebug() << "Stone remove failed.";
        return false;
    }
    else if ( gestone->isAlive() ) {
        gestone->setDead();
        gestone->setBCoord(std::make_pair(0, 0));
        qDebug() << "Stone successfully removed.";
        _removeActive = false;
        return true;
    }
    return false;
}

void GameWidget::onRejected(std::string &msg)
{
    unlockAll();
    unselectStone();
    showMsg(msg);
}

void GameWidget::enquirePhase()
{
    emit phaseInquiry();
}

void GameWidget::setPhase(GamePhase phase)
{
    _gamePhase = phase;
}

void GameWidget::actRemove()
{
    showMsg("Remove a stone of rival!");
    for(auto iter : myStones) {
        iter->lock();
    }
    _removeActive = true;
}

void GameWidget::setBeginner(bool meFirst)
{
    _myTurn = meFirst;
    if (_myTurn) {
        unlockAll();
        showMsg("You are the beginner.");
    }
    else {
        lockAll();
        showMsg("Your rival first.");
    }
    this->show();
}

void GameWidget::setPort(quint16 port)
{
    QString msg = "[port] " + QString::number(port) + " You Stone is red";
    ui->labelNote->setText(msg);
}


void GameWidget::changeTurn(bool myTurn)
{
    std::stringstream ss;
    unselectStone();
    if ( myTurn ) {        
        ss << "Your turn: ";
        switch (_gamePhase) {
        case GamePhase::Place:
            ss << " Place";
        break;
        case GamePhase::Move:
            ss << " Move";
        break;
        case GamePhase::Fly:
            ss << " Fly";
        break;
        }
        _myTurn = true;
        unlockAll();
    }
    else {
        ss << "Rival's Turn ";
        _myTurn = false;
        lockAll();
    }
    showMsg(ss.str());
}

void GameWidget::unselectStone()
{
    if ( _selectedStone != nullptr ) {
         _selectedStone->highlight(false);
    }
    _selectedStone = nullptr;
}

void GameWidget::gameOver(bool youWin)
{
    if (youWin) {
        showMsg("You win!");
    }
    else {
        showMsg("You lost!");
    }
    lockAll();
}
