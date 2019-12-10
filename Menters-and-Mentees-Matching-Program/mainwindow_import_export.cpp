#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

void MainWindow::import_data(QString addr,bool include_match_result)
{
    if(addr.isEmpty())
    {
        return;
    }

    QSqlQuery query(db);

    // [0] Drop exist tables
    query.exec("DELETE FROM 'mentor'");
    query.exec("DELETE FROM 'mentee'");

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
        college.replace("College of Arts and Social Sciences","1");
        college.replace("College of Business and Economics","2");
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

    // [5] Import Matching Results

    if (include_match_result)
    {
        QSqlQuery query(db);

        if ( !xlsxR.selectSheet("Group"))
        {
            QMessageBox::warning(this, tr("Group Sheet Missing"), tr("Unable to Import Matching Results."));
            return;
        }
        int group_max = xlsxR.dimension().lastRow();

        for (int row = 2;row <= group_max; row = row + 1)
        {
            QVariant group_id = xlsxR.cellAt(row, 1)->readValue().toString();
            QVariant uid = xlsxR.cellAt(row, 2)->readValue().toString();
            QVariant role = xlsxR.cellAt(row, 3)->readValue().toString();

            if (role == "mentor")
            {
                query.exec(QString("UPDATE mentor SET group_id=%1 WHERE uid=\'%2\'").arg(group_id.toString()).arg(uid.toString()));
                //qDebug() << query.lastError();
            }

            if (role == "mentee")
            {
                query.exec(QString("UPDATE mentee SET group_id=%1 WHERE uid=\'%2\'").arg(group_id.toString()).arg(uid.toString()));
                //qDebug() << query.lastError();
            }
        }
    }
}

// ---------------------------------------

void MainWindow::export_data(QString addr,bool include_match_result)
{
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

    xlsxW.write("A1","group_id");
    xlsxW.write("B1","uid");
    xlsxW.write("C1","role");

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
        QVariant tmp;
        QString tmp_str;

        // Confirmation
        if (r.value(1) == "y")
        {
            tmp = QVariant("I have read and understood the above text");
        }
        else
        {
            tmp = "";
        }
        xlsxW.write("A"+QVariant(row+2).toString(),tmp);

        xlsxW.write("B"+QVariant(row+2).toString(),r.value(2));     // First Name
        xlsxW.write("C"+QVariant(row+2).toString(),r.value(3));     // Last Name
        xlsxW.write("D"+QVariant(row+2).toString(),r.value(4));     // Uni ID

        if (r.value(5) == "y")
        {
            tmp = "Yes";
        }
        else
        {
            tmp = "No";
        }
        xlsxW.write("E"+QVariant(row+2).toString(),tmp);     // WWVP

        // O-Week
        if (r.value(6) == "0")
        {
            tmp = QVariant("Yes - I will be here");
        }
        else
        {
            tmp = QVariant("No - I won't be here during that time");
        }
        xlsxW.write("F"+QVariant(row+2).toString(),tmp);

        // UG/PG
        if (r.value(7) == "0")
        {
            tmp = "Undergraduate";
        }
        else
        {
            tmp = "Postgraduate (coursework)";
        }
        xlsxW.write("G"+QVariant(row+2).toString(),tmp);

        // Academic College
        tmp_str = r.value(8).toString().simplified();
        tmp_str.replace("0","College of Asia and the Pacific");
        tmp_str.replace("1","College of Arts and Social Sciences");
        tmp_str.replace("2","College of Business and Economics");
        tmp_str.replace("3","College of Engineering and Computer Science");
        tmp_str.replace("4","ANU College of Law");
        tmp_str.replace("5","College of Science");
        tmp_str.replace("6","College of Health and Medicine");
        xlsxW.write("H"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("I"+QVariant(row+2).toString(),r.value(9));     // Degree

        // Dom/Int
        if (r.value(10) == "0")
        {
            tmp = "Domestic";
        }
        else
        {
            tmp = "International";
        }
        xlsxW.write("J"+QVariant(row+2).toString(),tmp);

        // Gender
        if (r.value(11) == "0")
        {
            tmp = "Female";
        }
        else if (r.value(11) == "1")
        {
            tmp = "Male";
        }
        else if (r.value(11) == "2")
        {
            tmp = "Other";
        }
        else if (r.value(11) == "3")
        {
            tmp = "Prefer not to say";
        }
        else
        {
            tmp = "Prefer not to say";
        }
        xlsxW.write("K"+QVariant(row+2).toString(),tmp);

        // Languages
        tmp_str = r.value(12).toString().simplified();
        tmp_str.replace("0","Hindi");
        tmp_str.replace("1","Vietnamese");
        tmp_str.replace("2","German");
        tmp_str.replace("3","Korean");
        tmp_str.replace("4","Tamil");
        tmp_str.replace("5","Mandarin (Chinese)");
        tmp_str.replace("6","Spanish");
        tmp_str.replace("7","Cantonese");
        tmp_str.replace("8","Indonesian");
        tmp_str.replace("9","Japanese");
        tmp_str.replace("10","Urdu");
        xlsxW.write("L"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("M"+QVariant(row+2).toString(),r.value(13));    // Languages - Text
        xlsxW.write("N"+QVariant(row+2).toString(),r.value(14));    // Residential Hall

        tmp_str = r.value(15).toString().simplified();
        tmp_str.replace("0","An international student wanting to learn about Australian culture");
        tmp_str.replace("1","An exchange student (rather than a student new to university)");
        tmp_str.replace("2","Being a mature age student (greater than three years between school/uni or degrees)");
        tmp_str.replace("3","A mentee who lives with disability or mental illness");
        tmp_str.replace("4","A mentee who works more than 20hrs/wk while studying");
        tmp_str.replace("5","A mentee who identifies as LGBTIQ+");
        tmp_str.replace("6","A mentee who is the parent, guardian, or carer of children or another person");
        tmp_str.replace("7","A mentee from a rural area");
        tmp_str.replace("8","A mentee who is under the age of 18");
        tmp_str.replace("9","A mentee who is particularly uncomfortable speaking in English (but wants to improve)");
        tmp_str.replace("10","A mentee who is particularly shy or lacks confidence and wants someone patient (yes this is a request we get)");
        xlsxW.write("O"+QVariant(row+2).toString(),tmp_str);    // Special Categories

        xlsxW.write("P"+QVariant(row+2).toString(),r.value(16));    // Interests
        xlsxW.write("Q"+QVariant(row+2).toString(),r.value(17));    // Training 1
        xlsxW.write("R"+QVariant(row+2).toString(),r.value(18));    // Training 2
        xlsxW.write("S"+QVariant(row+2).toString(),r.value(19));    // Training 3
        xlsxW.write("T"+QVariant(row+2).toString(),r.value(20));    // Training Complete
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
        QVariant tmp;
        QString tmp_str;

        xlsxW.write("A"+QVariant(row+2).toString(),r.value(1));     // First Name
        xlsxW.write("B"+QVariant(row+2).toString(),r.value(2));     // Last Name
        xlsxW.write("C"+QVariant(row+2).toString(),r.value(3));     // Uni ID

        // Round
        if (r.value(4) == "0")
        {
            tmp = "Round 1";
        }
        else
        {
            tmp = "Round 2";
        }
        xlsxW.write("D"+QVariant(row+2).toString(),tmp);

        // UG/PG
        if (r.value(5) == "0")
        {
            tmp = "Undergraduate";
        }
        else
        {
            tmp = "Postgraduate Coursework";
        }
        xlsxW.write("E"+QVariant(row+2).toString(),tmp);

        // Academic College
        tmp_str = r.value(6).toString().simplified();
        tmp_str.replace("0","College of Asia and the Pacific");
        tmp_str.replace("1","College of Arts and Social Sciences");
        tmp_str.replace("2","College of Business and Economics");
        tmp_str.replace("3","College of Engineering and Computer Science");
        tmp_str.replace("4","College of Law");
        tmp_str.replace("5","College of Science");
        tmp_str.replace("6","College of Health and Medicine");
        xlsxW.write("F"+QVariant(row+2).toString(),tmp_str);

        xlsxW.write("G"+QVariant(row+2).toString(),r.value(7));     // u18(Degree)

        // Dom/Int
        if (r.value(8) == "0")
        {
            tmp = "Domestic";
        }
        else
        {
            tmp = "International";
        }
        xlsxW.write("H"+QVariant(row+2).toString(),tmp);

        // Gender
        if (r.value(9) == "0")
        {
            tmp = "Female";
        }
        else if (r.value(9) == "1")
        {
            tmp = "Male";
        }
        else if (r.value(9) == "2")
        {
            tmp = "Other";
        }
        else if (r.value(9) == "3")
        {
            tmp = "Prefer not to say";
        }
        else
        {
            tmp = "Prefer not to say";
        }
        xlsxW.write("I"+QVariant(row+2).toString(),tmp);

        // Languages
        tmp_str = r.value(10).toString().simplified();
        tmp_str.replace("0","Hindi");
        tmp_str.replace("1","Vietnamese");
        tmp_str.replace("2","German");
        tmp_str.replace("3","Korean");
        tmp_str.replace("4","Tamil");
        tmp_str.replace("5","Mandarin (Chinese)");
        tmp_str.replace("6","Spanish");
        tmp_str.replace("7","Cantonese");
        tmp_str.replace("8","Indonesian");
        tmp_str.replace("9","Japanese");
        tmp_str.replace("10","Urdu");
        xlsxW.write("J"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("K"+QVariant(row+2).toString(),r.value(11));    // Language - Text

        tmp_str = r.value(12).toString().simplified();
        tmp_str.replace("0","Australian culture");
        tmp_str.replace("1","Going on exchange");
        tmp_str.replace("2","Being a mature age student (greater than three years between school/uni or degrees)");
        tmp_str.replace("3","Living with disability or mental illness");
        tmp_str.replace("4","Working full or part time while studying");
        tmp_str.replace("5","Identifying as LGBTIQ+");
        tmp_str.replace("6","Being the parent, guardian, or carer of children or another person");
        tmp_str.replace("7","Living in a rural or regional area");
        xlsxW.write("L"+QVariant(row+2).toString(),tmp_str);    // Special Categories

        xlsxW.write("M"+QVariant(row+2).toString(),r.value(13));    // Requests
    }

    delete exmodel_mentees;

    // [4] matching

    if (include_match_result)
    {
        int row_count = 2;
        xlsxW.selectSheet("Group");

        // mentor

        QSqlTableModel * exmodel_mentors_matching = new QSqlTableModel(this,db);
        exmodel_mentors_matching->setTable("mentor");
        exmodel_mentors_matching->select();
        while(exmodel_mentors_matching->canFetchMore()){
            exmodel_mentors_matching->fetchMore();
        }

        for (int row = 0; row < exmodel_mentors_matching->rowCount(); row++)
        {
            QSqlRecord r = exmodel_mentors_matching->record(row);

            QVariant group_id = r.value(0);
            QVariant uid = r.value(4);

            if (group_id != "0")
            {
                xlsxW.write("A"+QVariant(row_count).toString(),group_id);
                xlsxW.write("B"+QVariant(row_count).toString(),uid);
                xlsxW.write("C"+QVariant(row_count).toString(),"mentor");

                row_count++;
            }
        }

        delete exmodel_mentors_matching;

        // mentee

        QSqlTableModel * exmodel_mentees_matching = new QSqlTableModel(this,db);
        exmodel_mentees_matching->setTable("mentee");
        exmodel_mentees_matching->select();
        while(exmodel_mentees_matching->canFetchMore()){
            exmodel_mentees_matching->fetchMore();
        }

        for (int row = 0; row < exmodel_mentees_matching->rowCount(); row++)
        {
            QSqlRecord r = exmodel_mentees_matching->record(row);

            QVariant group_id = r.value(0);
            QVariant uid = r.value(3);

            if (group_id != "0")
            {
                xlsxW.write("A"+QVariant(row_count).toString(),group_id);
                xlsxW.write("B"+QVariant(row_count).toString(),uid);
                xlsxW.write("C"+QVariant(row_count).toString(),"mentee");

                row_count++;
            }
        }

        delete exmodel_mentees_matching;
    }

    // [5]
    if ( !xlsxW.saveAs(addr) )
    {
        QMessageBox::warning(this, tr("File Saving failed"), tr("Failed to save data file."));
    }
}

// ---------------------------------------

void MainWindow::export_wattle_file(QString addr)
{
    if(addr.isEmpty())
    {
        return;
    }

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

*/


/*

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
