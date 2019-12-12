#include "delegate_language.h"
#include <QPainter>
#include <QComboBox>
#include <QDebug>

Delegate_Language::Delegate_Language(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Language::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList list = index.data().toString().split(",");
    list.sort();

    list.replaceInStrings("10","Urdu");
    list.replaceInStrings("11","English");
    list.replaceInStrings("0","Hindi");
    list.replaceInStrings("1","Vietnamese");
    list.replaceInStrings("2","German");
    list.replaceInStrings("3","Korean");
    list.replaceInStrings("4","Tamil");
    list.replaceInStrings("5","Mandarin (Chinese)");
    list.replaceInStrings("6","Spanish");
    list.replaceInStrings("7","Cantonese");
    list.replaceInStrings("8","Indonesian");
    list.replaceInStrings("9","Japanese");

    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, list.join("\n"));
}

//! [1]
QSize Delegate_Language::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int size = index.data().toString().split(",").size();
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(3*t.width());
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
