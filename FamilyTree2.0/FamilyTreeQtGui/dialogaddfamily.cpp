#include "dialogaddfamily.h"
#include "ui_dialogaddfamily.h"
#include "SocialPerson.hpp"
#include "Person.hpp"
#include <iostream>
#include <algorithm>
#include <QString>
#include <QLineEdit>

using namespace std;

namespace
{
    void sortUnique(auto& v)
    {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    }

    bool isInCollection(const auto& v, auto c)
    {
        return v.end() != std::find(v.begin(), v.end(), c);
    }
}

bool DialogAddFamily::showAndExecDialog(auto&& window)
{
    window.show();
    this->setEnabled(false);
    auto retval = (QDialog::Accepted == window.exec());
    this->setEnabled(true);
    return retval;
}

void DialogAddFamily::resetCompleter()
{
    if (completer) delete completer;
    completer = new QCompleter(peopleDescriptionStringList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->peopleList->setCompleter(completer);
}

DialogAddFamily::DialogAddFamily(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddFamily)
{
    ui->setupUi(this);
    ui->PrevFamily->hide();
    ui->NextFamily->hide();
    ui->filterByPerson->hide();
    ui->peopleList->lineEdit()->setClearButtonEnabled(true);
    resetCompleter();
}

void DialogAddFamily::setPeople()
{
    peopleDescriptionStringList.clear();
    for (const auto& p : ftf->getPeople())
    {
        peopleDescriptionStringList << QString::fromStdString((*p)->toString());
    }
    ui->peopleList->addItems(peopleDescriptionStringList);
    resetCompleter();
    currentPersonIndex = ui->peopleList->currentIndex();
}

DialogAddFamily::~DialogAddFamily()
{
    if (completer) delete completer;
    delete ui;
}

void DialogAddFamily::addPersonToFamily(auto& listView, auto& indexes)
{
    if (ui->peopleList->count()
        && !isInCollection(addedPartnersIndexes, currentPersonIndex)
        && !isInCollection(addedKidsIndexes, currentPersonIndex))
    {
        auto personDescription = (*ftf->getPeople()[currentPersonIndex])->toString();
        listView->addItem(QString::fromStdString(personDescription));
        indexes.push_back(currentPersonIndex);
    }
}

void DialogAddFamily::on_addPartner_clicked()
{
    addPersonToFamily(ui->familyPartnersMembersView, addedPartnersIndexes);
}

void DialogAddFamily::on_addKid_clicked()
{
    addPersonToFamily(ui->familyKidsMembersView, addedKidsIndexes);
}

void DialogAddFamily::on_peopleList_currentIndexChanged(int index)
{
    currentPersonIndex = index;
    if (currentPersonFilterIndex != -1)
    {
        currentPersonFilterIndex = index;
        for (unsigned i = 0; i < ftf->familiesSize(); ++i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                showFamily(i, currentPersonIndex);
                break;
            }
        }
    }
}

Family DialogAddFamily::getFamily()
{
    return family;
}

void DialogAddFamily::on_buttonBox_accepted()
{
    sortUnique(addedPartnersIndexes);
    sortUnique(addedKidsIndexes);
    for (auto i : addedPartnersIndexes) family.addPartner(ftf->getPeople()[i]);
    for (auto i : addedKidsIndexes) family.addKid(ftf->getPeople()[i]);
    if (currentFamilyIndex > -1)
    {
        ftf->getFamilies()[currentFamilyIndex]->update(family);
    }
}

void DialogAddFamily::toggleMemberViewButtons()
{
    auto shouldEnable = ui->familyPartnersMembersView->selectedItems().size()
                     || ui->familyKidsMembersView->selectedItems().size();
    ui->editPersonButton->setEnabled(shouldEnable);
    ui->deleteFromFamily->setEnabled(shouldEnable);
}

void DialogAddFamily::on_familyPartnersMembersView_itemSelectionChanged()
{
    if (ui->familyPartnersMembersView->selectedItems().size())
    {
        ui->familyKidsMembersView->clearSelection();
    }
    toggleMemberViewButtons();
}

void DialogAddFamily::on_familyKidsMembersView_itemSelectionChanged()
{
    if (ui->familyKidsMembersView->selectedItems().size())
    {
        ui->familyPartnersMembersView->clearSelection();
    }
    toggleMemberViewButtons();
}

void DialogAddFamily::removePersonFromFamily(auto& members, auto& indexes)
{
    if (members->selectedItems().size())
    {
        auto item = members->currentItem();
        auto index = members->row(item);
        indexes.erase(indexes.begin() + index);
        members->takeItem(index);
    }
}

void DialogAddFamily::on_deleteFromFamily_clicked()
{
    removePersonFromFamily(ui->familyPartnersMembersView, addedPartnersIndexes);
    removePersonFromFamily(ui->familyKidsMembersView, addedKidsIndexes);
}

void DialogAddFamily::setFamilyTreeFramework(std::shared_ptr<FamilyTreeFramework>& _ftf)
{
    ftf = _ftf;
    setPeople();
}

void DialogAddFamily::showFamily(int index, int personIndex)
{
    clearFamilyView();
    ui->NextFamily->show();
    ui->PrevFamily->show();
    ui->filterByPerson->show();

    currentFamilyIndex = index;
    if (currentPersonFilterIndex == -1)
    {
        if (currentFamilyIndex > 0) ui->PrevFamily->setEnabled(true);
        else ui->PrevFamily->setEnabled(false);

        if (currentFamilyIndex + 1 < static_cast<int>(ftf->getFamilies().size())) ui->NextFamily->setEnabled(true);
        else ui->NextFamily->setEnabled(false);
    }
    else
    {
        ui->NextFamily->setEnabled(false);
        for (unsigned i = currentFamilyIndex + 1; i < ftf->familiesSize(); ++i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                ui->NextFamily->setEnabled(true);
                break;
            }
        }
        ui->PrevFamily->setEnabled(false);
        for (unsigned i = currentFamilyIndex - 1; i < ftf->familiesSize(); --i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                ui->PrevFamily->setEnabled(true);
                break;
            }
        }
    }

    if (index < 0)
    {
        return;
    }

    const auto family = ftf->getFamilies()[currentFamilyIndex];
    const auto partners = family->getPartners();
    const auto kids = family->getKids();
    const auto people = ftf->getPeople();

    for (const auto& partner : partners)
    {
        auto i = 0;
        for (const auto& person : people)
        {
            if (person == partner)
            {
                currentPersonIndex = i;
                addPersonToFamily(ui->familyPartnersMembersView, addedPartnersIndexes);
                break;
            }
            ++i;
        }
    }

    for (const auto& kid : kids)
    {
        auto i = 0;
        for (const auto& person : people)
        {
            if (person == kid)
            {
                currentPersonIndex = i;
                addPersonToFamily(ui->familyKidsMembersView, addedKidsIndexes);
                break;
            }
            ++i;
        }
    }
    ui->peopleList->setCurrentIndex(personIndex);
    currentPersonIndex = personIndex;
}

void DialogAddFamily::addNewPerson(auto&& ap, auto& familyMembers, auto& familyIndexes)
{
    auto person = ftf->createPerson();
    person->setPerson(ap);
    ftf->getPeople() = ftf->getPeople();
    ui->peopleList->addItem(QString::fromStdString((*person)->toString()));
    peopleDescriptionStringList << QString::fromStdString((*person)->toString());
    resetCompleter();
    currentPersonIndex = ui->peopleList->count() - 1;
    ui->peopleList->setCurrentIndex(currentPersonIndex);
    addPersonToFamily(familyMembers, familyIndexes);
}

void DialogAddFamily::on_addNewPartner_clicked()
{
    DialogAddPerson ap;
    if (showAndExecDialog(ap))
    {
        addNewPerson(ap.getPerson(), ui->familyPartnersMembersView, addedPartnersIndexes);
    }
}

void DialogAddFamily::on_addNewKid_clicked()
{
    DialogAddPerson ap;
    if (showAndExecDialog(ap))
    {
        addNewPerson(ap.getPerson(), ui->familyKidsMembersView, addedKidsIndexes);
    }
}


void DialogAddFamily::editPerson(auto& members, auto& indexes)
{
    if (members->selectedItems().size())
    {
        auto item = members->currentItem();
        auto index = members->row(item);

        auto person = ftf->getPeople()[indexes[index]];
        DialogAddPerson ap;
        ap.setPerson(person);
        if (showAndExecDialog(ap))
        {
            ftf->getPeople()[indexes[index]]->setPerson(ap.getPerson());
            auto tmpIndex = ui->peopleList->currentIndex();
            ui->peopleList->removeItem(indexes[index]);
            auto personDescription = QString::fromStdString(person->toString());
            ui->peopleList->insertItem(indexes[index], personDescription);
            ui->peopleList->setCurrentIndex(tmpIndex);
            members->takeItem(index);
            members->insertItem(index, personDescription);
            members->setCurrentRow(index);
        }
    }
}

void DialogAddFamily::on_familyPartnersMembersView_itemDoubleClicked(QListWidgetItem*)
{
    editPerson(ui->familyPartnersMembersView, addedPartnersIndexes);
}

void DialogAddFamily::on_familyKidsMembersView_itemDoubleClicked(QListWidgetItem*)
{
    editPerson(ui->familyKidsMembersView, addedKidsIndexes);
}

void DialogAddFamily::on_editPersonButton_clicked()
{
    editPerson(ui->familyPartnersMembersView, addedPartnersIndexes);
    editPerson(ui->familyKidsMembersView, addedKidsIndexes);
}

void DialogAddFamily::clearFamilyView()
{
    ui->familyKidsMembersView->clear();
    ui->familyPartnersMembersView->clear();
    addedKidsIndexes.clear();
    addedPartnersIndexes.clear();
}

void DialogAddFamily::on_NextFamily_clicked()
{
    if (currentPersonFilterIndex == -1)
    {
        if (currentFamilyIndex + 1 < static_cast<int>(ftf->getFamilies().size()))
        {
            showFamily(currentFamilyIndex + 1, currentPersonIndex);
        }
    }
    else
    {
        for (unsigned i = currentFamilyIndex + 1; i < ftf->familiesSize(); ++i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                showFamily(i, currentPersonIndex);
                break;
            }
        }
    }
}

void DialogAddFamily::on_PrevFamily_clicked()
{
    if (currentPersonFilterIndex == -1)
    {
        if (currentFamilyIndex - 1 >= 0 && currentFamilyIndex - 1 < static_cast<int>(ftf->getFamilies().size()))
        {
            showFamily(currentFamilyIndex - 1, currentPersonIndex);
        }
    }
    else
    {
        for (unsigned i = currentFamilyIndex - 1; i < ftf->familiesSize(); --i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                showFamily(i, currentPersonIndex);
                break;
            }
        }
    }
}

void DialogAddFamily::on_filterByPerson_clicked(bool checked)
{
    if (checked)
    {
        currentPersonFilterIndex = currentPersonIndex;
        for (unsigned i = 0; i < ftf->familiesSize(); ++i)
        {
            if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[currentPersonFilterIndex]))
            {
                showFamily(i, currentPersonIndex);
                break;
            }
        }
    }
    else
    {
        currentPersonFilterIndex = -1;
        showFamily(currentFamilyIndex, currentPersonIndex);
    }
}
