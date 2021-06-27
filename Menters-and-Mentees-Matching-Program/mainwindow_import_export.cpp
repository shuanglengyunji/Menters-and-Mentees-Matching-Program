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

    QSqlQuery query(db);
    query.exec("DELETE FROM mentor");
    query.exec("DELETE FROM mentee");

    ui->plainTextEdit_msg_output->clear();
    if (include_match_result) {
        ui->plainTextEdit_msg_output->insertPlainText("Import data & matching result\n");
    } else {
        ui->plainTextEdit_msg_output->insertPlainText("Import data\n");
    }
    ui->plainTextEdit_msg_output->repaint();

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

    int mentors_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentor";
    qDebug() << "row max:" << mentors_max;

    ui->plainTextEdit_msg_output->insertPlainText("\nImporting mentors data\n");
    ui->plainTextEdit_msg_output->insertPlainText(QString("Mentors row count: %1\n\n").arg(mentors_max));
    ui->plainTextEdit_msg_output->repaint();

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

        model.insertRecord(-1, r);
        if (!model.submitAll()) {
            qDebug() << r;
            qDebug() << model.lastError();
            model.revertAll();
            ui->plainTextEdit_msg_output->insertPlainText(QString("Failed to import row %1\n").arg(row));
            ui->plainTextEdit_msg_output->repaint();
        }
    }

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    model.setTable("mentee");
    model.select();
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    qDebug() << "Mentee";
    qDebug() << "row max:" << mentees_max;

    ui->plainTextEdit_msg_output->insertPlainText("\nImporting mentees data\n");
    ui->plainTextEdit_msg_output->insertPlainText(QString("Mentees row count: %1\n\n").arg(mentees_max));
    ui->plainTextEdit_msg_output->repaint();

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
        model.insertRecord(-1, r);
        if (!model.submitAll()) {
            qDebug() << r;
            qDebug() << model.lastError();
            model.revertAll();
            ui->plainTextEdit_msg_output->insertPlainText(QString("Failed to import row %1\n").arg(row));
            ui->plainTextEdit_msg_output->repaint();
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

    ui->plainTextEdit_msg_output->clear();
    if (include_match_result) {
        ui->plainTextEdit_msg_output->insertPlainText("Export data & matching result\n");
    } else {
        ui->plainTextEdit_msg_output->insertPlainText("Export data\n");
    }
    ui->plainTextEdit_msg_output->repaint();

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

void MainWindow::export_grouping(QString addr) {
    if(addr.isEmpty())
    {
       return;
    }

    ui->plainTextEdit_msg_output->clear();
    ui->plainTextEdit_msg_output->insertPlainText("Export grouping\n");
    ui->plainTextEdit_msg_output->repaint();

    QSqlQueryModel model;

    QXlsx::Document xlsx;
    xlsx.addSheet("Output - Groupings");
    xlsx.write(1, 1, "groupID");
    xlsx.write(1, 2, "uID");
    xlsx.write(1, 3, "First Name");
    xlsx.write(1, 4, "Role");

    model.setQuery("SELECT * FROM mentor WHERE mentor.uid IN (SELECT DISTINCT mentor_uid from mentee);", db);
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    int group_id = 0;
    int row_count = 2;
    for (int row=0; row < model.rowCount(); row++) {
        QSqlRecord r = model.record(row);
        group_id = group_id + 1;
        xlsx.write(row_count, 1, QVariant(group_id));
        xlsx.write(row_count, 2, r.value("uid"));
        xlsx.write(row_count, 3, r.value("first_name"));
        xlsx.write(row_count, 4, "mentor");
        row_count++;
        QSqlQueryModel mentee;
        mentee.setQuery(QString("SELECT * FROM mentee WHERE mentee.mentor_uid = '%1';").arg(r.value("uid").toString()), db);
        while(mentee.canFetchMore()) {
            mentee.fetchMore();
        }
        for (int i=0; i<mentee.rowCount(); i++) {
            QSqlRecord r = mentee.record(i);
            xlsx.write(row_count, 1, QVariant(group_id));
            xlsx.write(row_count, 2, r.value("uid"));
            xlsx.write(row_count, 3, r.value("first_name"));
            xlsx.write(row_count, 4, "mentee");
            row_count++;
        }
    }

    if ( !xlsx.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

void MainWindow::export_mentor(QString addr) {

    if(addr.isEmpty())
    {
       return;
    }

    ui->plainTextEdit_msg_output->clear();
    ui->plainTextEdit_msg_output->insertPlainText("Export mentors info\n");
    ui->plainTextEdit_msg_output->repaint();

    myMentorsTableModel mentorsTable;
    myMenteesTableModel menteesTable;

    QSqlQueryModel model;

    QXlsx::Document xlsx;
    xlsx.addSheet("Output - Mentors");

    model.setQuery("SELECT max(c) FROM (SELECT count(*) AS c FROM mentee WHERE mentor_uid IS NOT NULL GROUP BY mentor_uid);", db);
    int count = model.record(0).value(0).toInt();

    // add headers
    xlsx.write(1, 1, "group");
    xlsx.write(1, 2, "mentor_email");
    xlsx.write(1, 3, "mentor_name");
    for (int i=0; i < count; i++) {
        xlsx.write(1, 3+i*5+1, QString("mentee%1_name").arg(i+1));
        xlsx.write(1, 3+i*5+2, QString("mentee%1_email").arg(i+1));
        xlsx.write(1, 3+i*5+3, QString("mentee%1_altemail").arg(i+1));
        xlsx.write(1, 3+i*5+4, QString("mentee%1_lvlofstudy").arg(i+1));
        xlsx.write(1, 3+i*5+5, QString("mentee%1_college").arg(i+1));
    }

    // add results
    if (count) {
        model.setQuery("SELECT * FROM mentor WHERE mentor.uid IN (SELECT DISTINCT mentor_uid from mentee);", db);
        while (model.canFetchMore()) {
            model.fetchMore();
        }

        int group_id = 0;
        for (int row=0; row < model.rowCount(); row++) {
            QSqlRecord r = model.record(row);
            group_id = group_id + 1;
            xlsx.write(row+2, 1, QVariant(group_id));
            xlsx.write(row+2, 2, QString("%1@anu.edu.au").arg(r.value("uid").toString()));
            xlsx.write(row+2, 3, r.value("first_name"));

            QSqlQueryModel mentee;
            mentee.setQuery(QString("SELECT * FROM mentee WHERE mentee.mentor_uid = '%1';").arg(r.value("uid").toString()), db);
            while(mentee.canFetchMore()) {
                mentee.fetchMore();
            }
            for (int i=0; i<mentee.rowCount(); i++) {
                QSqlRecord r = mentee.record(i);
                xlsx.write(row+2, 3+i*5+1, r.value("first_name"));
                xlsx.write(row+2, 3+i*5+2, QString("%1@anu.edu.au").arg(r.value("uid").toString()));
                xlsx.write(row+2, 3+i*5+3, r.value("email"));
                xlsx.write(row+2, 3+i*5+4, menteesTable.get_parser("academic_level").to_str(r.value("academic_level").toString()));
                xlsx.write(row+2, 3+i*5+5, menteesTable.get_parser("college").to_str(r.value("college").toString()));
            }
        }
    }

    if ( !xlsx.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }

}

void MainWindow::export_mentee(QString addr) {
    if(addr.isEmpty())
    {
       return;
    }

    ui->plainTextEdit_msg_output->clear();
    ui->plainTextEdit_msg_output->insertPlainText("Export mentees info\n");
    ui->plainTextEdit_msg_output->repaint();

    myMentorsTableModel mentorsTable;
    myMenteesTableModel menteesTable;

    QSqlQueryModel model;

    QXlsx::Document xlsx;
    xlsx.addSheet("Output - Mentees");
    xlsx.write(1, 1, "groupID");
    xlsx.write(1, 2, "mentee_name");
    xlsx.write(1, 3, "mentee_email");
    xlsx.write(1, 4, "mentee_altemail");
    xlsx.write(1, 5, "mentor_name");
    xlsx.write(1, 6, "mentor_email");
    xlsx.write(1, 7, "mentor_phone");
    xlsx.write(1, 8, "mentor_lvlofstudy");
    xlsx.write(1, 9, "mentor_college");

    model.setQuery("SELECT * FROM mentor WHERE mentor.uid IN (SELECT DISTINCT mentor_uid from mentee);", db);
    while (model.canFetchMore()) {
        model.fetchMore();
    }

    int group_id = 0;
    int row_count = 2;
    for (int row=0; row < model.rowCount(); row++) {
        QSqlRecord r_mentor = model.record(row);

        QSqlQueryModel mentee;
        mentee.setQuery(QString("SELECT * FROM mentee WHERE mentee.mentor_uid = '%1';").arg(r_mentor.value("uid").toString()), db);
        while(mentee.canFetchMore()) {
            mentee.fetchMore();
        }

        group_id = group_id + 1;
        for (int i=0; i<mentee.rowCount(); i++) {

            QSqlRecord r_mentee = mentee.record(i);

            xlsx.write(row_count, 1, QVariant(group_id));
            xlsx.write(row_count, 2, r_mentee.value("first_name"));
            xlsx.write(row_count, 3, QString("%1@anu.edu.au").arg(r_mentee.value("uid").toString()));
            xlsx.write(row_count, 4, r_mentee.value("email"));

            xlsx.write(row_count, 5, r_mentor.value("first_name"));
            xlsx.write(row_count, 6, QString("%1@anu.edu.au").arg(r_mentor.value("uid").toString()));
            xlsx.write(row_count, 7, r_mentor.value("phone"));
            xlsx.write(row_count, 8, r_mentor.value("phone"));
            xlsx.write(row_count, 9, r_mentor.value("phone"));
            xlsx.write(row_count, 8, mentorsTable.get_parser("academic_level").to_str(r_mentor.value("academic_level").toString()));
            xlsx.write(row_count, 9, mentorsTable.get_parser("college").to_str(r_mentor.value("college").toString()));

            row_count++;
        }
    }

    if ( !xlsx.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

void MainWindow::clear_data() {

    ui->plainTextEdit_msg_output->clear();
    ui->plainTextEdit_msg_output->insertPlainText("Clear data\n");
    ui->plainTextEdit_msg_output->repaint();

    QSqlQuery query(db);
    query.exec("DELETE FROM mentor");
    query.exec("DELETE FROM mentee");
}
