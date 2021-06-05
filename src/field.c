#include "field.h"

Field *create_field(int size_x, int size_y)
{
    Field *field = malloc(sizeof(*field));
    field->size_x = size_x;
    field->size_y = size_y;
    field->values = (byte *)malloc(sizeof(byte) * size_x * size_y);

    return field;
}

Field *create_field_clean(int size_x, int size_y)
{
    Field *field = create_field(size_x, size_y);

    for (size_t i = 0; i < size_x * size_y; i++)
    {
        field->values[i] = DEAD;
    }

    return field;
}

void destroy_field(Field *field)
{
    free(field->values);
    free(field);
}

byte get_value(Field *field, int x, int y)
{
    return field->values[x * field->size_x + y];
}

void set_value(Field *field, int x, int y, byte value)
{
    field->values[x * field->size_x + y] = value;
}