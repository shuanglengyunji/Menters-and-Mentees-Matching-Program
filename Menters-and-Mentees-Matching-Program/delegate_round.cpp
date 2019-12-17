#include "delegate_round.h"
#include <QPainter>
#include <QComboBox>

Delegate_Round::Delegate_Round(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Round::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if (index.data().toInt() == 0)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Round 1");
    }
    else if (index.data().toInt() == 1)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Round 2");
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QSize Delegate_Round::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(5*t.width());
    return t;
}

// Create New Editor Object
QWidget *Delegate_Round::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("Round 1");
    editor->addItem("Round 2");
    return editor;
}

// Model -> Editor
void Delegate_Round::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == 0){
        comboBox->setCurrentText("Round 1");
    }
    else {
        comboBox->setCurrentText("Round 2");
    }
}

// Editor -> Model
void Delegate_Round::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    if (value == "Round 1")
        model->setData(index, 0, Qt::EditRole);
    else
        model->setData(index, 1, Qt::EditRole);
}

// Geometry
void Delegate_Round::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
