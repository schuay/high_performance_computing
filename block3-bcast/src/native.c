#include "native.h"

int
bcast_native(void *buffer,
             int count,
             int root,
             MPI_Comm comm)
{
    return MPI_Bcast(buffer, count, MPI_INT, root, comm);
}
