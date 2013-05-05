#ifndef __LINEAR_H
#define __LINEAR_H

#include <mpi.h>

int
bcast_linear(int *buffer,
             int count,
             int root,
             MPI_Comm comm);

#endif /* __LINEAR_H */
