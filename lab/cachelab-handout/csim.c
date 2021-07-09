#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int hitCnt = 0, missCnt = 0, evictCnt = 0;
int turn = 0; 
bool debug = false;
char* filename;

typedef struct line {
    bool valid;
    int tag;
    int startpos;
    int last;
} line;

void simulate(int s, int b, int E) {
    line simulator[1 << s][E];
    while (true) {
        /* read from trace */
        ;
        break;
    }
}

int main(int argc, char* argv[]) {
    char opt;
    int b, E, s;
    while ((opt = getopt(argc, argv, "v:s:E:b:t")) != -1) {
        switch (opt) {
            case 'v':
                /* code */
                debug = true;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                filename = optarg;
                break;
            default:
                break;
        }
    }
    simulate(s, b, E);
    printSummary(hitCnt, missCnt, evictCnt);
    return 0;
}
