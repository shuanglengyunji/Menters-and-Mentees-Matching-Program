#include "mainwindow.h"
#include "ui_mainwindow.h"

// ---------------------------------------

// Import

void MainWindow::on_pushButton_manage_import_clicked()
{
    QString addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(addr.isEmpty())
    {
        return;
    }

    import_data(addr,false);
}

void MainWindow::on_pushButton_manage_import_match_clicked()
{
    QString addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(addr.isEmpty())
    {
        return;
    }

    import_data(addr,true);
}

// Export

void MainWindow::on_pushButton_manage_export_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    export_data(addr,false);
}

void MainWindow::on_pushButton_manage_export_match_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    export_data(addr,true);
}

// Wattle File

void MainWindow::on_pushButton_manage_export_wattle_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle_Label", tr("*.csv"));
    if(addr.isEmpty())
    {
       return;
    }

    export_wattle_file(addr,0);     // label
}

void MainWindow::on_pushButton_manage_export_wattle_2_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle_Group_ID", tr("*.csv"));
    if(addr.isEmpty())
    {
       return;
    }

    export_wattle_file(addr,1);     // group id
}

// Clear

void MainWindow::on_pushButton_manage_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM 'group'");
    query.exec("DELETE FROM 'mentor'");
    query.exec("DELETE FROM 'mentee'");
}







