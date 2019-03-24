#include "ExportDialog.h"
#include "FileDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "common/StrictDoubleValidator.h"

#include "gui/PanelSeparator.h"

ExportDetails::ExportDetails() : type(1), scale(1.0f), offset(0, 0, 0)
{
}

ExportDetails::ExportDetails(int type, const QString &path, float scale, const Vec3 &offset) : type(type), path(path), scale(scale), offset(offset)
{
}

bool ExportDetails::operator==(const ExportDetails &o) const
{
    return type == o.type && path == o.path && scale == o.scale && offset == o.offset;
}

ExportDialog::ExportDialog(const ExportDetails &details, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      det(details)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    typeCombo = new QComboBox();
    typeCombo->addItem("Rigged", 1);
    typeCombo->addItem("Static", 2);
    typeCombo->addItem("Static Indexed", 3);
    typeCombo->addItem("Rigged (bone names)", 4);
    typeCombo->addItem("Rigged (bone names) with colors", 5);
    typeCombo->addItem("Tile", 6);
    typeCombo->addItem("Static TexCoords", 7);
    typeCombo->addItem("Level", 8);
    typeCombo->addItem("Static TexCoods+Dummy Tangent", 9);

    for(int i = 0; i < typeCombo->count(); ++i)
    {
        if(typeCombo->itemData(i).toInt() == det.type)
        {
            typeCombo->setCurrentIndex(i);
            break;
        }
    }

    scaleEdit = new QLineEdit();
    scaleEdit->setValidator(new StrictDoubleValidator(3, scaleEdit));

    scaleEdit->setText(QString::number(det.scale, 'f', 3));

    offsetEdit = new QLineEdit();
    offsetEdit->setText(QString("%1, %2, %3").arg(QString::number(det.offset.x, 'f', 3)).arg(QString::number(det.offset.y, 'f', 3)).arg(QString::number(det.offset.z, 'f', 3)));

    QWidget *pathWidget = new QWidget();
    QHBoxLayout *pathLayout = new QHBoxLayout(pathWidget);
    pathLayout->setMargin(0);

    pathEdit = new QLineEdit();
    pathEdit->setMinimumWidth(200);

    QPushButton *browseButton = new QPushButton("...");
    browseButton->setFixedWidth(32);

    connect(browseButton, SIGNAL(clicked()), SLOT(browseClicked()));

    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);

    pathEdit->setText(det.path);

    form->addRow("Type", typeCombo);
    form->addRow("Scale", scaleEdit);
    form->addRow("Offset", offsetEdit);
    form->addRow("Path", pathWidget);

    layout->addLayout(form);
    layout->addWidget(new PanelSeparator());

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), SLOT(okClicked()));

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Export"));
}

void
ExportDialog::browseClicked()
{
    QString path = FileDialog::getSaveFileName(pathEdit->text(), "Any Files (*.*)", this);
    if(!path.isEmpty())
    {
        pathEdit->setText(path);
    }
}

void
ExportDialog::okClicked()
{
    det.type = typeCombo->itemData(typeCombo->currentIndex()).toInt();
    det.scale = scaleEdit->text().toFloat();
    det.path = pathEdit->text();

    det.offset = Vec3(0, 0, 0);

    auto list = offsetEdit->text().split(",");
    if(list.count() == 3)
    {
        det.offset.x = list[0].trimmed().toFloat();
        det.offset.y = list[1].trimmed().toFloat();
        det.offset.z = list[2].trimmed().toFloat();
    }

    accept();
}
