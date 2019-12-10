#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::algorithm_mentors_group()
{
    int max_num = ui->spinBox_mentors_num->value();                 // 1 - 5
    int round = ui->comboBox_mentor_round->currentIndex();          // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int level = ui->comboBox_mentor_academic_level->currentIndex(); // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int college = ui->comboBox_mentor_college->currentIndex();      // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int type = ui->comboBox_mentor_type->currentIndex();            // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int gender = ui->comboBox_mentor_gender->currentIndex();        // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int language = ui->comboBox_mentor_language->currentIndex();    // 0 - reave out; 1 - high priority; 2 - medium priority; 3 - low priority
    int special = ui->comboBox_mentor_special->currentIndex();      // 0 - reave out; 2 - high priority; 2 - medium priority; 3 - low priority

    QSqlTableModel * model_mentors = new QSqlTableModel(this,db);
    model_mentors->setTable("mentor");
    model_mentors->select();
    while(model_mentors->canFetchMore()){
        model_mentors->fetchMore();
    }

    QSqlTableModel * model_mentees = new QSqlTableModel(this,db);
    model_mentees->setTable("mentee");
    model_mentees->select();
    while(model_mentees->canFetchMore()){
        model_mentees->fetchMore();
    }

    // ------------------------------------------------------

    // your code goes here

    // Example:

    QSqlQuery query(db);
    if ( !query.exec("your sql sentence goes here") )
    {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
    }

    QSqlQueryModel querymodel;
    querymodel.setQuery("your sql sentence goes here",db);
    querymodel.record(23333).value(6666).toString();            // how to read a data








    // ------------------------------------------------------

    delete model_mentors;
    delete model_mentees;

}
