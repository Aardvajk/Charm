#ifndef IDISSUER_H
#define IDISSUER_H

#include <QtCore/QStack>

class IdIssuer
{
public:
    IdIssuer(int id = 1);

    int
    id();

    void
    remove(int id);

private:
    int next;
    QStack<int> unused;
};

#endif // IDISSUER_H
