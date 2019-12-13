#include "my_qsqltablemodel_grouping.h"
#include <qsqlrecord.h>

my_QSqlTableModel_Grouping::my_QSqlTableModel_Grouping(QObject *parent, QSqlDatabase db)
{

}

QVariant my_QSqlTableModel_Grouping::headerData(int section,  Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
    if( orientation == Qt::Vertical && role == Qt::DisplayRole )
    {
        return this->record(section).value("group_id");
    }
    else
    {
        return QSqlTableModel::headerData( section, orientation, role );
    }
}
