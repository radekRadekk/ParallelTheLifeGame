#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "../lib/gifenc.h"

#include "field.h"
#include "neighbourhood.h"

#include "gameplay.h"

void play_sequential(game_arguments *game_arguments)
{
    int size_x = game_arguments->start_field->size_x;
    int size_y = game_arguments->start_field->size_y;
    int iterations_num = game_arguments->iterations_number;

    ge_GIF *gif = ge_new_gif(
        game_arguments->output_name,      /* file name */
        size_x, size_y, /* canvas size */
        (uint8_t[]){
            /* palette */
            0x00, 0x00, 0x00, /* 0 -> black */
            0xFF, 0x00, 0x00, /* 1 -> red */
            0x00, 0xFF, 0x00, /* 2 -> green */
            0x00, 0x00, 0xFF, /* 3 -> blue */
        },
        2, /* palette depth == log2(# of colors) */
        0  /* infinite loop */
    );

    Field *current = game_arguments->start_field;
    Field *next = create_field(size_x, size_y);

    for (int i = 0; i < iterations_num; i++)
    {
        for (int x = 0; x < size_x; x++)
            for (int y = 0; y < size_y; y++)
            {
                byte next_state = get_cell_next_state(current, x, y);
                set_value(next, x, y, next_state);
            }

        memcpy(gif->frame, current->values, (size_t)(sizeof(byte) * current->size_x * current->size_y));
        ge_add_frame(gif, 10);

        destroy_field(current);
        current = next;
        next = create_field(size_x, size_y);
    }

    destroy_field(next);
    destroy_field(current);

    ge_close_gif(gif);
}

typedef struct
{
    int start_row, end_row;
    Field *current;
    Field *next;
} parameters;

parameters **create_params_array(int threads_number, Field *current, Field *next)
{
    parameters **params_arr = malloc(sizeof(parameters *) * threads_number);

    int chunk_size = current->size_y / threads_number;

    for (int i = 0; i < threads_number; i++)
    {
        params_arr[i] = malloc(sizeof(parameters));
        params_arr[i]->current = current;
        params_arr[i]->next = next;
        params_arr[i]->start_row = chunk_size * i;
        params_arr[i]->end_row = chunk_size * (i + 1);
    }

    params_arr[threads_number - 1]->end_row = current->size_y;

    return params_arr;
}

void update_params_array(parameters **params_arr, int threads_number, Field *current, Field *next)
{
    for (int i = 0; i < threads_number; i++)
    {
        params_arr[i]->current = current;
        params_arr[i]->next = next;
    }
}

void destroy_parameters_array(parameters **params_arr, int threads_number)
{
    for (int i = 0; i < threads_number; i++)
    {
        free(params_arr[i]);
    }

    free(params_arr);
}

void *iterate_part(void *arg)
{
    parameters *params = (parameters *)arg;

    for (int y = params->start_row; y < params->end_row; y++)
        for (int x = 0; x < params->current->size_x; x++)
        {
            byte next_state = get_cell_next_state(params->current, x, y);
            set_value(params->next, x, y, next_state);
        }

    return EXIT_SUCCESS;
}

void play_parallel(game_arguments *game_arguments)
{
    int size_x = game_arguments->start_field->size_x;
    int size_y = game_arguments->start_field->size_y;
    int threads_number = game_arguments->threads_number;
    int iterations_number = game_arguments->iterations_number;

    ge_GIF *gif = ge_new_gif(
        game_arguments->output_name, /* file name */
        size_x, size_y,     /* canvas size */
        (uint8_t[]){
            /* palette */
            0x00, 0x00, 0x00, /* 0 -> black */
            0xFF, 0x00, 0x00, /* 1 -> red */
            0x00, 0xFF, 0x00, /* 2 -> green */
            0x00, 0x00, 0xFF, /* 3 -> blue */
        },
        2, /* palette depth == log2(# of colors) */
        0  /* infinite loop */
    );

    Field *current = game_arguments->start_field;
    Field *next = create_field(size_x, size_y);

    parameters **params_arr = create_params_array(threads_number, current, next);

    pthread_t threads[threads_number];

    for (int i = 0; i < iterations_number; i++)
    {
        for (int i = 0; i < threads_number; i++)
        {
            pthread_create(&threads[i], NULL, iterate_part, (void *)params_arr[i]);
        }

        for (int i = 0; i < threads_number; i++)
        {
            pthread_join(threads[i], NULL);
        }

        memcpy(gif->frame, current->values, (size_t)(sizeof(byte) * current->size_x * current->size_y));
        ge_add_frame(gif, 10);

        destroy_field(current);
        current = next;
        next = create_field(size_x, size_y);

        update_params_array(params_arr, threads_number, current, next);
    }

    destroy_field(next);
    destroy_field(current);
    destroy_parameters_array(params_arr, threads_number);

    ge_close_gif(gif);
}

void play(game_arguments *game_arguments)
{
    if (game_arguments->game_type == SEQUENTIAL)
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