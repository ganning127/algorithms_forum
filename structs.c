#include <stdio.h>
struct bob
{
    int a;    // takes 4 bytes
    char s;   // takes 1 byte, so in total, the int and the char take up 8 bytes. gets grouped together.
    double b; // takes 8 bytes
};

union u // takes the size of the widest type
{
    int a;
    char s;
    double b;
};

int main(void)
{
    struct bob bob1;
    bob1.a = 1;
    bob1.b = 2.0;
    bob1.s = 'c';

    union u u1;
    u1.a = 1;
    u1.b = 2.0;
    u1.s = 'c';

    printf("%zu\n", sizeof(u1));
    return 1;
}

/*
    Abstract data structure: controls how we access the data (queue can only be accessed from one end)
    Concrete data strucutre: how we store the data
*/