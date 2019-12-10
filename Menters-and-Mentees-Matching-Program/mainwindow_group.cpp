#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::load_group_mentors()
{
    // mentors to be grouped

    // clear exist data
    if ( model_group_mentors_to_be_grouped != nullptr )
    {
        delete model_group_mentors_to_be_grouped;
        model_group_mentors_to_be_grouped = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_group_mentors_to_be_grouped = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_group_mentors_to_be_grouped->setTable("mentor");
    model_group_mentors_to_be_grouped->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_group_mentors_to_be_grouped->setFilter("group_id=0 AND is_confirmed='y' AND wwvp<>'' AND wwvp<>'n' AND train_complete='y'");
    model_group_mentors_to_be_grouped->select();
    while(model_group_mentors_to_be_grouped->canFetchMore()){
        model_group_mentors_to_be_grouped->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_to_be_group->setModel(model_group_mentors_to_be_grouped);
    ui->tableView_group_mentor_to_be_group->reset();
    ui->tableView_group_mentor_to_be_group->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_to_be_group->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    connect(ui->tableView_group_mentor_to_be_group->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_group_mentor_to_be_group,&QTableView::resizeRowsToContents);

    // hide columns
    ui->tableView_group_mentor_to_be_group->hideColumn(1);
    ui->tableView_group_mentor_to_be_group->hideColumn(5);
    ui->tableView_group_mentor_to_be_group->hideColumn(17);
    ui->tableView_group_mentor_to_be_group->hideColumn(18);
    ui->tableView_group_mentor_to_be_group->hideColumn(19);
    ui->tableView_group_mentor_to_be_group->hideColumn(20);

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
    model_group_mentors_grouped = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_group_mentors_grouped->setTable("mentor");
    model_group_mentors_grouped->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_group_mentors_grouped->setFilter("group_id<>0 AND is_confirmed='y' AND wwvp<>'' AND wwvp<>'n' AND train_complete='y'");
    model_group_mentors_grouped->select();
    while(model_group_mentors_grouped->canFetchMore()){
        model_group_mentors_grouped->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_grouped->setModel(model_group_mentors_grouped);
    ui->tableView_group_mentor_grouped->reset();
    ui->tableView_group_mentor_grouped->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();
    ui->tableView_group_mentor_grouped->sortByColumn(0,Qt::AscendingOrder);

    ui->tableView_group_mentor_grouped->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_grouped->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // resize row height according to column width
    connect(ui->tableView_group_mentor_grouped->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_group_mentor_grouped,&QTableView::resizeRowsToContents);

    // hide columns
    ui->tableView_group_mentor_grouped->hideColumn(1);
    ui->tableView_group_mentor_grouped->hideColumn(5);
    ui->tableView_group_mentor_grouped->hideColumn(17);
    ui->tableView_group_mentor_grouped->hideColumn(18);
    ui->tableView_group_mentor_grouped->hideColumn(19);
    ui->tableView_group_mentor_grouped->hideColumn(20);

    /*
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);
    */
}

// Add to Group
void MainWindow::on_toolButton_left_clicked()
{
    QItemSelectionModel * selections_grouped = ui->tableView_group_mentor_grouped->selectionModel();
    QModelIndexList selected_grouped = selections_grouped->selectedRows();

    int group_id = -1;
    if ( selected_grouped.isEmpty() )
    {
        group_id = model_group_mentors_grouped->record(model_group_mentors_grouped->rowCount()-1).value("group_id").toInt()+1;
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

    int max_row = model_group_mentors_grouped->rowCount();
    if (max_row == 0)
        return;

    int max_group_id = model_group_mentors_grouped->record(max_row-1).value("group_id").toInt();
    int minus_num = 0;
    for (int current_id = 1; current_id <= max_group_id; current_id++)
    {
        querymodel.setQuery(QString("SELECT uid from mentor WHERE group_id=%1").arg(current_id),db);
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
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();
}

void MainWindow::on_pushButton_mentor_auto_clicked()
{
    algorithm_mentors_group();
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();
}

void MainWindow::on_pushButton_mentor_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("UPDATE mentor SET group_id=0");
    model_group_mentors_to_be_grouped->select();
    model_group_mentors_grouped->select();
}

void MainWindow::on_tableView_group_mentor_grouped_clicked(const QModelIndex &index)
{
    // ui->tableView_group_mentor_grouped->selectionModel()->currentIndex().row();

}

