#ifndef SCOPEDPTR_H
#define SCOPEDPTR_H

template<typename T> class ScopedPtr
{
public:
    ScopedPtr(T *t = nullptr) : t(t) { }
    ScopedPtr(ScopedPtr &p) : t(p.release()) { }
    ScopedPtr(ScopedPtr &&p) : t(p.release()) { }
    ~ScopedPtr(){ delete t; }

    ScopedPtr &operator=(T *p){ if(t != p){ delete t; t = p; } return *this; }
    ScopedPtr &operator=(ScopedPtr &p){ if(t != p.t){ delete t; t = p.release(); } return *this; }
    ScopedPtr &operator=(ScopedPtr &&p){ if(t != p.t){ delete t; t = p.release(); } return *this; }

    bool operator==(const T *v) const { return t == v; }
    bool operator!=(const T *v) const { return t != v; }

    operator bool() const { return t; }

    T *operator->(){ return t; }
    const T *operator->() const { return t; }

    T &operator*(){ return *t; }
    const T &operator*() const { return *t; }

    T *get(){ return t; }
    const T *get() const { return t; }

    T &ref(){ return *t; }
    const T &ref() const { return *t; }

    T *release(){ T *p = t; t = nullptr; return p; }
    void destroy(){ delete t; t = nullptr; }

private:
    T *t;
};

#endif // SCOPEDPTR_H
