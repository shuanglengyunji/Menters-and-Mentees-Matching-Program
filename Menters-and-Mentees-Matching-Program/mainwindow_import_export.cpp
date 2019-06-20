#include "mainwindow.h"
#include "ui_mainwindow.h"

// Mentors

void MainWindow::on_actionImport_Mentors_triggered()
{
    qDebug() << "Import Mentors data";

    QString file_path;
    file_path = QFileDialog::getOpenFileName(this,tr("Import Mentors Data"),"",tr("Data file (*.csv)"));
    if(file_path.isEmpty())
    {
        qDebug() << "Open failed";
        return;
    }
    qDebug() << "Mentors file Path:" << file_path;


    QSqlQuery q("", db);

    QFile file(file_path);
    QTextStream ts (&file);
    int fs=1;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(fs) {
            QByteArray line=file.readLine();
            fs=0;
        }
        while(!ts.atEnd())
        {
            QString linedata="insert into mentor(uid, first_name, last_name, academic_level, college, languages, train_1, train_2, wwvp_card, is_confirm, role, gender) \n values (";
            QStringList line=ts.readLine().split(',');
            for(int i=0;i<line.length();i++){
               linedata.append("\'");
               QString temps=line.at(i);
               temps.remove("\"");
               linedata.append(temps);
               linedata.append("\'");
               linedata.append(",");
            }
            linedata.append("\'Mentor\', \'Other\')");
            q.exec(linedata);
        }
        file.close();
        model_mentors->select();
        ui->tableView_mentors->reset();
    }
    else
    {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while opening file: ") + db.lastError().text());
    }

}

// Mentees

void MainWindow::on_actionImport_Mentees_triggered()
{
    qDebug() << "Import Mentees data";

    QString file_path;
    file_path = QFileDialog::getOpenFileName(this,tr("Import Mentees Data"),"",tr("Data file (*.csv)"));
    if(file_path.isEmpty())
    {
        qDebug() << "Open failed";
        return;
    }
    qDebug() << "Mentees file Path:" << file_path;

    QSqlQuery q("", db);

    QFile file(file_path);
    QTextStream ts (&file);
    int fs=1;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(fs) {
            QByteArray line=file.readLine();
            fs=0;
        }
        while(!ts.atEnd())
        {
            QString linedata="insert into mentee(uid, first_name, last_name, languages, academic_level, college, consideration, role, gender) values(";
            QStringList line=ts.readLine().split(',');
            for(int i=0;i<line.length();i++){
               linedata.append("'");
               QString temps=line.at(i);
               temps.remove("\"");
               temps.remove("ANU ");
               if(temps.contains("Arts")) temps="CASS";
               else if(temps.contains("Law")) temps="COL";
               else if(temps.contains("Asia")) temps="CAP";
               else if(temps.contains("Business")) temps="CBE";
               else if(temps.contains("Engineering")) temps="CECS";
               else if(temps.contains("Health")) temps="CHM";
               else if(temps.contains("Science")) temps="COS";
               linedata.append(temps);
               linedata.append("\'");
               linedata.append(",");
            }
            linedata.append("\'Mentee\',\'Other\')");
            //qDebug()<<linedata<<endl;
            q.exec(linedata);
        }
        file.close();
        model_mentees->select();
        ui->tableView_mentees->reset();
    }
    else
    {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while opening file: ") + db.lastError().text());
    }

}
