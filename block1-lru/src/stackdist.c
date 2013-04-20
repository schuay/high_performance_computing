#include "stackdist.h"

int
stackdist_process_trace(const uint64_t *p,
                        const uint64_t n,
                        const stackdist_cb fn)
{
    fn(0, 1);

    return 0;
}
