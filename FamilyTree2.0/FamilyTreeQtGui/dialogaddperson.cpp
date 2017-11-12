#include "dialogaddperson.h"
#include "ui_dialogaddperson.h"
#include "SocialPerson.hpp"
#include <iostream>

using namespace std;

DialogAddPerson::DialogAddPerson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddPerson)
{
    ui->setupUi(this);
}

DialogAddPerson::~DialogAddPerson()
{
    delete ui;
}

Person DialogAddPerson::getPerson()
{
    return person;
}

void DialogAddPerson::on_aliveCheckBox_toggled(bool checked)
{
    ui->placeLabel->setText(QString((checked)?"Miejsce zamieszkania":"Miejsce pochówku"));
    ui->deathLabel->setEnabled(!checked);
    ui->deathDateInput->setEnabled(!checked);
}

void DialogAddPerson::on_buttonBox_accepted()
{
    person.setName(ui->nameInput->text().toStdString());
    person.setSurname(ui->surnameInput->text().toStdString());
    person.setBirthdate(ui->bornDateInput->value());
    person.setIsAlive(ui->aliveCheckBox->isChecked());
    if (!person.getIsAlive()) person.setDeathdate(ui->deathDateInput->value());
    person.setPlace(ui->placeInput->text().toStdString());
}

void DialogAddPerson::setPerson(const std::shared_ptr<SocialPerson>& p)
{
    ui->nameInput->setText(QString::fromStdString(p->getName()));
    ui->surnameInput->setText(QString::fromStdString(p->getSurname()));
    ui->bornDateInput->setValue(p->getBirthdate());
    if (p->getDeathdate() != 0) ui->aliveCheckBox->setChecked(false);
    ui->deathDateInput->setValue(p->getDeathdate());
    ui->placeInput->setText(QString::fromStdString(p->getPlace()));
}
