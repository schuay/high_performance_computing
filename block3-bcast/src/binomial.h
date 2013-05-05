#ifndef __BINOMIAL_H
#define __BINOMIAL_H

#include <mpi.h>

int
bcast_binomial(int *buffer,
               int count,
               int root,
               MPI_Comm comm);

#endif /* __BINOMIAL_H */
