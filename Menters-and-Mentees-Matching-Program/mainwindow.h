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

#include <delegate_training.h>
#include <delegate_wwvp.h>
#include <delegate_confirm.h>

# define MY_DATA_BASE_NAME "database.db"

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

    // Mentors

    void on_pushButton_mentors_delete_clicked();

    void on_pushButton_mentors_revert_clicked();

    void on_lineEdit_mentors_search_editingFinished();

    // void edit_finished();

    // Mentees

    void on_pushButton_mentees_delete_clicked();

    void on_pushButton_mentees_revert_clicked();

    void on_lineEdit_mentees_search_editingFinished();

    // Mentors Grouping



    // Mentees Grouping

    void on_pushButton_Auto_clicked();

    void on_pushButton_Up_clicked();

    void on_pushButton_Down_clicked();

    void on_pushButton_Refresh_clicked();

    void on_pushButton_Clear_clicked();

    void on_tableView_match_mentors_clicked(const QModelIndex &index);

    void on_lineEdit_match_search_mentors_editingFinished();

    void on_lineEdit_match_search_mentees_editingFinished();

    // Import and Export

    void on_actionImport_Data_triggered();

    void on_actionExport_Data_triggered();

    void on_actionClear_Data_triggered();

    void on_actionExport_Wattle_File_triggered();

    // Page

    void on_actionMentors_Editing_triggered();

    void on_actionMentees_Editing_triggered();

    void on_actionMentees_Grouping_triggered();

    void on_actionMentors_Grouping_triggered();

private:
    Ui::MainWindow *ui;

    // ------------------------------------

    // Database

    QSqlDatabase db;

    void init_database(QString work_path);

    // ------------------------------------

    // Mentor page

    QSqlTableModel * model_mentors = nullptr;

    void load_mentors();

    // Delegate_Training * delegate_model_mentor_training_1;
    // Delegate_Training * delegate_model_mentor_training_2;
    // Delegate_Training * delegate_model_mentor_training_3;
    // Delegate_WWVP * delegate_model_mentor_WWVP;
    // Delegate_confirm * delegate_model_mentor_confirm;

    // ------------------------------------

    // Mentee page

    QSqlTableModel * model_mentees = nullptr;

    void load_mentees();

    // ------------------------------------

    // Match page

    QSqlQueryModel * model_match_mentors;
    QSqlQueryModel * model_match_mentees_matched;
    QSqlQueryModel * model_match_mentees_to_be_match;

    QSortFilterProxyModel * model_proxy_match_mentors;
    QSortFilterProxyModel * model_proxy_match_mentees_to_be_match;

    void init_match_page();
    void init_match_mentors_model();
    void init_match_mentors_proxy_model();
    void init_match_mentors_view();
    void init_match_mentees_to_be_match_model();
    void init_match_mentees_to_be_match_proxy_model();
    void init_match_mentees_to_be_match_view();
    void init_match_mentees_matched_model();
    void init_match_mentees_matched_view();
    void refresh_match_mentors_view();
    void refresh_match_mentees_to_be_match_view();
    void refresh_match_mentees_matched_view();
    void refresh_match();

    void match_in_turn();

    // ------------------------------------

    void match(QSqlQueryModel *mentor, QSqlQueryModel *mentee, int college, int language, int gender, int academiclevel, int consideration, int max_mentees_num);
    void match_manual_add(QString Mentor_U_Num, QString Mentee_U_Num);
    void match_manual_remove(QString Mentor_U_Num, QString Mentee_U_Num);

    // ------------------------------------

    void import_data();
    void export_data();
    void export_wattle_file();

};

#endif // MAINWINDOW_H
