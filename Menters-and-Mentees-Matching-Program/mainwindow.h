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

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    QSqlTableModel * model;
    QTableView *view;
};

#endif // MAINWINDOW_H
