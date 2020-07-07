#ifndef MYMENTEESTABLEMODEL_H
#define MYMENTEESTABLEMODEL_H

#include <QObject>
#include <qsqltablemodel.h>


class myMenteesTableModel : public QSqlTableModel
{
public:
    myMenteesTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // MYMENTEESTABLEMODEL_H
