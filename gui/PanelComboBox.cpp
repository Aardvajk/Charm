#include "PanelComboBox.h"

#include "gui/PanelButton.h"
#include "gui/PanelListBox.h"
#include "gui/PopUpWindow.h"

#include <QtWidgets/QVBoxLayout>

PanelComboBox::PanelComboBox(QWidget *parent)
    : QWidget(parent),
      currIndex(-1)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    button = new PanelButton("");
    button->setComboStyle(true);

    connect(button, SIGNAL(pressed()), SLOT(buttonPressed()));

    layout->addWidget(button);

    listBox = new PanelListBox();

    connect(listBox, SIGNAL(clicked(int)), SLOT(rowClicked(int)));

    window = new PopUpWindow(listBox, this);
    window->setRelatedWidget(button);

    connect(window, SIGNAL(hidden()), SLOT(windowHidden()));
}

QSize
PanelComboBox::sizeHint() const
{
    return QSize(minimumWidth(), button->sizeHint().height());
}

void
PanelComboBox::setFrame(bool state)
{
    button->setFrame(state);
}

void
PanelComboBox::addItem(const QString &text, const QVariant &data)
{
    listBox->addItem(text, data);

    if(listBox->count() == 1)
    {
        setCurrentIndex(0);
    }

    int width = QFontMetrics(font()).width(text) + 26;
    if(width > minimumWidth())
    {
        setMinimumWidth(width);
    }
}

int
PanelComboBox::count() const
{
    return listBox->count();
}

QVariant
PanelComboBox::itemData(int index) const
{
    return listBox->itemData(index);
}

void
PanelComboBox::setCurrentIndex(int index)
{
    if(currIndex != index && index >= 0 && index < listBox->count())
    {
        currIndex = index;
        button->setText(listBox->itemText(index));

        listBox->setCurrentItem(index);

        update();

        emit currentIndexChanged(index);
    }
}

void
PanelComboBox::setCurrentData(const QVariant &data)
{
    for(int i = 0; i < listBox->count(); ++i)
    {
        if(listBox->itemData(i) == data)
        {
            setCurrentIndex(i);
            return;
        }
    }
}

QVariant
PanelComboBox::currentData() const
{
    return listBox->currentData();
}

void
PanelComboBox::buttonPressed()
{
    window->move(mapToGlobal(QPoint(0, height() - 1)));
    window->setSlideSizes(QSize(button->width(), 0), QSize(button->width(), listBox->heightForItems(qMin(listBox->count(), 6))));

    window->show();

    button->stopAnimation();
}

void
PanelComboBox::windowHidden()
{
    button->clearAnimation();
}

void
PanelComboBox::rowClicked(int index)
{
    window->hide();
    setCurrentIndex(index);
}
