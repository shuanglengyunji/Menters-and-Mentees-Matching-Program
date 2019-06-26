#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::import_mentors()
{
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
    //int fs=1;
    bool intext=false;

    QString linedatainit="insert into mentor(uid, first_name, last_name, gender, email, mobile, add_info, academic_level, college, languages, train_1, train_2, train_3, wwvp_card, is_confirm, role)  values (";
    //QString linedatainit="insert into mentor(uid, first_name, last_name, email, mobile, add_info, academic_level, college, languages, train_1, train_2, train_3, wwvp_card, is_confirmed, role, gender)  values (";
    QString linedata=linedatainit;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray line=file.readLine();
        while(!ts.atEnd())
        {
            if(!intext) linedata=linedatainit;
            else {
                linedata.chop(1);
            }
            QString update="Delete From mentor where uid=";
            QStringList line=ts.readLine().split(',');
            for(int i=0;i<line.length();i++){
               if(!intext){
                   linedata.append("\'");
               }
               QString temps=line.at(i);
               if(i==0 && !intext){
                   update.append("\'"+temps+"\'");
                   q.exec(update);
               }

               temps.replace("&","and");
               temps.replace("\'","\'\'");
               temps.remove("ANU ");

               if(temps.contains("\"")){
                   temps.remove("\"");
                   if(intext) {
                       intext=false;
                   }
                   else {
                       intext=true;
                   }
               }

               linedata.append(temps);
               if(!intext){
                   linedata.append("\'");
               }
               linedata.append(",");
            }

            if(!intext) {
                linedata.append("\'Mentor\')");
                //linedata.append("\'Mentor\',\'Other\')");
                qDebug()<<linedata<<endl;
                q.exec(linedata);
            }
        }
        file.close();

        // show
        model_mentors->select();
        ui->tableView_mentors->reset();
        ui->tableView_mentors->resizeColumnsToContents();
        refresh_match();
        training_Auto_confirm();
    }
    else
    {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while opening file: ") + db.lastError().text());
    }
}

void MainWindow::import_mentees()
{
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
     bool intext=false;

     QString linedatainit="insert into mentee(uid, first_name, last_name, gender, email, mobile, academic_level, college, languages, consideration, role) values(";
     QString linedata=linedatainit;

     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         if(fs) {
             QByteArray line=file.readLine();
             fs=0;
         }
         while(!ts.atEnd())
         {
             if(!intext) linedata=linedatainit;
             else {
                 linedata.chop(1);
             }
             QString update="Delete From mentee where uid=";
             QStringList line=ts.readLine().split(',');
             for(int i=0;i<line.length();i++){
                if(!intext){
                    linedata.append("\'");
                }
                QString temps=line.at(i);
                if(i==0 && !intext){
                    update.append("\'"+temps+"\'");
                    q.exec(update);
                }

                temps.replace("&","and");
                temps.replace("\'","\'\'");
                temps.remove("ANU ");

                if(temps.contains("\"")){
                    temps.remove("\"");
                    if(intext) {
                        intext=false;
                    }
                    else {
                        intext=true;
                    }
                }


                linedata.append(temps);
                if(!intext){
                    linedata.append("\'");
                }
                linedata.append(",");
             }
             if(!intext){
                 linedata.append("\'Mentee\')");
                 qDebug()<<linedata<<endl;
                 q.exec(linedata);
             }
         }
         file.close();

         // show
         model_mentees->select();
         ui->tableView_mentees->reset();
         ui->tableView_mentees->resizeColumnsToContents();
         refresh_match();
     }
     else
     {
         QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while opening file: ") + q.lastError().text());
     }
}

void MainWindow::import_match()
{
    QString file_path;
    file_path = QFileDialog::getOpenFileName(this,tr("Import Match result"),"",tr("Data file (*.csv)"));
    if(file_path.isEmpty())
    {
        qDebug() << "Open failed";
        return;
    }
    qDebug() << "Match file Path:" << file_path;

    QSqlQuery q("", db);

    QFile file(file_path);
    QTextStream ts (&file);
    int fs=1;
    bool insert=false;
    QString groupid="0";
    QString pgroupid="1";
    QString mentorid;
    QString menteeid;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(fs) {
            QByteArray line=file.readLine();
            fs=0;
        }
        while(!ts.atEnd())
        {
            QString linedata="insert into match (mentor_id,mentee_id) values (";
            QStringList line=ts.readLine().split(',');
            pgroupid=groupid;
            groupid=line.at(0);
            if(pgroupid==groupid){
                menteeid=line.at(1);
                insert=true;
            }
            else {
                mentorid=line.at(1);
                insert=false;
            }

            if(insert){
                q.exec(linedata+"\'"+mentorid+"\'"+","+"\'"+menteeid+"\')");
                qDebug()<<linedata+"\'"+mentorid+"\'"+","+"\'"+menteeid+"\')"<<endl;
            }
        }
        file.close();
    }
}

// ---------------------------------------

void MainWindow::export_mentors()
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
           datain="University ID,First Name,Family (Last) name,Gender,Email,Mobile,Additional information,Level of study ,College,Languages,Training 1,Training 2,Training 3,WWVP Card,Confirmed\r\n";
           in<<datain;
           fl=0;
       }
       for(int i=0;i<model_mentors->rowCount();i++){
           datain.clear();
           QSqlRecord data=model_mentors->record(i);
           for (int j=0;j<data.count();j++) {
               if(j==15) continue;
               QString temp=data.value(j).toString();
               if(temp.contains(","))
               {
                   datain.append("\""+temp+"\"");
               }
               else {
                   datain.append(temp);
               }
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

void MainWindow::export_mentees()
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
            datain="Student No,Firstname,Surname,Gender,Email,Mobile,Academic Level,College,Language,Considerations\r\n";
            in<<datain;
            fl=0;
        }
        for(int i=0;i<model_mentees->rowCount();i++){
            datain.clear();
            QSqlRecord data=model_mentees->record(i);
            for (int j=0;j<data.count();j++) {
                if(j==10) continue;
                QString temp=data.value(j).toString();
                if(temp.contains(","))
                {
                    datain.append("\""+temp+"\"");
                }
                else {
                    datain.append(temp);
                }
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

void MainWindow::export_match_result()
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
        datain.clear();
        QTextStream in(&file);
        if(fl){
            datain="groupid,uid,Firstname,Lastname,gender,email,mobile,academic_level,college,language,role\r\n";
            in<<datain;
            fl=0;
        }
        datain.clear();
        for (int i=0;i<model_match_mentors->rowCount();i++) {
            QSqlRecord mentordata=model_match_mentors->record(i);
            QString mentorid=mentordata.value(0).toString();
            QString groupid=QString::number(i+1,10);
            datain=(groupid+",");
            for(int j=0;j<mentordata.count();j++){
                if(j<9 || j==15){
                    QString temp=mentordata.value(j).toString();
                    if(temp.contains(","))
                    {
                        datain.append("\""+temp+"\"");
                    }
                    else
                    {
                        datain.append(temp);
                    }
                    datain.append(",");
                }
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
                for (int h=0;h<menteedata.count();h++) {
                    if (h<9||h==10) {
                        QString temp=menteedata.value(h).toString();
                        if(temp.contains(","))
                        {
                            datain.append("\""+temp+"\"");
                        }
                        else
                        {
                            datain.append(temp);
                        }
                        datain.append(",");
                    }
                }
                datain.chop(1);
                datain.append("\r\n");
                in<<datain;
            }
            matchr->clear();
        }
    }
    file.close();
}

void MainWindow::export_wattle_file()
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
            datain.append(data.value(15).toString());
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
            datain.append(data.value(10).toString());
            datain.append("\r\n");

            qDebug()<<datain<<endl;

            in<<datain;
        }
    }
    file.close();
}

// ---------------------------------------

// Import

void MainWindow::on_actionImport_Mentors_triggered()
{
    qDebug() << "Import Mentors data";
    import_mentors();
}

void MainWindow::on_actionImport_Mentees_triggered()
{
    qDebug() << "Import Mentees data";
    import_mentees();
}

void MainWindow::on_actionImport_Match_Result_triggered()
{
    qDebug() << "Import Mentees data";
    import_match();
}

// -----------------------------------------------------------

// Export

void MainWindow::on_actionExport_Mentors_triggered()
{
    qDebug() << "Export Mentors data";
    export_mentors();
}

void MainWindow::on_actionExport_Mentees_triggered()
{
    qDebug() << "Export Mentees data";
    export_mentees();
}

void MainWindow::on_actionExport_Match_Result_triggered()
{
    qDebug() << "Export Match Result";
    export_match_result();
}

void MainWindow::on_actionExport_Wattle_File_triggered()
{
    qDebug() << "Export Wattle";
    export_wattle_file();
}
