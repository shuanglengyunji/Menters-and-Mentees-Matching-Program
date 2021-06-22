#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mymenteestablemodel.h"
#include "mymentorstablemodel.h"

#include "../CSVWriter/include/CSVWriter.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

#define MENTORS_COLUMN_NUM 20;
#define MENTEES_COLUMN_NUM 16;

void MainWindow::import_data(QString addr,bool include_match_result)
{
    if(addr.isEmpty())
    {
        return;
    }

    QSqlQuery query(db);

    // [0] Remove recording from all tables
    query.exec("DELETE FROM 'mentor'");
    query.exec("DELETE FROM 'mentee'");

    // [1] Reading excel file(*.xlsx)
    Document xlsxR(addr);
    if ( !xlsxR.load() ) // load excel file
    {
        QMessageBox::warning(this, tr("File authorization failed"), tr("Failed to load xlsx file."));
        return;
    }

    // [2] Check Existence of Sheets ("Mentors" and "Mentees")
    if ( !xlsxR.selectSheet("Mentors") )
    {
        QMessageBox::warning(this, tr("Mentors Sheet Missing"), tr("Please make sure there is a sheet named \"Mentors\" in the data file."));
        return;
    }
    if ( !xlsxR.selectSheet("Mentees") )
    {
        QMessageBox::warning(this, tr("Mentees Sheet Missing"), tr("Please make sure there is a sheet named \"Mentees\" in the data file."));
        return;
    }

    // [3] Import Mentors Sheet
    xlsxR.selectSheet("Mentors");
    int mentors_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    int mentors_max_col = MENTORS_COLUMN_NUM;
    qDebug() << "Mentor";
    qDebug() << "row max:" << mentors_max;
    qDebug() << "row max:" << mentors_max_col;

    myMentorsTableModel mentorsTable;
    for (int row = 2; row <= mentors_max; row = row + 1)
    {
        QStringList content_list;
        for (int col = 1; col <= mentors_max_col; col = col + 1) {
            // get data
            QString data = QString();
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                data = xlsxR.cellAt(row, col)->readValue().toString().simplified();
            }
            // parse data
            content_list.append(mentorsTable.get_parser(col-1).to_idx(data));
        }

        //execute sql
        QString sql = "INSERT INTO mentor VALUES (" + content_list.join(",") + ")";     // qDebug() << sql;
        if ( !query.exec(sql) )
        {
            qDebug() << "Failed to Import Row " << row;
            // QMessageBox::warning(this, tr("Mentor Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            qDebug() << sql;
            qDebug() << query.lastError();
            qDebug() << "\n";
        }

    }

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    int mentees_max_col = MENTEES_COLUMN_NUM;

    qDebug() << "Mentee";
    qDebug() << "row max:" << mentees_max;
    qDebug() << "row max:" << mentees_max_col;

    myMenteesTableModel menteesTable;
    for (int row = 2; row <= mentees_max; row = row + 1)
    {
        QStringList content_list;
        for (int col = 1; col <= mentees_max_col-1; col = col + 1) {    // the last column is mentor uid
            // get data
            QString data = "";
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                data = xlsxR.cellAt(row, col)->readValue().toString().simplified();
            }
            // parse data
            content_list.append(menteesTable.get_parser(col-1).to_idx(data));
        }

        // mentor uid column
        if (include_match_result) {
            // get data
            if ( (int)xlsxR.cellAt(row, mentees_max_col) != 0) {
                content_list.append(menteesTable.get_parser(mentees_max_col-1).to_idx(
                    xlsxR.cellAt(row, mentees_max_col)->readValue().toString().simplified()
                ));
            } else {
                content_list.append("NULL");
            }
        } else {
            content_list.append("NULL");
        }

        // execute sql
        QString sql = "INSERT INTO mentee VALUES (" + content_list.join(",") + ")";     // qDebug() << sql;
        if ( !query.exec(sql) )
        {
            qDebug() << "Failed to Import Row " << row;
            qDebug() << sql;
            qDebug() << query.lastError();
            // QMessageBox::warning(this, tr("Mentee Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            qDebug() << sql;
        }
    }
}

// ---------------------------------------

void MainWindow::export_data(QString addr,bool include_match_result)
{
    if(addr.isEmpty())
    {
       return;
    }

    // [1]  Create excel file object

    QXlsx::Document xlsxW;

    // [2]  Create Sheets
    xlsxW.addSheet("Mentors");
    xlsxW.addSheet("Mentees");

    // [3] Fill data into xlsx file

    // mentors

    myMentorsTableModel * exmodel_mentors = new myMentorsTableModel(this,db);

    int mentors_max = exmodel_mentors->rowCount();      //qDebug() << "row max:" << row_max;
    int mentors_max_col = MENTORS_COLUMN_NUM;

    xlsxW.selectSheet("Mentors");
    // write header
    for (int col = 0; col < mentors_max_col; col = col + 1) {
        xlsxW.write(1, col+1, exmodel_mentors->get_parser(col).get_header());
    }
    // write data
    for (int row = 0; row < mentors_max; row = row + 1) {
        QSqlRecord r = exmodel_mentors->record(row);
        for (int col = 0; col < mentors_max_col; col = col + 1) {
            xlsxW.write(row+2, col+1, exmodel_mentors->get_parser(col).to_str(
                r.value(col).toString()
            ));
        }
    }

    delete exmodel_mentors;

    // mentees

    myMenteesTableModel * exmodel_mentees = new myMenteesTableModel(this,db);

    int mentees_max = exmodel_mentees->rowCount();      //qDebug() << "row max:" << row_max;
    int mentees_max_col = MENTEES_COLUMN_NUM;
    if (!include_match_result) { // ignore mentor uid column
        mentees_max_col--;
    }

    xlsxW.selectSheet("Mentees");
    // write header
    for (int col = 0; col < mentees_max_col; col = col + 1) {
        xlsxW.write(1, col+1, exmodel_mentees->get_parser(col).get_header());
    }
    // write data
    for (int row = 0; row < mentees_max; row = row + 1) {
        QSqlRecord r = exmodel_mentees->record(row);
        for (int col = 0; col < mentees_max_col; col = col + 1) {
            xlsxW.write(row+2, col+1, exmodel_mentees->get_parser(col).to_str(
                r.value(col).toString()
            ));
        }
    }

    delete exmodel_mentees;

    // [5]
    if ( !xlsxW.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

// ---------------------------------------

void MainWindow::export_wattle_file(QString addr, int type)
{
//    myMentorsTableModel * model = new myMentorsTableModel(this, db);
//    model->setTable("mentor");
//    QSqlRecord r = model->record();
//    r.setValue("'uid'", "'u123'");
//    r.setGenerated("'uid'", false);
//    model->insertRecord(-1, r);
//    model->submitAll();
//    qDebug() << model->lastError();
//    delete model;
//    // type: 0 - mentor/mentee label in group     1 - group_id in group

//    if(addr.isEmpty())
//    {
//        return;
//    }

//    // csv object
//    CSVWriter csv(",");
//    csv.newRow() << "user" << "group";  // header

//    // mentors data

//    QSqlTableModel * exwattle_mentors = new QSqlTableModel(this,db);
//    exwattle_mentors->setTable("mentor");
//    exwattle_mentors->setSort(0,Qt::AscendingOrder);
//    exwattle_mentors->select();
//    while(exwattle_mentors->canFetchMore()){
//        exwattle_mentors->fetchMore();
//    }

//    for (int row = 0; row < exwattle_mentors->rowCount(); row++)
//    {
//        QSqlRecord r = exwattle_mentors->record(row);

//        QVariant group_id = r.value(0);
//        QVariant uid = r.value(4);// change from r.value(4) to r.value(3)

//        if (group_id != "0")
//        {
//            if (type)
//                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
//            else
//                csv.newRow() << uid.toString().toStdString() << "mentor";
//        }
//    }

//    delete exwattle_mentors;

//    // mentees data

//    QSqlTableModel * exwattle_mentees = new QSqlTableModel(this,db);
//    exwattle_mentees->setTable("mentee");
//    exwattle_mentees->setSort(0,Qt::AscendingOrder);
//    exwattle_mentees->select();
//    while(exwattle_mentees->canFetchMore())
//    {
//        exwattle_mentees->fetchMore();
//    }

//    for (int row = 0; row < exwattle_mentees->rowCount(); row++)
//    {
//        QSqlRecord r = exwattle_mentees->record(row);

//        QVariant group_id = r.value(0);
//        QVariant uid = r.value(3);

//        if (group_id != "0")
//        {
//            if (type)
//                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
//            else
//                csv.newRow() << uid.toString().toStdString() << "mentee";
//        }
//    }

//    delete exwattle_mentees;

//    // write to file
//    csv.writeToFile(addr.toStdString());
}
