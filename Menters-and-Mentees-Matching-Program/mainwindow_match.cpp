#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::init_match()
{
    // [1] mentors

    // link db to mentors myMenteesTableModel
    model_match_mentors = new myMentorsTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentors->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_match_mentors->setFilter("train_complete=1");
    model_match_mentors->select();
    while(model_match_mentors->canFetchMore()){
        model_match_mentors->fetchMore();
    }

    // link mentors myMenteesTableModel to QTableView
    ui->tableView_match_mentors->setModel(model_match_mentors);
    ui->tableView_match_mentors->horizontalHeader()->setMaximumSectionSize(400);

    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table_header_menu(ui->tableView_match_mentors);
    table_menu(ui->tableView_match_mentors);

    // -----------------------------------------------------------------------------------
    // [2] matched mentees

    // link db to myMenteesTableModel
    model_match_mentees_matched = new myMenteesTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentees_matched->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_match_mentees_matched->setFilter("mentor_uid IS NOT NULL");
    model_match_mentees_matched->select();
    while(model_match_mentees_matched->canFetchMore()){
        model_match_mentees_matched->fetchMore();
    }

    // link myMenteesTableModel to QTableView
    ui->tableView_match_mentees_matched->setModel(model_match_mentees_matched);
    ui->tableView_match_mentees_matched->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentees_matched->setSortingEnabled(true);

    ui->tableView_match_mentees_matched->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_matched->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentees_matched->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table_header_menu(ui->tableView_match_mentees_matched);
    table_menu(ui->tableView_match_mentees_matched);

    // -----------------------------------------------------------------------------------
    // [3] mentees to be match

    // link db to mentors myMenteesTableModel
    model_match_mentees_to_be_match = new myMenteesTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentees_to_be_match->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_match_mentees_to_be_match->setFilter("mentor_uid IS NULL");
    model_match_mentees_to_be_match->select();
    while(model_match_mentees_to_be_match->canFetchMore()){
        model_match_mentees_to_be_match->fetchMore();
    }

    // link mentors myMenteesTableModel to QTableView
    ui->tableView_match_mentees_to_be_match->setModel(model_match_mentees_to_be_match);
    ui->tableView_match_mentees_to_be_match->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentees_to_be_match->setSortingEnabled(true);

    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);

    table_header_menu(ui->tableView_match_mentees_to_be_match);
    table_menu(ui->tableView_match_mentees_to_be_match);
}

void MainWindow::load_match()
{
    model_match_mentors->select();
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();

    model_match_mentees_matched->select();
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    model_match_mentees_to_be_match->select();
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();

}

void MainWindow::on_tableView_match_mentors_clicked(const QModelIndex &index)
{
    // Find uid
    QString uid = model_match_mentors->record(index.row()).value("uid").toString();

    // Mentees Matched
    model_match_mentees_matched->setFilter(QString("mentor_uid='%1'").arg(uid));

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();
}

void MainWindow::on_pushButton_Up_clicked()
{
    QString uid = model_match_mentors->record(ui->tableView_match_mentors->currentIndex().row())
            .value("uid")
            .toString();

    QModelIndexList selected = ui->tableView_match_mentees_to_be_match->selectionModel()->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        QSqlRecord r = model_match_mentees_to_be_match->record(selectedIndex.row());
        r.setValue("mentor_uid", uid);
        model_match_mentees_to_be_match->setRecord(selectedIndex.row(), r);
    }

    model_match_mentees_matched->select();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    model_match_mentees_to_be_match->select();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
}

void MainWindow::on_pushButton_Down_clicked()
{
    QModelIndexList selected = ui->tableView_match_mentees_matched->selectionModel()->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        QSqlRecord r = model_match_mentees_matched->record(selectedIndex.row());
        r.setNull("mentor_uid");
        model_match_mentees_matched->setRecord(selectedIndex.row(), r);
    }
    model_match_mentees_matched->select();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    model_match_mentees_to_be_match->select();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
}

void MainWindow::on_pushButton_Clear_clicked()
{
    QSqlQuery query(db);
    query.exec("UPDATE mentee SET mentor_uid = NULL");

    model_match_mentees_to_be_match->select();
    model_match_mentees_matched->select();
    model_match_mentors->select();

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();

    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();
}

void MainWindow::on_lineEdit_match_search_mentors_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentors->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_match_mentors->setFilter("train_complete=1");
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    QStringList out;
    foreach(QString tmp, list)
    {
        out.append(QString("uid LIKE '\%%1\%' OR first_name LIKE '\%%2\%' OR last_name LIKE '\%%3\%'").arg(tmp, tmp, tmp));
    }
    model_match_mentors->setFilter(QString("train_complete=1 AND (%1)").arg(out.join(" OR ")));
}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentees->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_match_mentees_to_be_match->setFilter("mentor_uid IS NULL");
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    QStringList out;
    foreach(QString tmp, list)
    {
        out.append(QString("uid LIKE '\%%1\%' OR first_name LIKE '\%%2\%' OR last_name LIKE '\%%3\%'").arg(tmp, tmp, tmp));
    }
    model_match_mentees_to_be_match->setFilter(QString("mentor_uid IS NULL AND (%1)").arg(out.join(" OR ")));
}

void MainWindow::on_pushButton_Auto_clicked()
{
    algorithm_mentees_match();
    model_match_mentees_matched->select();
    model_match_mentees_to_be_match->select();

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
}
