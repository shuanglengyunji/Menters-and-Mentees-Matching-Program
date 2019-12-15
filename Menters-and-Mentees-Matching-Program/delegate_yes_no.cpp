#include "delegate_yes_no.h"
#include <QComboBox>
#include <QPainter>
#include <QApplication>

Delegate_Yes_No::Delegate_Yes_No(QObject *parent)
{
}

void Delegate_Yes_No::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if (index.data() == "y")
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Yes");
    }
    else if (index.data() == "n")
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "No");
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QSize Delegate_Yes_No::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// Create New Editor Object
QWidget *Delegate_Yes_No::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("Yes");
    editor->addItem("No");
    return editor;
}

// Model -> Editor
void Delegate_Yes_No::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == "y"){
        comboBox->setCurrentText("Yes");
    }
    else {
        comboBox->setCurrentText("No");
    }
}

// Editor -> Model
void Delegate_Yes_No::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    if (value == "Yes")
        value = "y";
    else
        value = "n";

    model->setData(index, value, Qt::EditRole);
}

// Geometry
void Delegate_Yes_No::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


