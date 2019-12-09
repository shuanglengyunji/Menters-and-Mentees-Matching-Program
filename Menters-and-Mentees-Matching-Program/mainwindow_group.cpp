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
    model_group_mentors_to_be_grouped->setFilter("group_id=0");
    model_group_mentors_to_be_grouped->select();

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_to_be_group->setModel(model_group_mentors_to_be_grouped);
    ui->tableView_group_mentor_to_be_group->reset();
    ui->tableView_group_mentor_to_be_group->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();
    ui->tableView_group_mentor_to_be_group->resizeRowsToContents();

    ui->tableView_group_mentor_to_be_group->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_to_be_group->setSelectionMode(QAbstractItemView::MultiSelection);

    // resize row height according to column width
    connect(ui->tableView_group_mentor_to_be_group->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_group_mentor_to_be_group,&QTableView::resizeRowsToContents);

    //int row_num = model_group_mentors_grouped->rowCount();

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
    model_group_mentors_grouped->setFilter("group_id<>0");
    model_group_mentors_grouped->select();

    // link mentors QSqlTableModel to QTableView
    ui->tableView_group_mentor_grouped->setModel(model_group_mentors_grouped);
    ui->tableView_group_mentor_grouped->reset();
    ui->tableView_group_mentor_grouped->horizontalHeader()->setMaximumSectionSize(700);
    ui->tableView_group_mentor_grouped->resizeColumnsToContents();
    ui->tableView_group_mentor_grouped->resizeRowsToContents();
    ui->tableView_group_mentor_grouped->sortByColumn(0,Qt::AscendingOrder);

    ui->tableView_group_mentor_grouped->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_grouped->setSelectionMode(QAbstractItemView::MultiSelection);

    // resize row height according to column width
    connect(ui->tableView_group_mentor_grouped->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_group_mentor_grouped,&QTableView::resizeRowsToContents);


    /*
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);
    */
}

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
}


void MainWindow::on_tableView_group_mentor_to_be_group_clicked(const QModelIndex &index)
{
    // qDebug() << ui->tableView_group_mentor_to_be_group->selectionModel()->selection().size();
}

void MainWindow::on_tableView_group_mentor_grouped_clicked(const QModelIndex &index)
{
    ui->tableView_group_mentor_grouped->selectionModel()->currentIndex().row();
}

