#include "ConvertOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "dialogs/FileDialog.h"
#include "dialogs/ErrorDialog.h"

#include "converter/ObjFileConverter.h"
#include "converter/XFileConverter.h"
#include "converter/RawBufferConverter.h"
#include "converter/StaticBufferConverter.h"
#include "converter/TileConverter.h"
#include "converter/LevelConverter.h"

#include <QtCore/QFileInfo>

#include <memory>

ConvertOperations::ConvertOperations(ActionList *actions, Model *model, Graphics *graphics, QWidget *parentWidget, QObject *parent)
    : QObject(parent),
      model(model),
      graphics(graphics),
      parentWidget(parentWidget)
{
    Action *importAction = actions->add("File", "Import");
    importAction->setText("&Import...");

    connect(importAction, SIGNAL(triggered()), SLOT(input()));

    Action *exportAction = actions->add("File", "Export");
    exportAction->setText("&Export...");

    connect(exportAction, SIGNAL(triggered()), SLOT(output()));
}

void
ConvertOperations::input()
{
    QString path = FileDialog::getOpenFileName("OBJ Files (*.obj);;X Files (*.x);;Any Files (*.*)", parentWidget);

    if(path.isEmpty())
    {
        return;
    }

    std::auto_ptr<Converter> converter;

    QFileInfo info(path);
    if(info.suffix().toLower() == "x")
    {
        converter = std::auto_ptr<Converter>(new XFileConverter(graphics));
    }
    else if(info.suffix().toLower() == "obj")
    {
        converter = std::auto_ptr<Converter>(new ObjFileConverter());
    }
    else
    {
        ErrorDialog::show(parentWidget, "Unable to import file", "File type not supported");
        return;
    }

    if(!converter->toModel(path, model))
    {
        ErrorDialog::show(parentWidget, "Unable to import file", converter->error());
    }
}

void
ConvertOperations::output()
{
    ExportDialog dialog(model->lastExportDetails());
    if(dialog.exec() == QDialog::Accepted)
    {
        if(outputModel(dialog.details()))
        {
            model->setLastExportDetails(dialog.details());
        }
    }
}

bool
ConvertOperations::outputModel(const ExportDetails &details)
{
    std::auto_ptr<Converter> converter;

    switch(details.type)
    {
        case 1: converter = std::auto_ptr<Converter>(new RawBufferConverter(false, false)); break;
        case 2: converter = std::auto_ptr<Converter>(new StaticBufferConverter(false, false, false)); break;
        case 3: converter = std::auto_ptr<Converter>(new StaticBufferConverter(true, false, false)); break;
        case 4: converter = std::auto_ptr<Converter>(new RawBufferConverter(true, false)); break;
        case 5: converter = std::auto_ptr<Converter>(new RawBufferConverter(true, true)); break;
        case 6: converter = std::auto_ptr<Converter>(new TileConverter()); break;
        case 7: converter = std::auto_ptr<Converter>(new StaticBufferConverter(false, true, false)); break;
        case 8: converter = std::auto_ptr<Converter>(new LevelConverter()); break;
        case 9: converter = std::auto_ptr<Converter>(new StaticBufferConverter(false, true, true)); break;

        default: return false;
    }

    if(!converter->fromModel(details.scale, details.path, model))
    {
        ErrorDialog::show(parentWidget, "Unable to export file", converter->error());
        return false;
    }

    return true;
}
