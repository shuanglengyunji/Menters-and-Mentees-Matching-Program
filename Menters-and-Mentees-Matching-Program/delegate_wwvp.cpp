#include "delegate_wwvp.h"
#include <QComboBox>
#include <QPainter>
#include <QApplication>

delegate_WWVP::delegate_WWVP(QObject *parent)
{
}

QWidget *delegate_WWVP::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    return QItemDelegate::createEditor(parent,option,index);
}

void delegate_WWVP::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QItemDelegate::setEditorData(editor,index);
}

void delegate_WWVP::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor,model,index);
}

void delegate_WWVP::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{
    QItemDelegate::updateEditorGeometry(editor,option,index);
}

