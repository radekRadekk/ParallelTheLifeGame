#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "field.h"
#include "neighbourhood.h"
#include "field.h"

typedef unsigned char GAME_TYPE;
typedef unsigned char DATA_FRAGMENTATION;

#define SEQUENTIAL 0
#define PARALLEL 1

#define LINES 0
#define SQUARES 1

typedef struct
{
    Field *start_field;
    int threads_number;
    int iterations_number;
    GAME_TYPE game_type;
    char *output_name;
    DATA_FRAGMENTATION data_fragmentation;
} game_arguments;

void play(game_arguments *game_arguments);

#endif