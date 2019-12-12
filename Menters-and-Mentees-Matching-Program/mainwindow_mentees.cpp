#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentees

// load
void MainWindow::load_mentees()
{
    // connect
    disconnect(ui->checkBox_mentees_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_requests,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_special_categories,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    disconnect(ui->checkBox_mentees_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);

    // clear exist data
    if ( model_mentees != nullptr )
    {
        delete model_mentees;
        model_mentees = nullptr;
    }

    // link db to mentees QSqlTableModel
    model_mentees = new QSqlTableModel(this,db);    // model_mentees is a private pointer defined in header file
    model_mentees->setTable("mentee");
    model_mentees->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_mentees->select();

    // link mentees QSqlTableModel to QTableView
    ui->tableView_mentees->setModel(model_mentees);
    ui->tableView_mentees->reset();
    ui->tableView_mentees->horizontalHeader()->setMaximumSectionSize(400);

    // hide group id
    //ui->tableView_mentees->hideColumn(0);

    // delegate
    ui->tableView_mentees->setItemDelegateForColumn(4,delegate_round);
    ui->tableView_mentees->setItemDelegateForColumn(5,delegate_academic_level);
    ui->tableView_mentees->setItemDelegateForColumn(8,delegate_type);
    ui->tableView_mentees->setItemDelegateForColumn(9,delegate_gender);

    ui->tableView_mentees->resizeColumnsToContents();
    ui->tableView_mentees->resizeRowsToContents();

    connect(ui->tableView_mentees->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_mentees,&QTableView::resizeRowsToContents);

    // connect
    connect(ui->checkBox_mentees_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_requests,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_special_categories,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
}

// search
void MainWindow::on_lineEdit_mentees_search_editingFinished()
{
    QString str = ui->lineEdit_mentees_search->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_mentees->setFilter("");
        return;
    }
    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    int num = list.size();
    QString tmp = list.at(0);
    QString argument = "uid LIKE '%" + tmp + "%'";
    for (int i=0; i < num ; i++)
    {
        tmp = list.at(i);
        argument = argument
                + " OR " + "uid LIKE '%" + tmp + "%'"               // Uni ID
                + " OR " + "first_name LIKE '%" + tmp + "%'"        // First Name
                + " OR " + "last_name LIKE '%" + tmp + "%'";        // Last Name
    }
    model_mentees->setFilter(argument);
}

// delete
void MainWindow::on_pushButton_mentees_delete_clicked()
{
    while(model_mentees->canFetchMore())
    {
        model_mentees->fetchMore();
    }

    QSqlTableModel *tm = model_mentees;
    if (!tm)
        return;

    QModelIndexList currentSelection = ui->tableView_mentees->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        tm->removeRow(currentSelection.at(i).row());
    }

    if (tm){
        if(!tm->submitAll()){
            QSqlError err = tm->lastError();
            QMessageBox::warning(this, tr("Unable to delete"), tr("An error occurred while "
                                       "deleting the connection: ") + err.text());
        }
    }

    load_mentees();
}

void MainWindow::display_mentees_column()
{
    // round
    if (ui->checkBox_mentees_round->isChecked())
    {
        ui->tableView_mentees->showColumn(4);
    }
    else
    {
        ui->tableView_mentees->hideColumn(4);
    }

    // academic info
    if (ui->checkBox_mentees_academic_info->isChecked())
    {
        ui->tableView_mentees->showColumn(5);
        ui->tableView_mentees->showColumn(6);
        ui->tableView_mentees->showColumn(7);
    }
    else
    {
        ui->tableView_mentees->hideColumn(5);
        ui->tableView_mentees->hideColumn(6);
        ui->tableView_mentees->hideColumn(7);
    }

    // type
    if (ui->checkBox_mentees_type->isChecked())
    {
        ui->tableView_mentees->showColumn(8);
    }
    else
    {
        ui->tableView_mentees->hideColumn(8);
    }

    // gender
    if (ui->checkBox_mentees_gender->isChecked())
    {
        ui->tableView_mentees->showColumn(9);
    }
    else
    {
        ui->tableView_mentees->hideColumn(9);
    }

    // language
    if (ui->checkBox_mentees_language->isChecked())
    {
        ui->tableView_mentees->showColumn(10);
        ui->tableView_mentees->showColumn(11);
    }
    else
    {
        ui->tableView_mentees->hideColumn(10);
        ui->tableView_mentees->hideColumn(11);
    }

    // special categories
    if (ui->checkBox_mentees_special_categories->isChecked())
    {
        ui->tableView_mentees->showColumn(12);
    }
    else
    {
        ui->tableView_mentees->hideColumn(12);
    }

    // requests
    if (ui->checkBox_mentees_requests->isChecked())
    {
        ui->tableView_mentees->showColumn(13);
    }
    else
    {
        ui->tableView_mentees->hideColumn(13);
    }

}



/*
// clear
void MainWindow::on_pushButton_mentees_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM mentee");

    load_mentees();
}
*/

/*
// add
void MainWindow::on_pushButton_mentees_add_clicked()
{
    QSqlTableModel *tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentees->model());
    if (!tm)
        return;

    QModelIndex insertIndex = ui->tableView_mentees->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    tm->insertRow(row);
    insertIndex = tm->index(row, 0);

    ui->tableView_mentees->setCurrentIndex(insertIndex);
    ui->tableView_mentees->edit(insertIndex);
}
*/

//    model_mentees->setHeaderData(0, Qt::Horizontal, "Uni ID");
//    model_mentees->setHeaderData(1, Qt::Horizontal, "First Name");
//    model_mentees->setHeaderData(2, Qt::Horizontal, "Last Name");
//    model_mentees->setHeaderData(3, Qt::Horizontal, "Gender");
//    model_mentees->setHeaderData(4, Qt::Horizontal, "Email");
//    model_mentees->setHeaderData(5, Qt::Horizontal, "Mobile");
//    model_mentees->setHeaderData(6, Qt::Horizontal, "Academic Level");
//    model_mentees->setHeaderData(7, Qt::Horizontal, "College");
//    model_mentees->setHeaderData(8, Qt::Horizontal, "Language");
//    model_mentees->setHeaderData(9, Qt::Horizontal, "Consideration");
//    model_mentees->setHeaderData(10, Qt::Horizontal, "Role");


