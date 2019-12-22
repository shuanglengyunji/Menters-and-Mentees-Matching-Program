#include "delegate_yes_no_disp.h"
#include <QComboBox>
#include <QPainter>
#include <QApplication>

Delegate_Yes_No_Disp::Delegate_Yes_No_Disp(QObject *parent)
{
}

void Delegate_Yes_No_Disp::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    if (index.data() == "y" || index.data() == "n")
    {
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

        if (index.data() == "y")
        {
            painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "Yes");
        }
        else if (index.data() == "n")
        {
            painter->drawText(option.rect, Qt::AlignLeft|Qt::AlignVCenter, "No");
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QSize Delegate_Yes_No_Disp::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

// Create New Editor Object
QWidget *Delegate_Yes_No_Disp::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    return nullptr;
}

// Model -> Editor
void Delegate_Yes_No_Disp::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

// Editor -> Model
void Delegate_Yes_No_Disp::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

// Geometry
void Delegate_Yes_No_Disp::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


