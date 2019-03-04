#ifndef STRICTDOUBLEVALIDATOR_H
#define STRICTDOUBLEVALIDATOR_H

#include <QtGui/QDoubleValidator>

class StrictDoubleValidator : public QDoubleValidator
{
    Q_OBJECT

public:
    StrictDoubleValidator(int decimals, QObject *parent = 0);
    StrictDoubleValidator(double bottom, double top, int decimals, QObject *parent = 0);

    virtual
    QValidator::State
    validate(QString &input, int &pos) const;

    virtual
    void
    fixup(QString &input) const;
};

#endif // STRICTDOUBLEVALIDATOR_H
