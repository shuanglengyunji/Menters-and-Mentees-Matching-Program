#include "delegate_special_mentors.h"
#include <QPainter>
#include <QComboBox>
#include <QDebug>

Delegate_Special_Mentors::Delegate_Special_Mentors(QObject *parent) : QStyledItemDelegate(parent)
{

}

void Delegate_Special_Mentors::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList list = index.data().toString().split(",");
    list.sort();

    list.replaceInStrings("10","A mentee who is particularly shy or lacks confidence and wants someone patient (yes this is a request we get)");
    list.replaceInStrings("0","An international student wanting to learn about Australian culture");
    list.replaceInStrings("1","An exchange student (rather than a student new to university)");
    list.replaceInStrings("2","Being a mature age student (greater than three years between school/uni or degrees)");
    list.replaceInStrings("3","A mentee who lives with disability or mental illness");
    list.replaceInStrings("4","A mentee who works more than 20hrs/wk while studying");
    list.replaceInStrings("5","A mentee who identifies as LGBTIQ+");
    list.replaceInStrings("6","A mentee who is the parent, guardian, or carer of children or another person");
    list.replaceInStrings("7","A mentee from a rural area");
    list.replaceInStrings("8","A mentee who is under the age of 18");
    list.replaceInStrings("9","A mentee who is particularly uncomfortable speaking in English (but wants to improve)");

    painter->save();

    if (option.state & QStyle::State_Selected)
    {
        if (option.state & QStyle::State_Active)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(option.palette.highlightedText().color());
        }
        else
        {
            painter->fillRect(option.rect, option.palette.alternateBase());
            painter->setPen(Qt::black);
        }
    }

    painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap, list.join("\n"));

    painter->restore();
}

QSize Delegate_Special_Mentors::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStringList list = index.data().toString().split(",");
    int size = list.size();
    for (int i=0;i<list.size();i++)
    {
        if (list.at(i).toInt() == 10 || list.at(i).toInt() == 2 || list.at(i).toInt() == 6 || list.at(i).toInt() == 9)
            size ++;
    }
    QSize t = QStyledItemDelegate::sizeHint(option, index);
    t.setWidth(600);
    t.setHeight(size*t.height());

    return t;
}

// Create New Editor Object
QWidget *Delegate_Special_Mentors::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    return nullptr;
}

// Model -> Editor
void Delegate_Special_Mentors::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

// Editor -> Model
void Delegate_Special_Mentors::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
}

// Geometry
void Delegate_Special_Mentors::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
