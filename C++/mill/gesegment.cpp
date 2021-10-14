#include "gesegment.hpp"

GESegment::GESegment(bcoord_t& bcoord, std::shared_ptr<GESignalHandler> handler) : GECommon(handler) {
    _length = VIEW_LEN / NUM_SEG;
    _bcoord = bcoord;
    setFlag(ItemIsSelectable);
    computePos(VIEW_LEN, VIEW_LEN);
}

QRectF GESegment::boundingRect() const
{
    return QRectF(x(), y(), _length, _length);
}

void GESegment::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(QColor(255, 255, 255, 0), 0);
    QBrush brush(QColor(255, 255, 255, 0));
    QRectF rect = boundingRect();
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(rect);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void GESegment::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "Segment clicked> Board Coord: " << _bcoord << " / Graphic Coord: " << pos();
    getHandler()->emitSegmentClicked(_bcoord.first, _bcoord.second);
    update();
    QGraphicsItem::mousePressEvent(event);

}
