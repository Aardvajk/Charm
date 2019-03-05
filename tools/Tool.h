#ifndef TOOL_H
#define TOOL_H

#include <QtCore/QObject>
#include <QtCore/QSet>

#include <QtGui/QPixmap>

class QWidget;
class Action;
class Settings;

class Tool : public QObject
{
    Q_OBJECT

public:
    Tool(Settings *settings, QObject *parent = 0);

    virtual
    void
    connectSlots() = 0;

    virtual
    void
    disconnectSlots() = 0;

    virtual
    QPixmap
    image() const = 0;

    virtual
    QString
    name() const = 0;

    virtual
    QWidget*
    optionsWidget() = 0;

    virtual
    void
    focusGained();

    virtual
    void
    focusLost();

    void
    enableRelatedActions(bool state);

signals:
    void
    selected();

    void
    enabled(bool state);

protected:
    void
    addRelatedAction(Action *action);

    virtual
    void
    saveSettings(Settings *settings) const;

protected slots:
    void
    savingSettings();

private:
    QVector<Action*> relatedActions;
    Settings *settings;
};

#endif // TOOL_H
