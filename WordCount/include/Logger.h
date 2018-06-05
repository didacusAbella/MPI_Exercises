#ifndef LOGGER_H
#define LOGGER_H
#define info(fmt, ...) do { if(LOG) fprintf(stderr, fmt, __VA_ARGS__); } while(0)
#ifdef LOG_ON
#define LOG 1
#else
#define LOG 0
#endif 
#endif