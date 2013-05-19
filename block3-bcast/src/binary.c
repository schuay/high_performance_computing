#include "binary.h"

#include "util.h"

static int block_size = 65536;

void
binary_block_size(const int size)
{
    block_size = size;
}

int
bcast_binary(int *buffer,
             int count,
             int root,
             MPI_Comm comm)
{
    int ret;
    
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    /* 0-based rank for easier tree calculations. */
    const int rank0 = (rank + size - root) % size;

    const int parent0 = (rank0 - 1) / 2;
    const int l0 = rank0 * 2 + 1;
    const int r0 = rank0 * 2 + 2;

    const int has_l = (l0 < size);
    const int has_r = (r0 < size);

    const int parent = (root + parent0) % size;
    const int l = (root + l0) % size;
    const int r = (root + r0) % size;

    if (rank == root) {
        for (int i = 0; i < count; i += block_size) {
            const int n = MIN(block_size, count - i);

            if (has_l) {
                ret = MPI_Send(buffer + i, n, MPI_INT, l, i, comm);
                if (ret != MPI_SUCCESS) {
                    return -1;
                }
            }

            if (has_r) {
                ret = MPI_Send(buffer + i, n, MPI_INT, r, i, comm);
                if (ret != MPI_SUCCESS) {
                    return -1;
                }
            }
        }
    } else {
        int n = MIN(block_size, count);

        ret = MPI_Recv(buffer, n, MPI_INT, parent, 0, comm, MPI_STATUS_IGNORE);
        if (ret != MPI_SUCCESS) {
            return -1;
        }

        int last_i = 0, last_n = n;
        for (int i = block_size; i < count; i += block_size) {
            n = MIN(block_size, count - i);

            MPI_Request request;
            ret = MPI_Irecv(buffer + i, n, MPI_INT, parent, i, comm, &request);
            if (ret != MPI_SUCCESS) {
                return -1;
            }

            if (has_l) {
                ret = MPI_Send(buffer + last_i, last_n, MPI_INT, l, last_i, comm);
                if (ret != MPI_SUCCESS) {
                    return -1;
                }
            }

            if (has_r) {
                ret = MPI_Send(buffer + last_i, last_n, MPI_INT, r, last_i, comm);
                if (ret != MPI_SUCCESS) {
                    return -1;
                }
            }

            ret = MPI_Wait(&request, MPI_STATUS_IGNORE);
            if (ret != MPI_SUCCESS) {
                return -1;
            }

            last_i = i;
            last_n = n;
        }

        if (has_l) {
            ret = MPI_Send(buffer + last_i, last_n, MPI_INT, l, last_i, comm);
            if (ret != MPI_SUCCESS) {
                return -1;
            }
        }

        if (has_r) {
            ret = MPI_Send(buffer + last_i, last_n, MPI_INT, r, last_i, comm);
            if (ret != MPI_SUCCESS) {
                return -1;
            }
        }
    }

    return 0;
}
