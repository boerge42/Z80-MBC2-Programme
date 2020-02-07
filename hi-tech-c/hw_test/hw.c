
#include <stdio.h>

char sec, min, hour, day, mon, year, temp;

void main()
{
  printf("Z80-MBC2: test hardware in (Hi-Tech) C; Uwe Berger, 2019\n\n");
  printf("...read DS3231\n");
  outp(1, 0x84);
  sec  = inp(0);
  min  = inp(0);
  hour = inp(0);
  day  = inp(0);
  mon  = inp(0);
  year = inp(0);
  temp = inp(0);
  printf("%02d.%02d.%02d %02d:%02d:%02d\n", day, mon, year, hour, min, sec);
  printf("temperature [C]: %d\n\n", temp);
  printf("\n...end!\n\n");
}
