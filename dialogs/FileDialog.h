#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QtCore/QString>

class QWidget;

class FileDialog
{
public:
    static
    QString
    getOpenFileName(QWidget *parent);

    static
    QString
    getOpenFileName(const QString &filter, QWidget *parent);

    static
    QString
    getSaveFileName(QWidget *parent);

    static
    QString
    getSaveFileName(const QString &path, const QString &filter, QWidget *parent);

    static
    QString
    getSaveFileName(const QString &filter, QWidget *parent);

    static
    QString
    standardFilter();
};

#endif // FILEDIALOG_H
