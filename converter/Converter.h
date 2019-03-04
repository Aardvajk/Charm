#ifndef CONVERTER_H
#define CONVERTER_H

#include <QtCore/QString>

class Model;

class Converter
{
public:
    virtual ~Converter(){ }

    virtual
    bool
    toModel(const QString &path, Model *model) = 0;

    virtual
    bool
    fromModel(float scale, const QString &path, Model *model) = 0;

    QString
    error() const { return err; }

protected:
    QString err;
};

#endif // CONVERTER_H
