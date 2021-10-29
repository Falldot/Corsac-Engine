#include <iostream>

#include "allocator.h"

typedef struct
{
	char X, Y, Z, E, T, I, U, O, Q;
} Position;

int main() {
    Position arr = Position{1, 2, 3, 4, 5, 6, 7, 8, 9};

    corsac::allocator* alloc = new corsac::allocator();

    alloc->allocate(sizeof(arr));

    return 0;
}