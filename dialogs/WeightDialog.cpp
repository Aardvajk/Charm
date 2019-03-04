#include "WeightDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "maths/Maths.h"

#include "model/Model.h"

#include "gui/PanelSeparator.h"

#include "common/StrictDoubleValidator.h"

WeightDialog::WeightDialog(Model *model, const WeightInfo &info, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      model(model),
      info(info)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        addRow(layout, i);
    }

    layout->addWidget(new PanelSeparator());

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    okButton = new QPushButton("OK");
    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), SLOT(okClicked()));

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Vertex Weights"));

    okButton->setEnabled(false);

    comboChanged();
}

WeightInfo
WeightDialog::weights() const
{
    WeightInfo data = makeCurrentInfo();
    WeightInfo result;

    int index = 0;
    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        if(data.influences[i].joint >= 0)
        {
            result.influences[index++] = data.influences[i];
        }
    }

    if(!index)
    {
        return result;
    }

    float total = 0;
    for(int i = 0; i < index; ++i)
    {
        total += result.influences[i].weight;
    }

    for(int i = 0; i < index; ++i)
    {
        result.influences[i].weight = result.influences[i].weight / total;
    }

    return result;
}

void
WeightDialog::okClicked()
{
    accept();
}

void
WeightDialog::enableOk()
{
    okButton->setEnabled(makeCurrentInfo() != info);
}

void
WeightDialog::comboChanged()
{
    for(int i = 0; i < combos.count(); ++i)
    {
        if(combos[i]->itemData(combos[i]->currentIndex()).toInt() < 0)
        {
            edits[i]->setText("");
            edits[i]->setPlaceholderText("");
            edits[i]->setEnabled(false);
        }
        else
        {
            edits[i]->setPlaceholderText("0");
            edits[i]->setEnabled(true);
        }
    }

    enableOk();
}

void
WeightDialog::addRow(QBoxLayout *layout, int row)
{
    QHBoxLayout *rowLayout = new QHBoxLayout();
    rowLayout->setMargin(0);

    QComboBox *combo = new QComboBox();
    combo->setMinimumWidth(200);

    populateCombo(combo, row);

    connect(combo, SIGNAL(currentIndexChanged(int)), SLOT(comboChanged()));

    QLineEdit *edit = new QLineEdit();
    edit->setMaximumWidth(48);
    edit->setValidator(new StrictDoubleValidator(0, 1, 2, edit));

    edit->setText(formatFloat(info.influences[row].weight));

    connect(edit, SIGNAL(textChanged(QString)), SLOT(enableOk()));

    rowLayout->addWidget(combo);
    rowLayout->addWidget(edit);

    combos.append(combo);
    edits.append(edit);

    layout->addLayout(rowLayout);
}

void
WeightDialog::populateCombo(QComboBox *combo, int row)
{
    int index = 0;

    combo->addItem("None", -1);
    for(int i = 0; i < model->jointCount(); ++i)
    {
        combo->addItem(model->joint(i).details.name, i);

        if(i == info.influences[row].joint)
        {
            index = combo->count() - 1;
        }
    }

    combo->setCurrentIndex(index);

    QFont font = combo->font();
    font.setItalic(true);

    combo->setItemData(0, font, Qt::FontRole);
}

WeightInfo
WeightDialog::makeCurrentInfo() const
{
    WeightInfo data;
    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        data.influences[i].joint = combos[i]->itemData(combos[i]->currentIndex()).toInt();
        data.influences[i].weight = edits[i]->text().toFloat();
    }

    return data;
}
