#ifndef NEIGHBOURHOOD_H
#define NEIGHBOURHOOD_H

#include "field.h"

typedef unsigned char METHOD;

#define MOORE 0
#define VON_NEUMANN 1

byte count_neighbours(Field *field, int x, int y, METHOD method);

byte get_cell_next_state(Field *field, int x, int y);

#endif