#include "delegate_wwvp.h"
#include <QComboBox>
#include <QPainter>
#include <QApplication>

Delegate_WWVP::Delegate_WWVP(QObject *parent)
{
}

QWidget *Delegate_WWVP::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    return QItemDelegate::createEditor(parent,option,index);
}

void Delegate_WWVP::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QItemDelegate::setEditorData(editor,index);
}

void Delegate_WWVP::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor,model,index);
}

void Delegate_WWVP::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    QItemDelegate::updateEditorGeometry(editor,option,index);
}

