#ifndef PARSER_H
#define PARSER_H

#include <QObject>

class parser
{
public:
    parser(
        int index,
        int mode,
        QString header,
        QString table_header,
        QStringList str_list = QStringList(),
        QString sep = ","
    );
    int get_index() { return this->index; }
    QString get_header() { return this->header; }
    QString get_table_header() { return this->table_header; }
    QString to_idx(QString str);
    QString to_str(QString idx);
    enum Mode {
        string_matching,
        yes_or_no,
        pass_through,
    };
private:
    int index;
    int mode;
    QString header;
    QString table_header;
    QStringList str_list;
    QString sep;
};

#endif // PARSER_H
