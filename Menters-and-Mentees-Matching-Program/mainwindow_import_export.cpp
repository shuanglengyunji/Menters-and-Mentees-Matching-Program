#include "mainwindow.h"
#include "ui_mainwindow.h"

// Import

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
            int uid=1;
            QString linedata="insert into mentor(uid, first_name, last_name, academic_level, college, languages, train_1, train_2, wwvp_card, is_confirm, role, gender) \n values (";
            QString update="Delete From mentor where uid=";
            QStringList line=ts.readLine().split(',');
            for(int i=0;i<line.length();i++){
               linedata.append("\'");
               QString temps=line.at(i);
               temps.remove("\"");
               if(uid){
                   update.append("\'"+temps+"\'");
                   uid=0;
               }
               linedata.append(temps);
               linedata.append("\'");
               linedata.append(",");
            }
            linedata.append("\'Mentor\', \'Other\')");
            q.exec(update);
            q.exec(linedata);
        }
        file.close();

        // show
        model_mentors->select();
        ui->tableView_mentors->reset();
        ui->tableView_mentors->resizeColumnsToContents();
        refresh();
        training_Auto_confirm();
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
            int uid=1;
            QString linedata="insert into mentee(uid, first_name, last_name, languages, academic_level, college, consideration, role, gender) values(";
            QString update="Delete From mentee where uid=";
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
               if(uid){
                   uid=0;
                   update.append("\'"+temps+"\'");
               }
               linedata.append(temps);
               linedata.append("\'");
               linedata.append(",");
            }
            linedata.append("\'Mentee\',\'Other\')");
            //qDebug()<<linedata<<endl;
            q.exec(linedata);
        }
        file.close();

        // show
        model_mentees->select();
        ui->tableView_mentees->reset();
        ui->tableView_mentees->resizeColumnsToContents();
        refresh();
    }
    else
    {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while opening file: ") + db.lastError().text());
    }

}

// Match Result

void MainWindow::on_actionImport_Match_Result_triggered()
{

}

// -----------------------------------------------------------

// Export

void MainWindow::on_actionExport_Mentors_triggered()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentor Data"), "Mentor", tr("*.csv")); //选择路径
    if(addr.isEmpty())
    {
        return;
    }

    model_mentors = new QSqlTableModel(this,db);
    model_mentors->setTable("mentor");
    model_mentors->select();
    int fl=1;

    if(QFile().exists(addr)){
        QFile().remove(addr);
    }
    QFile file(addr);
    QString datain;
    if(file.open(QIODevice::WriteOnly)){
        qDebug()<<addr<<endl;
        QTextStream in(&file);
        if(fl){
            datain.clear();
            datain="University ID,First Name,Family Last name,Level of study ,College,Languages,Training 1,Training 2,WWVP Card,Confirmed\r\n";
            in<<datain;
            fl=0;
        }
        for(int i=0;i<model_mentors->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_mentors->record(i);
            for (int j=0;j<data.count();j++) {
                if(j==3) continue;
                if(j==11) continue;
                datain.append(data.value(j).toString());
                datain.append(",");
            }
            datain.chop(1);
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }
    }
    file.close();
}

void MainWindow::on_actionExport_Mentees_triggered()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentee Data"), "Mentee", tr("*.csv")); //选择路径
    if(addr.isEmpty())
    {
        return;
    }

    model_mentees = new QSqlTableModel(this,db);
    model_mentees->setTable("mentee");
    model_mentees->select();
    int fl=1;

    if(QFile().exists(addr)){
        QFile().remove(addr);
    }
    QFile file(addr);
    QString datain;
    if(file.open(QIODevice::WriteOnly)){
        qDebug()<<addr<<endl;
        QTextStream in(&file);
        if(fl){
            datain.clear();
            datain="Student No,Firstname,Surname,Language,Academic Level,College,Considerations\r\n";
            in<<datain;
            fl=0;
        }
        for(int i=0;i<model_mentees->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_mentees->record(i);
            for (int j=0;j<data.count();j++) {
                if(j==3) continue;
                if(j==8) continue;
                datain.append(data.value(j).toString());
                datain.append(",");
            }
            datain.chop(1);
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }
    }
    file.close();
}

void MainWindow::on_actionExport_Match_Result_triggered()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Match Result"), "Match", tr("*.csv")); //选择路径
    if(addr.isEmpty())
    {
        return;
    }

    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_match_mentors->setQuery(str,db);

    int fl=1;

    if(QFile().exists(addr)){
        QFile().remove(addr);
    }
    QFile file(addr);
    QString datain;

    QSqlQueryModel * matchr=new QSqlQueryModel(this);

    if(file.open(QIODevice::WriteOnly)){
        /*qDebug()<<addr<<endl;
        QTextStream in(&file);
        if(fl){
            datain.clear();
            datain="m_id,mentor_id,mentee_id\r\n";
            in<<datain;
            fl=0;
        }
        for(int i=0;i<model_match->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_match->record(i);
            for (int j=0;j<data.count();j++) {
                datain.append(data.value(j).toString());
                datain.append(",");
            }
            datain.chop(1);
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }*/
        datain.clear();
        QTextStream in(&file);
        if(fl){
            datain="groupid,uid,Firstname,Lastname,gender,academic_level,college,language,role\r\n";
            in<<datain;
            fl=0;
        }
        datain.clear();
        for (int i=0;i<model_match_mentors->rowCount();i++) {
            QSqlRecord mentordata=model_match_mentors->record(i);
            QString mentorid=mentordata.value(0).toString();
            QString groupid=QString::number(i+1,10);
            datain.append(groupid+",");
            for(int j=0;j<mentordata.count();j++){
                if(j<7 || j==11)
                    datain.append(mentordata.value(j).toString()+",");
            }
            datain.chop(1);
            datain.append("\r\n");
            in<<datain;
            datain.clear();
            str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + mentorid + "'";
            matchr->setQuery(str,db);
            for (int j=0;j<matchr->rowCount();j++) {
                datain=groupid+",";
                QSqlRecord menteedata=matchr->record(j);

                datain.append(menteedata.value(0).toString()+",");
                datain.append(menteedata.value(1).toString()+",");
                datain.append(menteedata.value(2).toString()+",");
                datain.append(menteedata.value(3).toString()+",");
                datain.append(menteedata.value(6).toString()+",");
                datain.append(menteedata.value(4).toString()+",");
                datain.append(menteedata.value(5).toString()+",");
                datain.append(menteedata.value(8).toString());

                datain.append("\r\n");
                in<<datain;
            }
            matchr->clear();
        }

    }
    file.close();
}

void MainWindow::on_actionExport_Wattle_File_triggered()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.csv")); //选择路径
    if(addr.isEmpty())
    {
        return;
    }

    model_mentees = new QSqlTableModel(this,db);
    model_mentees->setTable("mentee");
    model_mentees->select();
    model_mentors = new QSqlTableModel(this,db);
    model_mentors->setTable("mentor");
    model_mentors->select();
    int fl=1;

    if(QFile().exists(addr)){
        QFile().remove(addr);
    }
    QFile file(addr);
    QString datain;

    if(file.open(QIODevice::WriteOnly)){
        qDebug()<<addr<<endl;
        QTextStream in(&file);
        if(fl){
            datain.clear();
            datain="userid,course1,role1,group1\r\n";
            in<<datain;
            fl=0;
        }
        for(int i=0;i<model_mentors->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_mentors->record(i);
            datain.append(data.value(0).toString());
            datain.append(",");
            datain.append("Set4_ANU,Student,");
            datain.append(data.value(11).toString());
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }

        for(int i=0;i<model_mentees->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_mentees->record(i);

            datain.append(data.value(0).toString());
            datain.append(",");
            datain.append("Set4_ANU,Student,");
            datain.append(data.value(8).toString());
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }
    }
    file.close();
}
