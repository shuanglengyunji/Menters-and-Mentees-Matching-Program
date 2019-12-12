#include "delegate_academic_level.h"
#include <QPainter>
#include <QComboBox>

Delegate_Academic_Level::Delegate_Academic_Level(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Academic_Level::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.data().toInt() == 0)
    {
        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Undergraduate");
    }
    else if (index.data().toInt() == 1)
    {
        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Postgraduate");
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

//! [1]
QSize Delegate_Academic_Level::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// Create New Editor Object
QWidget *Delegate_Academic_Level::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("Undergraduate");
    editor->addItem("Postgraduate");
    return editor;
}

// Model -> Editor
void Delegate_Academic_Level::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == 0){
        comboBox->setCurrentText("Undergraduate");
    }
    else {
        comboBox->setCurrentText("Postgraduate");
    }
}

// Editor -> Model
void Delegate_Academic_Level::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    if (value == "Undergraduate")
        model->setData(index, 0, Qt::EditRole);
    else
        model->setData(index, 1, Qt::EditRole);
}

// Geometry
void Delegate_Academic_Level::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
