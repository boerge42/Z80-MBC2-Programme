
#include <stdio.h>

void main()
{
     printf("Z80-MBC2 hardware-tests in C; Uwe Berger, 2019\n\n");
     printf("...read DS3231\n");
     outp(1, 0x84);
     printf("--> %c", inp(0));
}
