#ifndef MYMENTORSTABLEMODEL_H
#define MYMENTORSTABLEMODEL_H

#include <QObject>
#include <qsqltablemodel.h>

class myMentorsTableModel : public QSqlTableModel
{
public:
    myMentorsTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase(), bool use_group_id_as_row_number = false);
    QVariant headerData(int section,  Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    bool use_group_id_as_row_number;
};

#endif // MYMENTORSTABLEMODEL_H
