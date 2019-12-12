#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentors

// load
void MainWindow::load_mentors()
{
    // disconnect
    disconnect(ui->checkBox_mentors_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_hall,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_special,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_interests,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_wwvp,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_training,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    disconnect(ui->checkBox_mentors_confirmation,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);

    // clear exist data
    if ( model_mentors != nullptr )
    {
        delete model_mentors;
        model_mentors = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_mentors = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(QSqlTableModel::OnFieldChange);
    model_mentors->select();

    // link mentors QSqlTableModel to QTableView
    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->reset();
    ui->tableView_mentors->horizontalHeader()->setMaximumSectionSize(300);

    // hide group id
    //ui->tableView_mentors->hideColumn(0);

    // delegate
    ui->tableView_mentors->setItemDelegateForColumn(1,delegate_yes_no);
    ui->tableView_mentors->setItemDelegateForColumn(17,delegate_yes_no);
    ui->tableView_mentors->setItemDelegateForColumn(18,delegate_yes_no);
    ui->tableView_mentors->setItemDelegateForColumn(19,delegate_yes_no);
    ui->tableView_mentors->setItemDelegateForColumn(20,delegate_yes_no);
    ui->tableView_mentors->setItemDelegateForColumn(6,delegate_round);
    ui->tableView_mentors->setItemDelegateForColumn(7,delegate_academic_level);
    ui->tableView_mentors->setItemDelegateForColumn(10,delegate_type);
    ui->tableView_mentors->setItemDelegateForColumn(11,delegate_gender);
    ui->tableView_mentors->setItemDelegateForColumn(12,delegate_language);

    ui->tableView_mentors->resizeColumnsToContents();
    ui->tableView_mentors->resizeRowsToContents();

    // resize row height according to column width
    connect(ui->tableView_mentors->horizontalHeader(),&QHeaderView::sectionResized,
            ui->tableView_mentors,&QTableView::resizeRowsToContents);

    // connect
    connect(ui->checkBox_mentors_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_hall,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_special,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_interests,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_wwvp,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_training,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_confirmation,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
}

// search
void MainWindow::on_lineEdit_mentors_search_editingFinished()
{
    QString str = ui->lineEdit_mentors_search->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_mentors->setFilter("");
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
    model_mentors->setFilter(argument);
}

// delete
void MainWindow::on_pushButton_mentors_delete_clicked()
{
    while (model_mentors->canFetchMore())
    {
        model_mentors->fetchMore();
    }

    QSqlTableModel *tm = model_mentors;
    if (!tm)
        return;

    QModelIndexList currentSelection = ui->tableView_mentors->selectionModel()->selectedIndexes();
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

    load_mentors();
}

void MainWindow::display_mentors_column()
{
    // confirmation
    if (ui->checkBox_mentors_confirmation->isChecked())
    {
        ui->tableView_mentors->showColumn(1);
    }
    else
    {
        ui->tableView_mentors->hideColumn(1);
    }

    // wwvp
    if (ui->checkBox_mentors_wwvp->isChecked())
    {
        ui->tableView_mentors->showColumn(5);
    }
    else
    {
        ui->tableView_mentors->hideColumn(5);
    }

    // round
    if (ui->checkBox_mentors_round->isChecked())
    {
        ui->tableView_mentors->showColumn(6);
    }
    else
    {
        ui->tableView_mentors->hideColumn(6);
    }

    // academic info
    if (ui->checkBox_mentors_academic_info->isChecked())
    {
        ui->tableView_mentors->showColumn(7);
        ui->tableView_mentors->showColumn(8);
        ui->tableView_mentors->showColumn(9);
    }
    else
    {
        ui->tableView_mentors->hideColumn(7);
        ui->tableView_mentors->hideColumn(8);
        ui->tableView_mentors->hideColumn(9);
    }

    // type
    if (ui->checkBox_mentors_type->isChecked())
    {
        ui->tableView_mentors->showColumn(10);
    }
    else
    {
        ui->tableView_mentors->hideColumn(10);
    }

    // gender
    if (ui->checkBox_mentors_gender->isChecked())
    {
        ui->tableView_mentors->showColumn(11);
    }
    else
    {
        ui->tableView_mentors->hideColumn(11);
    }

    // language
    if (ui->checkBox_mentors_language->isChecked())
    {
        ui->tableView_mentors->showColumn(12);
        ui->tableView_mentors->showColumn(13);
    }
    else
    {
        ui->tableView_mentors->hideColumn(12);
        ui->tableView_mentors->hideColumn(13);
    }

    // hall
    if (ui->checkBox_mentors_hall->isChecked())
    {
        ui->tableView_mentors->showColumn(14);
    }
    else
    {
        ui->tableView_mentors->hideColumn(14);
    }

    // special
    if (ui->checkBox_mentors_special->isChecked())
    {
        ui->tableView_mentors->showColumn(15);
    }
    else
    {
        ui->tableView_mentors->hideColumn(15);
    }

    // interests
    if (ui->checkBox_mentors_interests->isChecked())
    {
        ui->tableView_mentors->showColumn(16);
    }
    else
    {
        ui->tableView_mentors->hideColumn(16);
    }

    // training
    if (ui->checkBox_mentors_training->isChecked())
    {
        ui->tableView_mentors->showColumn(17);
        ui->tableView_mentors->showColumn(18);
        ui->tableView_mentors->showColumn(19);
        ui->tableView_mentors->showColumn(20);
    }
    else
    {
        ui->tableView_mentors->hideColumn(17);
        ui->tableView_mentors->hideColumn(18);
        ui->tableView_mentors->hideColumn(19);
        ui->tableView_mentors->hideColumn(20);
    }

}

/*
// add
void MainWindow::on_pushButton_mentors_add_clicked()
{
    QSqlTableModel *tm = model_mentors;
    if (tm == nullptr)
        return;

    QModelIndex insertIndex = ui->tableView_mentors->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    tm->insertRow(row);
    insertIndex = tm->index(row, 0);

    ui->tableView_mentors->setCurrentIndex(insertIndex);
    ui->tableView_mentors->edit(insertIndex);
}
*/

//    model_mentors->setHeaderData(0, Qt::Horizontal, "Uni ID");
//    model_mentors->setHeaderData(1, Qt::Horizontal, "First Name");
//    model_mentors->setHeaderData(2, Qt::Horizontal, "Last Name");
//    model_mentors->setHeaderData(3, Qt::Horizontal, "Gender");
//    model_mentors->setHeaderData(4, Qt::Horizontal, "Email");
//    model_mentors->setHeaderData(5, Qt::Horizontal, "Mobile");
//    model_mentors->setHeaderData(6, Qt::Horizontal, "Addtional information");
//    model_mentors->setHeaderData(7, Qt::Horizontal, "Academic Level");
//    model_mentors->setHeaderData(8, Qt::Horizontal, "College");
//    model_mentors->setHeaderData(9, Qt::Horizontal, "Language");
//    model_mentors->setHeaderData(10, Qt::Horizontal, "Training 1");
//    model_mentors->setHeaderData(11, Qt::Horizontal, "Training 2");
//    model_mentors->setHeaderData(12, Qt::Horizontal, "Training 3");
//    model_mentors->setHeaderData(13, Qt::Horizontal, "WWVP Card Num");
//    model_mentors->setHeaderData(14, Qt::Horizontal, "Confirm");
//    model_mentors->setHeaderData(15, Qt::Horizontal, "Role");

/*

void MainWindow::training_Auto_confirm()
{
    qDebug() << "auto confirmed";

    while(model_mentors->canFetchMore())
    {
        model_mentors->fetchMore();
    }
    int row = model_mentors->rowCount();

    for(int i=0; i<row; i++)
    {
        QString train_1 = model_mentors->index(i,10).data().toString();
        QString train_2 = model_mentors->index(i,11).data().toString();
        QString train_3 = model_mentors->index(i,12).data().toString();
        QString WWVP = model_mentors->index(i,13).data().toString();

        if(train_1 != "y")
        {
            model_mentors->setData(model_mentors->index(i,10),"n");
        }

        if(train_2 != "y")
        {
            model_mentors->setData(model_mentors->index(i,11),"n");
        }

        if(train_3 != "y")
        {
            model_mentors->setData(model_mentors->index(i,12),"n");
        }

        if(train_1 == "y" && train_2 == "y" && train_3 == "y" && !WWVP.isEmpty())
        {
            model_mentors->setData(model_mentors->index(i,14),"y");
        }
        else
        {
            model_mentors->setData(model_mentors->index(i,14),"n");
        }
    }
    model_mentors->submitAll();
}

*/

/*

void MainWindow::edit_finished()
{
    qDebug() << "edit finished, auto confirm";
    int row = ui->tableView_mentors->currentIndex().row();

    QString train_1 = model_mentors->index(row,10).data().toString();
    QString train_2 = model_mentors->index(row,11).data().toString();
    QString train_3 = model_mentors->index(row,12).data().toString();
    QString WWVP = model_mentors->index(row,13).data().toString();
    QString confirm = model_mentors->index(row,14).data().toString();

    if(train_1 == "y" && train_2 == "y" && train_3 == "y" && !WWVP.isEmpty())
    {
        model_mentors->setData(model_mentors->index(row,14),"y");
    }
    else
    {
        if (confirm == "y")
        {
            QSqlQuery query(db);
            QString str = "";

            str = "DELETE FROM match";
            query.exec(str);

            refresh_match();
        }
        model_mentors->setData(model_mentors->index(row,14),"n");
    }
    model_mentors->submit();
}

*/



