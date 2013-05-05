#ifndef __UTIL_H
#define __UTIL_H

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int *
array_random(const int seed,
             const int n);

#endif /* __UTIL_H */
