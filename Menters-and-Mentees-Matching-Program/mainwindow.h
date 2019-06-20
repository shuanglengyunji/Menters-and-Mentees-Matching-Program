#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QTableView>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFileDialog>

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

<<<<<<< HEAD
    void on_lineEdit_mentors_search_editingFinished();

    void on_lineEdit_mentees_search_editingFinished();
=======
    void on_pushButton_mentees_add_clicked();

    void on_pushButton_mentees_delete_clicked();

    void on_pushButton_mentees_revert_clicked();

    void on_pushButton_mentees_submit_clicked();

    void on_pushButton_mentors_add_clicked();

    void on_pushButton_mentors_delete_clicked();

    void on_pushButton_mentors_revert_clicked();

    void on_pushButton_mentors_submit_clicked();
>>>>>>> be0666371d7b33d3aeec8d91f13fe5c917e48238

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    QSqlTableModel * model_mentors;
    QSqlTableModel * model_mentees;

};

#endif // MAINWINDOW_H
