#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

void MainWindow::import_data()
{
    QSqlQuery query(db);

    QString addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(addr.isEmpty())
    {
        return;
    }

    // [0] Drop exist tables
    query.exec("DELETE * FROM 'mentor'");
    query.exec("DELETE * FROM 'mentee'");

    // [1] Reading excel file(*.xlsx)
    Document xlsxR(addr);
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

        QString group_id = "0";

        QString is_confirmed = xlsxR.cellAt(row, 1)->readValue().toString();
        if (is_confirmed == "I have read and understood the above text")
        {
            is_confirmed = "y";
        }
        else
        {
            is_confirmed = "n";
        }

        QString first_name = xlsxR.cellAt(row, 2)->readValue().toString();
        QString last_name = xlsxR.cellAt(row, 3)->readValue().toString();
        QString uid = xlsxR.cellAt(row, 4)->readValue().toString();

        QString wwvp = xlsxR.cellAt(row, 5)->readValue().toString();
        if ( wwvp.toUpper() == "NO" || wwvp.isEmpty() ) {
            wwvp = "n";
        }
        else if ( wwvp.toUpper() == "YES" || wwvp.toUpper() == "Y") {
            wwvp = "y";
        }

        QString round = xlsxR.cellAt(row, 6)->readValue().toString();
        if (round == "Yes - I will be here")
        {
            round = "0";
        }
        else if (round == "Likely - I plan to be here, but can't be certain at this stage" || round == "No - I won't be here during that time")
        {
            round = "1";
        }
        else
        {
            qDebug() << "[Import] Unexpected round!";
        }

        QString academic_level = xlsxR.cellAt(row, 7)->readValue().toString();
        if (academic_level == "Postgraduate (coursework)")
        {
            academic_level = "1";
        }
        else if (academic_level == "Undergraduate")
        {
            academic_level = "0";
        }
        else
        {
            qDebug() << "[Import] Unexpected academic level!";
            academic_level = "0";
        }

        QString college = xlsxR.cellAt(row, 8)->readValue().toString();
        college = college.simplified();
        college.replace("College of Asia and the Pacific","0");
        college.replace("College of Asia & the Pacific","0");
        college.replace("College of Arts and Social Sciences","1");
        college.replace("ANU College of Arts & Social Sciences","1");
        college.replace("College of Business and Economics","2");
        college.replace("College of Business & Economics","2");
        college.replace("College of Engineering and Computer Science","3");
        college.replace("ANU College of Law","4");
        college.replace("College of Science","5");
        college.replace("College of Health and Medicine","6");

        QString degree = xlsxR.cellAt(row, 9)->readValue().toString();

        QString type = xlsxR.cellAt(row, 10)->readValue().toString();
        if (type == "International")
            type = "1";
        else if (type == "Domestic")
            type = "0";
        else
            qDebug() << "[Import] Unexpected type!";

        QString gender = xlsxR.cellAt(row, 11)->readValue().toString();
        if (gender == "Female")
        {
            gender = "0";
        }
        else if (gender == "Male")
        {
            gender = "1";
        }
        else if (gender == "Other")
        {
            gender = "2";
        }
        else if (gender == "Prefer not to say")
        {
            gender = "3";
        }
        else
        {
            qDebug() << "[Import] Unexpected gender!";
        }

        QString languages = xlsxR.cellAt(row, 12)->readValue().toString();
        languages = languages.simplified();
        languages.replace("Hindi","0");
        languages.replace("Vietnamese","1");
        languages.replace("German","2");
        languages.replace("Korean","3");
        languages.replace("Tamil","4");
        languages.replace("Mandarin (Chinese)","5");
        languages.replace("Spanish","6");
        languages.replace("Cantonese","7");
        languages.replace("Indonesian","8");
        languages.replace("Japanese","9");
        languages.replace("Urdu","10");
        languages.replace(",Other (please specify)","");
        languages.replace("Other (please specify)","");
        languages.replace(" ","");
        if (languages.isEmpty())
        {
            languages = "11";
        }

        QString languages_text = xlsxR.cellAt(row, 13)->readValue().toString();
        QString hall = xlsxR.cellAt(row, 14)->readValue().toString();

        QString special = xlsxR.cellAt(row, 15)->readValue().toString();
        special = special.simplified();
        special.replace("An international student wanting to learn about Australian culture","0");
        special.replace("An exchange student (rather than a student new to university)","1");
        special.replace("Being a mature age student (greater than three years between school/uni or degrees)","2");
        special.replace("A mentee who lives with disability or mental illness","3");
        special.replace("A mentee who works more than 20hrs/wk while studying","4");
        special.replace("A mentee who identifies as LGBTIQ+","5");
        special.replace("A mentee who is the parent, guardian, or carer of children or another person","6");
        special.replace("A mentee from a rural area","7");
        special.replace("A mentee who is under the age of 18","8");
        special.replace("A mentee who is particularly uncomfortable speaking in English (but wants to improve)","9");
        special.replace("A mentee who is particularly shy or lacks confidence and wants someone patient (yes this is a request we get)","10");

        QString interests = xlsxR.cellAt(row, 16)->readValue().toString();

        QString train_1 = xlsxR.cellAt(row, 17)->readValue().toString();
        if (train_1 != "y")
            train_1 = "n";

        QString train_2 = xlsxR.cellAt(row, 18)->readValue().toString();
        if (train_2 != "y")
            train_2 = "n";

        QString train_3 = xlsxR.cellAt(row, 19)->readValue().toString();
        if (train_3 != "y")
            train_3 = "n";

        QString train_complete = "n";
        if (train_1=="y" && train_1=="y" && train_1=="y")
            train_complete = "y";

        // create sql

        QString sql = "INSERT INTO mentor VALUES (" +
                QString("%1,").arg(group_id) +
                QString("\"%1\",").arg(is_confirmed) +
                QString("\"%1\",").arg(first_name) +
                QString("\"%1\",").arg(last_name) +
                QString("\"%1\",").arg(uid) +
                QString("\"%1\",").arg(wwvp) +
                QString("%1,").arg(round) +
                QString("%1,").arg(academic_level) +
                QString("\"%1\",").arg(college) +
                QString("\"%1\",").arg(degree) +
                QString("%1,").arg(type) +
                QString("%1,").arg(gender) +
                QString("\"%1\",").arg(languages) +
                QString("\"%1\",").arg(languages_text) +
                QString("\"%1\",").arg(hall) +
                QString("\"%1\",").arg(special) +
                QString("\"%1\",").arg(interests) +
                QString("\"%1\",").arg(train_1) +
                QString("\"%1\",").arg(train_2) +
                QString("\"%1\",").arg(train_3) +
                QString("\"%1\"").arg(train_complete) +
                ")";

        //qDebug() << sql;

        // execute sql
        if ( !query.exec(sql) )
        {
            //qDebug() << "Failed to Import Row " << row;
            QMessageBox::warning(this, tr("Mentor Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            //qDebug() << sql;
            //qDebug() << query.lastError();
        }

    }

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;

    for (int row = 2;row <= mentees_max; row = row + 1)
    {
        // read data

        QString group_id = "0";

        QString first_name = xlsxR.cellAt(row, 1)->readValue().toString();
        QString last_name = xlsxR.cellAt(row, 2)->readValue().toString();
        QString uid = xlsxR.cellAt(row, 3)->readValue().toString();

        QString round = xlsxR.cellAt(row, 4)->readValue().toString();
        if (round == "Round 1")
        {
            round = "0";
        }
        else if (round == "Round 2")
        {
            round = "1";
        }
        else
        {
            round = "0";
            qDebug() << "[Import] Unexpected round!";
        }

        QString academic_level = xlsxR.cellAt(row, 5)->readValue().toString();
        if (academic_level == "Postgraduate Coursework")
        {
            academic_level = "1";
        }
        else if (academic_level == "Undergraduate")
        {
            academic_level = "0";
        }
        else
        {
            academic_level = "0";
            qDebug() << "[Import] Unexpected academic level!";
        }

        QString college = xlsxR.cellAt(row, 6)->readValue().toString();
        college = college.simplified();
        college.replace(",School of Art and Design (as part of the College of Arts and Social Sciences)","");
        college.replace("College of Asia and the Pacific","0");
        college.replace("College of Asia & the Pacific","0");
        college.replace("College of Arts and Social Sciences","1");
        college.replace("College of Arts & Social Sciences","1");
        college.replace("College of Business and Economics","2");
        college.replace("College of Business & Economics","2");
        college.replace("College of Engineering and Computer Science","3");
        college.replace("College of Engineering & Computer Science","3");
        college.replace("College of Law","4");
        college.replace("College of Science","5");
        college.replace("College of Health and Medicine","6");
        college.replace("College of Health & Medicine","6");
        college.replace("ANU ","");

        QString degree = xlsxR.cellAt(row, 7)->readValue().toString();

        QString type = xlsxR.cellAt(row, 8)->readValue().toString();
        if (type == "International")
        {
            type = "1";
        }
        else if (type == "Domestic")
        {
            type = "0";
        }
        else
        {
            type = "0";
            qDebug() << "[Import] Unexpected type!";
        }

        QString gender = xlsxR.cellAt(row, 9)->readValue().toString();
        if (gender == "Female")
        {
            gender = "0";
        }
        else if (gender == "Male")
        {
            gender = "1";
        }
        else if (gender == "Other")
        {
            gender = "2";
        }
        else if (gender == "Prefer not to say")
        {
            gender = "3";
        }
        else
        {
            gender = "3";
            qDebug() << "[Import] Unexpected gender!";
        }

        QString languages = xlsxR.cellAt(row, 10)->readValue().toString();
        languages = languages.simplified();
        languages.replace("Hindi","0");
        languages.replace("Vietnamese","1");
        languages.replace("German","2");
        languages.replace("Korean","3");
        languages.replace("Tamil","4");
        languages.replace("Mandarin (Chinese)","5");
        languages.replace("Spanish","6");
        languages.replace("Cantonese","7");
        languages.replace("Indonesian","8");
        languages.replace("Japanese","9");
        languages.replace("Urdu","10");
        languages.replace(",Other (please specify)","");
        languages.replace("Other (please specify)","");
        languages.replace(" ","");
        if (languages.isEmpty())
        {
            languages = "11";
        }

        QString languages_text = xlsxR.cellAt(row, 11)->readValue().toString();

        QString special_categories = xlsxR.cellAt(row, 12)->readValue().toString();
        special_categories = special_categories.simplified();
        special_categories.replace("Australian culture","0");
        special_categories.replace("Going on exchange","1");
        special_categories.replace("Being a mature age student (greater than three years between school/uni or degrees)","2");
        special_categories.replace("Living with disability or mental illness","3");
        special_categories.replace("Working full or part time while studying","4");
        special_categories.replace("Identifying as LGBTIQ+","5");
        special_categories.replace("Being the parent, guardian, or carer of children or another person","6");
        special_categories.replace("Living in a rural or regional area","7");

        QString requests = xlsxR.cellAt(row, 13)->readValue().toString();

        // create sql
        QString sql = "INSERT INTO mentee VALUES (" +
                QString("%1,").arg(group_id) +
                QString("\"%1\",").arg(first_name) +
                QString("\"%1\",").arg(last_name) +
                QString("\"%1\",").arg(uid) +
                QString("%1,").arg(round) +
                QString("\"%1\",").arg(academic_level) +
                QString("\"%1\",").arg(college) +
                QString("\"%1\",").arg(degree) +
                QString("%1,").arg(type) +
                QString("%1,").arg(gender) +
                QString("\"%1\",").arg(languages) +
                QString("\"%1\",").arg(languages_text) +
                QString("\"%1\",").arg(special_categories) +
                QString("\"%1\"").arg(requests) +
                ")";

        // execute sql
        if ( !query.exec(sql) )
        {
            //qDebug() << "Failed to Import Row " << row;
            //qDebug() << sql;
            //qDebug() << query.lastError();
            QMessageBox::warning(this, tr("Mentee Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            //qDebug() << sql;
        }
    }
}

// ---------------------------------------

void MainWindow::export_data()
{

    QString addr = QFileDialog::getSaveFileName(this, tr("Save Mentors and Mentees Data"), "Data", tr("*.xlsx"));
    if(addr.isEmpty())
    {
       return;
    }

    // [1]  Create excel file object

    QXlsx::Document xlsxW;

    // [2]  Create Sheets and Sheet Headers
    // Mentors
    xlsxW.addSheet("Mentors");

    xlsxW.write("A1","Confirmation");
    xlsxW.write("B1","First Name");
    xlsxW.write("C1","Last Name");
    xlsxW.write("D1","Uni ID");
    xlsxW.write("E1","WWVP");
    xlsxW.write("F1","O-Week");
    xlsxW.write("G1","UG/PG");
    xlsxW.write("H1","Academic College");
    xlsxW.write("I1","Degree");
    xlsxW.write("J1","Dom/Int");
    xlsxW.write("K1","Gender");
    xlsxW.write("L1","Languages");
    xlsxW.write("M1","Language - Text");
    xlsxW.write("N1","Residential Hall");
    xlsxW.write("O1","Special Categories");
    xlsxW.write("P1","Interests");
    xlsxW.write("Q1","Training 1");
    xlsxW.write("R1","Training 2");
    xlsxW.write("S1","Training 3");
    xlsxW.write("T1","Training Complete");

    // Mentees
    xlsxW.addSheet("Mentees");

    xlsxW.write("A1","First Name");
    xlsxW.write("B1","Last Name");
    xlsxW.write("C1","Uni ID");
    xlsxW.write("D1","Round");
    xlsxW.write("E1","UG/PG");
    xlsxW.write("F1","Academic College");
    xlsxW.write("G1","u18");
    xlsxW.write("H1","Dom/Int");
    xlsxW.write("I1","Gender");
    xlsxW.write("J1","Languages");
    xlsxW.write("K1","Language - Text");
    xlsxW.write("L1","Special Categories");
    xlsxW.write("M1","Requests");

    // Group
    xlsxW.addSheet("Group");

    // [3] Fill data into xlsx file

    // mentors

    QSqlTableModel * exmodel_mentors = new QSqlTableModel(this,db);
    exmodel_mentors->setTable("mentor");
    exmodel_mentors->select();
    while(exmodel_mentors->canFetchMore()){
        exmodel_mentors->fetchMore();
    }

    xlsxW.selectSheet("Mentors");
    for (int row = 0; row < exmodel_mentors->rowCount(); row++)
    {
        QSqlRecord r = exmodel_mentors->record(row);

        xlsxW.write("A"+QVariant(row+2).toString(),r.value(19));
        xlsxW.write("B"+QVariant(row+2).toString(),r.value(1));
        xlsxW.write("C"+QVariant(row+2).toString(),r.value(2));
        xlsxW.write("D"+QVariant(row+2).toString(),r.value(0));
        xlsxW.write("E"+QVariant(row+2).toString(),r.value(13));
        xlsxW.write("F"+QVariant(row+2).toString(),r.value(18));
        xlsxW.write("G"+QVariant(row+2).toString(),r.value(4));
        xlsxW.write("H"+QVariant(row+2).toString(),r.value(5));
        xlsxW.write("I"+QVariant(row+2).toString(),r.value(6));
        xlsxW.write("J"+QVariant(row+2).toString(),r.value(7));
        xlsxW.write("K"+QVariant(row+2).toString(),r.value(3));
        xlsxW.write("L"+QVariant(row+2).toString(),r.value(8));
        xlsxW.write("M"+QVariant(row+2).toString(),r.value(9));
        xlsxW.write("N"+QVariant(row+2).toString(),r.value(10));
        xlsxW.write("O"+QVariant(row+2).toString(),r.value(11));
        xlsxW.write("P"+QVariant(row+2).toString(),r.value(12));
        xlsxW.write("Q"+QVariant(row+2).toString(),r.value(14));
        xlsxW.write("R"+QVariant(row+2).toString(),r.value(15));
        xlsxW.write("S"+QVariant(row+2).toString(),r.value(16));
        xlsxW.write("T"+QVariant(row+2).toString(),r.value(17));
    }

    delete exmodel_mentors;

    // mentees

    QSqlTableModel * exmodel_mentees = new QSqlTableModel(this,db);
    exmodel_mentees->setTable("mentee");
    exmodel_mentees->select();
    while(exmodel_mentees->canFetchMore()){
        exmodel_mentees->fetchMore();
    }

    xlsxW.selectSheet("Mentees");
    for (int row = 0; row < exmodel_mentees->rowCount(); row++)
    {
        QSqlRecord r = exmodel_mentees->record(row);

        xlsxW.write("A"+QVariant(row+2).toString(),r.value(1));
        xlsxW.write("B"+QVariant(row+2).toString(),r.value(2));
        xlsxW.write("C"+QVariant(row+2).toString(),r.value(0));
        xlsxW.write("D"+QVariant(row+2).toString(),r.value(12));
        xlsxW.write("E"+QVariant(row+2).toString(),r.value(4));
        xlsxW.write("F"+QVariant(row+2).toString(),r.value(5));
        xlsxW.write("G"+QVariant(row+2).toString(),r.value(6));
        xlsxW.write("H"+QVariant(row+2).toString(),r.value(7));
        xlsxW.write("I"+QVariant(row+2).toString(),r.value(3));
        xlsxW.write("J"+QVariant(row+2).toString(),r.value(8));
        xlsxW.write("K"+QVariant(row+2).toString(),r.value(9));
        xlsxW.write("L"+QVariant(row+2).toString(),r.value(11));
        xlsxW.write("M"+QVariant(row+2).toString(),r.value(10));
    }

    delete exmodel_mentees;

    // [4]
    if ( !xlsxW.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

// ---------------------------------------

void MainWindow::export_wattle_file()
{
//    QString addr = QFileDialog::getSaveFileName(this, tr("Save Wattle File"), "Wattle", tr("*.csv")); //choose a path
//    if(addr.isEmpty())
//    {
//        return;
//    }

    QString addr = QFileDialog::getOpenFileName(this,tr("Import Data"),"",tr("Data file (*.xlsx)"));   // qDebug() << file_path;
    if(addr.isEmpty())
    {
        return;
    }

}

// ---------------------------------------

// Import

void MainWindow::on_pushButton_manage_import_clicked()
{
    import_data();
}

// Export

void MainWindow::on_pushButton_manage_export_clicked()
{
    export_data();
}

// Clear

void MainWindow::on_pushButton_manage_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE * FROM 'group'");
    query.exec("DELETE * FROM 'mentor'");
    query.exec("DELETE * FROM 'mentee'");
}

// Wattle File

void MainWindow::on_pushButton_manage_export_wattle_clicked()
{
    export_wattle_file();
}

/*
// Mentors
xlsxW.addSheet("Mentors");

xlsxW.write("A1","Confirmation");
xlsxW.write("B1","First Name");
xlsxW.write("C1","Last Name");
xlsxW.write("D1","Uni ID");
xlsxW.write("E1","WWVP");
xlsxW.write("F1","O-Week");
xlsxW.write("G1","UG/PG");
xlsxW.write("H1","Academic College");
xlsxW.write("I1","Degree");
xlsxW.write("J1","Dom/Int");
xlsxW.write("K1","Gender");
xlsxW.write("L1","Languages");
xlsxW.write("M1","Language - Text");
xlsxW.write("N1","Residential Hall");
xlsxW.write("O1","Special Categories");
xlsxW.write("P1","Interests");
xlsxW.write("Q1","Training 1");
xlsxW.write("R1","Training 2");
xlsxW.write("S1","Training 3");
xlsxW.write("T1","Training Complete");

// Mentees
xlsxW.addSheet("Mentees");

xlsxW.write("A1","First Name");
xlsxW.write("B1","Last Name");
xlsxW.write("C1","Uni ID");
xlsxW.write("D1","Round");
xlsxW.write("E1","UG/PG");
xlsxW.write("F1","Academic College");
xlsxW.write("G1","u18");
xlsxW.write("H1","Dom/Int");
xlsxW.write("I1","Gender");
xlsxW.write("J1","Languages");
xlsxW.write("K1","Language - Text");
xlsxW.write("L1","Special Categories");
xlsxW.write("M1","Requests");

// Group
xlsxW.addSheet("Group");
*/


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
