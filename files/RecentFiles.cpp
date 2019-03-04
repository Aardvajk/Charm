#include "RecentFiles.h"

#include "actions/Action.h"

#include "settings/Settings.h"

#include <QtWidgets/QMenu>

RecentFiles::RecentFiles(QMenu *menu, Settings *settings, QObject *parent)
    : QObject(parent),
      menu(menu),
      settings(settings)
{
    for(int i = 0; i < settings->childCount(); ++i)
    {
        files.append(settings->child(i)->value().toString());
    }

    createMenu();
}

void
RecentFiles::fileUsed(const QString &path)
{
    files.removeAll(path);
    files.insert(0, path);

    while(files.count() > 6)
    {
        files.removeLast();
    }

    recreateSettings();
    createMenu();
}

void
RecentFiles::fileFailed(const QString &path)
{
    files.removeAll(path);

    recreateSettings();
    createMenu();
}

void
RecentFiles::fileClicked()
{
    emit fileRequested(static_cast<Action*>(sender())->text());
}

void
RecentFiles::clearClicked()
{
    files.clear();

    recreateSettings();
    createMenu();
}

void
RecentFiles::createMenu()
{
    menu->clear();

    if(files.isEmpty())
    {
        Action *noneAction = new Action("None", menu);
        noneAction->setEnabled(false);

        menu->addAction(noneAction);
    }
    else
    {
        for(int i = 0; i < files.count(); ++i)
        {
            Action *fileAction = new Action(files[i], menu);
            connect(fileAction, SIGNAL(triggered()), SLOT(fileClicked()));

            menu->addAction(fileAction);
        }

        menu->addSeparator();

        Action *clearAction = new Action("Clear Menu", menu);
        connect(clearAction, SIGNAL(triggered()), SLOT(clearClicked()));

        menu->addAction(clearAction);
    }
}

void
RecentFiles::recreateSettings()
{
    settings->clear();
    for(int i = 0; i < files.count(); ++i)
    {
        settings->add("RecentFile", files[i]);
    }
}
