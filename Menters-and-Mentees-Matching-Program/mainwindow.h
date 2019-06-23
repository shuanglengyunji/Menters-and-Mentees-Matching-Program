#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QTableView>
#include <QFileDialog>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSortFilterProxyModel>

# define MY_DATA_BASE_NAME "db_mm.db"
# define MY_DATABASE_DEMO_NAME "db_mm_demo.db"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionImport_Mentors_triggered();

    void on_actionImport_Mentees_triggered();

    void on_actionManage_Mentors_triggered();

    void on_actionManage_Mentees_triggered();

    void on_actionManage_Matching_triggered();

    void on_lineEdit_mentors_search_editingFinished();

    void on_lineEdit_mentees_search_editingFinished();

    void on_pushButton_mentees_add_clicked();

    void on_pushButton_mentees_delete_clicked();

    void on_pushButton_mentees_revert_clicked();

    void on_pushButton_mentees_submit_clicked();

    void on_pushButton_mentors_add_clicked();

    void on_pushButton_mentors_delete_clicked();

    void on_pushButton_mentors_revert_clicked();

    void on_pushButton_mentors_submit_clicked();

    void on_pushButton_Auto_clicked();

    void on_pushButton_Up_clicked();

    void on_pushButton_Down_clicked();

    void on_pushButton_Refresh_clicked();

    void on_pushButton_Clear_clicked();

    void on_tableView_match_mentors_clicked(const QModelIndex &index);

    void on_actionImport_Match_Result_triggered();

    void on_actionExport_Mentors_triggered();

    void on_actionExport_Mentees_triggered();

    void on_actionExport_Match_Result_triggered();

    void on_actionExport_Wattle_File_triggered();

    void on_lineEdit_match_search_mentors_editingFinished();

    void on_lineEdit_match_search_mentees_editingFinished();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    QSqlTableModel * model_mentors;
    QSqlTableModel * model_mentees;

    QSqlQueryModel * model_match_mentors;
    QSqlQueryModel * model_match_mentees_matched;
    QSqlQueryModel * model_match_mentees_to_be_match;

    QSortFilterProxyModel * model_proxy_match_mentors;
    QSortFilterProxyModel * model_proxy_match_mentees_to_be_match;

    void init_database(QString work_path);
    void init_mentors_page();
    void init_mentees_page();
    void init_match_page();

    void match(QSqlQueryModel * mentor, QSqlQueryModel * mentee, bool college,bool language, bool gender,bool academiclevel, bool consideration);
    void refresh();

};

#endif // MAINWINDOW_H
