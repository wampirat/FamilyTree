#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "PeopleCollection.hpp"
#include <QMainWindow>
#include <memory>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addPerson_clicked();


    void on_makeFamily_clicked();

private:
    std::shared_ptr<PeopleCollection> peopleCollection;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
