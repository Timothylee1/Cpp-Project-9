/*******************************************************************************
\File ocean.hpp
\Author Timothy Lee Ke xin
\Par DP email: timothykexin.lee@digipen.edu.sg
\Par Course: RSE 1202
\Par Section: Object-Oriented Battleship
\Par Programming Assignment #5
\Date 17-02-2022

  \Brief
  This program is used for the implementation of a simple version of the board 
  game Battleship. It is modified from Assignment 2 to incoporate object-orient
  such as class. There are 5 functions in total to simulate the game. The rules
  of the game are as follows:
  1) Each ship/boat occupies BOAT_LENGTH (4) spaces and are set horizontally or 
     vertically. 
  2) Boats are to be placed within the ocean and the initial tiles of the ocean 
     is set to DamageType::dtOK (0), if the boat can be placed into the ocean, 
     the ID of that boat replaces the 0. 
  3) When a shot is taken, the will be an output to display if the shot was 
     ILLEGAL (outisde of ocean), HIT (a boat was hit), MISS (empty tile hit),
     DUPLICATE (tile was shot at previously), SUNK (all 4 tiles of the boat 
     was hit).
  4) If the shot hit an empty tile, the tile will be set to 
     DamageType::dtBLOWNUP (-1). If it hit a boat the tile will be set to the 
     boat's ID + HIT_OFFSET (100). 
  5) Any negative inputs such as boat placement or locations, will be rejected
     or illegal.
  
  - Ocean
      A constructor to essentially create the ocean, a playing field of boats.
      Creates the dynamically allocated ocean grid of int with horizontal by 
      vertical size as denoted by y and x axis, and assigned with its 
      appropriate values (mainly 0) based on the private class members and what 
      encompasses it. It sets the ocean's grid, num_boats, boats hits and ID, 
      and ShotStats stats variables to contain zeros. x_size and y_size to 
      contain the corresponding values. Boat boats is also dynamically 
      allocated with size numboats.

  - ~Ocean
      A deconstructor to deallocate the dynamically allocated memory in the 
      constructor, Ocean. This deconstructor is automatically invoked when
      the program goes out of scope, such as calling delete.
      
  - PlaceBoat
      Places the boats into the grid of the ocean. The boat must be of 
      BOAT_LENGTH size and fit into the ocean's grid. It must not overlap with
      other boats and should it be placed, it shall be done so in the specified
      orientation, horizontally or vertically. Once placed, the tiles will have 
      the boat's ID. If a negative location is given, it will be rejected. It 
      then returns a BoatPlacement value to indicate if the specified 
      placement is accepted or rejected.

  - Takeshot
      Takes the grid locations to indicate the target. If an empty tile is hit
      the tile's value will be -1 and returns MISS. If a location outside the 
      grid was specified, this function returns ILLEGAL. If the tile containing
      a boat was hit, it alters the tile to contain the boat ID + 100, and 
      returns HIT. If 4 tiles of the boat has been hit, it returns SUNK. If the 
      grid location was indicated again, it returns DUPLICATE. If a negative 
      location is given, it will be considered ILLEGAL.

  - GetShotStats
      Returns the statistics from the free store object referenced by the ocean
      class containing ShotStats stats.

  - GetDimensions
      Returns the dimensions of the ocean using struct Point to indicate the
      x and y coordinates.
      
  - GetGrid
      Returns the grid of the ocean.
 
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
#ifndef OCEAN_H
#define OCEAN_H
////////////////////////////////////////////////////////////////////////////////

namespace HLP2 {
  namespace WarBoats {

        inline int const BOAT_LENGTH {4};   //!< Length of a boat
        inline int const HIT_OFFSET  {100}; //!< Add this to the boat ID

        enum Orientation   { oHORIZONTAL, oVERTICAL };
        enum ShotResult    { srHIT, srMISS, srDUPLICATE, srSUNK, srILLEGAL };
        enum DamageType    { dtOK = 0, dtBLOWNUP = -1 };
        enum BoatPlacement { bpACCEPTED, bpREJECTED };

          //! A coordinate in the Ocean
        struct Point {
          int x; //!< x-coordinate (column)
          int y; //!< y-coordinate (row)
        };

          //! A boat in the Ocean
        struct Boat {
          int hits;                 //!< Hits taken so far
          int ID;                   //!< Unique ID 
          Orientation orientation;  //!< Horizontal/Vertical
          Point position;           //!< x-y coordinate (left-top)
        };

          //! Statistics of the "game"
        struct ShotStats {
          int hits;       //!< The number of boat hits
          int misses;     //!< The number of boat misses
          int duplicates; //!< The number of duplicate (misses/hits)
          int sunk;       //!< The number of boats sunk
        };

  } // namespace WarBoats

} // namespace HLP2

namespace HLP2 {
  namespace WarBoats {
      //! The attributes of the ocean
    class Ocean {

      public:
        Ocean(int numboats, int xsize, int ysize); 
        ~Ocean(); 
        ShotResult TakeShot(Point const& coordinate);
        BoatPlacement PlaceBoat(Boat boat) const;
        ShotStats GetShotStats() const;
        Point GetDimensions() const;
        int* GetGrid() const;
        
      private:
        int *grid;        //!< The 2D ocean 
        Boat *boats;      //!< The dynamic array of boats
        int num_boats;    //!< Number of boats in the ocean
        int x_size;       //!< Ocean size along x-axis
        int y_size;       //!< Ocean size along y-axis
        ShotStats stats;  //!< Status of the attack

    };
  } // namespace WarBoats
} // namespace HLP2

#endif // OCEAN_H
////////////////////////////////////////////////////////////////////////////////
