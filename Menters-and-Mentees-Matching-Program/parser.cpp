#include "parser.h"
#include <QtDebug>

parser::parser(
        int index,
        int mode,
        QString header,
        QString table_header,
        QStringList str_list,
        QString sep
) {
    this->index = index;
    this->header = header;
    this->table_header = table_header;
    this->mode = mode;
    this->str_list = str_list;
    this->sep = sep;
}

QString parser::to_idx(QString str) {
    if (this->mode == Mode::string_matching) {
        QStringList out;
        for (int idx = 0; idx < this->str_list.size(); idx++) {
            if (str.contains(this->str_list[idx])) {
                out.append(QString::number(idx));
            }
        }
        if (out.isEmpty()) {
            qDebug() << "[Parser] Invalid data" << str << "from header " << header;
        }
        return out.join(this->sep);
    } else if (this->mode == Mode::yes_or_no) {
        if (str.isEmpty() || str.toLower() == "n" || str.toLower() == "no") {
            return QString::number(0);
        } else {
            return QString::number(1);
        }
    } else if (this->mode == Mode::pass_through) {
        return str;
    } else {
        qDebug() << "[Parser] Unexpected parsing mode!";
        return str;
    }
}

QString parser::to_str(QString idx) {
    if (this->mode == Mode::string_matching) {
        QStringList out;
        QStringList idx_list = idx.split(this->sep);
        for (int idx = 0; idx < this->str_list.size(); idx++) {
            if (idx_list.contains(QString::number(idx))) {
                out.append(this->str_list[idx]);
            }
        }
        return out.join(this->sep);
    } else if (this->mode == Mode::yes_or_no) {
        if (idx == QString::number(0)) {
            return "No";
        } else {
            return "Yes";
        }
    } else if (this->mode == Mode::pass_through) {
        return idx;
    } else {
        qDebug() << "Unexpected parsing mode!";
        return idx;
    }
}
