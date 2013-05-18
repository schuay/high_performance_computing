#ifndef __BINARY_H
#define __BINARY_H

#include <mpi.h>

void
binary_block_size(const int size);

int
bcast_binary(int *buffer,
             int count,
             int root,
             MPI_Comm comm);

#endif /* __BINARY_H */
