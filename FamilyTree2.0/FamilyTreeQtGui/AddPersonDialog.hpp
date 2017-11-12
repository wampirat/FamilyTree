#ifndef ADDPERSONDIALOG_HPP
#define ADDPERSONDIALOG_HPP

#include "Person.hpp"
#include <QDialog>

namespace Ui
{
class AddPersonDialog;
}

class AddPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPersonDialog(QWidget *parent = 0);
    ~AddPersonDialog();
    Person getPerson() const;
    bool isPersonCreated() const;

private slots:
    void on_diedCheckBox_toggled(bool checked);

    void on_buttonBox_accepted();

private:
    Ui::AddPersonDialog *ui;
    Person person;
    bool personCreated = false;
};

#endif // ADDPERSONDIALOG_HPP
