#include "WeightCombo.h"

#include "gui/PanelButton.h"
#include "gui/PanelListBox.h"
#include "gui/PopUpWindow.h"

#include "model/Model.h"

#include "style/Style.h"

#include "common/Color.h"

#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

WeightSlider::WeightSlider(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model),
      jointIndex(-1),
      val(1.0f)
{
}

void
WeightSlider::setJoint(int index)
{
    jointIndex = index;
    update();
}

void
WeightSlider::setValue(float value)
{
    val = value;
    update();
}

void
WeightSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(rect(), Style::baseColor().darker(105));

    painter.setBrush(Qt::NoBrush);
    painter.setPen(Style::baseColor().darker(200));

    painter.drawRect(rect().adjusted(0, 0, 0, -1));

    if(jointIndex != -1)
    {
        QRect r = rect();
        r.setWidth(static_cast<int>(static_cast<float>(rect().width()) * val));

        if(r.width() > 0)
        {
            QColor color(145, 175, 215);

            QLinearGradient g(r.topLeft(), r.bottomLeft());
            g.setColorAt(0, setAlpha(color.lighter(120), 0.2f));
            g.setColorAt(1, setAlpha(color.lighter(120), 0.8f));

            painter.setPen(color);
            painter.setBrush(g);

            painter.drawRect(r.adjusted(1, 1, -1, -2));
        }

        painter.setPen(Qt::black);
        painter.drawText(rect().adjusted(6, 0, -18, 0), Qt::AlignVCenter | Qt::AlignLeft, model->joint(jointIndex).details.name);
    }
}

void
WeightSlider::mousePressEvent(QMouseEvent *event)
{
    prev = val;
    change(event->pos().x());
}

void
WeightSlider::mouseMoveEvent(QMouseEvent *event)
{
    change(event->pos().x());
}

void
WeightSlider::mouseReleaseEvent(QMouseEvent *event)
{
    if(prev != val)
    {
        emit changed(val);
    }
}

void
WeightSlider::change(int x)
{
    if(jointIndex != -1)
    {
        val = valueFromPos(x);
        update();
    }
}

float
WeightSlider::valueFromPos(int x) const
{
    float w = static_cast<float>(rect().width());

    float v = static_cast<float>(x) / w;

    if(v < 0) return 0;
    if(v > 1) return 1;

    return v;
}

WeightCombo::WeightCombo(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    slider = new WeightSlider(model);

    connect(slider, SIGNAL(changed(float)), SLOT(sliderChanged(float)));

    button = new PanelButton("");
    button->setComboStyle(true);
    button->setMaximumWidth(20);

    connect(button, SIGNAL(pressed()), SLOT(buttonPressed()));

    layout->addWidget(slider);
    layout->addWidget(button);

    listBox = new PanelListBox();

    connect(listBox, SIGNAL(clicked(int)), SLOT(rowClicked(int)));

    window = new PopUpWindow(listBox, this);
    window->setRelatedWidget(button);

    connect(window, SIGNAL(hidden()), SLOT(windowHidden()));
}

void
WeightCombo::setJoint(int index)
{
    slider->setJoint(index);
}

void
WeightCombo::setValue(float value)
{
    slider->setValue(value);
}

QSize
WeightCombo::sizeHint() const
{
    return QSize(minimumWidth(), button->sizeHint().height());
}

void
WeightCombo::buttonPressed()
{
    listBox->clear();

    listBox->addItem("None", -1);

    QFont font = listBox->itemFont(0);
    font.setItalic(true);

    listBox->setItemFont(0, font);

    for(int i = 0; i < model->jointCount(); ++i)
    {
        listBox->addItem(model->joint(i).details.name, i);
    }

    listBox->setCurrentItem(slider->joint() + 1);

    window->move(mapToGlobal(QPoint(0, height() - 1)));
    window->setSlideSizes(QSize(width(), 0), QSize(width(), listBox->heightForItems(qMin(listBox->count(), 6))));

    window->show();

    button->stopAnimation();
}

void
WeightCombo::windowHidden()
{
    button->clearAnimation();
}

void
WeightCombo::rowClicked(int index)
{
    window->hide();

    int joint = listBox->itemData(index).toInt();

    if(joint != slider->joint())
    {
        slider->setJoint(joint);
        emit changed();
    }
}

void
WeightCombo::sliderChanged(float value)
{
    emit changed();
}
