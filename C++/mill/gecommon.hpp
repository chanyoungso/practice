#ifndef GECOMMON_HPP
#define GECOMMON_HPP

#include <QtGui>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>
#include <memory>
#include "geconfig.hpp"
#include "comconfig.hpp"
#include "gesignalhandler.hpp"

/// common graphic element class for stone and segment
class GECommon : public QGraphicsItem
{
public:
    /// constructor of GECommon class takes a poiniter to an object of GESignalHandler class for signal handling
    /// @param handler a pointer to a GESignalHandler object
    explicit GECommon(std::shared_ptr<GESignalHandler> handler = nullptr);
    virtual ~GECommon() {};

    /// get boundry of graphic element
    /// @brief necessary virtual function inherited from QGraphicsItem
    /// @returns boundry of graphic element
    QRectF boundingRect() const;

    /// paint the shape on the graphic element
    /// @brief necessary virtual function inherited from QGraphicsItem
    /// @param painter object that actually paints the shape, handled by the parent class
    /// @param option not used
    /// @param widget not used
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /// disable graphic element
    void lock();

    /// enable graphic element
    void unlock();

    /// get board coordinate of graphic element
    /// @returns board coordinate in bcoord_t type
    bcoord_t getBCoord();

    /// get a signal handler
    /// @returns a pointer to a GESignalHandler object
    std::shared_ptr<GESignalHandler> getHandler();

protected:
    std::shared_ptr<GESignalHandler> _handler;
    qreal _length;
    bcoord_t _bcoord;

    /// compute graphical position of graphic element in qreal type from board coordinate
    /// @param width of the board in qreal type
    /// @param height of the board in qreal type
    virtual void computePos(qreal width, qreal height);
};

#endif // GECOMMON_HPP
