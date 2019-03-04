#ifndef RECENTFILENOTOPENEDDIALOG_H
#define RECENTFILENOTOPENEDDIALOG_H

#include <QtWidgets/QMessageBox>

class RecentFileNotOpenedDialog : public QMessageBox
{
    Q_OBJECT

public:
    RecentFileNotOpenedDialog(const QString &path, QWidget *parent = 0);

    static
    int
    show(QWidget *parent, const QString &path);
};

#endif // RECENTFILENOTOPENEDDIALOG_H
