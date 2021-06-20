#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::load_group_mentors()
{
    disconnect(ui->checkBox_group_mentor_hall,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_type,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_round,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_gender,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_interests,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_language,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_requests,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    disconnect(ui->checkBox_group_mentor_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);

    // mentors to be grouped

    // clear exist data
    if ( model_group_mentors_to_be_grouped != nullptr )
    {
        delete model_group_mentors_to_be_grouped;
        model_group_mentors_to_be_grouped = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_group_mentors_to_be_grouped = new myMentorsTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_group_mentors_to_be_grouped->setFilter("group_id=0 AND wwvp=1 AND train_complete=1");
    model_group_mentors_to_be_grouped->select();
    while(model_group_mentors_to_be_grouped->canFetchMore()){
        model_group_mentors_to_be_grouped->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_to_be_group->setModel(model_group_mentors_to_be_grouped);
    ui->tableView_group_mentor_to_be_group->reset();
    ui->tableView_group_mentor_to_be_group->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_group_mentor_to_be_group->setSortingEnabled(true);

    ui->tableView_group_mentor_to_be_group->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView_group_mentor_to_be_group->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_group_mentor_to_be_group->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // resize row height according to column width
    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();

    // hide columns
    ui->tableView_group_mentor_to_be_group->hideColumn(0);
    ui->tableView_group_mentor_to_be_group->hideColumn(1);
    ui->tableView_group_mentor_to_be_group->hideColumn(6);
    ui->tableView_group_mentor_to_be_group->hideColumn(18);
    ui->tableView_group_mentor_to_be_group->hideColumn(19);
    ui->tableView_group_mentor_to_be_group->hideColumn(20);
    ui->tableView_group_mentor_to_be_group->hideColumn(21);

    //--------------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------

    // mentors grouped

    // clear exist data
    if ( model_group_mentors_grouped != nullptr )
    {
        delete model_group_mentors_grouped;
        model_group_mentors_grouped = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_group_mentors_grouped = new myMentorsTableModel(this,db,true);    // model_mentors is a private pointer defined in header file
    model_group_mentors_grouped->setFilter("group_id<>0 AND wwvp=1 AND train_complete=1");
    model_group_mentors_grouped->select();
    while(model_group_mentors_grouped->canFetchMore()){
        model_group_mentors_grouped->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_grouped->setModel(model_group_mentors_grouped);
    ui->tableView_group_mentor_grouped->reset();
    ui->tableView_group_mentor_grouped->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_group_mentor_grouped->sortByColumn(0,Qt::AscendingOrder);

    ui->tableView_group_mentor_grouped->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->tableView_group_mentor_grouped->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();

    // hide columns
    ui->tableView_group_mentor_grouped->hideColumn(0);
    ui->tableView_group_mentor_grouped->hideColumn(1);
    ui->tableView_group_mentor_grouped->hideColumn(6);
    ui->tableView_group_mentor_grouped->hideColumn(18);
    ui->tableView_group_mentor_grouped->hideColumn(19);
    ui->tableView_group_mentor_grouped->hideColumn(20);
    ui->tableView_group_mentor_grouped->hideColumn(21);

    connect(ui->checkBox_group_mentor_hall,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_type,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_round,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_gender,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_interests,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_language,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_requests,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
    connect(ui->checkBox_group_mentor_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_group_column);
}

// Grouped mentor search
void MainWindow::on_lineEdit_group_mentor_grouped_search_editingFinished()
{
    QString str = ui->lineEdit_group_mentor_grouped_search->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "group_id<>0 AND wwvp=1 AND train_complete=1";
    if(str.isEmpty()) {
        model_group_mentors_grouped->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp).arg(tmp).arg(tmp);       // Uni ID + First Name + Last Name
        //qDebug() << argument;
    }
    model_group_mentors_grouped->setFilter(argument);
}



// To be grouped mentor search
void MainWindow::on_lineEdit_group_mentor_to_be_group_search_editingFinished()
{
    QString str = ui->lineEdit_group_mentor_to_be_group_search->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "group_id=0 AND wwvp<>'' AND wwvp<>'n' AND train_complete='y'";
    if(str.isEmpty()) {
        model_group_mentors_to_be_grouped->setFilter(argument);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    for (int i=0; i < list.size() ; i++)
    {
        QString tmp = list.at(i);
        argument = argument + QString(" AND ( uid LIKE \'%%1%\' OR first_name LIKE '%%2%' OR last_name LIKE '%%3%' )").arg(tmp).arg(tmp).arg(tmp);       // Uni ID + First Name + Last Name
        //qDebug() << argument;
    }
    model_group_mentors_to_be_grouped->setFilter(argument);
}

// Add to Group
void MainWindow::on_toolButton_left_clicked()
{
    QItemSelectionModel * selections_grouped = ui->tableView_group_mentor_grouped->selectionModel();
    QModelIndexList selected_grouped = selections_grouped->selectedRows();

    int group_id;
    if ( selected_grouped.isEmpty() )
    {
        QSqlQueryModel querymodel;
        querymodel.setQuery("SELECT MAX(group_id) FROM mentor",db);
        while(querymodel.canFetchMore()){
            querymodel.fetchMore();
        }
        int max_group_id = querymodel.record(0).value(0).toInt();
        group_id = max_group_id+1;
    }
    else
    {
        int row = selected_grouped.at(0).row();
        group_id = model_group_mentors_grouped->record(row).value("group_id").toInt();
    }

    QItemSelectionModel * selections = ui->tableView_group_mentor_to_be_group->selectionModel();
    QModelIndexList selected = selections->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        int row = selectedIndex.row();
        QSqlRecord r = model_group_mentors_to_be_grouped->record(row);
        r.setValue("group_id",group_id);
        model_group_mentors_to_be_grouped->setRecord(row,r);
    }

    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();

    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();
}

// Remove from group
void MainWindow::on_toolButton_right_clicked()
{
    QItemSelectionModel * selections = ui->tableView_group_mentor_grouped->selectionModel();
    QModelIndexList selected = selections->selectedRows();
    foreach(QModelIndex selectedIndex, selected)
    {
        int row = selectedIndex.row();
        QSqlRecord r = model_group_mentors_grouped->record(row);
        r.setValue("group_id",0);
        model_group_mentors_grouped->setRecord(row,r);
    }
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();

    // re-arrange group-id
    QSqlQueryModel querymodel;
    QSqlQuery query(db);

    // get max group_id
    querymodel.setQuery("SELECT MAX(group_id) FROM mentor",db);
    while(querymodel.canFetchMore()){
        querymodel.fetchMore();
    }
    int max_group_id = querymodel.record(0).value(0).toInt();

    if (max_group_id == 0)
        return;

    int minus_num = 0;
    for (int current_id = 1; current_id <= max_group_id; current_id++)
    {
        querymodel.setQuery(QString("SELECT uid from mentor WHERE group_id=%1").arg(current_id),db);
        while(querymodel.canFetchMore()){
            querymodel.fetchMore();
        }

        if ( querymodel.rowCount()==0 )     // a group doesn't have any menber
        {
            minus_num++;
        }

        for (int i = 0; i < querymodel.rowCount(); i++)
        {
            QVariant uid = querymodel.record(i).value("uid");
            query.exec(QString("UPDATE mentor SET group_id=%1 WHERE uid=\'%2\'").arg(QVariant(current_id-minus_num).toString()).arg(uid.toString()));
        }
    }

    query.exec("UPDATE mentee SET group_id=0");

    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();

    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();
}

void MainWindow::on_pushButton_mentor_auto_clicked()
{
    algorithm_mentors_group();
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();

    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();
}

void MainWindow::on_pushButton_mentor_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("UPDATE mentor SET group_id=0");
    query.exec("UPDATE mentee SET group_id=0");
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();

    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();
}

void MainWindow::display_group_column()
{
    // round
    if (ui->checkBox_group_mentor_round->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(7);
        ui->tableView_group_mentor_to_be_group->showColumn(7);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(7);
        ui->tableView_group_mentor_to_be_group->hideColumn(7);
    }

    // academic info
    if (ui->checkBox_group_mentor_academic_info->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(8);
        ui->tableView_group_mentor_grouped->showColumn(9);
        ui->tableView_group_mentor_grouped->showColumn(10);
        ui->tableView_group_mentor_to_be_group->showColumn(8);
        ui->tableView_group_mentor_to_be_group->showColumn(9);
        ui->tableView_group_mentor_to_be_group->showColumn(10);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(8);
        ui->tableView_group_mentor_grouped->hideColumn(9);
        ui->tableView_group_mentor_grouped->hideColumn(10);
        ui->tableView_group_mentor_to_be_group->hideColumn(8);
        ui->tableView_group_mentor_to_be_group->hideColumn(9);
        ui->tableView_group_mentor_to_be_group->hideColumn(10);
    }

    // type
    if (ui->checkBox_group_mentor_type->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(11);
        ui->tableView_group_mentor_to_be_group->showColumn(11);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(11);
        ui->tableView_group_mentor_to_be_group->hideColumn(11);
    }

    // gender
    if (ui->checkBox_group_mentor_gender->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(12);
        ui->tableView_group_mentor_to_be_group->showColumn(12);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(12);
        ui->tableView_group_mentor_to_be_group->hideColumn(12);
    }

    // language
    if (ui->checkBox_group_mentor_language->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(13);
        ui->tableView_group_mentor_grouped->showColumn(14);
        ui->tableView_group_mentor_to_be_group->showColumn(13);
        ui->tableView_group_mentor_to_be_group->showColumn(14);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(13);
        ui->tableView_group_mentor_grouped->hideColumn(14);
        ui->tableView_group_mentor_to_be_group->hideColumn(13);
        ui->tableView_group_mentor_to_be_group->hideColumn(14);
    }

    // hall
    if (ui->checkBox_group_mentor_hall->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(15);
        ui->tableView_group_mentor_to_be_group->showColumn(15);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(15);
        ui->tableView_group_mentor_to_be_group->hideColumn(15);
    }

    // interests
    if (ui->checkBox_group_mentor_interests->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(16);
        ui->tableView_group_mentor_to_be_group->showColumn(16);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(16);
        ui->tableView_group_mentor_to_be_group->hideColumn(16);
    }

    // requests
    if (ui->checkBox_group_mentor_requests->isChecked())
    {
        ui->tableView_group_mentor_grouped->showColumn(17);
        ui->tableView_group_mentor_to_be_group->showColumn(17);
    }
    else
    {
        ui->tableView_group_mentor_grouped->hideColumn(17);
        ui->tableView_group_mentor_to_be_group->hideColumn(17);
    }
}


