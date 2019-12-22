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

    // delegate
    ui->tableView_match_mentors->setItemDelegateForColumn(1,delegate_yes_no);
    ui->tableView_match_mentors->setItemDelegateForColumn(17,delegate_yes_no);
    ui->tableView_match_mentors->setItemDelegateForColumn(18,delegate_yes_no);
    ui->tableView_match_mentors->setItemDelegateForColumn(19,delegate_yes_no);
    ui->tableView_match_mentors->setItemDelegateForColumn(20,delegate_yes_no);
    ui->tableView_match_mentors->setItemDelegateForColumn(6,delegate_round);
    ui->tableView_match_mentors->setItemDelegateForColumn(7,delegate_academic_level);
    ui->tableView_match_mentors->setItemDelegateForColumn(10,delegate_type);
    ui->tableView_match_mentors->setItemDelegateForColumn(11,delegate_gender);
    ui->tableView_match_mentors->setItemDelegateForColumn(12,delegate_language);
    ui->tableView_match_mentors->setItemDelegateForColumn(8,delegate_college);
    ui->tableView_match_mentors->setItemDelegateForColumn(15,delegate_special_mentors);

    // resize row height according to column width
    ui->tableView_match_mentors->resizeColumnsToContents();
    ui->tableView_match_mentors->resizeRowsToContents();
    //connect(ui->tableView_match_mentors->horizontalHeader(),&QHeaderView::sectionResized,
    //        ui->tableView_match_mentors,&QTableView::resizeRowsToContents);

    ui->tableView_match_mentors->hideColumn(0);
    ui->tableView_match_mentors->hideColumn(1);
    ui->tableView_match_mentors->hideColumn(5);
    ui->tableView_match_mentors->hideColumn(17);
    ui->tableView_match_mentors->hideColumn(18);
    ui->tableView_match_mentors->hideColumn(19);
    ui->tableView_match_mentors->hideColumn(20);

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

    ui->tableView_match_mentees_matched->setItemDelegateForColumn(4,delegate_round);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(5,delegate_academic_level);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(8,delegate_type);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(9,delegate_gender);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(10,delegate_language);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(6,delegate_college);
    ui->tableView_match_mentees_matched->setItemDelegateForColumn(12,delegate_special_mentees);

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

    // delegate
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(4,delegate_round);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(5,delegate_academic_level);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(8,delegate_type);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(9,delegate_gender);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(10,delegate_language);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(6,delegate_college);
    ui->tableView_match_mentees_to_be_match->setItemDelegateForColumn(12,delegate_special_mentees);

    ui->tableView_match_mentees_to_be_match->hideColumn(0);

    // resize row height according to column width
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
    ui->tableView_match_mentees_to_be_match->resizeRowsToContents();
    //connect(ui->tableView_match_mentees_to_be_match->horizontalHeader(),&QHeaderView::sectionResized,
    //        ui->tableView_match_mentees_to_be_match,&QTableView::resizeRowsToContents);

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

