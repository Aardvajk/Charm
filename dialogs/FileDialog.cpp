#include "FileDialog.h"

#include <QtCore/QDir>

#include <QtWidgets/QFileDialog>

QString
FileDialog::getOpenFileName(QWidget *parent)
{
    return getOpenFileName(standardFilter(), parent);
}

QString
FileDialog::getOpenFileName(const QString &filter, QWidget *parent)
{
    return QFileDialog::getOpenFileName(parent, "Open", QDir::homePath() + QDir::separator() + "Desktop", filter);
}

QString
FileDialog::getSaveFileName(QWidget *parent)
{
    return getSaveFileName(standardFilter(), parent);
}

QString
FileDialog::getSaveFileName(const QString &path, const QString &filter, QWidget *parent)
{
    QString usePath = path;
    if(usePath.isEmpty())
    {
        usePath = QDir::homePath() + QDir::separator() + "Desktop";
    }

    return QFileDialog::getSaveFileName(parent, "Save", usePath, filter);
}

QString
FileDialog::getSaveFileName(const QString &filter, QWidget *parent)
{
    return getSaveFileName(QString(), filter, parent);
}

QString
FileDialog::standardFilter()
{
    return "Charm Files (*.charm);;Any Files (*.*)";
}
