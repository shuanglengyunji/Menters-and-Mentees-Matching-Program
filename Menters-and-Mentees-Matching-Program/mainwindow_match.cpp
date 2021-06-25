#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::load_match_mentees()
{
    // [1] grouped mentors

    // clear exist data
    if ( model_match_mentors != nullptr )
    {
        delete model_match_mentors;
        model_match_mentors = nullptr;
    }

    // link db to mentors myMenteesTableModel
    model_match_mentors = new myMentorsTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentors->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_match_mentors->setFilter("wwvp=1 AND train_complete=1");
    model_match_mentors->select();
    while(model_match_mentors->canFetchMore()){
        model_match_mentors->fetchMore();
    }

    // link mentors myMenteesTableModel to QTableView
    ui->tableView_match_mentors->setModel(model_match_mentors);
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentors->sortByColumn(0,Qt::AscendingOrder);

    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();

    table_header_menu(ui->tableView_match_mentors);

    // -----------------------------------------------------------------------------------
    // [2] matched mentees

    // clear exist data
    if ( model_match_mentees_matched != nullptr )
    {
        delete model_match_mentees_matched;
        model_match_mentees_matched = nullptr;
    }

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
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentees_matched->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView_match_mentees_matched->setSortingEnabled(true);

    ui->tableView_match_mentees_matched->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_matched->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentees_matched->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_match_mentees_matched->hideColumn(0);

    // resize row height according to column width
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    table_header_menu(ui->tableView_match_mentees_matched);

    // -----------------------------------------------------------------------------------
    // [3] mentees to be match

    // clear exist data
    if ( model_match_mentees_to_be_match != nullptr )
    {
        delete model_match_mentees_to_be_match;
        model_match_mentees_to_be_match = nullptr;
    }

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
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentees_to_be_match->setSortingEnabled(true);

    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_match_mentees_to_be_match->hideColumn(0);

    // resize row height according to column width
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();

    table_header_menu(ui->tableView_match_mentees_matched);
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
    QSqlQuery query(db);
    QString uid = model_match_mentors->record(ui->tableView_match_mentors->currentIndex().row())
            .value("uid")
            .toString();
    QModelIndexList selected = ui->tableView_match_mentees_to_be_match->selectionModel()->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        QString mentee_uid = model_match_mentees_to_be_match->record(selectedIndex.row())
                .value("uid")
                .toString();
        query.exec(QString("UPDATE mentee SET mentor_uid = '%1' WHERE uid = '%2'").arg(uid, mentee_uid));
    }
    model_match_mentees_matched->select();
    model_match_mentees_to_be_match->select();

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
}

void MainWindow::on_pushButton_Down_clicked()
{
    QSqlQuery query(db);
    QModelIndexList selected = ui->tableView_match_mentees_matched->selectionModel()->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        QString uid = model_match_mentees_matched->record(selectedIndex.row())
                .value("uid")
                .toString();
        query.exec(QString("UPDATE mentee SET mentor_uid = NULL WHERE uid = '%1'").arg(uid));
    }
    model_match_mentees_matched->select();
    model_match_mentees_to_be_match->select();

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

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
    QString argument = "wwvp=1 AND train_complete=1";
    if(str.isEmpty()) {
        model_match_mentors->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp, tmp, tmp);       // Uni ID + First Name + Last Name
        //qDebug() << argument;
    }
    model_match_mentors->setFilter(argument);
}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentees->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "mentor_uid IS NOT NULL";
    if(str.isEmpty()) {
        model_match_mentees_to_be_match->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp, tmp, tmp);       // Uni ID + First Name + Last Name
        //qDebug() << argument;
    }
    model_match_mentees_to_be_match->setFilter(argument);
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
