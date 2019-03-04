#include "KeyboardOptions.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "gui/LineEdit.h"
#include "gui/KeySequenceWidget.h"

KeyboardOptions::KeyboardOptions(ActionList *actions, QWidget *parent)
    : QWidget(parent),
      actions(actions)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    filterEdit = new LineEdit(this);
    filterEdit->setHintText("Filter");

    layout->addWidget(filterEdit);

    tree = new QTreeWidget(this);

    tree->setColumnCount(3);
    tree->setHeaderLabels(QStringList() << tr("Name") << tr("Description") << tr("Shortcut"));

    tree->setColumnWidth(0, 170);
    tree->setColumnWidth(1, 250);

    tree->setMinimumWidth(550);
    tree->setMinimumHeight(300);

    layout->addWidget(tree);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    keyWidget = new KeySequenceWidget(this);
    keyWidget->setHintText("Click to modify");

    form->addRow(tr("Shortcut:"), keyWidget);
    keyLabel = form->labelForField(keyWidget);

    keyWidget->setEnabled(false);
    keyWidget->setEnabled(false);

    layout->addSpacing(4);
    layout->addLayout(form);

    populateTree();

    connect(filterEdit, SIGNAL(textChanged(QString)), SLOT(filterChanged(QString)));
    connect(keyWidget, SIGNAL(sequenceChanged(QKeySequence)), SLOT(keySequenceChanged(QKeySequence)));
    connect(tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void
KeyboardOptions::commit()
{
    for(int i = 0; i < tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *parent = tree->topLevelItem(i);

        for(int j = 0; j < parent->childCount(); ++j)
        {
            QTreeWidgetItem *item = parent->child(j);

            Action *action = actions->find(parent->text(0), item->text(0));
            if(action)
            {
                action->setShortcut(QKeySequence(item->text(2)));
            }
        }
    }
}

void
KeyboardOptions::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    keyWidget->blockSignals(true);

    if(!current || !current->parent())
    {
        keyWidget->setEnabled(false);
        keyWidget->setSequence(QKeySequence());
    }
    else
    {
        keyWidget->setEnabled(true);
        keyWidget->setSequence(QKeySequence(current->data(2, Qt::DisplayRole).toString()));
    }

    keyWidget->blockSignals(false);
    keyLabel->setEnabled(keyWidget->isEnabled());
}

void
KeyboardOptions::keySequenceChanged(const QKeySequence &sequence)
{
    QTreeWidgetItem *current = tree->currentItem();

    if(current && current->parent())
    {
        current->setData(2, Qt::DisplayRole, sequence.toString(QKeySequence::NativeText));
    }
}

void
KeyboardOptions::filterChanged(const QString &text)
{
    QString filter = text.trimmed();

    for(int i = 0; i < tree->model()->rowCount(); ++i)
    {
        QModelIndex sectionIndex = tree->model()->index(i, 0);

        bool anyVisible = false;

        for(int j = 0; j < tree->model()->rowCount(sectionIndex); ++j)
        {
            bool show = false;

            QTreeWidgetItem *item = tree->topLevelItem(i)->child(j);

            if(filter.isEmpty())
            {
                show = true;
            }
            else
            {
                for(int k = 0; k < 2; ++k)
                {
                    QString s = item->data(k, Qt::DisplayRole).toString();
                    s.remove("&");

                    if(s.contains(filter, Qt::CaseInsensitive))
                    {
                        show = true;
                        break;
                    }
                }
            }

            QModelIndex current = tree->selectionModel()->currentIndex();

            if(show)
            {
                tree->setRowHidden(j, sectionIndex, false);
                anyVisible = true;

                if(current.parent() == sectionIndex && current.row() == j)
                {
                    currentItemChanged(tree->currentItem(), 0);
                }
            }
            else
            {
                tree->setRowHidden(j, sectionIndex, true);

                if(current.parent() == sectionIndex && current.row() == j)
                {
                    currentItemChanged(0, 0);
                }
            }
        }

        tree->setRowHidden(i, QModelIndex(), !anyVisible);
    }
}

void
KeyboardOptions::populateTree()
{
    QMap<QString, QList<Action*> > map = actions->toMap();

    QFont boldFont = tree->font();
    boldFont.setBold(true);

    foreach(const QString &key, map.keys())
    {
        QTreeWidgetItem *parent = new QTreeWidgetItem(tree, QStringList(key));

        foreach(Action *action, map[key])
        {
            QString text = action->text();
            int i = text.indexOf('&');
            if(i >= 0)
            {
                text.remove(i, 1);
            }

            QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList() << action->objectName() << action->statusTip() << action->shortcut().toString(QKeySequence::NativeText));
            parent->addChild(item);
        }

        tree->addTopLevelItem(parent);;
        parent->setData(0, Qt::FontRole, boldFont);
    }

    tree->expandAll();
}
