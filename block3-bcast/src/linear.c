#include "linear.h"

#define BLOCK_SIZE (1024)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int
bcast_linear(int *buffer,
             int count,
             int root,
             MPI_Comm comm)
{
    int ret;
    
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    const int next = (rank + 1) % size;
    const int prev = (rank + size - 1) % size;
    const int last = (root + size - 1) % size;

    if (rank == root) {
        for (int i = 0; i < count; i += BLOCK_SIZE) {
            const int n = MIN(BLOCK_SIZE, count - i);

            ret = MPI_Send(buffer + i, n, MPI_INT, next, i, comm);
            if (ret != MPI_SUCCESS) {
                return -1;
            }
        }
    } else if (rank == last) {
        for (int i = 0; i < count; i += BLOCK_SIZE) {
            const int n = MIN(BLOCK_SIZE, count - i);

            ret = MPI_Recv(buffer + i, n, MPI_INT, prev, i, comm, MPI_STATUS_IGNORE);
            if (ret != MPI_SUCCESS) {
                return -1;
            }
        }
    } else {
        int n = MIN(BLOCK_SIZE, count);

        ret = MPI_Recv(buffer, n, MPI_INT, prev, 0, comm, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            return -1;
        }

        int last_i = 0, last_n = n;
        for (int i = BLOCK_SIZE; i < count; i += BLOCK_SIZE) {
            n = MIN(BLOCK_SIZE, count - i);

            ret = MPI_Sendrecv(
                    buffer + last_i, last_n, MPI_INT, next, last_i,
                    buffer + i, n, MPI_INT, prev, i,
                    comm, MPI_STATUS_IGNORE);
            if (ret != MPI_SUCCESS) {
                return -1;
            }

            last_i = i;
            last_n = n;
        }

        ret = MPI_Send(buffer + last_i, last_n, MPI_INT, next, last_i, comm);
        if (ret != MPI_SUCCESS) {
            return -1;
        }
    }

    return 0;
}
