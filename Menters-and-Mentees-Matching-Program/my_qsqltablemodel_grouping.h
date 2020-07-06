#ifndef MY_QSQLTABLEMODEL_GROUPING_H
#define MY_QSQLTABLEMODEL_GROUPING_H

#include <QObject>
#include <qsqltablemodel.h>

class my_QSqlTableModel_Grouping : public QSqlTableModel
{
public:
    my_QSqlTableModel_Grouping(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant headerData(int section,  Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // MY_QSQLTABLEMODEL_GROUPING_H
