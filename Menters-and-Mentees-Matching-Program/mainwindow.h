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

#include <delegate_yes_no.h>
#include <delegate_round.h>
#include <delegate_academic_level.h>
#include <delegate_type.h>
#include <delegate_gender.h>
#include <delegate_language.h>
#include <delegate_college.h>
#include <delegate_special_mentors.h>
#include <delegate_special_mentees.h>

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

    void on_lineEdit_mentors_search_editingFinished();

    void display_mentors_column();

    // void edit_finished();

    // Mentees

    void on_pushButton_mentees_delete_clicked();

    void on_lineEdit_mentees_search_editingFinished();

    void display_mentees_column();

    // Grouping

    void on_toolButton_left_clicked();

    void on_toolButton_right_clicked();

    void on_pushButton_mentor_auto_clicked();

    void on_pushButton_mentor_clear_clicked();

    void display_group_column();

    // Matching

    void on_pushButton_Auto_clicked();

    void on_pushButton_Up_clicked();

    void on_pushButton_Down_clicked();

    void on_pushButton_Clear_clicked();

    void on_tableView_match_mentors_clicked(const QModelIndex &index);

    void on_lineEdit_match_search_mentors_editingFinished();

    void on_lineEdit_match_search_mentees_editingFinished();

    // Manage

    void on_pushButton_manage_import_clicked();

    void on_pushButton_manage_import_match_clicked();

    void on_pushButton_manage_export_clicked();

    void on_pushButton_manage_export_match_clicked();

    void on_pushButton_manage_export_wattle_clicked();

    void on_pushButton_manage_clear_clicked();

    // Page

    void on_actionManage_triggered();

    void on_actionMentors_Editing_triggered();

    void on_actionMentees_Editing_triggered();

    void on_actionMentees_Grouping_triggered();

    void on_actionMentors_Grouping_triggered();

    void on_lineEdit_group_mentor_grouped_search_editingFinished();

    void on_lineEdit_group_mentor_to_be_group_search_editingFinished();

private:
    Ui::MainWindow *ui;

    // ------------------------------------

    // Database

    QSqlDatabase db;

    void init_database(QString work_path);

    // ------------------------------------

    // Manage

    void import_data(QString addr, bool include_match_result);

    void export_data(QString addr, bool include_match_result);

    void export_wattle_file(QString addr);

    // ------------------------------------

    // Mentors

    QSqlTableModel * model_mentors = nullptr;

    void load_mentors();

    // ------------------------------------

    // Mentees

    QSqlTableModel * model_mentees = nullptr;

    void load_mentees();

    // ------------------------------------

    // Group

    QSqlTableModel * model_group_mentors_to_be_grouped = nullptr;
    QSqlTableModel * model_group_mentors_grouped = nullptr;

    void load_group_mentors();

    void algorithm_mentors_group();

    // ------------------------------------

    // Match

    QSqlTableModel * model_match_mentors = nullptr;
    QSqlTableModel * model_match_mentees_matched = nullptr;
    QSqlTableModel * model_match_mentees_to_be_match = nullptr;

    void load_match_mentees();

    void algorithm_mentees_match();

    // ------------------------------------

    // Delegrate

    Delegate_Yes_No * delegate_yes_no = nullptr;
    Delegate_Round * delegate_round = nullptr;
    Delegate_Academic_Level * delegate_academic_level = nullptr;
    Delegate_Type * delegate_type = nullptr;
    Delegate_Gender * delegate_gender = nullptr;
    Delegate_Language * delegate_language = nullptr;
    Delegate_College * delegate_college = nullptr;
    Delegate_Special_Mentors * delegate_special_mentors = nullptr;
    Delegate_Special_Mentees * delegate_special_mentees = nullptr;
};

#endif // MAINWINDOW_H

