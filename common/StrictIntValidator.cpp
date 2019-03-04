#include "StrictIntValidator.h"

StrictIntValidator::StrictIntValidator(QObject *parent)
    : QIntValidator(parent)
{
}

QValidator::State
StrictIntValidator::validate(QString &input, int &pos) const
{
    if(input.isEmpty())
    {
        return QValidator::Intermediate;
    }

    return QIntValidator::validate(input, pos);
}

void
StrictIntValidator::fixup(QString &input) const
{
    int v = input.toInt();

    if(v < bottom())
    {
        input = QString::number(bottom());
    }

    if(v > top())
    {
        input = QString::number(top());
    }
}
