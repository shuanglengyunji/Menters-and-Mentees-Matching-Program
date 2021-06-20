#ifndef MYMENTEESTABLEMODEL_H
#define MYMENTEESTABLEMODEL_H

#include <QObject>
#include <QDebug>
#include <qsqltablemodel.h>
#include "parser.h"

class myMenteesTableModel : public QSqlTableModel
{
public:
    myMenteesTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase()) {
        this->setTable("mentee");
        this->setEditStrategy(QSqlTableModel::OnFieldChange);
        this->select();
        while(this->canFetchMore()){
            this->fetchMore();
        }
    };
    QVariant headerData(int section,  Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void init_table();
    parser get_parser(int idx) {
        for(int i=0; i<this->parser_list.size(); i++) {
            parser p = parser_list[i];
            if (p.get_index() == idx) {
                return p;
            }
        }
        qDebug() << "unexpected parser index";
        exit(1);
    }

private:
    QString table_init_query = "CREATE TABLE IF NOT EXISTS [mentee] (           \
        group_id			INTEGER NOT NULL DEFAULT 0,    \
        first_name			VARCHAR(1000),      \
        last_name			VARCHAR(1000),      \
        uid					VARCHAR(20) NOT NULL UNIQUE,  \
        email				VARCHAR(1000),              \
        round               INTEGER(1),                     \
        academic_level		INTEGER(1),               \
        college				VARCHAR(50),                \
        u18					INTEGER(1),                \
        type				INTEGER(1),                \
        gender				INTEGER(1),                \
        languages			VARCHAR(50),                \
        languages_text		TEXT(1000),                \
        interests			VARCHAR(50),                \
        requests			TEXT(1000),                \
        importance          INTEGER(1),                \
        PRIMARY KEY(uid)                               \
    )";
    QList<parser> parser_list = {
        parser(1, parser::Mode::pass_through, "First Name", "first_name"),
        parser(2, parser::Mode::pass_through, "Last Name", "last_name"),
        parser(3, parser::Mode::pass_through, "Uni ID", "uid"),
        parser(4, parser::Mode::pass_through, "Alternate email", "email"),
        parser(5, parser::Mode::string_matching, "Round", "round", QStringList(QList<QString>() << "Round 1" << "Round 2")),
        parser(6, parser::Mode::string_matching, "UG/PG", "academic_level",
            QStringList(QList<QString>() << "Postgraduate (coursework)" << "Undergraduate")),
        parser(7, parser::Mode::string_matching_multiple, "Academic College", "college",
            QStringList(QList<QString>() << "College of Asia and the Pacific"
            << "College of Arts and Social Sciences" << "College of Business and Economics"
            << "College of Engineering and Computer Science" << "College of Law"
            << "College of Science" << "College of Health and Medicine"
        )),
        parser(8, parser::Mode::yes_or_no, "u18", "u18"),
        parser(9, parser::Mode::string_matching, "Dom/Int", "type",
            QStringList(QList<QString>() << "Domestic student not living on campus" << "International student")),
        parser(10, parser::Mode::string_matching, "Gender", "gender",
            QStringList(QList<QString>() << "Female" << "Male" << "Other" << "Prefer not to say")),
        parser(11, parser::Mode::string_matching_multiple, "Languages", "languages",
            QStringList(QList<QString>() << "Hindi" << "Vietnamese" << "German" << "Korean"
            << "Tamil" << "Mandarin (Chinese)" << "Spanish" << "Cantonese" << "Indonesian" << "Japanese" << "Urdu" << "Other (please specify)"
            << "I only speak English"
        )),
        parser(12, parser::Mode::pass_through, "Language - Text", "languages_text"),
        parser(13, parser::Mode::string_matching_multiple, "Interests", "interests",
            QStringList(QList<QString>() << "Travel" << "Comics, manga, and anime"
            << "Learning languages" << "Science fiction and fantasy" << "Gardening" << "Hiking, nature, and outdoor recreation"
            << "Cooking or food" << "Watching movies" << "Performing arts - theatre, dance, etc." << "Visual arts - drawing, painting, craft, etc."
            << "Playing or creating music" << "Playing sport"
        )),
        parser(14, parser::Mode::pass_through, "Requests", "requests"),
        parser(15, parser::Mode::yes_or_no, "Importance", "importance"),
    };

};

#endif // MYMENTEESTABLEMODEL_H
