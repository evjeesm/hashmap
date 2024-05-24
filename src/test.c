#include <stddef.h>
#include <stdio.h>

struct A
{
    size_t a;
    int b;
};

struct B
{
    char a[8];
    char b[4];
};

size_t calc_aligned_size(size_t a, size_t b, size_t alignment)
{
    size_t sum = a + b;
    size_t rem = sum % alignment;
    return sum + (alignment * !!rem) - rem;
}

int main(void)
{
    printf("sizeof A : %zu\n", sizeof(struct A));
    printf("sizeof B : %zu\n", sizeof(struct B));


    printf("offsetof A.b : %zu\n", offsetof(struct A, b));
    printf("offsetof B.b : %zu\n", offsetof(struct B, b));

    printf("calculate aligned size: %zu\n", calc_aligned_size(8, 4, 8));
}
