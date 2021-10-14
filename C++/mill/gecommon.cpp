#include "gecommon.hpp"

GECommon::GECommon(std::shared_ptr<GESignalHandler> handler)
{
    _handler = handler;
}

QRectF GECommon::boundingRect() const
{
    return QRectF(x(), y(), _length, _length);
}

void GECommon::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void GECommon::lock() {
    setEnabled(false);
}

void GECommon::unlock() {
    setEnabled(true);
}

bcoord_t GECommon::getBCoord()
{
    return _bcoord;
}

std::shared_ptr<GESignalHandler> GECommon::getHandler() {
    return _handler;
}

void GECommon::computePos(qreal width, qreal height)
{
    qreal xSegLen = width / NUM_SEG;
    qreal ySegLen = height/ NUM_SEG;
    qreal xFromLeftTop = _bcoord.first * (xSegLen/2);
    qreal yFromLeftTop = _bcoord.second * (ySegLen/2);
    setX( xFromLeftTop );
    setY( yFromLeftTop );
}
