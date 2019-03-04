#ifndef STRICTINTVALIDATOR_H
#define STRICTINTVALIDATOR_H

#include <QtGui/QIntValidator>

class StrictIntValidator : public QIntValidator
{
    Q_OBJECT

public:
    StrictIntValidator(QObject *parent = 0);

    virtual
    QValidator::State
    validate(QString &input, int &pos) const;

    virtual
    void
    fixup(QString &input) const;
};

#endif // STRICTINTVALIDATOR_H
