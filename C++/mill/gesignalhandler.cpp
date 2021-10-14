#include "gesignalhandler.hpp"

void GESignalHandler::emitStoneClicked(bsize_t x, bsize_t y)
{
    emit stoneClicked(x, y);
}

void GESignalHandler::emitSegmentClicked(bsize_t x, bsize_t y)
{
    emit segmentClicked(x, y);
}
