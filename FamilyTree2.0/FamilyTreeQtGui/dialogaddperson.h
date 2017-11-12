#ifndef DIALOGADDPERSON_H
#define DIALOGADDPERSON_H

#include <QDialog>
#include "Person.hpp"

class SocialPerson;

namespace Ui {
class DialogAddPerson;
}

class DialogAddPerson : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddPerson(QWidget *parent = 0);
    Person getPerson();
    void setPerson(const std::shared_ptr<SocialPerson> &);
    ~DialogAddPerson();

private slots:

    void on_aliveCheckBox_toggled(bool checked);

    void on_buttonBox_accepted();

private:
    Ui::DialogAddPerson *ui;
    Person person;
};

#endif // DIALOGADDPERSON_H
