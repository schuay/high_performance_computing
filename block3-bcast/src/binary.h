#ifndef __BINARY_H
#define __BINARY_H

#include <mpi.h>

int
bcast_binary(void *buffer,
             int count,
             int root,
             MPI_Comm comm);

#endif /* __BINARY_H */
