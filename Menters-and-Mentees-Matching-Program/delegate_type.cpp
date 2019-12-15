#include "delegate_type.h"
#include <QPainter>
#include <QComboBox>
#include <qdebug.h>

Delegate_Type::Delegate_Type(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Type::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if (index.data().toInt() == 0)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Domestic");
    }
    else if (index.data().toInt() == 1)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "International");
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QSize Delegate_Type::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(10*t.width());
    return t;
}

// Create New Editor Object
QWidget *Delegate_Type::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("Domestic");
    editor->addItem("International");
    return editor;
}

// Model -> Editor
void Delegate_Type::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == 0){
        comboBox->setCurrentText("Domestic");
    }
    else {
        comboBox->setCurrentText("International");
    }
}

// Editor -> Model
void Delegate_Type::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    if (value == "Domestic")
        model->setData(index, 0, Qt::EditRole);
    else
        model->setData(index, 1, Qt::EditRole);
}

// Geometry
void Delegate_Type::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
