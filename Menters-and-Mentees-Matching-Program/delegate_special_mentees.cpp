#include "delegate_special_mentees.h"
#include <QPainter>
#include <QComboBox>
#include <QDebug>

Delegate_Special_Mentees::Delegate_Special_Mentees(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Special_Mentees::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList list = index.data().toString().split(",");
    list.sort();

    list.replaceInStrings("0","Australian culture");
    list.replaceInStrings("1","Going on exchange");
    list.replaceInStrings("2","Being a mature age student (greater than three years between school/uni or degrees)");
    list.replaceInStrings("3","Living with disability or mental illness");
    list.replaceInStrings("4","Working full or part time while studying");
    list.replaceInStrings("5","Identifying as LGBTIQ+");
    list.replaceInStrings("6","Being the parent, guardian, or carer of children or another person");
    list.replaceInStrings("7","Living in a rural or regional area");

    painter->save();

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(Qt::white);
    }

    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap, list.join("\n"));
    // QStyledItemDelegate::paint(painter, option, index);

    painter->restore();
}

QSize Delegate_Special_Mentees::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int size = index.data().toString().split(",").size();
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(600);
    t.setHeight(size*t.height());

    return t;
}

// Create New Editor Object
QWidget *Delegate_Special_Mentees::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    return nullptr;
}

// Model -> Editor
void Delegate_Special_Mentees::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

// Editor -> Model
void Delegate_Special_Mentees::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

// Geometry
void Delegate_Special_Mentees::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
