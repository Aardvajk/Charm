#ifndef COMMAND_H
#define COMMAND_H

#include <QtCore/QObject>

class Model;
class ModelData;

class Command : public QObject
{
    Q_OBJECT

public:
    Command(Model *model, QObject *parent = 0);

    void
    begin(ModelData *modelData);

    virtual
    QString
    name() const = 0;

    virtual
    bool
    isValid() const = 0;

    virtual
    bool
    doesModifyPersistentState() const = 0;

    virtual
    void
    undo() = 0;

    virtual
    void
    redo() = 0;

protected:
    ModelData*
    modelData() const { return data; }

private:
    ModelData *data;
};

#endif // COMMAND_H
