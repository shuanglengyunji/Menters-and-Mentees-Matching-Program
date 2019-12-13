#include "delegate_college.h"
#include <QPainter>
#include <QComboBox>
#include <QDebug>

Delegate_College::Delegate_College(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_College::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList list = index.data().toString().split(",");
    list.sort();

    list.replaceInStrings("0","College of Asia and the Pacific");
    list.replaceInStrings("1","College of Arts and Social Sciences");
    list.replaceInStrings("2","College of Business and Economics");
    list.replaceInStrings("3","College of Engineering and Computer Science");
    list.replaceInStrings("4","College of Law");
    list.replaceInStrings("5","College of Science");
    list.replaceInStrings("6","College of Health and Medicine");

    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, list.join("\n"));
}

QSize Delegate_College::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int size = index.data().toString().split(",").size();
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(250);
    t.setHeight(size*t.height());

    return t;
}

// Create New Editor Object
QWidget *Delegate_College::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    return nullptr;
}

// Model -> Editor
void Delegate_College::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

// Editor -> Model
void Delegate_College::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

// Geometry
void Delegate_College::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
