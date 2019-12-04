#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

void MainWindow::import_mentors_and_mentees()
{
    QSqlQuery query(db);

    QString file_path = QFileDialog::getOpenFileName(this,tr("Import Mentors and Mentees Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(file_path.isEmpty())
    {
        return;
    }

    // [1] Reading excel file(*.xlsx)
    Document xlsxR(file_path);
    if ( !xlsxR.load() ) // load excel file
    {
        QMessageBox::warning(this, tr("File authorization failed"), tr("Failed to load xlsx file."));
        return;
    }

    // [2] Check Existence of Sheets ("Mentors" and "Mentees")
    if ( !xlsxR.selectSheet("Mentors") )
    {
        QMessageBox::warning(this, tr("Mentors Sheet Missing"), tr("Please make sure there is a sheet named \"Mentors\" in the data file."));
        return;
    }
    if ( !xlsxR.selectSheet("Mentees") )
    {
        QMessageBox::warning(this, tr("Mentees Sheet Missing"), tr("Please make sure there is a sheet named \"Mentees\" in the data file."));
        return;
    }

    // [3] Import Mentors Sheet
    xlsxR.selectSheet("Mentors");
    int mentors_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;

    for (int row = 2;row <= mentors_max; row = row + 1)
    {
        // read data
        QString uid = xlsxR.cellAt(row, 4)->readValue().toString();
        QString first_name = xlsxR.cellAt(row, 2)->readValue().toString();
        QString last_name = xlsxR.cellAt(row, 3)->readValue().toString();
        QString gender = xlsxR.cellAt(row, 11)->readValue().toString();
        QString academic_level = xlsxR.cellAt(row, 7)->readValue().toString();
        QString college = xlsxR.cellAt(row, 8)->readValue().toString();
        QString degree = xlsxR.cellAt(row, 9)->readValue().toString();
        QString type = xlsxR.cellAt(row, 10)->readValue().toString();
        QString languages = xlsxR.cellAt(row, 12)->readValue().toString();
        QString languages_text = xlsxR.cellAt(row, 13)->readValue().toString();
        QString hall = xlsxR.cellAt(row, 14)->readValue().toString();
        QString special = xlsxR.cellAt(row, 15)->readValue().toString();
        QString interests = xlsxR.cellAt(row, 16)->readValue().toString();
        QString wwvp = xlsxR.cellAt(row, 5)->readValue().toString();
        QString train_1 = xlsxR.cellAt(row, 17)->readValue().toString();
        QString train_2 = xlsxR.cellAt(row, 18)->readValue().toString();
        QString train_3 = xlsxR.cellAt(row, 19)->readValue().toString();
        QString train_complete = "n";
        if (train_1=="y" && train_1=="y" && train_1=="y")
            train_complete = "y";
        QString round = xlsxR.cellAt(row, 6)->readValue().toString();
        if (round == "Yes - I will be here")
            round = "Round 1";
        else
            round = "Round 2";
        QString is_confirmed = xlsxR.cellAt(row, 1)->readValue().toString();
        QString is_grouped = "0";

        // create sql
        QString sql = "INSERT INTO mentor VALUES (\"" +
                uid + "\",\"" + first_name + "\",\"" + last_name + "\",\"" + gender + "\",\"" +
                academic_level + "\",\"" + college + "\",\"" + degree + "\",\"" + type + "\",\"" +
                languages + "\",\"" + languages_text + "\",\"" + hall + "\",\"" + special + "\",\"" +
                interests + "\",\"" + wwvp + "\",\"" + train_1 + "\",\"" + train_2 + "\",\"" +
                train_3 + "\",\"" + train_complete + "\",\"" + round + "\",\"" + is_confirmed + "\",\"" + is_grouped + "\")";    // qDebug() << sql;

        // execute sql
        if ( !query.exec(sql) )
        {
            //qDebug() << "Failed to Import Row " << row;
            QMessageBox::warning(this, tr("Mentor Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            //qDebug() << sql;
        }
    }

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;

    for (int row = 2;row <= mentees_max; row = row + 1)
    {
        // read data
        QString uid = xlsxR.cellAt(row, 3)->readValue().toString();
        QString first_name = xlsxR.cellAt(row, 1)->readValue().toString();
        QString last_name = xlsxR.cellAt(row, 2)->readValue().toString();
        QString gender = xlsxR.cellAt(row, 9)->readValue().toString();
        QString academic_level = xlsxR.cellAt(row, 5)->readValue().toString();
        QString college = xlsxR.cellAt(row, 6)->readValue().toString();
        QString degree = xlsxR.cellAt(row, 7)->readValue().toString();
        QString type = xlsxR.cellAt(row, 8)->readValue().toString();
        QString languages = xlsxR.cellAt(row, 10)->readValue().toString();
        QString languages_text = xlsxR.cellAt(row, 11)->readValue().toString();
        QString requests = xlsxR.cellAt(row, 13)->readValue().toString();
        QString special_categories = xlsxR.cellAt(row, 12)->readValue().toString();
        QString round = xlsxR.cellAt(row, 4)->readValue().toString();

        // create sql
        QString sql = "INSERT INTO mentee VALUES (\"" +
                uid + "\",\"" + first_name + "\",\"" + last_name + "\",\"" + gender + "\",\"" +
                academic_level + "\",\"" + college + "\",\"" + degree + "\",\"" + type + "\",\"" +
                languages + "\",\"" + languages_text + "\",\"" + requests + "\",\"" + special_categories + "\",\"" +
                round + "\")";    // qDebug() << sql;

        // execute sql
        if ( !query.exec(sql) )
        {
            qDebug() << "Failed to Import Row " << row;
            //QMessageBox::warning(this, tr("Mentee Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            //qDebug() << sql;
        }
    }

    load_mentors();
    load_mentees();
}

void MainWindow::import_grouping_results()
{
    QString file_path = QFileDialog::getOpenFileName(this,tr("Import Grouping results"),"",tr("Data file (*.xlsx)"));
    if(file_path.isEmpty())
    {
        return;
    }

}

// ---------------------------------------

void MainWindow::export_mentors_and_mentees()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Mentors and Mentees", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }
}

void MainWindow::export_grouping_results()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Grouping Results"), "Grouping", tr("*.xlsx")); // choose a path
    if(addr.isEmpty())
    {
        return;
    }
}

void MainWindow::export_wattle_file()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.csv")); //choose a path
    if(addr.isEmpty())
    {
        return;
    }
}

// ---------------------------------------

// Import

void MainWindow::on_actionImport_Mentors_and_Mentees_triggered()
{
    import_mentors_and_mentees();
}

void MainWindow::on_actionImport_Grouping_Results_triggered()
{
    //qDebug() << "Import Grouping";
}

// -----------------------------------------------------------

// Export

void MainWindow::on_actionExport_Mentors_and_Mentees_triggered()
{
    //qDebug() << "Export Mentors and Mentees";
}

void MainWindow::on_actionExport_Grouping_Results_triggered()
{
    //qDebug() << "Export Grouping";
}

void MainWindow::on_actionExport_Wattle_File_triggered()
{
    //qDebug() << "Export Wattle";
}

/*

void MainWindow::on_actionImport_Mentors_triggered()
{
    qDebug() << "Import Mentors data";
    import_mentors();

    // show
    model_mentors->select();
    ui->tableView_mentors->reset();
    ui->tableView_mentors->resizeColumnsToContents();
    refresh_match();
    training_Auto_confirm();
}

void MainWindow::on_actionImport_Mentees_triggered()
{
    qDebug() << "Import Mentees data";
    import_mentees();

    // show
    model_mentees->select();
    ui->tableView_mentees->reset();
    ui->tableView_mentees->resizeColumnsToContents();
    refresh_match();
}

void MainWindow::on_actionImport_Match_Result_triggered()
{
    qDebug() << "Import Mentees data";
    import_match();
}

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

*/

/*

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
                     //qDebug()<<linedata<<endl;
                     q.exec(linedata);
                 }
             }
             file.close();

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
        QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentor Data"), "Mentor", tr("*.csv"));
        if(addr.isEmpty())
        {
           return;
        }
        QSqlTableModel * exmodel_mentors = new QSqlTableModel(this,db);
        exmodel_mentors->setTable("mentor");
        exmodel_mentors->select();
        while(exmodel_mentors->canFetchMore()){
            exmodel_mentors->fetchMore();
        }

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
           for(int i=0;i<exmodel_mentors->rowCount();i++){
               datain.clear();
               QSqlRecord data=exmodel_mentors->record(i);
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
        delete exmodel_mentors;
}

void MainWindow::export_mentees()
{
        QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentee Data"), "Mentee", tr("*.csv"));
        if(addr.isEmpty())
        {
            return;
        }
        QSqlTableModel * exmodel_mentees = new QSqlTableModel(this,db);
        exmodel_mentees->setTable("mentee");
        exmodel_mentees->select();
        while(exmodel_mentees->canFetchMore()){
            exmodel_mentees->fetchMore();
        }
        qDebug()<<exmodel_mentees->rowCount()<<endl;
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
            for(int i=0;i<exmodel_mentees->rowCount();i++){
                datain.clear();
                QSqlRecord data=exmodel_mentees->record(i);
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
        delete exmodel_mentees;
}

void MainWindow::export_match_result()
{
        QString addr = QFileDialog::getSaveFileName(this, tr("Save Match Result"), "Match", tr("*.csv")); //选择路径
        if(addr.isEmpty())
        {
            return;
        }
        QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
        QSqlQueryModel * exmodel_match_mentors=new QSqlQueryModel(this);;
        exmodel_match_mentors->setQuery(str,db);
        while(exmodel_match_mentors->canFetchMore()){
            exmodel_match_mentors->fetchMore();
        }
        int fl=1;
        if(QFile().exists(addr)){
            QFile().remove(addr);
        }
        QFile file(addr);
        QString datain;
        QSqlQueryModel * matchr = new QSqlQueryModel(this);
        if(file.open(QIODevice::WriteOnly)){
            datain.clear();
            QTextStream in(&file);
            if(fl){
                datain="Group,Username,Firstname,Lastname,Gender,Email,Mobile,Academic_Level,College,Language,Role\r\n";
                in<<datain;
                fl=0;
            }
            datain.clear();
            for (int i=0;i<exmodel_match_mentors->rowCount();i++) {
                QSqlRecord mentordata=exmodel_match_mentors->record(i);
                QString mentorid=mentordata.value(0).toString();
                QString groupid=QString::number(i+1,10);
                datain=(groupid+",");
                for(int j=0;j<mentordata.count();j++){
                    if((j<10 || j==15) && j!=6){
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
                while(matchr->canFetchMore()){
                    matchr->fetchMore();
                }
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
        delete matchr;
        delete exmodel_match_mentors;
}

void MainWindow::export_wattle_file()
{
    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.csv")); //选择路径
        if(addr.isEmpty())
        {
            return;
        }
        QSqlTableModel * exmodel_mentees = new QSqlTableModel(this,db);
        exmodel_mentees->setTable("mentee");
        exmodel_mentees->select();
        while(exmodel_mentees->canFetchMore())
        {
            exmodel_mentees->fetchMore();
        }
        QSqlTableModel * exmodel_mentors = new QSqlTableModel(this,db);
        exmodel_mentors->setTable("mentor");
        exmodel_mentors->select();
        while(exmodel_mentors->canFetchMore()){
            exmodel_mentors->fetchMore();
        }
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
            for(int i=0;i<exmodel_mentors->rowCount();i++){
                datain.clear();
                QSqlRecord data=exmodel_mentors->record(i);
                datain.append(data.value(0).toString());
                datain.append(",");
                datain.append("Set4_ANU,Student,");
                datain.append(data.value(15).toString());
                datain.append("\r\n");

                qDebug()<<datain<<endl;

                in<<datain;
            }
            for(int i=0;i<exmodel_mentees->rowCount();i++){
                datain.clear();
                QSqlRecord data=exmodel_mentees->record(i);
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
        delete exmodel_mentees;
        delete exmodel_mentors;
}

*/
