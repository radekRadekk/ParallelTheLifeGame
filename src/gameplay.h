#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "field.h"

typedef unsigned char GAME_TYPE;

#define SEQUENTIAL 0
#define PARALLEL 1

typedef struct
{
    Field *start_field;
    int threads_number;
    int iterations_number;
    GAME_TYPE game_type;
    char *output_name;
} game_arguments;

void play(game_arguments *game_arguments);

#endif