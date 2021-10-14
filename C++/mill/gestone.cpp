#include "gestone.hpp"

GEStone::GEStone(std::shared_ptr<GESignalHandler> handler)
    : GECommon(handler), _color{StoneColor::Red}, _owner{Owner::Me}, _alive{true}, _highlighted{false} {
    _bcoord = std::make_pair(0, 0);
    _length = STONE_DIAM;
    setFlag(ItemIsSelectable);
};

QRectF GEStone::boundingRect() const
{
    return QRectF(x(), y(), _length, _length);
}

void GEStone::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::white, 2);
    if (_highlighted ) {
        pen.setWidth(5);
    }
    QBrush brush(Qt::red);
    if ( _color == StoneColor::Blue ) {
        brush.setColor(Qt::blue);
    }
    QRectF rect = boundingRect();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void GEStone::setBCoord(bcoord_t bcoord)
{
    _bcoord = bcoord;
    computePos(VIEW_LEN, VIEW_LEN);
}

void GEStone::setColor(StoneColor color)
{
    _color = color;
}

void GEStone::setOwner(Owner owner)
{
    _owner = owner;
}


void GEStone::setId(sid_t id)
{
    _id = id;
}

void GEStone::setDead()
{
    _alive = false;
    setEnabled(false);
    setVisible(false);
}

StoneColor GEStone::getColor()
{
    return _color;
}

Owner GEStone::getOwner()
{
    return _owner;
}

sid_t GEStone::getId()
{
    return _id;
}

bool GEStone::isAlive()
{
    return _alive;
}

void GEStone::highlight(bool high)
{
    _highlighted = high;
    update();
}

bool GEStone::isMyStone()
{
    if ( getOwner() == Owner::Me ) {
        return true;
    }
    else {
        return false;
    }
}

void GEStone::computePos(qreal width, qreal height)
{
    qreal xSegLen = width / NUM_SEG;
    qreal ySegLen = height/ NUM_SEG;
    qreal xFromLeftTop = ( _bcoord.first * (xSegLen/2) ) + STONE_OFFSET;
    qreal yFromLeftTop = ( _bcoord.second * (ySegLen/2) ) + STONE_OFFSET;
    setX( xFromLeftTop );
    setY( yFromLeftTop );
}

void GEStone::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Stone clicked> Id: " << _id << "/ Board Coord: " << _bcoord << " / Graphic Coord: " << pos();
    getHandler()->emitStoneClicked(_bcoord.first, _bcoord.second);
    update();
    QGraphicsItem::mousePressEvent(event);
}

