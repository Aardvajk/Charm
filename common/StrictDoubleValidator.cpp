#include "StrictDoubleValidator.h"

#include "maths/Maths.h"

StrictDoubleValidator::StrictDoubleValidator(int decimals, QObject *parent)
    : QDoubleValidator(parent)
{
    setNotation(QDoubleValidator::StandardNotation);
    setDecimals(decimals);
}

StrictDoubleValidator::StrictDoubleValidator(double bottom, double top, int decimals, QObject *parent)
    : QDoubleValidator(bottom, top, decimals, parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QValidator::State
StrictDoubleValidator::validate(QString &input, int &pos) const
{
    if(input.isEmpty() || input == ".")
    {
        return Intermediate;
    }

    if(QDoubleValidator::validate(input, pos) != Acceptable)
    {
        return Invalid;
    }

    return Acceptable;
}

void
StrictDoubleValidator::fixup(QString &input) const
{
    if(input.isEmpty())
    {
        input = "0";
    }

    float v = input.toFloat();

    if(v < bottom())
    {
        input = formatFloat(bottom());
    }

    if(v > top())
    {
        input = formatFloat(top());
    }
}
