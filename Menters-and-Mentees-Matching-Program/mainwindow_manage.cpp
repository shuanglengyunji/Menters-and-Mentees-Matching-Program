#include "mainwindow.h"
#include "ui_mainwindow.h"

// ---------------------------------------
// Path

void MainWindow::on_toolButton_import_path_clicked()
{
    QString addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(addr.isEmpty())
    {
        return;
    }
    ui->lineEdit_import_path->setText(addr);
}

void MainWindow::on_toolButton_export_path_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }
    ui->lineEdit_export_path->setText(addr);
}

void MainWindow::on_toolButton_wattle_file_path_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }
    ui->lineEdit_wattle_file_path->setText(addr);
}

// ---------------------------------------

// Import

void MainWindow::on_pushButton_manage_import_clicked()
{
    QString addr = ui->lineEdit_import_path->text().simplified();
    if (addr.isEmpty())
    {
        addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
        if(addr.isEmpty())
        {
            return;
        }
        ui->lineEdit_import_path->setText(addr);
    }
    import_data(addr,false);
}

void MainWindow::on_pushButton_manage_import_match_clicked()
{
    QString addr = ui->lineEdit_import_path->text().simplified();
    if (addr.isEmpty())
    {
        addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
        if(addr.isEmpty())
        {
            return;
        }
        ui->lineEdit_import_path->setText(addr);
    }
    import_data(addr,true);
}

// Export

void MainWindow::on_pushButton_manage_export_clicked()
{
    QString addr = ui->lineEdit_export_path->text().simplified();
    if (addr.isEmpty())
    {
        addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
        if(addr.isEmpty())
        {
           return;
        }
        ui->lineEdit_export_path->setText(addr);
    }
    export_data(addr,false);
}

void MainWindow::on_pushButton_manage_export_match_clicked()
{
    QString addr = ui->lineEdit_export_path->text().simplified();
    if (addr.isEmpty())
    {
        addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
        if(addr.isEmpty())
        {
           return;
        }
        ui->lineEdit_export_path->setText(addr);
    }
    export_data(addr,true);
}

// Wattle File

void MainWindow::on_pushButton_manage_export_wattle_clicked()
{
    QString addr = ui->lineEdit_wattle_file_path->text().simplified();
    if (addr.isEmpty())
    {
        addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.xlsx"));
        if(addr.isEmpty())
        {
           return;
        }
        ui->lineEdit_wattle_file_path->setText(addr);
    }
    export_wattle_file(addr);
}

// Clear

void MainWindow::on_pushButton_manage_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM 'group'");
    query.exec("DELETE FROM 'mentor'");
    query.exec("DELETE FROM 'mentee'");
}







