#include "cachelab.h"
#include "stdio.h"
#include "stdlib.h"

int hit_count = 0, miss_count = 0, eviction_count = 0;

typedef struct Line {
    int valid;
    int tag;
    struct Line* prev;
    struct Line* next;
} Line;

typedef struct {
    Line* head;
    Line* rear;
} Set;

Set* Cache;

void printHelp();
void initCache(int s, int E);
void freeCache(int s);
void accessData(int addr, int v, int b, int s);
void updateSet(int set, Line* tmp);

int main(int argc, char* argv[])
{
    int v = 0, s = 0, E = 0, b = 0;
    int addr, size;
    char *t, c;
    FILE* fp = NULL;
    for (int i = 0; i < argc; i ++) {
        if (argv[i][0] == '-') {
            int pos = 1;
            while ((c = argv[i][pos++]) != 0) {
                if (c == 'h') {
                    printHelp();
                    return 0;
                }
                else if (c == 'v') {
                    v = 1;
                }
                else if (c == 's') {
                    if (argv[i][pos]) {
                        s = atol(argv[i] + pos);
                    }
                    else {
                        s = atol(argv[++i]);
                    }
                    break;
                }
                else if (c == 'E') {
                    if (argv[i][pos]) {
                        E = atol(argv[i] + pos);
                    }
                    else {
                        E = atol(argv[++i]);
                    }
                    break;
                }
                else if (c == 'b') {
                    if (argv[i][pos]) {
                        b = atol(argv[i] + pos);
                    }
                    else {
                        b = atol(argv[++i]);
                    }
                    break;
                }
                else if (c == 't') {
                    if (argv[i][pos]) {
                        t = argv[i] + pos;
                    }
                    else {
                        t = argv[++i];
                    }
                    fp = fopen(t, "r");
                    if (fp == NULL) {
                        printf("%s: No such file or directory\n", t);
                        return 0;
                    }
                    break;
                }
                else {
                    printf("./csim: invalid option -- \'%c\'\n", c);
                    printHelp();
                    return 0;
                }
            }
        }
    }
    if (!s || !E || !b || !fp) {
        printf("./csim: Missing required command line argument\n");
        printHelp();
        return 0;
    }
    //printf("v = %d, s = %d, E = %d, b = %d, t = %s\n", v, s, E, b, t);
    initCache(s, E);
    while (fscanf(fp, "%s %x,%d", &c, &addr, &size) != -1) {
        if (v) printf("%c %x,%d", c, addr, size);
        switch (c) {
            case 'M': accessData(addr, v, b, s);
            case 'L':
            case 'S': accessData(addr, v, b, s); break;
        }
        if (v) printf("\n");
    }
    freeCache(s);
    printSummary(hit_count, miss_count, eviction_count);
    fclose(fp);
    return 0;
}

void printHelp() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

void initCache(int s, int E) {
    int S = 1 << s;
    Cache = (Set*)malloc(S * sizeof(Set));
    for (int i = 0; i < S; i ++) {
        int l = E;
        Cache[i].head = Cache[i].rear = (Line*)malloc(sizeof(Line));
        while (--l) {
            Line* tmp = (struct Line*)malloc(sizeof(Line));
            Cache[i].rear->next = tmp;
            tmp->prev = Cache[i].rear;
            Cache[i].rear = tmp;
        }
    }
}

void freeCache(int s) {
    for (int i = 0; i < s; i ++) {
        Line* tmp = Cache[i].head;
        while (tmp) {
            Cache[i].head = tmp->next;
            free(tmp);
            tmp = Cache[i].head;
        }
    }
    free(Cache);
}

void accessData(int addr, int v, int b, int s) {
    int tag = addr >> (b + s);
    int set = (addr >> b) ^ (tag << s);
    Line* tmp = Cache[set].rear;
    while (tmp && tmp->valid) {
        if (tmp->tag == tag) {
            if (v) printf(" hit");
            updateSet(set, tmp);
            hit_count ++;
            return;
        }
        tmp = tmp->prev;
    }
    miss_count ++;
    tmp = Cache[set].head;
    if (tmp->valid) {
        eviction_count ++;
        if (v) printf(" eviction");
    }
    else {
        if (v) printf(" miss");
    }
    tmp->valid = 1;
    tmp->tag = tag;
    updateSet(set, tmp);
    return;
}

void updateSet(int set, Line* tmp) {
    if (tmp != Cache[set].rear) {
        if (tmp == Cache[set].head) {
            Cache[set].head = tmp->next;
        }
        else {
            tmp->prev->next = tmp->next;
        }
        tmp->next->prev = tmp->prev;
        tmp->prev = Cache[set].rear;
        tmp->next = NULL;
        Cache[set].rear->next = tmp;
        Cache[set].rear = tmp;
    }
}