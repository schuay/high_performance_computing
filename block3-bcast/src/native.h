#ifndef __NATIVE_H
#define __NATIVE_H

#include <mpi.h>

int
bcast_native(int *buffer,
             int count,
             int root,
             MPI_Comm comm);

#endif /* __NATIVE_H */
