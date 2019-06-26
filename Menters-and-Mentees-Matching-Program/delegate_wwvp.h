#ifndef DELEGATE_WWVP_H
#define DELEGATE_WWVP_H

#include <QItemDelegate>

class delegate_WWVP : public QItemDelegate
{
    Q_OBJECT

public:
    delegate_WWVP(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DELEGATE_WWVP_H
