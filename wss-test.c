/*
 *
 * For TIGER LAKE:
sudo perf stat -e cycles:u,instructions:u,mem_inst_retired.all_loads:uP,mem_load_retired.l1_hit:uP,mem_load_retired.l1_miss:uP,mem_load_retired.l2_hit:uP,mem_load_retired.l2_miss:uP,mem_load_retired.l3_hit:uP,mem_load_retired.l3_miss:uP,mem_load_l3_hit_retired.xsnp_hit:uP,mem_load_l3_hit_retired.xsnp_hitm:uP,mem_load_l3_hit_retired.xsnp_miss:uP,mem_load_l3_hit_retired.xsnp_none:uP,l2_rqsts.rfo_hit:u,l2_rqsts.rfo_miss:u,l2_rqsts.all_rfo:u,offcore_requests.demand_rfo:u,cycle_activity.cycles_mem_any:u,cycle_activity.stalls_mem_any:u,cycle_activity.stalls_total:u -- ./wss-test 30 rnd 1
perf 6.18:
sudo perf stat -e cycles:u,instructions:u,mem_inst_retired.all_loads:uP,mem_load_retired.l1_hit:uP,mem_load_retired.l1_miss:uP,mem_load_retired.l2_hit:uP,mem_load_retired.l2_miss:uP,mem_load_retired.l3_hit:uP,mem_load_retired.l3_miss:uP,mem_load_l3_hit_retired.xsnp_no_fwd:uP,mem_load_l3_hit_retired.xsnp_fwd:uP,mem_load_l3_hit_retired.xsnp_miss:uP,mem_load_l3_hit_retired.xsnp_none:uP,l2_rqsts.rfo_hit:u,l2_rqsts.rfo_miss:u,l2_rqsts.all_rfo:u,offcore_requests.demand_rfo:u,cycle_activity.cycles_mem_any:u,cycle_activity.stalls_mem_any:u,cycle_activity.stalls_total:u -- ./wss-test 30 rnd 1

 * For ICE LAKE:
sudo perf stat -e cycles:u,instructions:u,mem_inst_retired.all_loads:uP,mem_load_retired.l1_hit:uP,mem_load_retired.l1_miss:uP,mem_load_retired.l2_hit:uP,mem_load_retired.l2_miss:uP,mem_load_retired.l3_hit:uP,mem_load_retired.l3_miss:uP,mem_load_l3_hit_retired.xsnp_fwd:uP,mem_load_l3_hit_retired.xsnp_no_fwd:uP,mem_load_l3_hit_retired.xsnp_miss:uP,mem_load_l3_hit_retired.xsnp_none:uP,mem_load_l3_miss_retired.local_dram:uP,mem_load_l3_miss_retired.remote_dram:uP,mem_load_l3_miss_retired.remote_fwd:uP,mem_load_l3_miss_retired.remote_hitm:uP,l2_rqsts.rfo_hit:u,l2_rqsts.rfo_miss:u,l2_rqsts.all_rfo:u,offcore_requests_outstanding.cycles_with_demand_rfo:u,cycle_activity.cycles_mem_any:u,cycle_activity.stalls_mem_any:u,cycle_activity.stalls_total:u   -- ./wss-test 30 rnd 1

 * For cross numa tests on ICE LAKE:
sudo perf stat -e cycles:u,instructions:u,mem_inst_retired.all_loads:uP,mem_load_retired.l1_hit:uP,mem_load_retired.l1_miss:uP,mem_load_retired.l2_hit:uP,mem_load_retired.l2_miss:uP,mem_load_retired.l3_hit:uP,mem_load_retired.l3_miss:uP,mem_load_l3_hit_retired.xsnp_fwd:uP,mem_load_l3_hit_retired.xsnp_no_fwd:uP,mem_load_l3_hit_retired.xsnp_miss:uP,mem_load_l3_hit_retired.xsnp_none:uP,mem_load_l3_miss_retired.local_dram:uP,mem_load_l3_miss_retired.remote_dram:uP,mem_load_l3_miss_retired.remote_fwd:uP,mem_load_l3_miss_retired.remote_hitm:uP,l2_rqsts.rfo_hit:u,l2_rqsts.rfo_miss:u,l2_rqsts.all_rfo:u,offcore_requests_outstanding.cycles_with_demand_rfo:u,cycle_activity.cycles_mem_any:u,cycle_activity.stalls_mem_any:u,cycle_activity.stalls_total:u   -- numactl --physcpubind=10 --membind 0 ./wss-test 20 rnd 1 2

numactl --strict --preferred=0 --cpunodebind=0 ./wss-test 30 rnd 10 2
numactl --strict --preferred=0 --cpunodebind=0 --phycpubind=0-15 ./wss-test 30 rnd 10 2
numactl --strict --preferred=0 --cpunodebind=0 --physcpubind=0-15 ./wss-test 30 rnd 10 2
numactl --preferred=0 --cpunodebind=0 --physcpubind=0-15 ./wss-test 30 rnd 10 2
time numactl --preferred=0 --cpunodebind=0 --physcpubind=0-15 ./wss-test 30 rnd 1 1
time numactl --preferred=0 --cpunodebind=0 --physcpubind=0-15 ./wss-test 30 rnd 1 2

*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
//#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sdt.h>
#include <sys/mman.h>
#include <numaif.h>
#include <numa.h>

#define NPAD 7

struct elem {
    struct elem *next;
    int64_t data[NPAD];
} *list;

int main(int argc, void **argv) {
    long long WSS = 1<<20;
    uint64_t MAX_ITER = 1000000000;
    char list_type[5] = "seq"; //rnd|seq|arr
    long maxnode = numa_max_node() + 1;
    unsigned long nodemask = 1UL << (maxnode - 1); //default is to allocate on the last node

    if (argc > 1)
        WSS = 1<<atoi((char *)argv[1]);
    if (argc > 2)
        strncpy(list_type, (char *)argv[2], sizeof(list_type));           
    if (argc > 3)
        MAX_ITER = (uint64_t)(atoi((char *)argv[3]))*1000000000;
    if (argc > 4)
        nodemask = atoi((char *)argv[4]);

    const long long N = (long long)((float)WSS/sizeof(struct elem));
    printf("WSS=%lld KB, N=%lld struct size:%ld nodemask:0x%lx\n", (long long) (float)WSS/1024, N, sizeof(struct elem), nodemask);

    list = (struct elem *) mmap(NULL, WSS, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

    //to test numa directly from program as opposed to from bash via numactl
    //if (mbind(list, WSS, MPOL_BIND, &nodemask, 8*sizeof(nodemask), MPOL_MF_STRICT) < 0)
    //    perror("mbind");

    // initialize sequentially linked list
    if (!strcmp(list_type, "seq")) {
        printf("SEQUENTIAL LINKED LIST\n");
        for (long long i = 0; i < N; i++) {
            list[i].next = &list[(i+1)%N];
        }
    }

    // initialize randomly linked list
    if (!strcmp(list_type, "rnd")) {
        printf("RANDOM LINKED LIST\n");
        for (long long i = 0; i < N>>4; i ++) {
            list[i*16].next = &list[(i*16+1)];
            list[(i*16+1)].next = &list[(i*16+9)];
            list[(i*16+9)].next = &list[(i*16+2)];
            list[(i*16+2)].next = &list[(i*16+10)];
            list[(i*16+10)].next = &list[(i*16+3)];
            list[(i*16+3)].next = &list[(i*16+11)];
            list[(i*16+11)].next = &list[(i*16+4)];
            list[(i*16+4)].next = &list[(i*16+12)];
            list[(i*16+12)].next = &list[(i*16+5)];
            list[(i*16+5)].next = &list[(i*16+13)];
            list[(i*16+13)].next = &list[(i*16+6)];
            list[(i*16+6)].next = &list[(i*16+14)];
            list[(i*16+14)].next = &list[(i*16+7)];
            list[(i*16+7)].next = &list[(i*16+15)];
            list[(i*16+15)].next = &list[(i*16+8)];
            list[(i*16+8)].next = &list[(i*16+16)%N];
        }
    }

    printf("STARTING %lu ITERATIONS ...\n", MAX_ITER);
    struct elem *e = list;
    uint64_t s;

    if (strcmp(list_type, "arr") != 0) {
        for (uint64_t iter = 0; iter < MAX_ITER; iter++) {
            e = e->next;
        }
    } else {
        for (uint64_t iter = 0; iter < (uint64_t)((double)MAX_ITER/(double)N); iter++) {
            for (uint64_t i = 0; i < N; i++) {
                s = (uint64_t) list[i].next;
                //asm volatile("" : : "m" (s));
            }
        }
    }
    return s;
}