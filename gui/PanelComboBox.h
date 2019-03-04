#ifndef PANELCOMBOBOX_H
#define PANELCOMBOBOX_H

#include <QtCore/QVariant>

#include <QtWidgets/QWidget>

class PanelButton;
class PanelListBox;
class PopUpWindow;

class PanelComboBox : public QWidget
{
    Q_OBJECT

public:
    PanelComboBox(QWidget *parent = 0);

    virtual
    QSize
    sizeHint() const;

    void
    setFrame(bool state);

    void
    addItem(const QString &text, const QVariant &data = QVariant());

    int
    count() const;

    QVariant
    itemData(int index) const;

    void
    setCurrentIndex(int index);

    void
    setCurrentData(const QVariant &data);

    QVariant
    currentData() const;

signals:
    void
    currentIndexChanged(int index);

private slots:
    void
    buttonPressed();

    void
    windowHidden();

    void
    rowClicked(int index);

private:
    PanelButton *button;
    PanelListBox *listBox;
    PopUpWindow *window;

    int currIndex;
};

#endif // PANELCOMBOBOX_H
