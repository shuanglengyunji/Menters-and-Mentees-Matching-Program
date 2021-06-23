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

#define MENTORS_COLUMN_NUM 20
#define MENTEES_COLUMN_NUM 15
#define MENTEES_MENTOR_COL 16

void MainWindow::import_data(QString addr, bool include_match_result)
{
    if(addr.isEmpty())
    {
        return;
    }

    myMentorsTableModel mentorsTable;
    myMenteesTableModel menteesTable;

    QSqlTableModel model(this, db);
    model.setEditStrategy(QSqlTableModel::OnManualSubmit);

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
    model.setTable("mentor");
    model.select();
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    model.removeRows(0, model.rowCount());
    model.submitAll();

    model.select();
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    int mentors_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentor";
    qDebug() << "row max:" << mentors_max;

    for (int row = 2; row <= mentors_max; row = row + 1)
    {
        QSqlRecord r = model.record();
        for (int col = 1; col <= MENTORS_COLUMN_NUM; col = col + 1) {
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                r.setValue(
                    mentorsTable.get_parser(col-1).get_table_header(),
                    mentorsTable.get_parser(col-1).to_idx(xlsxR.cellAt(row, col)->readValue().toString())
                );
            } else {
                r.setNull(mentorsTable.get_parser(col-1).get_table_header());
            }
        }
        if (!model.insertRecord(-1, r)) {
            qDebug() << r;
            qDebug() << model.lastError();
        }
    }
    model.submitAll();

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    model.setTable("mentee");
    model.select();
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    model.removeRows(0, model.rowCount());
    model.submitAll();

    model.select();
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentee";
    qDebug() << "row max:" << mentees_max;

    for (int row = 2; row <= mentees_max; row = row + 1)
    {
        QSqlRecord r = model.record();
        for (int col = 1; col <= MENTEES_COLUMN_NUM; col = col + 1) {    // the last column is mentor uid
            parser p = menteesTable.get_parser(col-1);
            if ( (int)xlsxR.cellAt(row, col) != 0) {
                r.setValue(
                    p.get_table_header(),
                    p.to_idx(xlsxR.cellAt(row, col)->readValue().toString())
                );
            } else {
                r.setNull(p.get_table_header());
            }
        }
        if (include_match_result) {
            parser p = menteesTable.get_parser(MENTEES_MENTOR_COL-1);
            if ( (int)xlsxR.cellAt(row, MENTEES_MENTOR_COL) != 0) {
                r.setValue(
                    p.get_table_header(),
                    p.to_idx(xlsxR.cellAt(row, MENTEES_MENTOR_COL)->readValue().toString())
                );
            }
        }
        if (!model.insertRecord(-1, r)) {
            qDebug() << r;
            qDebug() << model.lastError();
        }
    }
    model.submitAll();
}

// ---------------------------------------

void MainWindow::export_data(QString addr,bool include_match_result)
{
    if(addr.isEmpty())
    {
       return;
    }

    myMentorsTableModel mentorsTable;
    myMenteesTableModel menteesTable;

    QSqlTableModel model(this, db);

    // [1]  Create excel file object

    QXlsx::Document xlsxW;

    // [2]  Create Sheets
    xlsxW.addSheet("Mentors");
    xlsxW.addSheet("Mentees");

    // [3] Fill data into xlsx file

    // mentors

    model.setTable("mentor");
    model.select();
    while(model.canFetchMore()){
        model.fetchMore();
    }

    int mentors_max = model.rowCount();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentor";
    qDebug() << "Row max" << mentors_max;

    xlsxW.selectSheet("Mentors");
    // write header
    for (int col = 0; col < MENTORS_COLUMN_NUM; col = col + 1) {
        xlsxW.write(1, col+1, mentorsTable.get_parser(col).get_header());
    }
    // write data
    for (int row = 0; row < mentors_max; row = row + 1) {
        QSqlRecord r = model.record(row);
        for (int col = 0; col < MENTORS_COLUMN_NUM; col = col + 1) {
            xlsxW.write(row+2, col+1, mentorsTable.get_parser(col).to_str(r.value(col).toString()));
        }
    }

    // mentees

    model.setTable("mentee");
    model.select();
    while(model.canFetchMore()){
        model.fetchMore();
    }

    int mentees_max = model.rowCount();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentee";
    qDebug() << "Row max" << mentees_max;

    xlsxW.selectSheet("Mentees");
    // write header
    for (int col = 0; col < MENTEES_COLUMN_NUM; col = col + 1) {
        xlsxW.write(1, col+1, menteesTable.get_parser(col).get_header());
    }
    if (include_match_result) {
        xlsxW.write(1, MENTEES_MENTOR_COL, menteesTable.get_parser(MENTEES_MENTOR_COL-1).get_header());
    }
    // write data
    for (int row = 0; row < mentees_max; row = row + 1) {
        QSqlRecord r = model.record(row);
        for (int col = 0; col < MENTEES_COLUMN_NUM; col = col + 1) {
            xlsxW.write(row+2, col+1, menteesTable.get_parser(col).to_str(r.value(col).toString()));
        }
        if (include_match_result) {
            xlsxW.write(row+2, MENTEES_MENTOR_COL, menteesTable.get_parser(MENTEES_MENTOR_COL-1).to_str(r.value(MENTEES_MENTOR_COL-1).toString()));
        }
    }

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
