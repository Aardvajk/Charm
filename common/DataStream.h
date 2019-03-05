#ifndef DATASTREAM_H
#define DATASTREAM_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include <windows.h>

#include "common/ScopedPtr.h"

#include "maths/Maths.h"

class DataOutStream
{
public:
    bool fail() const { return os->fail(); }

    DataOutStream &operator<<(char c){ os->write(reinterpret_cast<const char*>(&c), sizeof(char)); return *this; }
    DataOutStream &operator<<(unsigned char c){ os->write(reinterpret_cast<const char*>(&c), sizeof(unsigned char)); return *this; }
    DataOutStream &operator<<(int i){ os->write(reinterpret_cast<const char*>(&i), sizeof(int)); return *this; }
    DataOutStream &operator<<(float f){ os->write(reinterpret_cast<const char*>(&f), sizeof(float)); return *this; }
    DataOutStream &operator<<(DWORD d){ os->write(reinterpret_cast<const char*>(&d), sizeof(DWORD)); return *this; }
    DataOutStream &operator<<(ushort u){ os->write(reinterpret_cast<const char*>(&u), sizeof(ushort)); return *this; }
    DataOutStream &operator<<(uint s){ os->write(reinterpret_cast<const char*>(&s), sizeof(uint)); return *this; }
    DataOutStream &operator<<(std::size_t s){ os->write(reinterpret_cast<const char*>(&s), sizeof(std::size_t)); return *this; }
    DataOutStream &operator<<(bool b){ int i = b ? 1 : 0; os->write(reinterpret_cast<const char*>(&i), sizeof(int)); return *this; }
    DataOutStream &operator<<(const char *s){ uint n = std::strlen(s); (*this) << n; write(s, n); return *this; }
    DataOutStream &operator<<(const std::string &s){ (*this) << s.length(); write(s.c_str(), s.length()); return *this; }

    DataOutStream &operator<<(const Vec3 &v){ (*this) << v.x << v.y << v.z; return *this; }
    DataOutStream &operator<<(const Quaternion &q){ (*this) << q.x << q.y << q.z << q.w; return *this; }

    void write(const char *data, uint size){ os->write(data, size); }

    void seek(uint pos){ os->seekp(pos); }
    uint position(){ return os->tellp(); }

protected:
    DataOutStream(std::ostream *o = 0) : os(o) { }

    ScopedPtr<std::ostream> os;
};

class DataOutFileStream : public DataOutStream
{
public:
    explicit DataOutFileStream(const std::string &path) : DataOutStream(new std::ofstream(path.c_str(), std::ios::binary)) { }
};

class DataInStream
{
public:
    bool fail() const { return is->fail(); }

    DataInStream &operator>>(int &i){ is->read(reinterpret_cast<char*>(&i), sizeof(int)); return *this; }
    DataInStream &operator>>(float &f){ is->read(reinterpret_cast<char*>(&f), sizeof(float)); return *this; }
    DataInStream &operator>>(DWORD &d){ is->read(reinterpret_cast<char*>(&d), sizeof(DWORD)); return *this; }
    DataInStream &operator>>(ushort &u){ is->read(reinterpret_cast<char*>(&u), sizeof(ushort)); return *this; }
    DataInStream &operator>>(uint &s){ is->read(reinterpret_cast<char*>(&s), sizeof(uint)); return *this; }
    DataInStream &operator>>(bool &b){ int i; is->read(reinterpret_cast<char*>(&i), sizeof(int)); b = i != 0; return *this; }
    DataInStream &operator>>(std::size_t &s){ is->read(reinterpret_cast<char*>(&s), sizeof(std::size_t)); return *this; }
    DataInStream &operator>>(std::string &s);

    DataInStream &operator>>(Vec3 &v){ (*this) >> v.x >> v.y >> v.z; return *this; }
    DataInStream &operator>>(Quaternion &q){ (*this) >> q.x >> q.y >> q.z >> q.w; return *this; }

    void read(char *data, uint size){ is->read(data, size); }
    void read(std::vector<char> &data, uint size){ data.resize(size); is->read(&(data[0]), size); }

    void seek(uint pos){ is->seekg(pos); }
    uint position(){ return is->tellg(); }

protected:
    DataInStream(std::istream *i) : is(i) { }

    ScopedPtr<std::istream> is;
};

class DataInFileStream : public DataInStream
{
public:
    explicit DataInFileStream(const std::string &path) : DataInStream(new std::ifstream(path.c_str(), std::ios::binary)) { }
};

#endif // DATASTREAM_H
