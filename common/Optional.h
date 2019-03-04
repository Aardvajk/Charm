#ifndef OPTIONAL_H
#define OPTIONAL_H

template<class T> class optional
{
public:
    optional() : v(false) { }
    optional(const T &t) : v(true), t(t) { }

    optional &operator=(const T &value){ v = true; t = value; return *this; }

    bool valid() const { return v; }
    void clear(){ v = false; }

    T value() const { return t; }

private:
    bool v;
    T t;
};

#endif // OPTIONAL_H
