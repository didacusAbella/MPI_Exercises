#ifndef PINGPONGBOUNDED_H
#define PINGPONGBOUNDED_H
#define MASTER 0
#define DEFAULT_TAG 0
void master_work(int limit, int slots);
void slave_work(int rank, int slots);
#endif