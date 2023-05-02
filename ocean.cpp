/*****************************************************************************
\File ocean.cpp
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
#include "ocean.h"

namespace HLP2 {
  namespace WarBoats {
    /**************************************************************************/
    /*! 
      \brief
        A constructor to essentially create the ocean, a playing field of 
        boats. Creates the dynamically allocated ocean grid of int with 
        horizontal by vertical size as denoted by y and x axis, and assigned 
        with its appropriate values (mainly 0) based on the private class 
        members and what encompasses it. It sets the ocean's grid, num_boats, 
        boats hits and ID, and ShotStats stats variables to contain zeros. 
        x_size and y_size to contain the corresponding values. Boat boats is 
        also dynamically allocated with size numboats.

      \param numboats
        Contains the number of boats.

      \param xsize
        The horizontal number of columns the grid should contain.

      \param ysize
        The vertical number of rows the grid should contain.

      \param i
        Indicator for number of rows in a for loop.

      \param j
        Indicator for number of columns in a for loop.
    */    
    /**************************************************************************/
    Ocean::Ocean (int numboats, int xsize, int ysize)
        : grid{new int [ysize * xsize]}, boats{new Boat [numboats]}
        { 
          for (int i{}; i < ysize; ++i) {
            for (int j{}; j < xsize; ++j) {
              *(grid + i * xsize + j) = DamageType::dtOK; 
            }
          }
          num_boats = numboats; 
          x_size = xsize; 
          y_size = ysize;
          for (int i{}; i < numboats; ++i) {
            boats->hits = DamageType::dtOK;
            boats->ID = i; 
            boats->position = {0,0};
          }
          stats = {0,0,0,0};
    }

    /**************************************************************************/
    /*! 
      \brief
        A deconstructor to deallocate the dynamically allocated memory in the 
        constructor, Ocean. This deconstructor is automatically invoked when
        the program goes out of scope, such as calling delete.
    */
    /**************************************************************************/
    Ocean::~Ocean() {
      delete[] grid;  //deallocate grid memory
      delete[] boats; //deallocate boats memory
    }

    /**************************************************************************/
    /*! 
      \brief
        Places the boats into the grid of the ocean. The boat must be of 
        BOAT_LENGTH size and fit into the ocean's grid. It must not overlap 
        with other boats and should it be placed, it shall be done so in the 
        specified orientation, horizontally or vertically. Once placed, the 
        tiles will have the boat's ID. If a negative location is given, it will
        be rejected. It then returns a BoatPlacement value to indicate if the 
        specified placement is accepted or rejected.

      \param boat
        The struct Boat pointer containing the boat details.

      \param col
        Contains the x axis size of the boat.

      \param row
        Contains the y axis size of the boat.    

      \param i
        Indicator for number of rows/colums in a for loop.
    */
    /**************************************************************************/
    BoatPlacement Ocean::PlaceBoat(Boat boat) const {
      int col = boat.position.x;
      int row = boat.position.y;
      if (col < 0 || row < 0 || col > x_size || row > y_size)
        return BoatPlacement::bpREJECTED; // negative values

      if (boat.orientation == Orientation::oHORIZONTAL) { 
        if (row <= y_size) { //if can exist on y axis
          if (col + BOAT_LENGTH > x_size) { //if cannot exit on x axis
            return BoatPlacement::bpREJECTED;
          }
          else {  //can exit on x axis
            for (int i{}; i < BOAT_LENGTH; i++) {
              if ( *(grid + row * x_size + col+i) > 0) {  //boat exists
                return BoatPlacement::bpREJECTED;                
              }
            }
            for (int i{}; i < BOAT_LENGTH; i++) { //to place boat ID
              *(grid + row * x_size + col+i) = boat.ID;
            }
            return BoatPlacement::bpACCEPTED;
          }
        }
        else { //cannot fit into y axis
          return BoatPlacement::bpREJECTED;
        }
      }
      else { //if (boat.orientation == Orientation::oVERTICAL)  
        if (col <= x_size) { //check if can exist on x axis
          if (row + BOAT_LENGTH > y_size) { //if cannot exit on y axis
              return BoatPlacement::bpREJECTED;
            }
            else {  //can exit on y axis
              for (int i{}; i < BOAT_LENGTH; i++) {
                if ( *(grid + (row+i) * x_size + col) > 0) {  //boat exists
                  return BoatPlacement::bpREJECTED;                
                }
              }
              for (int i{}; i < BOAT_LENGTH; i++) { //to place boat ID
                *(grid + (row+i) * x_size + col) = boat.ID;
              }
              return BoatPlacement::bpACCEPTED;
            }
          }
          else {
            return BoatPlacement::bpREJECTED;
          }
      } 
    }

    /**************************************************************************/
    /*! 
      \brief
        Takes the grid locations to indicate the target. If an empty tile is 
        hit the tile's value will be -1 and returns MISS. If a location outside 
        the grid was specified, this function returns ILLEGAL. If the tile 
        containing a boat was hit, it alters the tile to contain the boat 
        ID + 100, and returns HIT. If 4 tiles of the boat has been hit, it 
        returns SUNK. If the grid location was indicated again, it returns 
        DUPLICATE. If a negative location is given, it will be considered 
        ILLEGAL.

      \param coordinate
        The struct Point pointer containing the coordinates of where the shot 
        will be taken.

      \param col
        Contains the x coordinate to target.

      \param row
        Contains the y coordinate to target.  

      \param tile
        Pointer to the struct Ocean ocean targeted location on the grid.
    */
    /**************************************************************************/
    ShotResult Ocean::TakeShot(Point const& coordinate) {
      int col = coordinate.x;
      int row = coordinate.y;
      if (col < 0 || row < 0 || col > x_size || row > y_size)
        return ShotResult::srILLEGAL; // negative values

      int *tile = (grid + row * x_size + col);

      if (col > x_size || row > y_size) {
        return ShotResult::srILLEGAL;   //ILLEGAL
      }
      else if (*tile == DamageType::dtOK) {
        *tile = DamageType::dtBLOWNUP; //set tile to -1
        (stats.misses)++; //increment misses count
        return ShotResult::srMISS;
      }
      else if (*tile == DamageType::dtBLOWNUP 
                || *tile > HIT_OFFSET) {
        (stats.duplicates)++; //increment duplicates count
        return ShotResult::srDUPLICATE;
      }
        (stats.hits)++;  //increment hits count
        ((boats+*tile)->hits)++; //boat id hit counter
        *tile += HIT_OFFSET;  //increment tile by hit_offset
        
        if (((boats+*tile-HIT_OFFSET)->hits) == 4) {
          (stats.sunk)++;
          return ShotResult::srSUNK;
        }

      return ShotResult::srHIT; //Hit as last return cuz if it falls into any 
                                //other condition, it returns something else 
    }

    /**************************************************************************/
    /*! 
      \brief
        Returns the statistics from the free store object referenced by the 
        ocean class containing ShotStats stats.
    */
    /**************************************************************************/       
    ShotStats Ocean::GetShotStats() const {
      return stats;
    }

    /**************************************************************************/
    /*! 
      \brief
        Returns the dimensions of the ocean using struct Point to indicate the
        x and y coordinates.

      \param pos
        The struct name that is initialized with x_size and y_size which are 
        the dimensions of the ocean. 
    */
    /**************************************************************************/ 
    Point Ocean::GetDimensions() const {
      Point pos {x_size, y_size};
      return pos;
    }

    /**************************************************************************/
    /*! 
      \brief
        Returns the grid of the ocean.
    */
    /**************************************************************************/ 
    int* Ocean::GetGrid() const {
      return grid;
    }  
  } // namespace WarBoats
} // namespace HLP2
