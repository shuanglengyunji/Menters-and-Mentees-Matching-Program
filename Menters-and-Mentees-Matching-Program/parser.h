#ifndef PARSER_H
#define PARSER_H

#include <QObject>

class parser
{
public:
    parser(QString header = QString(),
                    QStringList str_list = QStringList(),
                    int mode = Mode::string_matching,
                    QString sep = ",");
    QString get_header() { return this->header; }
    QString to_idx(QString str);
    QString to_str(QString idx);
    enum Mode {
        string_matching,
        yes_or_no,
    };
private:
    QString header;
    QStringList str_list;
    int mode;
    QString sep;
};

#endif // PARSER_H
