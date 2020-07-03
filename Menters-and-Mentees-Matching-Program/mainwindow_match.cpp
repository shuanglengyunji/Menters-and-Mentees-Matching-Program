#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::load_match_mentees()
{
    // connect
    disconnect(ui->checkBox_match_gender,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_type,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_language,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_hall,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_special,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_interests,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    disconnect(ui->checkBox_match_round,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);

    // [1] grouped mentors

    // clear exist data
    if ( model_match_mentors != nullptr )
    {
        delete model_match_mentors;
        model_match_mentors = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_match_mentors = new my_QSqlTableModel_Grouping(this,db);    // model_mentors is a private pointer defined in header file
    model_match_mentors->setTable("mentor");
    model_match_mentors->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_match_mentors->setFilter("group_id<>0");
    model_match_mentors->select();
    while(model_match_mentors->canFetchMore()){
        model_match_mentors->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_match_mentors->setModel(model_match_mentors);
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentors->sortByColumn(0,Qt::AscendingOrder);

    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();
    //connect(ui->tableView_match_mentors->horizontalHeader(),&QHeaderView::sectionResized,
    //        ui->tableView_match_mentors,&QTableView::resizeRowsToContents);

//    ui->tableView_match_mentors->hideColumn(0);
//    ui->tableView_match_mentors->hideColumn(1);
//    ui->tableView_match_mentors->hideColumn(5);
//    ui->tableView_match_mentors->hideColumn(17);
//    ui->tableView_match_mentors->hideColumn(18);
//    ui->tableView_match_mentors->hideColumn(19);
//    ui->tableView_match_mentors->hideColumn(20);

    // -----------------------------------------------------------------------------------
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
    //connect(ui->tableView_match_mentees_matched->horizontalHeader(),&QHeaderView::sectionResized,
    //        ui->tableView_match_mentees_matched,&QTableView::resizeRowsToContents);

    // -----------------------------------------------------------------------------------
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
    ui->tableView_match_mentees_to_be_match->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_match_mentees_to_be_match->setSortingEnabled(true);

    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::MultiSelection);
    //ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableView_match_mentees_to_be_match->hideColumn(0);

    // resize row height according to column width
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
    //connect(ui->tableView_match_mentees_to_be_match->horizontalHeader(),&QHeaderView::sectionResized,
    //        ui->tableView_match_mentees_to_be_match,&QTableView::resizeRowsToContents);

    connect(ui->checkBox_match_gender,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_type,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_language,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_hall,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_special,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_interests,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
    connect(ui->checkBox_match_round,&QCheckBox::stateChanged,this,&MainWindow::display_match_column);
}

void MainWindow::on_tableView_match_mentors_clicked(const QModelIndex &index)
{
    // Selected Row
    int row = index.row();
    int group_id = model_match_mentors->record(row).value("group_id").toInt();

    ui->tableView_match_mentors->clearSelection();
    for (int i=0; i < model_match_mentors->rowCount(); i++)
    {
        if (model_match_mentors->record(i).value("group_id").toInt()==group_id)
        {
            ui->tableView_match_mentors->selectRow(i);
        }
    }

    // Mentees Matched
    model_match_mentees_matched->setFilter(QString("group_id=%1").arg(group_id));

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();
}

void MainWindow::on_pushButton_Up_clicked()
{
    int row = ui->tableView_match_mentors->currentIndex().row();
    int group_id = model_match_mentors->record(row).value("group_id").toInt();

    QItemSelectionModel * selections = ui->tableView_match_mentees_to_be_match->selectionModel();
    QModelIndexList selected = selections->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        int row = selectedIndex.row();
        QSqlRecord r = model_match_mentees_to_be_match->record(row);
        r.setValue("group_id",group_id);
        model_match_mentees_to_be_match->setRecord(row,r);
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
    QItemSelectionModel * selections = ui->tableView_match_mentees_matched->selectionModel();
    QModelIndexList selected = selections->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        int row = selectedIndex.row();
        QSqlRecord r = model_match_mentees_matched->record(row);
        r.setValue("group_id",0);
        model_match_mentees_matched->setRecord(row,r);
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
    query.exec("UPDATE mentee SET group_id=0");
    model_match_mentees_to_be_match->select();
    model_match_mentees_matched->select();

    ui->tableView_match_mentees_matched->resizeColumnsToContents();
    ui->tableView_match_mentees_matched->resizeRowsToContents();

    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
}

void MainWindow::on_lineEdit_match_search_mentors_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentors->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "group_id<>0";
    if(str.isEmpty()) {
        model_match_mentors->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp).arg(tmp).arg(tmp);       // Uni ID + First Name + Last Name
        //qDebug() << argument;
    }
    model_match_mentors->setFilter(argument);
}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentees->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "group_id=0";
    if(str.isEmpty()) {
        model_match_mentees_to_be_match->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp).arg(tmp).arg(tmp);       // Uni ID + First Name + Last Name
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

void MainWindow::display_match_column()
{
    // round
    if (ui->checkBox_match_round->isChecked())
    {
        ui->tableView_match_mentors->showColumn(6);
        ui->tableView_match_mentees_matched->showColumn(4);
        ui->tableView_match_mentees_to_be_match->showColumn(4);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(6);
        ui->tableView_match_mentees_matched->hideColumn(4);
        ui->tableView_match_mentees_to_be_match->hideColumn(4);
    }

    // academic info
    if (ui->checkBox_match_academic_info->isChecked())
    {
        ui->tableView_match_mentors->showColumn(7);
        ui->tableView_match_mentors->showColumn(8);
        ui->tableView_match_mentors->showColumn(9);
        ui->tableView_match_mentees_matched->showColumn(5);
        ui->tableView_match_mentees_matched->showColumn(6);
        ui->tableView_match_mentees_matched->showColumn(7);
        ui->tableView_match_mentees_to_be_match->showColumn(5);
        ui->tableView_match_mentees_to_be_match->showColumn(6);
        ui->tableView_match_mentees_to_be_match->showColumn(7);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(7);
        ui->tableView_match_mentors->hideColumn(8);
        ui->tableView_match_mentors->hideColumn(9);
        ui->tableView_match_mentees_matched->hideColumn(5);
        ui->tableView_match_mentees_matched->hideColumn(6);
        ui->tableView_match_mentees_matched->hideColumn(7);
        ui->tableView_match_mentees_to_be_match->hideColumn(5);
        ui->tableView_match_mentees_to_be_match->hideColumn(6);
        ui->tableView_match_mentees_to_be_match->hideColumn(7);
    }

    // type
    if (ui->checkBox_match_type->isChecked())
    {
        ui->tableView_match_mentors->showColumn(10);
        ui->tableView_match_mentees_matched->showColumn(8);
        ui->tableView_match_mentees_to_be_match->showColumn(8);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(10);
        ui->tableView_match_mentees_matched->hideColumn(8);
        ui->tableView_match_mentees_to_be_match->hideColumn(8);
    }

    // gender
    if (ui->checkBox_match_gender->isChecked())
    {
        ui->tableView_match_mentors->showColumn(11);
        ui->tableView_match_mentees_matched->showColumn(9);
        ui->tableView_match_mentees_to_be_match->showColumn(9);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(11);
        ui->tableView_match_mentees_matched->hideColumn(9);
        ui->tableView_match_mentees_to_be_match->hideColumn(9);
    }

    // language
    if (ui->checkBox_match_language->isChecked())
    {
        ui->tableView_match_mentors->showColumn(12);
        ui->tableView_match_mentors->showColumn(13);
        ui->tableView_match_mentees_matched->showColumn(10);
        ui->tableView_match_mentees_matched->showColumn(11);
        ui->tableView_match_mentees_to_be_match->showColumn(10);
        ui->tableView_match_mentees_to_be_match->showColumn(11);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(12);
        ui->tableView_match_mentors->hideColumn(13);
        ui->tableView_match_mentees_matched->hideColumn(10);
        ui->tableView_match_mentees_matched->hideColumn(11);
        ui->tableView_match_mentees_to_be_match->hideColumn(10);
        ui->tableView_match_mentees_to_be_match->hideColumn(11);
    }

    // hall
    if (ui->checkBox_match_hall->isChecked())
    {
        ui->tableView_match_mentors->showColumn(14);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(14);
    }

    // special
    if (ui->checkBox_match_special->isChecked())
    {
        ui->tableView_match_mentors->showColumn(15);
        ui->tableView_match_mentees_matched->showColumn(12);
        ui->tableView_match_mentees_to_be_match->showColumn(12);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(15);
        ui->tableView_match_mentees_matched->hideColumn(12);
        ui->tableView_match_mentees_to_be_match->hideColumn(12);
    }

    // interests or requests
    if (ui->checkBox_match_interests->isChecked())
    {
        ui->tableView_match_mentors->showColumn(16);
        ui->tableView_match_mentees_matched->showColumn(13);
        ui->tableView_match_mentees_to_be_match->showColumn(13);
    }
    else
    {
        ui->tableView_match_mentors->hideColumn(16);
        ui->tableView_match_mentees_matched->hideColumn(13);
        ui->tableView_match_mentees_to_be_match->hideColumn(13);
    }
}

