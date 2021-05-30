#include "mymentorstablemodel.h"
#include <qsqlrecord.h>
#include <QDebug>

myMentorsTableModel::myMentorsTableModel(QObject *parent, QSqlDatabase db, bool use_group_id_as_row_number)
{
    this->use_group_id_as_row_number = use_group_id_as_row_number;
}

QVariant myMentorsTableModel::headerData(int section,  Qt::Orientation orientation, int role = Qt::DisplayRole) const
{
    if( this->use_group_id_as_row_number && orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return this->record(section).value("group_id");
    }
    else
    {
        return QSqlTableModel::headerData( section, orientation, role );
    }
}

QVariant myMentorsTableModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    // if (index.row() == 1) {
    //     QVariant header = this->headerData(index.column(), Qt::Horizontal);
    //     qDebug() << "colomn " << index.column() << "header " << header.toString();
    // }

    //confirmation
    if (index.column() == 1 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if(data == "y"){
            data = "I have read and understood the above text";
        }
        else{
            data = " ";
        }
        return QVariant(data);
    }
    //round
    if (index.column() == 7 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if(data == "0"){
            data = "Round 1";
        }
        else{
            data = "Round 2";
        }
        return QVariant(data);
    }
    //academic level
    if (index.column() == 8 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if(data == "0"){
            data = "Undergraduate";
        }
        else{
            data = "Postgraduate (coursework)";
        }
        //qDebug() << data;
        return QVariant(data);
    }
    //college
    QStringList college_tmp_list;
    if (index.column() == 9 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if (data.contains("0",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Asia and the Pacific");
               }
        if (data.contains("1",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Arts and Social Sciences");
               }
        if (data.contains("2",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Business and Economics");
               }
        if (data.contains("3",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Engineering and Computer Science");
               }
        if (data.contains("4",Qt::CaseInsensitive)) {
                   college_tmp_list.append("ANU College of Law");
               }
        if (data.contains("5",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Science");
               }
        if (data.contains("6",Qt::CaseInsensitive)) {
                   college_tmp_list.append("College of Health and Medicine");
               }
        data = college_tmp_list.join(",");
        return QVariant(data);
    }
    //type
    if (index.column() == 11 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if(data == "0"){
            data = "Domestic";
        }
        else{
            data = "International";
        }
        //qDebug() << data;
        return QVariant(data);
    }
    //gender
    if (index.column() == 12 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        if(data == "0"){
            data = "Female";
        }
        else if (data == "1"){
            data = "Male";
        }
        else if (data == "2"){
            data = "Other";
        }
        else{
            data = "Prefer not to say";
        }
        //qDebug() << data;
        return QVariant(data);
    }
    //language
    if (index.column() == 13 && role == Qt::DisplayRole) {
        QString data = QSqlTableModel::data(index,role).toString();
        data.replace("11","Other (please specify)");
        data.replace("10","Urdu");
        data.replace("9","Japanese");
        data.replace("8","Indonesian");
        data.replace("7","Cantonese");
        data.replace("6","Spanish");
        data.replace("5","Mandarin (Chinese)");
        data.replace("4","Tamil");
        data.replace("3","Korean");
        data.replace("2","German");
        data.replace("1","Vietnamese");
        data.replace("0","Hindi");
        return QVariant(data);
    }
        //interests
        if (index.column() == 16 && role == Qt::DisplayRole) {
            QString data = QSqlTableModel::data(index,role).toString();
            data.replace("11","Playing sport");
            data.replace("10","Playing or creating music");
            data.replace("9","Visual arts - drawing, painting, craft, etc.");
            data.replace("8","Performing arts - theatre, dance, etc.");
            data.replace("7","Watching movies");
            data.replace("6","Cooking or food");
            data.replace("5","Hiking, nature, and outdoor recreation");
            data.replace("4","Gardening");
            data.replace("3","Science fiction and fantasy");
            data.replace("2","Learning languages");
            data.replace("1","Comics, manga, and anime");
            data.replace("0","Travel");
            return QVariant(data);
        }

    return QSqlTableModel::data(index,role);
}

Qt::ItemFlags myMentorsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = QSqlTableModel::flags(index);
    if ((index.column() >= 1 && index.column() <= 4) || (index.column() >= 7 && index.column() <= 17))
    {
       result &= ~Qt::ItemIsEditable;
    }
    return result;
}
