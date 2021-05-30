#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QString>
#include <QDir>
#include <QTableView>
#include <QFileDialog>
#include <qversionnumber.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSortFilterProxyModel>

#include <mymentorstablemodel.h>
#include <mymenteestablemodel.h>

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

    void edit_finished();

    // Mentees

    void on_pushButton_mentees_delete_clicked();

    void on_lineEdit_mentees_search_editingFinished();

    void display_mentees_column();

    // Grouping

    void on_toolButton_left_clicked();

    void on_toolButton_right_clicked();

    void on_pushButton_mentor_auto_clicked();

    void on_pushButton_mentor_clear_clicked();

    void on_lineEdit_group_mentor_grouped_search_editingFinished();

    void on_lineEdit_group_mentor_to_be_group_search_editingFinished();

    void display_group_column();

    // Matching

    void on_pushButton_Auto_clicked();

    void on_pushButton_Up_clicked();

    void on_pushButton_Down_clicked();

    void on_pushButton_Clear_clicked();

    void on_tableView_match_mentors_clicked(const QModelIndex &index);

    void on_lineEdit_match_search_mentors_editingFinished();

    void on_lineEdit_match_search_mentees_editingFinished();

    void display_match_column();

    // Manage

    void on_pushButton_manage_import_clicked();

    void on_pushButton_manage_import_match_clicked();

    void on_pushButton_manage_export_clicked();

    void on_pushButton_manage_export_match_clicked();

    void on_pushButton_manage_export_wattle_clicked();

    void on_pushButton_manage_export_wattle_2_clicked();

    void on_pushButton_manage_clear_clicked();

    // Page

    void on_actionManage_triggered();

    void on_actionMentors_Editing_triggered();

    void on_actionMentees_Editing_triggered();

    void on_actionMentees_Grouping_triggered();

    void on_actionMentors_Grouping_triggered();

private:
    Ui::MainWindow *ui;

    // ------------------------------------

    // Database

    QSqlDatabase db;

    // ------------------------------------

    // Manage

    void import_data(QString addr, bool include_match_result);

    void export_data(QString addr, bool include_match_result);

    void export_wattle_file(QString addr, int type);

    // ------------------------------------

    // Mentors

    myMentorsTableModel * model_mentors = nullptr;

    void load_mentors();

    // ------------------------------------

    // Mentees

    myMenteesTableModel * model_mentees = nullptr;

    void load_mentees();

    // ------------------------------------

    // Group

    myMentorsTableModel * model_group_mentors_to_be_grouped = nullptr;
    myMentorsTableModel * model_group_mentors_grouped = nullptr;

    void load_group_mentors();

    void algorithm_mentors_group();

    // ------------------------------------

    // Match

    myMentorsTableModel * model_match_mentors = nullptr;
    myMenteesTableModel * model_match_mentees_matched = nullptr;
    myMenteesTableModel * model_match_mentees_to_be_match = nullptr;

    void load_match_mentees();

    void algorithm_mentees_match();

    // ------------------------------------

};

#endif // MAINWINDOW_H

