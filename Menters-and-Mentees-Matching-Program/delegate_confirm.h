#ifndef DELEGATE_CONFIRM_H
#define DELEGATE_CONFIRM_H

#include <QItemDelegate>

class Delegate_confirm : public QItemDelegate
{
public:
    Delegate_confirm(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // DELEGATE_CONFIRM_H
