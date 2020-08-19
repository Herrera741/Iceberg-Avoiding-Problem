///////////////////////////////////////////////////////////////////////////////
// ices_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
// Sergio Herrera
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
// Solve the iceberg avoiding problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_exhaustive(const grid& setting) {
    
  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2;
  assert(steps < 64);

  unsigned int count_paths = 0;

  //for loop to iterate through powerset of paths
  for (unsigned int bits=0; bits < pow(2,steps); bits++) {
    path candidate(setting);
    bool path = false; //default path invalid
    //for loop to iterate through grid moves
    for (unsigned int k=0; k < steps; k++) {
      int bit = ((bits >> k) & 1); //bit assigned binary value depending on move
      if (bit == 1) {
        //check if move is valid right move
        if (candidate.is_step_valid(STEP_DIRECTION_RIGHT)) {
          candidate.add_step(STEP_DIRECTION_RIGHT); //add move to path
          path = true;
        }
        else {
          break; //invalid move to cell
        }
      }
      else {
        //check if move is valid right move
        if (candidate.is_step_valid(STEP_DIRECTION_DOWN)) {
          candidate.add_step(STEP_DIRECTION_DOWN); //add move to path
          path = true; //path is currently true
        }
        else {
          break; //invalid move to cell
        }
      }
    } //end second for loop
    //check each path if true after adding all moves 
    if (path) {
      count_paths++; //update valid paths counter
    }
  } //end first for loop
  return count_paths;
}

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);


  const int DIM=100;
  std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));

  A[0][0] = 1;
    
  //for i=0 to r-1
  for (unsigned int i=0; i < setting.rows(); i++) {
    //for j=0 to c-1
    for (unsigned int j=0; j < setting.rows(); j++) {
      //if G[i][j] == X
      if (setting.get(i,j) == CELL_ICEBERG) {
        A[i][j] = 0; //A[i][j] = None
        continue;
      }
      //from_above = from _left = 0
      int from_above, from_left;
      from_above = from_left = 0;

      //if i>0 and A[i-1][j] is not None
      if ((i > 0 && A[i-1][j]) != 0) {
        from_above = A[i-1][j]; //from_above = A[i-1][j]
      }
      //if j>0 and A[i][j-1] is not None
      if ((j > 0 && A[i][j-1]) != 0) {
        from_left = A[i][j-1];//from_left = A[i][j-1]
      }
      //A[i][j] = from_above + from_left; or None if both are None
      A[i][j] = from_left + from_above;
    }//end of second for loop
  }//end of first for loop

  //return A[r-1]]c-1]
  return A[setting.rows()-1][setting.columns()-1];
}

}
