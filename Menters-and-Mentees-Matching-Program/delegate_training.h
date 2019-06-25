#ifndef DELEGATE_TRAINING_H
#define DELEGATE_TRAINING_H

#include <QItemDelegate>

class Delegate_Training : public QItemDelegate
{
    Q_OBJECT

public:
    Delegate_Training(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DELEGATE_TRAINING_H
