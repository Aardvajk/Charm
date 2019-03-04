#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

class QMenu;
class Settings;

class RecentFiles : public QObject
{
    Q_OBJECT

public:
    RecentFiles(QMenu *menu, Settings *settings, QObject *parent = 0);

signals:
    void
    fileRequested(const QString &path);

public slots:
    void
    fileUsed(const QString &path);

    void
    fileFailed(const QString &path);

private slots:
    void
    fileClicked();

    void
    clearClicked();

private:
    void
    createMenu();

    void
    recreateSettings();

    QMenu *menu;
    QStringList files;
    Settings *settings;
};

#endif // RECENTFILES_H
