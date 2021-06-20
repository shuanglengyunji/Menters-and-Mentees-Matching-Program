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
#define MENTEES_COLUMN_NUM 15;

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
        content_list.append(QString::number(0));
        for (int col = 1; col <= mentors_max_col; col = col + 1) {
            // get data
            QString data = QString();
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                data = xlsxR.cellAt(row, col)->readValue().toString().simplified();
            }
            // parse data
            content_list.append("\"" + mentorsTable.get_parser(col).to_idx(data) + "\"");
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
        content_list.append(QString::number(0));
        for (int col = 1; col <= mentees_max_col; col = col + 1) {
            // get data
            QString data = QString();
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                data = xlsxR.cellAt(row, col)->readValue().toString().simplified();
            }
            // parse data
            content_list.append("\"" + menteesTable.get_parser(col).to_idx(data) + "\"");
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

    // [5] Import Matching Results

    if (include_match_result)
    {
        QSqlQuery query(db);

        if ( !xlsxR.selectSheet("Group"))
        {
            QMessageBox::warning(this, tr("Group Sheet Missing"), tr("Unable to Import Matching Results."));
            return;
        }
        int group_max = xlsxR.dimension().lastRow();

        for (int row = 2;row <= group_max; row = row + 1)
        {
            QVariant group_id = xlsxR.cellAt(row, 1)->readValue().toString();
            QVariant uid = xlsxR.cellAt(row, 2)->readValue().toString();
            QVariant role = xlsxR.cellAt(row, 3)->readValue().toString();

            if (role == "mentor")
            {
                query.exec(QString("UPDATE mentor SET group_id=%1 WHERE uid=\'%2\'").arg(group_id.toString()).arg(uid.toString()));
                //qDebug() << query.lastError();
            }

            if (role == "mentee")
            {
                query.exec(QString("UPDATE mentee SET group_id=%1 WHERE uid=\'%2\'").arg(group_id.toString()).arg(uid.toString()));
                //qDebug() << query.lastError();
            }
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
    exmodel_mentors->setTable("mentor");
    exmodel_mentors->select();
    while(exmodel_mentors->canFetchMore()){
        exmodel_mentors->fetchMore();
    }

    int mentors_max = exmodel_mentors->rowCount();      //qDebug() << "row max:" << row_max;
    int mentors_max_col = MENTORS_COLUMN_NUM;

    xlsxW.selectSheet("Mentors");
    // write header
    for (int col = 1; col <= mentors_max_col; col = col + 1) {
        xlsxW.write(1, col, exmodel_mentors->get_parser(col).get_header());
    }
    // write data
    for (int row = 2; row < mentors_max+2; row = row + 1) {
        QSqlRecord r = exmodel_mentors->record(row);
        for (int col = 1; col <= mentors_max_col; col = col + 1) {
            xlsxW.write(row, col, exmodel_mentors->get_parser(col).to_str(r.value(col).toString()));
        }
    }

    delete exmodel_mentors;

    // mentees

    myMenteesTableModel * exmodel_mentees = new myMenteesTableModel(this,db);
    exmodel_mentees->setTable("mentee");
    exmodel_mentees->select();
    while(exmodel_mentees->canFetchMore()){
        exmodel_mentees->fetchMore();
    }


    int mentees_max = exmodel_mentees->rowCount();      //qDebug() << "row max:" << row_max;
    int mentees_max_col = MENTEES_COLUMN_NUM;

    xlsxW.selectSheet("Mentees");
    // write header
    for (int col = 1; col <= mentees_max_col; col = col + 1) {
        xlsxW.write(1, col, exmodel_mentees->get_parser(col).get_header());
    }
    // write data
    for (int row = 2; row < mentees_max+2; row = row + 1) {
        QSqlRecord r = exmodel_mentees->record(row);
        for (int col = 1; col <= mentees_max_col; col = col + 1) {
            xlsxW.write(row, col, exmodel_mentees->get_parser(col).to_str(r.value(col).toString()));
        }
    }

    delete exmodel_mentees;

//    // [4] matching

//    if (include_match_result)
//    {
//        // Add Sheet and Sheet Headers
//        xlsxW.addSheet("Group");

//        xlsxW.write("A1","group_id");
//        xlsxW.write("B1","uid");
//        xlsxW.write("C1","role");

//        // Export data

//        int row_count = 2;

//        // mentor

//        QSqlTableModel * exmodel_mentors_matching = new QSqlTableModel(this,db);
//        exmodel_mentors_matching->setTable("mentor");
//        exmodel_mentors_matching->select();
//        while(exmodel_mentors_matching->canFetchMore()){
//            exmodel_mentors_matching->fetchMore();
//        }

//        for (int row = 0; row < exmodel_mentors_matching->rowCount(); row++)
//        {
//            QSqlRecord r = exmodel_mentors_matching->record(row);

//            QVariant group_id = r.value(0);
//            QVariant uid = r.value(4);

//            if (group_id != "0")
//            {
//                xlsxW.write("A"+QVariant(row_count).toString(),group_id);
//                xlsxW.write("B"+QVariant(row_count).toString(),uid);
//                xlsxW.write("C"+QVariant(row_count).toString(),"mentor");

//                row_count++;
//            }
//        }

//        delete exmodel_mentors_matching;

//        // mentee

//        QSqlTableModel * exmodel_mentees_matching = new QSqlTableModel(this,db);
//        exmodel_mentees_matching->setTable("mentee");
//        exmodel_mentees_matching->select();
//        while(exmodel_mentees_matching->canFetchMore()){
//            exmodel_mentees_matching->fetchMore();
//        }

//        for (int row = 0; row < exmodel_mentees_matching->rowCount(); row++)
//        {
//            QSqlRecord r = exmodel_mentees_matching->record(row);

//            QVariant group_id = r.value(0);
//            QVariant uid = r.value(3);

//            if (group_id != "0")
//            {
//                xlsxW.write("A"+QVariant(row_count).toString(),group_id);
//                xlsxW.write("B"+QVariant(row_count).toString(),uid);
//                xlsxW.write("C"+QVariant(row_count).toString(),"mentee");

//                row_count++;
//            }
//        }

//        delete exmodel_mentees_matching;
//    }

    // [5]
    if ( !xlsxW.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

// ---------------------------------------

void MainWindow::export_wattle_file(QString addr, int type)
{
    // type: 0 - mentor/mentee label in group     1 - group_id in group

    if(addr.isEmpty())
    {
        return;
    }

    // csv object
    CSVWriter csv(",");
    csv.newRow() << "user" << "group";  // header

    // mentors data

    QSqlTableModel * exwattle_mentors = new QSqlTableModel(this,db);
    exwattle_mentors->setTable("mentor");
    exwattle_mentors->setSort(0,Qt::AscendingOrder);
    exwattle_mentors->select();
    while(exwattle_mentors->canFetchMore()){
        exwattle_mentors->fetchMore();
    }

    for (int row = 0; row < exwattle_mentors->rowCount(); row++)
    {
        QSqlRecord r = exwattle_mentors->record(row);

        QVariant group_id = r.value(0);
        QVariant uid = r.value(4);// change from r.value(4) to r.value(3)

        if (group_id != "0")
        {
            if (type)
                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
            else
                csv.newRow() << uid.toString().toStdString() << "mentor";
        }
    }

    delete exwattle_mentors;

    // mentees data

    QSqlTableModel * exwattle_mentees = new QSqlTableModel(this,db);
    exwattle_mentees->setTable("mentee");
    exwattle_mentees->setSort(0,Qt::AscendingOrder);
    exwattle_mentees->select();
    while(exwattle_mentees->canFetchMore())
    {
        exwattle_mentees->fetchMore();
    }

    for (int row = 0; row < exwattle_mentees->rowCount(); row++)
    {
        QSqlRecord r = exwattle_mentees->record(row);

        QVariant group_id = r.value(0);
        QVariant uid = r.value(3);

        if (group_id != "0")
        {
            if (type)
                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
            else
                csv.newRow() << uid.toString().toStdString() << "mentee";
        }
    }

    delete exwattle_mentees;

    // write to file
    csv.writeToFile(addr.toStdString());
}



/*
// Mentors
xlsxW.addSheet("Mentors");

xlsxW.write("A1","Confirmation");
xlsxW.write("B1","First Name");
xlsxW.write("C1","Last Name");
xlsxW.write("D1","Uni ID");
xlsxW.write("E1","WWVP");
xlsxW.write("F1","O-Week");
xlsxW.write("G1","UG/PG");
xlsxW.write("H1","Academic College");
xlsxW.write("I1","Degree");
xlsxW.write("J1","Dom/Int");
xlsxW.write("K1","Gender");
xlsxW.write("L1","Languages");
xlsxW.write("M1","Language - Text");
xlsxW.write("N1","Residential Hall");
xlsxW.write("O1","Special Categories");
xlsxW.write("P1","Interests");
xlsxW.write("Q1","Training 1");
xlsxW.write("R1","Training 2");
xlsxW.write("S1","Training 3");
xlsxW.write("T1","Training Complete");

// Mentees
xlsxW.addSheet("Mentees");

xlsxW.write("A1","First Name");
xlsxW.write("B1","Last Name");
xlsxW.write("C1","Uni ID");
xlsxW.write("D1","Round");
xlsxW.write("E1","UG/PG");
xlsxW.write("F1","Academic College");
xlsxW.write("G1","u18");
xlsxW.write("H1","Dom/Int");
xlsxW.write("I1","Gender");
xlsxW.write("J1","Languages");
xlsxW.write("K1","Language - Text");
xlsxW.write("L1","Special Categories");
xlsxW.write("M1","Requests");

*/
