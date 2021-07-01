#include "mymentorstablemodel.h"
#include <QSqlQuery>
#include <qsqlrecord.h>
#include <QDebug>
#include <QList>

void myMentorsTableModel::create_table() {
    QSqlQuery query(this->database());
    query.exec(this->table_init_query);
}

QVariant myMentorsTableModel::headerData(int section,  Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        QList<parser> list = this->parser_list;
        for(int i=0; i<list.size(); i++) {
            parser p = list[i];
            if (p.get_index() == section) {
                return p.get_header();
            }
        }
        return QSqlTableModel::headerData( section, orientation, role );
    } else {
        return QSqlTableModel::headerData( section, orientation, role );
    }
}

QVariant myMentorsTableModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    if (role == Qt::DisplayRole) {
        QList<parser> list = this->parser_list;
        for(int i=0; i<list.size(); i++) {
            parser p = list[i];
            if (p.get_index() == index.column()) {
                return p.to_str(QSqlTableModel::data(index,role).toString());
            }
        }
    }
    return QSqlTableModel::data(index,role);
}

Qt::ItemFlags myMentorsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QSqlTableModel::flags(index);
//    if (!QList<int>({4, 15, 16, 17, 18, 19}).contains(index.column())) {
//       result &= ~Qt::ItemIsEditable;
//    }
    return result;
}
