#include <stdio.h>

#define FACT  12
#define COUNT 2000

/* **************************************** */  
unsigned long factorial(unsigned int n)
{
     if (n <= 1) {
          return n;
     } else {
          return n * factorial(n-1);
     }
} 

/* **************************************** */
long get_ts(void)
{
     int s, m, h;
     outp(1, 0x84);
     s = inp(0);
     m = inp(0);
     h = inp(0);
     return (((long)s) + ((long)m*60) + ((long)h*3600));
}

/* **************************************** */ 
/* **************************************** */
/* **************************************** */
void main(void)
{
     unsigned i;
     long ts;

     printf("%d! = %ld\n", FACT, factorial(FACT));

     ts = get_ts();
     for (i=0; i<COUNT; i++) {
          factorial(FACT);
     }
     printf("Laufzeit (%d x %d!): %d\n", 
               COUNT, FACT, (get_ts()-ts)); 
}
