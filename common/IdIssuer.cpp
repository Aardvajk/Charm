#include "IdIssuer.h"

IdIssuer::IdIssuer(int id) : next(id)
{
}

int
IdIssuer::id()
{
    if(!unused.isEmpty())
    {
        return unused.pop();
    }

    return next++;
}

void
IdIssuer::remove(int id)
{
    unused.push(id);
}
