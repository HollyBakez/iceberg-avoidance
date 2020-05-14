 ///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
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
  const size_t steps = setting.rows() + setting.columns() - 2; // len = r + c - 2
  assert(steps < 64);

  unsigned int count_paths = 0; // counter = 0 

  // TODO: implement the exhaustive optimization algorithm, then delete this
  // comment.

  for (size_t bits = 0; bits <= (pow(2,steps) - 1); bits++) { // for bits from 0 to 2^len-1 inclusive
    // initialize path with only STEP_DIRECTION_START
    path candidate(setting); // candidate = [start]

    bool valid = true; // initialize flag to true, if not changed then valid path

    for (size_t k = 0; k <= steps - 1; k++){
      size_t bit = (bits >> k) & 1;

      if (bit == 1) {
        if (!candidate.is_step_valid(STEP_DIRECTION_RIGHT)) { // check first if the direction right is a valid step
          valid = false; // if not change flag to false
          continue;
        }

        candidate.add_step(STEP_DIRECTION_RIGHT); // add the step direction right if valid

      } else {
        if (!candidate.is_step_valid(STEP_DIRECTION_DOWN)) { // check first if the direction down is a valid step
          valid = false; // if not flag to false
          continue;
        }

        candidate.add_step(STEP_DIRECTION_DOWN); // add the step direction down if valid
      }
    }

    if (valid) { // if candidate stays inside the grid and never crosses an X cell
      count_paths++; // add to the counter
    }
  }

  return count_paths; // return counter
}

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {
  // setting == G

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  const int DIM=100; // dimenstion of the grid/iceberg
  std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM)); // create the 2 dimensional vector named 'A'

  A[0][0] = 1; // base case

  // TODO: implement the dynamic programming algorithm, then delete this
  // comment.
  // initialize  from_above and from_left cases
  // general cases
  int from_above;
  int from_left;
  for(int i = 0; i < setting.rows(); i++){
    for(int j = 0; j < setting.columns(); j++){


      if (setting.get(i,j) == CELL_ICEBERG){ // check if the there is an iceberg
        A[i][j] = 0;
        continue;
      }
      from_above = from_left = 0;

      if(i > 0 && A[i-1][j] != 0){ // checking from left to move down
        from_above = A[i-1][j];
      }

      if(j > 0 && A[i][j-1] != 0){ // checking from above to move right
        from_left = A[i][j-1];
      }

      if( i || j){ // if from_above and from_left  are not None 
      A[i][j] = from_above + from_left; // sum of from_above and from_left
      }

    }
  }
  return A[setting.rows()-1][setting.columns()-1];
}

}
