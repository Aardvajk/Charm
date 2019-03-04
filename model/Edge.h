#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
    Edge(){ }
    Edge(int face, int a, int b) : face(face) { indices[0] = a; indices[1] = b; }

    int face;
    int indices[2];
};

class EdgeKey
{
public:
    EdgeKey(int a, int b) : a(a < b ? a : b), b(a < b ? b : a) { }

    bool operator<(const EdgeKey &k) const { if(a < k.a) return true; else if(a > k.a) return false; if(b < k.b) return true; else if(b > k.b) return false; return false; }

private:
    int a, b;
};

#endif // EDGE_H
