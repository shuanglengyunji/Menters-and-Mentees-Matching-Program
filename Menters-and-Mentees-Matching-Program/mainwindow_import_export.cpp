#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parser.h"

#include "../CSVWriter/include/CSVWriter.h"

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

#define MENTORS_COLUMN_NUM 21;
#define MENTEES_COLUMN_NUM 15;

QString get_header(Document* xlsx, int col) {
    return xlsx->cellAt(1, col)->readValue().toString();
}

void MainWindow::import_data(QString addr,bool include_match_result)
{
    if(addr.isEmpty())
    {
        return;
    }

    QSqlQuery query(db);

    // [0] Remove recording from all tables
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
    int mentors_max_col = MENTORS_COLUMN_NUM;
    qDebug() << "Mentor";
    qDebug() << "row max:" << mentors_max;
    qDebug() << "row max:" << mentors_max_col;

    QList<parser> mentor_parser_list;
    mentor_parser_list.append(parser("Confirmation", QStringList("I have read and understood the above text")));
    mentor_parser_list.append(parser("WWVP", QStringList(), parser::Mode::yes_or_no));
    mentor_parser_list.append(parser("Round", QStringList(QList<QString>() << "Round 1" << "Round 2")));
    mentor_parser_list.append(parser("UG/PG", QStringList(QList<QString>() << "Postgraduate (coursework)" << "Undergraduate")));
    mentor_parser_list.append(parser("Academic College", QStringList(QList<QString>() << "College of Asia and the Pacific"
        << "College of Arts and Social Sciences" << "College of Business and Economics"
        << "College of Engineering and Computer Science" << "College of Law"
        << "College of Science" << "College of Health and Medicine"
    )));
    mentor_parser_list.append(parser("Dom/Int", QStringList(QList<QString>() << "Domestic" << "International")));
    mentor_parser_list.append(parser("Gender", QStringList(QList<QString>() << "Female" << "Male" << "Other" << "Prefer not to say")));
    mentor_parser_list.append(parser("Languages", QStringList(QList<QString>() << "Hindi" << "Vietnamese" << "German" << "Korean"
        << "Tamil" << "Mandarin (Chinese)" << "Spanish" << "Cantonese" << "Indonesian" << "Japanese" << "Urdu" << "Other (please specify)"
        << "I only speak English"
    )));
    mentor_parser_list.append(parser("Interests", QStringList(QList<QString>() << "Travel" << "Comics, manga, and anime"
        << "Learning languages" << "Science fiction and fantasy" << "Gardening" << "Hiking, nature, and outdoor recreation"
        << "Cooking or food" << "Watching movies" << "Performing arts - theatre, dance, etc." << "Visual arts - drawing, painting, craft, etc."
        << "Playing or creating music" << "Playing sport"
    )));
    mentor_parser_list.append(parser("Training 1", QStringList(), parser::Mode::yes_or_no));
    mentor_parser_list.append(parser("Training 2", QStringList(), parser::Mode::yes_or_no));
    mentor_parser_list.append(parser("Training 3", QStringList(), parser::Mode::yes_or_no));
    mentor_parser_list.append(parser("Training Complete", QStringList(), parser::Mode::yes_or_no));

    QList<parser> mentee_parser_list;
    mentee_parser_list.append(parser("Round", QStringList(QList<QString>() << "Round 1" << "Round 2")));
    mentee_parser_list.append(parser("UG/PG", QStringList(QList<QString>() << "Postgraduate (coursework)" << "Undergraduate")));
    mentee_parser_list.append(parser("Academic College", QStringList(QList<QString>() << "College of Asia and the Pacific"
        << "College of Arts and Social Sciences" << "College of Business and Economics"
        << "College of Engineering and Computer Science" << "College of Law"
        << "College of Science" << "College of Health and Medicine"
    )));
    mentee_parser_list.append(parser("u18", QStringList(), parser::Mode::yes_or_no));
    mentee_parser_list.append(parser("Dom/Int", QStringList(QList<QString>() << "Domestic student not living on campus" << "International student")));
    mentee_parser_list.append(parser("Gender", QStringList(QList<QString>() << "Female" << "Male" << "Other" << "Prefer not to say")));
    mentee_parser_list.append(parser("Languages", QStringList(QList<QString>() << "Hindi" << "Vietnamese" << "German" << "Korean"
        << "Tamil" << "Mandarin (Chinese)" << "Spanish" << "Cantonese" << "Indonesian" << "Japanese" << "Urdu" << "Other (please specify)"
        << "I only speak English"
    )));
    mentee_parser_list.append(parser("Interests", QStringList(QList<QString>() << "Travel" << "Comics, manga, and anime"
        << "Learning languages" << "Science fiction and fantasy" << "Gardening" << "Hiking, nature, and outdoor recreation"
        << "Cooking or food" << "Watching movies" << "Performing arts - theatre, dance, etc." << "Visual arts - drawing, painting, craft, etc."
        << "Playing or creating music" << "Playing sport"
    )));
    mentee_parser_list.append(parser("Importance", QStringList(), parser::Mode::yes_or_no));

    for (int row = 2; row <= mentors_max; row = row + 1)
    {
        QStringList content_list;
        content_list.append(QString::number(0));
        for (int col = 1; col <= mentors_max_col; col = col + 1) {
            // get data
            QString data;
            if ( (int)xlsxR.cellAt(row, col) == 0) {
                data = QString();
            } else {
                data = xlsxR.cellAt(row, col)->readValue().toString();
            }
            // parse data
            QString out;
            for(int i = 0; i < mentor_parser_list.size(); i++) {
                if (mentor_parser_list[i].get_header() == get_header(&xlsxR, col)) {
                    out = mentor_parser_list[i].to_idx(data);
                    break;
                }
            }
            if (out.isEmpty()) {
                // no parser found
                out = data;
            }
            content_list.append("\"" + out + "\"");
        }

        //execute sql
        QString sql = "INSERT INTO mentor VALUES (" + content_list.join(",") + ")";     // qDebug() << sql;
        if ( !query.exec(sql) )
        {
            qDebug() << "Failed to Import Row " << row;
            // QMessageBox::warning(this, tr("Mentor Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            qDebug() << sql;
            qDebug() << query.lastError();
            qDebug() << "\n";
        }

    }

    // [4] Import Mentees Sheet
    xlsxR.selectSheet("Mentees");
    int mentees_max = xlsxR.dimension().lastRow();      //qDebug() << "row max:" << row_max;
    int mentees_max_col = MENTEES_COLUMN_NUM;

    qDebug() << "Mentee";
    qDebug() << "row max:" << mentees_max;
    qDebug() << "row max:" << mentees_max_col;

    for (int row = 2; row <= mentees_max; row = row + 1)
    {
        QStringList content_list;
        content_list.append(QString::number(0));
        for (int col = 1; col <= mentees_max_col; col = col + 1) {
            // get data
            QString data;
            if ( (int)xlsxR.cellAt(row, col) == 0) {
                data = QString();
            } else {
                data = xlsxR.cellAt(row, col)->readValue().toString();
            }
            // parse data
            QString out;
            for(int i = 0; i < mentee_parser_list.size(); i++) {
                if (mentee_parser_list[i].get_header() == get_header(&xlsxR, col)) {
                    out = mentee_parser_list[i].to_idx(data);
                    break;
                }
            }
            if (out.isEmpty()) {
                // no parser found
                out = data;
            }
            content_list.append("\"" + out + "\"");
        }

        // execute sql
        QString sql = "INSERT INTO mentee VALUES (" + content_list.join(",") + ")";     // qDebug() << sql;
        if ( !query.exec(sql) )
        {
            qDebug() << "Failed to Import Row " << row;
            qDebug() << sql;
            qDebug() << query.lastError();
            // QMessageBox::warning(this, tr("Mentee Record Import Failure"), tr("Failed to Import Row ") + QVariant(row).toString());
            qDebug() << sql;
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
    xlsxW.write("E1","Phone");
    xlsxW.write("F1","WWVP");
    xlsxW.write("G1","Round");
    xlsxW.write("H1","UG/PG");
    xlsxW.write("I1","Academic College");
    xlsxW.write("J1","Degree");
    xlsxW.write("K1","Dom/Int");
    xlsxW.write("L1","Gender");
    xlsxW.write("M1","Languages");
    xlsxW.write("N1","Language - Text");
    xlsxW.write("O1","Residential Hall");
    xlsxW.write("P1","Interests");
    xlsxW.write("Q1","Requests");
    xlsxW.write("R1","Training 1");
    xlsxW.write("S1","Training 2");
    xlsxW.write("T1","Training 3");
    xlsxW.write("U1","Training Complete");

    // Mentees
    xlsxW.addSheet("Mentees");

    xlsxW.write("A1","First Name");
    xlsxW.write("B1","Last Name");
    xlsxW.write("C1","Uni ID");
    xlsxW.write("D1","Alternate email");
    xlsxW.write("E1","Round");
    xlsxW.write("F1","UG/PG");
    xlsxW.write("G1","Academic College");
    xlsxW.write("H1","u18");
    xlsxW.write("I1","Dom/Int");
    xlsxW.write("J1","Gender");
    xlsxW.write("K1","Languages");
    xlsxW.write("L1","Language - Text");
    xlsxW.write("M1","Interests");
    xlsxW.write("N1","Requests");
    xlsxW.write("O1","Importance");

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
        xlsxW.write("E"+QVariant(row+2).toString(),r.value(5));     // Phone

        if (r.value(6) == "y")
        {
            tmp = "Yes";
        }
        else
        {
            tmp = "No";
        }
        xlsxW.write("F"+QVariant(row+2).toString(),tmp);     // WWVP

        // Round
        if (r.value(7) == "0")
        {
            tmp = QVariant("Round 1");
        }
        else
        {
            tmp = QVariant("Round 2");
        }
        xlsxW.write("G"+QVariant(row+2).toString(),tmp);

        // UG/PG
        if (r.value(8) == "0")
        {
            tmp = "Undergraduate";
        }
        else
        {
            tmp = "Postgraduate (coursework)";
        }
        xlsxW.write("H"+QVariant(row+2).toString(),tmp);

        // Academic College
        tmp_str = r.value(9).toString().simplified();
        tmp_str.replace("0","College of Asia and the Pacific");
        tmp_str.replace("1","College of Arts and Social Sciences");
        tmp_str.replace("2","College of Business and Economics");
        tmp_str.replace("3","College of Engineering and Computer Science");
        tmp_str.replace("4","ANU College of Law");
        tmp_str.replace("5","College of Science");
        tmp_str.replace("6","College of Health and Medicine");
        xlsxW.write("I"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("J"+QVariant(row+2).toString(),r.value(10));     // Degree

        // Dom/Int
        if (r.value(11) == "0")
        {
            tmp = "Domestic";
        }
        else
        {
            tmp = "International";
        }
        xlsxW.write("K"+QVariant(row+2).toString(),tmp);

        // Gender
        if (r.value(12) == "0")
        {
            tmp = "Female";
        }
        else if (r.value(12) == "1")
        {
            tmp = "Male";
        }
        else if (r.value(12) == "2")
        {
            tmp = "Other";
        }
        else if (r.value(12) == "3")
        {
            tmp = "Prefer not to say";
        }
        else
        {
            tmp = "Prefer not to say";
        }
        xlsxW.write("L"+QVariant(row+2).toString(),tmp);

        // Languages
        tmp_str = r.value(13).toString().simplified();
        tmp_str.replace("11","Other (please specify)");
        tmp_str.replace("10","Urdu");
        tmp_str.replace("9","Japanese");
        tmp_str.replace("8","Indonesian");
        tmp_str.replace("7","Cantonese");
        tmp_str.replace("6","Spanish");
        tmp_str.replace("5","Mandarin (Chinese)");
        tmp_str.replace("4","Tamil");
        tmp_str.replace("3","Korean");
        tmp_str.replace("2","German");
        tmp_str.replace("1","Vietnamese");
        tmp_str.replace("0","Hindi");
        xlsxW.write("M"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("N"+QVariant(row+2).toString(),r.value(14));    // Languages - Text
        xlsxW.write("O"+QVariant(row+2).toString(),r.value(15));    // Residential Hall

        tmp_str = r.value(16).toString().simplified();
        tmp_str.replace("11","Playing sport");
        tmp_str.replace("10","Playing or creating music");
        tmp_str.replace("9","Visual arts - drawing, painting, craft, etc.");
        tmp_str.replace("8","Performing arts - theatre, dance, etc.");
        tmp_str.replace("7","Watching movies");
        tmp_str.replace("6","Cooking or food");
        tmp_str.replace("5","Hiking, nature, and outdoor recreation");
        tmp_str.replace("4","Gardening");
        tmp_str.replace("3","Science fiction and fantasy");
        tmp_str.replace("2","Learning languages");
        tmp_str.replace("1","Comics, manga, and anime");
        tmp_str.replace("0","Travel");
        xlsxW.write("P"+QVariant(row+2).toString(),tmp_str);    // Interests


        xlsxW.write("Q"+QVariant(row+2).toString(),r.value(17)); // Requests

        xlsxW.write("R"+QVariant(row+2).toString(),r.value(18));    // Training 1
        xlsxW.write("S"+QVariant(row+2).toString(),r.value(19));    // Training 2
        xlsxW.write("T"+QVariant(row+2).toString(),r.value(20));    // Training 3
        xlsxW.write("U"+QVariant(row+2).toString(),r.value(21));    // Training Complete
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
        xlsxW.write("D"+QVariant(row+2).toString(),r.value(4));     // Alternate email

        // Round
        if (r.value(5) == "0")
        {
            tmp = "Round 1";
        }
        else
        {
            tmp = "Round 2";
        }
        xlsxW.write("E"+QVariant(row+2).toString(),tmp);

        // UG/PG
        if (r.value(6) == "0")
        {
            tmp = "Undergraduate";
        }
        else
        {
            tmp = "Postgraduate (coursework)";
        }
        xlsxW.write("F"+QVariant(row+2).toString(),tmp);

        // Academic College
        tmp_str = r.value(7).toString().simplified();
        tmp_str.replace("0","College of Asia and the Pacific");
        tmp_str.replace("1","College of Arts and Social Sciences");
        tmp_str.replace("2","College of Business and Economics");
        tmp_str.replace("3","College of Engineering and Computer Science");
        tmp_str.replace("4","College of Law");
        tmp_str.replace("5","College of Science");
        tmp_str.replace("6","College of Health and Medicine");
        xlsxW.write("G"+QVariant(row+2).toString(),tmp_str);

        // u18
        if (r.value(8) == "0")
        {
            tmp = "No";
        }
        else
        {
            tmp = "Yes";
        }
        xlsxW.write("H"+QVariant(row+2).toString(),tmp);

        // Dom/Int
        if (r.value(9) == "0")
        {
            tmp = "Domestic";
        }
        else
        {
            tmp = "International";
        }
        xlsxW.write("I"+QVariant(row+2).toString(),tmp);

        // Gender
        if (r.value(10) == "0")
        {
            tmp = "Female";
        }
        else if (r.value(10) == "1")
        {
            tmp = "Male";
        }
        else if (r.value(10) == "2")
        {
            tmp = "Other";
        }
        else if (r.value(10) == "3")
        {
            tmp = "Prefer not to say";
        }
        else
        {
            tmp = "Prefer not to say";
        }
        xlsxW.write("J"+QVariant(row+2).toString(),tmp);

        // Languages
        tmp_str = r.value(11).toString().simplified();
        tmp_str.replace("11","Other (please specify)");
        tmp_str.replace("10","Urdu");
        tmp_str.replace("9","Japanese");
        tmp_str.replace("8","Indonesian");
        tmp_str.replace("7","Cantonese");
        tmp_str.replace("6","Spanish");
        tmp_str.replace("5","Mandarin (Chinese)");
        tmp_str.replace("4","Tamil");
        tmp_str.replace("3","Korean");
        tmp_str.replace("2","German");
        tmp_str.replace("1","Vietnamese");
        tmp_str.replace("0","Hindi");
        xlsxW.write("K"+QVariant(row+2).toString(),QVariant(tmp_str));

        xlsxW.write("L"+QVariant(row+2).toString(),r.value(12));    // Language - Text

        tmp_str = r.value(13).toString().simplified();
        tmp_str.replace("11","Playing sport");
        tmp_str.replace("10","Playing or creating music");
        tmp_str.replace("9","Visual arts - drawing, painting, craft, etc.");
        tmp_str.replace("8","Performing arts - theatre, dance, etc.");
        tmp_str.replace("7","Watching movies");
        tmp_str.replace("6","Cooking or food");
        tmp_str.replace("5","Hiking, nature, and outdoor recreation");
        tmp_str.replace("4","Gardening");
        tmp_str.replace("3","Science fiction and fantasy");
        tmp_str.replace("2","Learning languages");
        tmp_str.replace("1","Comics, manga, and anime");
        tmp_str.replace("0","Travel");
        xlsxW.write("M"+QVariant(row+2).toString(),tmp_str);    // Interests

        xlsxW.write("N"+QVariant(row+2).toString(),r.value(14));    // Requests

        // Importance
        if (r.value(15) == "0")
        {
            tmp = "No";
        }
        else
        {
            tmp = "Yes";
        }
        xlsxW.write("O"+QVariant(row+2).toString(),tmp);
    }

    delete exmodel_mentees;

    // [4] matching

    if (include_match_result)
    {
        // Add Sheet and Sheet Headers
        xlsxW.addSheet("Group");

        xlsxW.write("A1","group_id");
        xlsxW.write("B1","uid");
        xlsxW.write("C1","role");

        // Export data

        int row_count = 2;

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

void MainWindow::export_wattle_file(QString addr, int type)
{
    // type: 0 - mentor/mentee label in group     1 - group_id in group

    if(addr.isEmpty())
    {
        return;
    }

    // csv object
    CSVWriter csv(",");
    csv.newRow() << "user" << "group";  // header

    // mentors data

    QSqlTableModel * exwattle_mentors = new QSqlTableModel(this,db);
    exwattle_mentors->setTable("mentor");
    exwattle_mentors->setSort(0,Qt::AscendingOrder);
    exwattle_mentors->select();
    while(exwattle_mentors->canFetchMore()){
        exwattle_mentors->fetchMore();
    }

    for (int row = 0; row < exwattle_mentors->rowCount(); row++)
    {
        QSqlRecord r = exwattle_mentors->record(row);

        QVariant group_id = r.value(0);
        QVariant uid = r.value(4);// change from r.value(4) to r.value(3)

        if (group_id != "0")
        {
            if (type)
                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
            else
                csv.newRow() << uid.toString().toStdString() << "mentor";
        }
    }

    delete exwattle_mentors;

    // mentees data

    QSqlTableModel * exwattle_mentees = new QSqlTableModel(this,db);
    exwattle_mentees->setTable("mentee");
    exwattle_mentees->setSort(0,Qt::AscendingOrder);
    exwattle_mentees->select();
    while(exwattle_mentees->canFetchMore())
    {
        exwattle_mentees->fetchMore();
    }

    for (int row = 0; row < exwattle_mentees->rowCount(); row++)
    {
        QSqlRecord r = exwattle_mentees->record(row);

        QVariant group_id = r.value(0);
        QVariant uid = r.value(3);

        if (group_id != "0")
        {
            if (type)
                csv.newRow() << uid.toString().toStdString() << group_id.toString().toStdString();
            else
                csv.newRow() << uid.toString().toStdString() << "mentee";
        }
    }

    delete exwattle_mentees;

    // write to file
    csv.writeToFile(addr.toStdString());
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
