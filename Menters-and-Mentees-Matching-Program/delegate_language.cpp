#include "delegate_language.h"
#include <QPainter>
#include <QComboBox>
#include <QDebug>

Delegate_Language::Delegate_Language(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Language::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString str = index.data().toString();
    str.replace("0","Hindi");
    str.replace("1","Vietnamese");
    str.replace("2","German");
    str.replace("3","Korean");
    str.replace("4","Tamil");
    str.replace("5","Mandarin (Chinese)");
    str.replace("6","Spanish");
    str.replace("7","Cantonese");
    str.replace("8","Indonesian");
    str.replace("9","Japanese");
    str.replace("10","Urdu");
    str.replace(",",";\n");
    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, str);
}

//! [1]
QSize Delegate_Language::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int size = index.data().toString().split(",").size();
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(5*t.width());
    t.setHeight(size*t.height());

    return t;
}

// Create New Editor Object
QWidget *Delegate_Language::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
//    QComboBox *editor = new QComboBox(parent);
//    editor->setFrame(false);
//    editor->addItem("Round 1");
//    editor->addItem("Round 2");
//    return editor;
    return nullptr;
}

// Model -> Editor
void Delegate_Language::setEditorData(QWidget *editor, const QModelIndex &index) const
{
//    int value = index.model()->data(index, Qt::EditRole).toInt();

//    QComboBox *comboBox = static_cast<QComboBox*>(editor);
//    if (value == 0){
//        comboBox->setCurrentText("Round 1");
//    }
//    else {
//        comboBox->setCurrentText("Round 2");
//    }
}

// Editor -> Model
void Delegate_Language::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
//    QComboBox *comboBox = static_cast<QComboBox*>(editor);
//    QString value = comboBox->currentText();

//    if (value == "Round 1")
//        model->setData(index, 0, Qt::EditRole);
//    else
//        model->setData(index, 1, Qt::EditRole);
}

// Geometry
void Delegate_Language::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
