#include "DataStream.h"

#include <vector>

DataInStream &DataInStream::operator>>(std::string &s)
{
    uint sz;
    (*this) >> sz;

    if(sz == 0)
    {
        s = std::string();
        return *this;
    }

    std::vector<char> v(sz + 1);
    is->read(&(v[0]), sz);

    v[v.size() - 1] = 0;

    s = &(v[0]);

    return *this;
}
