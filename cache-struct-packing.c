/* Cache Line = 64Bytes 
64byte aligned address: 0xffff40, 0xffff80, ...
|-w1l1-|-w2l1-|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line
|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|--8B--|  -> cache line

mov #0xffff48,rax
mov (rax),rsi

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_ITER (1000000000)

#ifdef ALIGNED
struct elem {
    uint64_t w1l1;
    uint64_t w1l2;
    uint64_t w1l3;
    uint64_t w1l4;
    uint64_t w1l5;
    uint64_t w1l6;
    uint64_t w1l7;
    uint64_t w1l8;
    char pad[448];
} *list;
#else
struct elem {
    volatile uint64_t w1l1;
    char padl1[56];

    uint64_t w1l2;
    char padl2[56];

    uint64_t w1l3;
    char padl3[56];

    uint64_t w1l4;
    char padl4[56];

    uint64_t w1l5;
    char padl5[56];

    uint64_t w1l6;
    char padl6[56];

    uint64_t w1l7;
    char padl7[56];

    uint64_t w1l8;
    char padl8[56];
} *list;
#endif

void main(){
    const uint64_t WSS = 1<<30;
    const uint64_t N = (uint64_t)((float)WSS/sizeof(struct elem));
    printf("WSS=%lu KB, N=%lu struct size:%ld\n", (uint64_t) (float)WSS/1024, N, sizeof(struct elem));

    list = (struct elem *)calloc(N, sizeof(struct elem));

    uint64_t s = 0;

    for (uint64_t iter = 0; iter < (uint64_t)(double)MAX_ITER/(double)N; iter++){
        /*
        for (uint64_t i = 0; i < N; i++){
            s += list[i].w1l1;
            s += list[i].w1l2;
            s += list[i].w1l3;
            s += list[i].w1l4;
            s += list[i].w1l5;
            s += list[i].w1l6;
            s += list[i].w1l7;
            s += list[i].w1l8;
        }
    */
        
        for (uint64_t i = 0; i < N>>4; i++){
            s += list[(i*16)%N].w1l1;
            s += list[(i*16+1)%N].w1l1;
            s += list[(i*16+9)%N].w1l1;
            s += list[(i*16+2)%N].w1l1;
            s += list[(i*16+10)%N].w1l1;
            s += list[(i*16+3)%N].w1l1;
            s += list[(i*16+11)%N].w1l1;
            s += list[(i*16+4)%N].w1l1;
            s += list[(i*16+12)%N].w1l1;
            s += list[(i*16+5)%N].w1l1;
            s += list[(i*16+13)%N].w1l1;
            s += list[(i*16+6)%N].w1l1;
            s += list[(i*16+14)%N].w1l1;
            s += list[(i*16+7)%N].w1l1;
            s += list[(i*16+15)%N].w1l1;
            s += list[(i*16+8)%N].w1l1;

            s += list[(i*16)%N].w1l2;
            s += list[(i*16+1)%N].w1l2;
            s += list[(i*16+9)%N].w1l2;
            s += list[(i*16+2)%N].w1l2;
            s += list[(i*16+10)%N].w1l2;
            s += list[(i*16+3)%N].w1l2;
            s += list[(i*16+11)%N].w1l2;
            s += list[(i*16+4)%N].w1l2;
            s += list[(i*16+12)%N].w1l2;
            s += list[(i*16+5)%N].w1l2;
            s += list[(i*16+13)%N].w1l2;
            s += list[(i*16+6)%N].w1l2;
            s += list[(i*16+14)%N].w1l2;
            s += list[(i*16+7)%N].w1l2;
            s += list[(i*16+15)%N].w1l2;
            s += list[(i*16+8)%N].w1l2;

            s += list[(i*16)%N].w1l3;
            s += list[(i*16+1)%N].w1l3;
            s += list[(i*16+9)%N].w1l3;
            s += list[(i*16+2)%N].w1l3;
            s += list[(i*16+10)%N].w1l3;
            s += list[(i*16+3)%N].w1l3;
            s += list[(i*16+11)%N].w1l3;
            s += list[(i*16+4)%N].w1l3;
            s += list[(i*16+12)%N].w1l3;
            s += list[(i*16+5)%N].w1l3;
            s += list[(i*16+13)%N].w1l3;
            s += list[(i*16+6)%N].w1l3;
            s += list[(i*16+14)%N].w1l3;
            s += list[(i*16+7)%N].w1l3;
            s += list[(i*16+15)%N].w1l3;
            s += list[(i*16+8)%N].w1l3;

            s += list[(i*16)%N].w1l4;
            s += list[(i*16+1)%N].w1l4;
            s += list[(i*16+9)%N].w1l4;
            s += list[(i*16+2)%N].w1l4;
            s += list[(i*16+10)%N].w1l4;
            s += list[(i*16+3)%N].w1l4;
            s += list[(i*16+11)%N].w1l4;
            s += list[(i*16+4)%N].w1l4;
            s += list[(i*16+12)%N].w1l4;
            s += list[(i*16+5)%N].w1l4;
            s += list[(i*16+13)%N].w1l4;
            s += list[(i*16+6)%N].w1l4;
            s += list[(i*16+14)%N].w1l4;
            s += list[(i*16+7)%N].w1l4;
            s += list[(i*16+15)%N].w1l4;
            s += list[(i*16+8)%N].w1l4;

            s += list[(i*16)%N].w1l5;
            s += list[(i*16+1)%N].w1l5;
            s += list[(i*16+9)%N].w1l5;
            s += list[(i*16+2)%N].w1l5;
            s += list[(i*16+10)%N].w1l5;
            s += list[(i*16+3)%N].w1l5;
            s += list[(i*16+11)%N].w1l5;
            s += list[(i*16+4)%N].w1l5;
            s += list[(i*16+12)%N].w1l5;
            s += list[(i*16+5)%N].w1l5;
            s += list[(i*16+13)%N].w1l5;
            s += list[(i*16+6)%N].w1l5;
            s += list[(i*16+14)%N].w1l5;
            s += list[(i*16+7)%N].w1l5;
            s += list[(i*16+15)%N].w1l5;
            s += list[(i*16+8)%N].w1l5;

            s += list[(i*16)%N].w1l6;
            s += list[(i*16+1)%N].w1l6;
            s += list[(i*16+9)%N].w1l6;
            s += list[(i*16+2)%N].w1l6;
            s += list[(i*16+10)%N].w1l6;
            s += list[(i*16+3)%N].w1l6;
            s += list[(i*16+11)%N].w1l6;
            s += list[(i*16+4)%N].w1l6;
            s += list[(i*16+12)%N].w1l6;
            s += list[(i*16+5)%N].w1l6;
            s += list[(i*16+13)%N].w1l6;
            s += list[(i*16+6)%N].w1l6;
            s += list[(i*16+14)%N].w1l6;
            s += list[(i*16+7)%N].w1l6;
            s += list[(i*16+15)%N].w1l6;
            s += list[(i*16+8)%N].w1l6;

            s += list[(i*16)%N].w1l7;
            s += list[(i*16+1)%N].w1l7;
            s += list[(i*16+9)%N].w1l7;
            s += list[(i*16+2)%N].w1l7;
            s += list[(i*16+10)%N].w1l7;
            s += list[(i*16+3)%N].w1l7;
            s += list[(i*16+11)%N].w1l7;
            s += list[(i*16+4)%N].w1l7;
            s += list[(i*16+12)%N].w1l7;
            s += list[(i*16+5)%N].w1l7;
            s += list[(i*16+13)%N].w1l7;
            s += list[(i*16+6)%N].w1l7;
            s += list[(i*16+14)%N].w1l7;
            s += list[(i*16+7)%N].w1l7;
            s += list[(i*16+15)%N].w1l7;
            s += list[(i*16+8)%N].w1l7;

            s += list[(i*16)%N].w1l8;
            s += list[(i*16+1)%N].w1l8;
            s += list[(i*16+9)%N].w1l8;
            s += list[(i*16+2)%N].w1l8;
            s += list[(i*16+10)%N].w1l8;
            s += list[(i*16+3)%N].w1l8;
            s += list[(i*16+11)%N].w1l8;
            s += list[(i*16+4)%N].w1l8;
            s += list[(i*16+12)%N].w1l8;
            s += list[(i*16+5)%N].w1l8;
            s += list[(i*16+13)%N].w1l8;
            s += list[(i*16+6)%N].w1l8;
            s += list[(i*16+14)%N].w1l8;
            s += list[(i*16+7)%N].w1l8;
            s += list[(i*16+15)%N].w1l8;
            s += list[(i*16+8)%N].w1l8;
        }  
    }
}