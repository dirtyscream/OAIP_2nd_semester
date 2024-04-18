#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "function.h"

typedef union Bits {
    struct {
        unsigned int last_bit : 1;
        unsigned int next_bit : 1;
    };
    unsigned int value;
    void (*find_bits)(union Bits* object, int number);
    void (*print_bits)(union Bits* object, int number);
} Bits;

void find_bits(Bits* object, int number) {
    uint8_t last_bit = 0;
    uint8_t next_bit = 0;
    int size = (int)(log2(number) + 1);

    for (int i = 0; i < size; i++) {
        next_bit = last_bit;
        last_bit = number & 1;
        number >>= 1;
    }
    object->last_bit = last_bit;
    object->next_bit = next_bit;
    return;
}

void print_bits(Bits* object) {
    printf("Number in binary system is: ");
    printf("Last: %u, next: %u\n", object->last_bit, object->next_bit);
}

int task1() {
    int number;
    Bits object;
    printf("Enter the number: ");
    scanf("%d", &number);
    find_bits(&object, number);
    print_bits(&object);
    return 0;
}
