#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "../lib/gifenc.h"

#include "gameplay.h"
#include "sequential_gameplay.h"
#include "parallel_gameplay_rows.h"
#include "paralle_gameplay_squares.h"

void play_parallel(game_arguments *game_arguments)
{
    if (game_arguments->data_fragmentation == LINES)
    {
        play_parallel_with_rows(game_arguments);
    }
    else if (game_arguments->data_fragmentation == SQUARES)
    {
        play_parallel_with_squares(game_arguments);
    }
    else
    {
        fprintf(stderr, "Invalid data fragmentation type!");
    }
}

void play(game_arguments *game_arguments)
{
    if (game_arguments->game_type == SEQUENTIAL || game_arguments->threads_number == 1)
    {
        play_sequential(game_arguments);
    }
    else if (game_arguments->game_type == PARALLEL)
    {
        play_parallel(game_arguments);
    }
    else
    {
        fprintf(stderr, "Invalid game type!");
    }
}