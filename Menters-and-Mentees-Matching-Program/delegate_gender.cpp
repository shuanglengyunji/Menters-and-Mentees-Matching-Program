#include "delegate_gender.h"
#include <QPainter>
#include <QComboBox>

Delegate_Gender::Delegate_Gender(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Gender::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if (index.data().toInt() == 0)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Female");
    }
    else if (index.data().toInt() == 1)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Male");
    }
    else if (index.data().toInt() == 2)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Other");
    }
    else if (index.data().toInt() == 3)
    {
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(Qt::white);
        }

        painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Prefer not to say");
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QSize Delegate_Gender::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(5*t.width());
    return t;
}

// Create New Editor Object
QWidget *Delegate_Gender::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("Female");
    editor->addItem("Male");
    editor->addItem("Other");
    editor->addItem("Prefer not to say");
    return editor;
}

// Model -> Editor
void Delegate_Gender::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == 0){
        comboBox->setCurrentText("Female");
    }
    if (value == 1){
        comboBox->setCurrentText("Male");
    }
    if (value == 2){
        comboBox->setCurrentText("Other");
    }
    else {
        comboBox->setCurrentText("Prefer not to say");
    }
}

// Editor -> Model
void Delegate_Gender::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    if (value == "Female")
        model->setData(index, 0, Qt::EditRole);
    else if (value == "Male")
        model->setData(index, 1, Qt::EditRole);
    else if (value == "Other")
        model->setData(index, 2, Qt::EditRole);
    else
        model->setData(index, 3, Qt::EditRole);
}

// Geometry
void Delegate_Gender::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
