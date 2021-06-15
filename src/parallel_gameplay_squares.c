#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../lib/gifenc.h"

#include "paralle_gameplay_squares.h"

#define SQUARE_DIM 4

typedef struct
{
    int start_x, end_x,
        start_y, end_y;
} square;

typedef struct
{
    int squares_number;
    square **squares;
    Field *current;
    Field *next;
} parameters_for_squares;

parameters_for_squares **create_params_array_for_squares(int threads_number, Field *current, Field *next)
{
    parameters_for_squares **params_arr = malloc(sizeof(parameters_for_squares *) * threads_number);

    for (int i = 0; i < threads_number; i++)
    {
        params_arr[i] = malloc(sizeof(parameters_for_squares));
        params_arr[i]->current = current;
        params_arr[i]->next = next;
        params_arr[i]->squares = malloc(sizeof(square *));
    }

    int thread_num = 0;
    int current_x = 0, current_y = 0;
    while (current_x < current->size_x && current_y < current->size_y)
    {
        square *s = malloc(sizeof(square));
        s->start_x = current_x;
        s->end_x = (current_x + SQUARE_DIM) < current->size_x ? current_x + SQUARE_DIM : current->size_x;
        s->start_y = current_y;
        s->end_y = (current_y + SQUARE_DIM) < current->size_y ? current_y + SQUARE_DIM : current->size_y;

        params_arr[thread_num]->squares[params_arr[thread_num]->squares_number] = s;
        params_arr[thread_num]->squares_number++;
        params_arr[thread_num]->squares = realloc(params_arr[thread_num]->squares, sizeof(square) * (params_arr[thread_num]->squares_number + 1));
        thread_num = (thread_num + 1) % threads_number;

        if (s->end_x == current->size_x)
        {
            current_x = 0;
            current_y += SQUARE_DIM;
        }
        else
        {
            current_x += SQUARE_DIM;
        }

        if (s->end_y == current->size_y && s->end_x == current->size_x)
        {
            break;
        }
    }

    return params_arr;
}

void update_parameters_for_squares_array(parameters_for_squares **params_arr, int threads_number, Field *current, Field *next)
{
    for (int i = 0; i < threads_number; i++)
    {
        params_arr[i]->current = current;
        params_arr[i]->next = next;
    }
}

void destroy_parameters_for_squares_array(parameters_for_squares **params_arr, int threads_number)
{
    for (int i = 0; i < threads_number; i++)
    {
        for (int j = 0; j <= params_arr[i]->squares_number; j++)
        {
            free(params_arr[i]->squares[j]);
        }
        free(params_arr[i]->squares);
        free(params_arr[i]);
    }

    free(params_arr);
}

void *iterate_part_squares(void *arg)
{
    parameters_for_squares *params = (parameters_for_squares *)arg;

    for (int n = 0; n < params->squares_number; n++)
    {
        for (int x = params->squares[n]->start_x; x < params->squares[n]->end_x; x++)
        {
            for (int y = params->squares[n]->start_y; y < params->squares[n]->end_y; y++)
            {
                byte next_state = get_cell_next_state(params->current, x, y);
                set_value(params->next, x, y, next_state);
            }
        }
    }

    return EXIT_SUCCESS;
}

void play_parallel_with_squares(game_arguments *game_arguments)
{
    int size_x = game_arguments->start_field->size_x;
    int size_y = game_arguments->start_field->size_y;
    int threads_number = game_arguments->threads_number;
    int iterations_number = game_arguments->iterations_number;

    ge_GIF *gif = ge_new_gif(
        game_arguments->output_name, /* file name */
        size_x, size_y,              /* canvas size */
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

    parameters_for_squares **params_arr = create_params_array_for_squares(threads_number, current, next);

    pthread_t threads[threads_number];

    for (int i = 0; i < iterations_number; i++)
    {
        for (int i = 0; i < threads_number; i++)
        {
            pthread_create(&threads[i], NULL, iterate_part_squares, (void *)params_arr[i]);
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

        update_parameters_for_squares_array(params_arr, threads_number, current, next);
    }

    // destroy_field(next);
    // destroy_field(current);
    // destroy_parameters_for_squares_array(params_arr, threads_number);

    ge_close_gif(gif);
}
