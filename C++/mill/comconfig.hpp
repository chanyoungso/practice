#ifndef COMCONFIG_HPP
#define COMCONFIG_HPP

#define MAX_STONES 9

// type for stone ID
using sid_t = quint8;

// type for coordinate value on game board
using bsize_t = quint8;

// type for Himmelsrichting-Quadrat Coordinate
using hqcoord_t = std::pair<bsize_t, bsize_t>;

// type for Cartesian Board Coordinate (origin: left top corner)
using bcoord_t = std::pair<bsize_t, bsize_t>;

// Pair of board coordinate for a line
using bcpair_t = std::pair<bcoord_t, bcoord_t>;

// game phase, mode
enum class GamePhase {Place, Move, Fly};
enum class GameMode {ClientFirst, ServerFirst, Random};

#endif // COMCONFIG_HPP
