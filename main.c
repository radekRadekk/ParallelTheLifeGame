#include <stdlib.h>
#include <string.h>

#include "src/gameplay.h"

#define DEFAULT_SIZE 100
#define DEFAULT_ITERATIONS_NUMBER 100
#define DEFAULT_THREADS_NUMBER 4

Field *prepare_defaul_start_field(int size_x, int size_y)
{
    Field *field = create_field_clean(size_x, size_y);

    for (int i = 0; i < size_y - 10; i += 10)
    {
        set_value(field, 2, i, ALIVE);
        set_value(field, 2, i + 1, ALIVE);
        set_value(field, 2, i + 2, ALIVE);
        set_value(field, 1, i + 2, ALIVE);
        set_value(field, 0, i + 1, ALIVE);
    }

    return field;
}

void read_parameters(game_arguments *game_arguments, int argc, char **argv)
{
    int size = 0;

    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp("-mode", argv[i]) == 0)
        {
            if (strcmp("SEQ", argv[i + 1]) == 0)
            {
                game_arguments->game_type = SEQUENTIAL;
            }
            else if (strcmp("PAR", argv[i + 1]) == 0)
            {
                game_arguments->game_type = PARALLEL;
            }
        }
        else if (strcmp("-i", argv[i]) == 0)
        {
            if (atoi(argv[i + 1]) > 0)
            {
                game_arguments->iterations_number = atoi(argv[i + 1]);
            }
        }
        else if (strcmp("-size", argv[i]) == 0)
        {
            if (atoi(argv[i + 1]) > 3)
            {
                size = atoi(argv[i + 1]);
            }
        }
        else if (strcmp("-t", argv[i]) == 0)
        {
            if (atoi(argv[i + 1]) > 0)
            {
                game_arguments->threads_number = atoi(argv[i + 1]);
            }
        }
        else if (strcmp("-out", argv[i]) == 0)
        {
            game_arguments->output_name = argv[i + 1];
        }
        else if (strcmp("-frag", argv[i]) == 0)
        {
            if (strcmp("SQU", argv[i + 1]) == 0)
            {
                game_arguments->data_fragmentation = SQUARES;
            }
            else if (strcmp("LIN", argv[i + 1]) == 0)
            {
                game_arguments->data_fragmentation = LINES;
            }
        }
    }

    if (size)
    {
        game_arguments->start_field = prepare_defaul_start_field(size, size);
    }
}

int main(int argc, char **argv)
{

    game_arguments game_arguments = game_arguments;
    game_arguments.game_type = SEQUENTIAL;
    game_arguments.iterations_number = DEFAULT_ITERATIONS_NUMBER;
    game_arguments.threads_number = DEFAULT_THREADS_NUMBER;
    game_arguments.output_name = "out.gif";
    game_arguments.data_fragmentation = LINES;

    read_parameters(&game_arguments, argc, argv);

    if (game_arguments.start_field == NULL)
    {
        game_arguments.start_field = prepare_defaul_start_field(DEFAULT_SIZE, DEFAULT_SIZE);
    }

    play(&game_arguments);

    return EXIT_SUCCESS;
}