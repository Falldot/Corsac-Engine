#include <stdio.h>

#include "slice.h"

int
main(void)
{
    int* slice = corsac_slice(int);

    corsac_slice_push(slice, int, 3);

    int* a  = slice[0];

    printf("%d\n", a);

    return 0;
}