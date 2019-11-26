#include "delegate_confirm.h"

Delegate_confirm::Delegate_confirm(QObject *parent)
{

}

QWidget *Delegate_confirm::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    return nullptr;
}

void Delegate_confirm::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void Delegate_confirm::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{

}

void Delegate_confirm::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex & index ) const
{

}
