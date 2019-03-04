#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "MainWindow.h"

#include "debug/Assert.h"
#include "common/Fatal.h"

#include "style/Style.h"

#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Charm");
    QCoreApplication::setOrganizationName("Aardvajk");

    qApp->setStyle(new Style(qApp->style()));

    try
    {
        MainWindow w;
        w.show();

        return a.exec();
    }

    catch(const AssertException&)
    {
        return -1;
    }

    catch(const FatalException&)
    {
        return -1;
    }

    catch(...)
    {
        QMessageBox::critical(0, "Error", "Unknown exception caught");
        return -1;
    }
}
