#ifndef GESTONE_HPP
#define GESTONE_HPP

#include "gecommon.hpp"
#include "stone_.hpp"

enum class StoneColor { Red, Blue };
enum class Owner { Me, Rival };

/// graphic element stone class inherited from GECommon
class GEStone : public GECommon
{
public:
    /// constructor of GEStone class takes a poiniter to an object of GESignalHandler class for signal handling
    /// @param handler a pointer to a GESignalHandler object
    explicit GEStone(std::shared_ptr<GESignalHandler> handler = nullptr);
    virtual ~GEStone() {};

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

    /// set board coordinate of the graphic element stone
    /// @param bcoord board coordinate in bcoord_t type.
    void setBCoord(bcoord_t bcoord);

    /// set color of the graphic element stone
    /// @param color in StoneColor type
    void setColor(StoneColor color);

    /// set owner of the graphic element stone
    /// @param owner in Owner type
    void setOwner(Owner owner);

    /// set ID of the graphic element stone. matches to the ID of Stone object
    /// @param id ID of stone in sid_t type
    void setId(sid_t id);

    /// set the graphic element stone dead(alive false, invisible)
    void setDead();

    /// get color of graphic element stone
    /// @returns color in enum class StoneColor type(Red or Blue)
    StoneColor getColor();

    /// get owner of the graphic element stone
    /// @returns owner in enum clas Owner type(Me or Rival)
    Owner getOwner();

    /// get ID of the graphic element stone
    /// @returns ID in sid_t type
    sid_t getId();

    /// check if the graphic element stone is alive
    /// @return alive in boolean
    bool isAlive();

    /// highlight the stone
    void highlight(bool high);

    /// check if it is my stone
    /// @returns bool
    bool isMyStone();

private:
    sid_t _id;
    StoneColor _color;
    Owner _owner;
    bool _alive;
    bool _highlighted;

    /// compute graphical position of stone in qreal type from board coordinate
    /// @param width of the board in qreal type
    /// @param height of the board in qreal type
    void computePos(qreal width, qreal height);

protected:
    /// function called when mouse clicked
    /// @param event transferred from its owner object of QGraphicsScene class
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
};

#endif // GESTONE_HPP
