#ifndef INDEXPAIRKEY_H
#define INDEXPAIRKEY_H

#include <QtCore/QHash>

class IndexPairKey
{
public:
    IndexPairKey(int a, int b) : a(a < b ? a : b), b(a < b ? b : a) { }

    bool
    operator==(const IndexPairKey &k) const { return a == k.a && b == k.b; }

    bool
    operator<(const IndexPairKey &k) const { if(a < k.a) return true; else if(a > k.a) return false; if(b < k.b) return true; else if(b > k.b) return false; return false; }

    int a, b;
};

inline uint qHash(const IndexPairKey &k)
{
    return qHash(qMakePair(k.a, k.b));
}

#endif // INDEXPAIRKEY_H
