#ifndef GESEGMENT_HPP
#define GESEGMENT_HPP

#include "gecommon.hpp"

/// graphic element segment class inherited from GECommon
class GESegment : public GECommon
{
public:
    /// constructor of GESegment class takes a poiniter to an object of GESignalHandler class for signal handling
    /// @param handler a pointer to a GESignalHandler object
    explicit GESegment(bcoord_t& bcoord, std::shared_ptr<GESignalHandler> handler = nullptr);
    virtual ~GESegment() {};

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

protected:
    /// function called when mouse clicked
    /// @param event transferred from its owner object of QGraphicsScene class
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // GESEGMENT_HPP
