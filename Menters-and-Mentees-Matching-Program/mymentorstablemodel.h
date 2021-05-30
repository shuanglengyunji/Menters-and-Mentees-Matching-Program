#ifndef MYMENTORSTABLEMODEL_H
#define MYMENTORSTABLEMODEL_H

#include <QObject>
#include <QDebug>
#include <qsqltablemodel.h>
#include "parser.h"

class myMentorsTableModel : public QSqlTableModel
{
public:
    myMentorsTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase(), bool use_group_id_as_row_number = false) {
        this->setTable("mentor");
        this->setEditStrategy(QSqlTableModel::OnFieldChange);
        this->select();
        while(this->canFetchMore()){
            this->fetchMore();
        }
        this->use_group_id_as_row_number = use_group_id_as_row_number;
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
    bool use_group_id_as_row_number;
    QString table_init_query = "CREATE TABLE IF NOT EXISTS [mentor] (           \
        group_id         INTEGER NOT NULL DEFAULT 0,         \
        is_confirmed 	CHAR(1) NOT NULL DEFAULT 0,         \
        first_name		VARCHAR(50),                    \
        last_name		VARCHAR(50),                    \
        uid				VARCHAR(20) NOT NULL UNIQUE,    \
        phone			VARCHAR(50),                    \
        wwvp             VARCHAR(10),                    \
        round			INTEGER(1),                     \
        academic_level	INTEGER(1),                     \
        college			INTEGER(1),                     \
        degree			VARCHAR(100),                   \
        type             INTEGER(1),                         \
        gender			INTEGER(1),                     \
        languages		VARCHAR(50),                    \
        languages_text	TEXT(500),                      \
        hall             VARCHAR(50),                        \
        interests		VARCHAR(50),                     \
        requests		    TEXT(1000),                     \
        train_1			CHAR(1) NOT NULL DEFAULT 0,   \
        train_2			CHAR(1) NOT NULL DEFAULT 0,   \
        train_3			CHAR(1) NOT NULL DEFAULT 0,   \
        train_complete	CHAR(1) NOT NULL DEFAULT 0,   \
        PRIMARY KEY(uid)                                 \
    )";
    QList<parser> parser_list = {
        parser(1, parser::Mode::string_matching, "Confirmation", "is_confirmed",
            QStringList(QList<QString>() << "I have read and understood the above text")),
        parser(2, parser::Mode::pass_through, "First Name", "first_name"),
        parser(3, parser::Mode::pass_through, "Last Name", "last_name"),
        parser(4, parser::Mode::pass_through, "Uni ID", "uid"),
        parser(5, parser::Mode::pass_through, "Phone", "phone"),
        parser(6, parser::Mode::yes_or_no, "WWVP", "wwvp"),
        parser(7, parser::Mode::string_matching, "Round", "round", QStringList(QList<QString>() << "Round 1" << "Round 2")),
        parser(8, parser::Mode::string_matching, "UG/PG", "academic_level",
            QStringList(QList<QString>() << "Postgraduate (coursework)" << "Undergraduate")),
        parser(9, parser::Mode::string_matching, "Academic College", "college",
            QStringList(QList<QString>() << "College of Asia and the Pacific"
            << "College of Arts and Social Sciences" << "College of Business and Economics"
            << "College of Engineering and Computer Science" << "College of Law"
            << "College of Science" << "College of Health and Medicine"
        )),
        parser(10, parser::Mode::pass_through, "Degree", "degree"),
        parser(11, parser::Mode::string_matching, "Dom/Int", "type", QStringList(QList<QString>() << "Domestic" << "International")),
        parser(12, parser::Mode::string_matching, "Gender", "gender", QStringList(QList<QString>() << "Female" << "Male" << "Other" << "Prefer not to say")),
        parser(13, parser::Mode::string_matching, "Languages", "languages",
            QStringList(QList<QString>() << "Hindi" << "Vietnamese" << "German" << "Korean"
            << "Tamil" << "Mandarin (Chinese)" << "Spanish" << "Cantonese" << "Indonesian" << "Japanese" << "Urdu" << "Other (please specify)"
            << "I only speak English"
        )),
        parser(14, parser::Mode::pass_through, "Language - Text", "languages_text"),
        parser(15, parser::Mode::pass_through, "Residential Hall", "hall"),
        parser(16, parser::Mode::string_matching, "Interests", "interests",
            QStringList(QList<QString>() << "Travel" << "Comics, manga, and anime"
            << "Learning languages" << "Science fiction and fantasy" << "Gardening" << "Hiking, nature, and outdoor recreation"
            << "Cooking or food" << "Watching movies" << "Performing arts - theatre, dance, etc." << "Visual arts - drawing, painting, craft, etc."
            << "Playing or creating music" << "Playing sport"
        )),
        parser(17, parser::Mode::pass_through, "Requests", "requests"),
        parser(18, parser::Mode::yes_or_no, "Training 1", "train_1"),
        parser(19, parser::Mode::yes_or_no, "Training 2", "train_2"),
        parser(20, parser::Mode::yes_or_no, "Training 3", "train_3"),
        parser(21, parser::Mode::yes_or_no, "Training Complete", "train_complete"),
    };
};

#endif // MYMENTORSTABLEMODEL_H
