#include <string.h>

#include "sequential_gameplay.h"
#include "../lib/gifenc.h"

void play_sequential(game_arguments *game_arguments)
{
    int size_x = game_arguments->start_field->size_x;
    int size_y = game_arguments->start_field->size_y;
    int iterations_num = game_arguments->iterations_number;

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