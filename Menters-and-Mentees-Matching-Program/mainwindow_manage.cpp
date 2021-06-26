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

void MainWindow::on_pushButton_manage_export_grouping_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save grouping result"), "grouping", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    export_grouping(addr);
}


void MainWindow::on_pushButton_manage_export_mentor_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save mentor result"), "mentor", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    export_mentor(addr);
}


void MainWindow::on_pushButton_manage_export_mentee_clicked()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save mentee result"), "mentee", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    export_mentee(addr);
}

// Clear

void MainWindow::on_pushButton_manage_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM mentor");
    query.exec("DELETE FROM mentee");
}
