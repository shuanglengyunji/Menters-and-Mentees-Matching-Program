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

    // link db to mentors QSqlTableModel
    model_match_mentors = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentors->setTable("mentor");
    model_match_mentors->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_match_mentors->setFilter("group_id<>0 AND is_confirmed='y' AND wwvp<>'' AND wwvp<>'n' AND train_complete='y'");
    model_match_mentors->select();
    while(model_match_mentors->canFetchMore()){
        model_match_mentors->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_match_mentors->setModel(model_match_mentors);
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();
    ui->tableView_match_mentors->sortByColumn(0);
    ui->tableView_match_mentors->setSortingEnabled(true);

    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    connect(ui->tableView_match_mentors->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_match_mentors,&QTableView::resizeRowsToContents);

    // [2] matched mentees

    // clear exist data
    if ( model_match_mentees_matched != nullptr )
    {
        delete model_match_mentees_matched;
        model_match_mentees_matched = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_match_mentees_matched = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentees_matched->setTable("mentee");
    model_match_mentees_matched->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_match_mentees_matched->setFilter("group_id<>0");
    model_match_mentees_matched->select();
    while(model_match_mentees_matched->canFetchMore()){
        model_match_mentees_matched->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_match_mentees_matched->setModel(model_match_mentees_matched);
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();
    ui->tableView_match_mentees_matched->setSortingEnabled(true);

    ui->tableView_match_mentees_matched->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_matched->setSelectionMode(QAbstractItemView::MultiSelection);
    //ui->tableView_match_mentees_matched->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    connect(ui->tableView_match_mentees_matched->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_match_mentees_matched,&QTableView::resizeRowsToContents);

    // [3] mentees to be match

    // clear exist data
    if ( model_match_mentees_to_be_match != nullptr )
    {
        delete model_match_mentees_to_be_match;
        model_match_mentees_to_be_match = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_match_mentees_to_be_match = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentees_to_be_match->setTable("mentee");
    model_match_mentees_to_be_match->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_match_mentees_to_be_match->setFilter("group_id=0");
    model_match_mentees_to_be_match->select();
    while(model_match_mentees_to_be_match->canFetchMore()){
        model_match_mentees_to_be_match->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_match_mentees_to_be_match->setModel(model_match_mentees_to_be_match);
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
    ui->tableView_match_mentees_to_be_match->setSortingEnabled(true);

    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::MultiSelection);
    //ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    connect(ui->tableView_match_mentees_to_be_match->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_match_mentees_to_be_match,&QTableView::resizeRowsToContents);

}

void MainWindow::on_pushButton_Clear_clicked()
{
    QSqlQuery query(db);
    query.exec("UPDATE mentee SET group_id=0");
    model_match_mentees_to_be_match->select();
    model_match_mentees_matched->select();
}

void MainWindow::on_pushButton_Up_clicked()
{
//    // get u_num
//    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
//    QString Mentor_U_Num = model_proxy_match_mentors->index(row_mentor,0).data().toString();

//    int row_mentee = ui->tableView_match_mentees_to_be_match->selectionModel()->currentIndex().row();
//    QString Mentee_U_Num = model_proxy_match_mentees_to_be_match->index(row_mentee,0).data().toString();

//    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
//    {
//        return;
//    }

//    match_manual_add(Mentor_U_Num,Mentee_U_Num);

}

void MainWindow::on_pushButton_Down_clicked()
{
//    // get u_num
//    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
//    QString Mentor_U_Num = model_proxy_match_mentors->index(row_mentor,0).data().toString();

//    int row_mentee = ui->tableView_match_mentees_matched->selectionModel()->currentIndex().row();
//    QString Mentee_U_Num = model_match_mentees_matched->record(row_mentee).value(0).toString();

//    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
//    {
//        return;
//    }

//    match_manual_remove(Mentor_U_Num,Mentee_U_Num);
}

void MainWindow::on_tableView_match_mentors_clicked(const QModelIndex &index)
{
//    // Selected Row
//    int row = index.row();
//    QString Mentor_U_Num = model_proxy_match_mentors->index(row,0).data().toString();

//    // Mentees Matched
//    QString str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
//    model_match_mentees_matched->setQuery(str,db);
//    refresh_match_mentees_matched_view();
}

void MainWindow::on_lineEdit_match_search_mentors_editingFinished()
{

}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{

}

void MainWindow::on_pushButton_Auto_clicked()
{
    int gender = ui->comboBox_gender->currentIndex();
    int college = ui->comboBox_college->currentIndex();
    int language = ui->comboBox_language->currentIndex();
    int academiclevel = ui->comboBox_academic_level->currentIndex();
    int consideration = ui->comboBox_consideration->currentIndex();
    int max_mentees_num = ui->spinBox_mentees_num->value();
}

