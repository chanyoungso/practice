#ifndef GESIGNALHANDLER_HPP
#define GESIGNALHANDLER_HPP

#include <QObject>
#include <QDebug>
#include <memory>
#include "stone_.hpp"
#include "geconfig.hpp"
#include "comconfig.hpp"

/// signal handler intermediating GameWidget and graphic elements
class GESignalHandler : public QObject
{
    Q_OBJECT
public:
    GESignalHandler() {};
    virtual ~GESignalHandler() {};

    /// function emitting stoneClicked signal called by graphic element
    /// @param x x_coordinate in bsize_t type
    /// @param x y_coordinate in bsize_t type
    void emitStoneClicked(bsize_t x, bsize_t y);

    /// function emitting segmentClicked signal called by graphic element
    /// @param x x_coordinate in bsize_t type
    /// @param x y_coordinate in bsize_t type
    void emitSegmentClicked(bsize_t x, bsize_t y);

signals:    
    /// signal emitted when stone clicked
    /// @param x x_coordinate in bsize_t type
    /// @param x y_coordinate in bsize_t type
    void stoneClicked(bsize_t x, bsize_t y);

    /// signal emitted when segment clicked
    /// @param x x_coordinate in bsize_t type
    /// @param x y_coordinate in bsize_t type
    void segmentClicked(bsize_t x, bsize_t y);
};

#endif // GESIGNALHANDLER_HPP
