#ifndef FIELD_H
#define FIELD_H

#include <stdlib.h>

typedef unsigned char byte;
#define ALIVE 1
#define DEAD 0

typedef struct
{
    int size_x, size_y;
    byte *values;
} Field;

Field *create_field(int size_x, int size_y);

Field *create_field_clean(int size_x, int size_y);

void destroy_field(Field *field);

byte get_value(Field *field, int x, int y);

void set_value(Field *field, int x, int y, byte value);

#endif