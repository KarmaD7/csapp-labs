#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cachelab.h"

int hitCnt = 0, missCnt = 0, evictCnt = 0;
int turn = 0;
bool debug = false;
char* filename;

typedef struct line {
  bool valid;
  int tag;
  int startpos;
  int last;
} block;

void simulate(int s, int b, int E) {
  block simulator[1 << s][E];
  // simulator[0][0].valid = false
  for (int i = 0; i < (1 << s); ++i) {
    for (int j = 0; j < E; ++j) {
      simulator[i][j].valid = false;
      simulator[i][j].tag = 0;
      simulator[i][j].startpos = 0;
      simulator[i][j].last = 0x7fffffff;
    }
  }
  char line[128];
  FILE* fp = fopen(filename, "r");
  while (fgets(line, 127, fp)) {
    // fgets(line, 127, fp);

    if (debug) {
      printf("%s", line);
    }
    int pos = 0, i = 0;
    while (line[pos] == ' ') pos++;
    char opt = line[pos];
    // printf("%c\n", opt);
    if (opt == 'I') continue;
    pos += 2;
    // pos += 2;
    long addr = 0;
    while (line[pos] != ',') {
      addr <<= 4;
      addr += ('0' <= line[pos] && line[pos] <= '9') ? line[pos] - '0'
                                                     : line[pos] - 'a' + 10;
      ++pos;
    }
    int s_idx = (addr >> b) & (0xffffffff >> (32 - s));
    int tag = addr >> (b + s);
    block* l = simulator[s_idx];
    for (i = 0; i < E; ++i) {
      if (l[i].tag == tag && l[i].valid) {
        if (debug) printf("hit ");
        hitCnt++;
        l[i].last = turn++;
        break;
      }
    }
    if (i == E) {
      // write
      missCnt++;
      if (debug) printf("miss ");
      int oldest_idx = 0;
      for (i = 0; i < E; ++i) {
        if (!l[i].valid) {
          l[i].valid = true;
          l[i].tag = tag;
          l[i].last = turn++;
          break;
        }
        if (l[i].last < l[oldest_idx].last) {
          oldest_idx = i;
        }
      }
      if (i == E) {
        if (debug) printf("eviction ");
        evictCnt++;
        l[oldest_idx].tag = tag;
        l[oldest_idx].last = turn++;
      }
    }
    if (opt == 'M') {
      hitCnt++;
      if (debug) printf("hit ");
    }
    if (debug) printf("\n");
    // read from xc
    // } else if (opt == 'M') {
    //     // return;
    // } else if (opt == 'S') {
    //     // return;
    // }
    // if

    // if
    // /* read from trace */
    // ;
  }
}

int main(int argc, char* argv[]) {
  char opt;
  int b, E, s;
  while ((opt = getopt(argc, argv, "s:E:b:t:v")) != -1) {
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
        // printf("here\n");
        // printf("%s\n", optarg);
        filename = optarg;
        // printf("%s\n", filename);
        break;
      default:
        break;
    }
  }
  simulate(s, b, E);
  printSummary(hitCnt, missCnt, evictCnt);
  return 0;
}