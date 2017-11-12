#include "AddPersonDialog.hpp"
#include "ui_AddPersonDialog.h"

AddPersonDialog::AddPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPersonDialog)
{
    ui->setupUi(this);
}

AddPersonDialog::~AddPersonDialog()
{
    delete ui;
}

bool AddPersonDialog::isPersonCreated() const
{
    return personCreated;
}

Person AddPersonDialog::getPerson() const
{
    return person;
}

void AddPersonDialog::on_diedCheckBox_toggled(bool checked)
{
    if (checked) ui->actualPlaceLabel->setText("Miejsce pochówku");
    else ui->actualPlaceLabel->setText("Miejsce zamieszkania");
    ui->diedDateEdit->setEnabled(checked);
}

void AddPersonDialog::on_buttonBox_accepted()
{
    auto name = ui->nameLineEdit->text().toStdString();
    auto surname = ui->surnameLineEdit->text().toStdString();
    auto place = ui->actualPlaceLineEdit->text().toStdString();
    auto born = ui->bornDateEdit->value();
    auto died = ui->diedCheckBox->isChecked();
    auto death = ui->diedDateEdit->value();
    person.setName(name);
    person.setSurname(surname);
    person.setPlace(place);
    person.setBirthdate(born);
    person.setIsAlive(!died);
    person.setDeathdate(death);
    personCreated = true;
}
