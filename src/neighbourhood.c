#include "neighbourhood.h"

byte count_neighbours_moore(Field *field, int x, int y)
{
    if (x == 0 && y == 0)
    {
        return get_value(field, 1, 0) +
               get_value(field, 1, 1) +
               get_value(field, 0, 1);
    }

    if (x == 0 && y == field->size_y - 1)
    {
        return get_value(field, 1, field->size_y - 1) +
               get_value(field, 1, field->size_y - 2) +
               get_value(field, 0, field->size_y - 2);
    }

    if (x == field->size_x - 1 && y == 0)
    {
        return get_value(field, field->size_x - 2, 0) +
               get_value(field, field->size_x - 1, 1) +
               get_value(field, field->size_x - 2, 1);
    }

    if (x == field->size_x - 1 && y == field->size_y - 1)
    {
        return get_value(field, field->size_x - 2, field->size_y - 1) +
               get_value(field, field->size_x - 2, field->size_y - 2) +
               get_value(field, field->size_x - 1, field->size_y - 2);
    }

    if (x == 0)
    {
        return get_value(field, 0, y - 1) +
               get_value(field, 0, y + 1) +
               get_value(field, 1, y - 1) +
               get_value(field, 1, y) +
               get_value(field, 1, y + 1);
    }

    if (x == field->size_x - 1)
    {
        return get_value(field, field->size_x - 1, y - 1) +
               get_value(field, field->size_x - 1, y + 1) +
               get_value(field, field->size_x - 2, y - 1) +
               get_value(field, field->size_x - 2, y) +
               get_value(field, field->size_x - 2, y + 1);
    }

    if (y == 0)
    {
        return get_value(field, x - 1, 0) +
               get_value(field, x + 1, 0) +
               get_value(field, x - 1, 1) +
               get_value(field, x, 1) +
               get_value(field, x + 1, 1);
    }

    if (y == field->size_y - 1)
    {
        return get_value(field, x - 1, field->size_y - 1) +
               get_value(field, x + 1, field->size_y - 1) +
               get_value(field, x - 1, field->size_y - 2) +
               get_value(field, x, field->size_y - 2) +
               get_value(field, x + 1, field->size_y - 2);
    }

    return get_value(field, x - 1, y - 1) +
           get_value(field, x, y - 1) +
           get_value(field, x + 1, y - 1) +
           get_value(field, x - 1, y) +
           get_value(field, x + 1, y) +
           get_value(field, x - 1, y + 1) +
           get_value(field, x, y + 1) +
           get_value(field, x + 1, y + 1);
}

byte count_neighbours(Field *field, int x, int y, METHOD method)
{
    if (method == MOORE)
        return count_neighbours_moore(field, x, y);

    return 255;
}

byte get_cell_next_state(Field *field, int x, int y)
{
    byte neighbours_num = count_neighbours(field, x, y, MOORE);
    byte current_state = get_value(field, x, y);

    if (current_state == DEAD && neighbours_num == 3)
    {
        return ALIVE;
    }

    if (current_state == ALIVE && (neighbours_num < 2 || neighbours_num > 3))
    {
        return DEAD;
    }

    return current_state;
}