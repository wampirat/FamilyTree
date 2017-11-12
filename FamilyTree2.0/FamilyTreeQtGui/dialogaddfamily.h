#ifndef DIALOGADDFAMILY_H
#define DIALOGADDFAMILY_H

#include <QDialog>
#include <QListWidgetItem>
#include <QCompleter>
#include <vector>
#include <memory>
#include "Family.h"
#include "FamilyTreeFramework.hpp"
#include "dialogaddperson.h"

class SocialPerson;

namespace Ui {
class DialogAddFamily;
}

class DialogAddFamily : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddFamily(QWidget *parent = 0);
    ~DialogAddFamily();
    void setPeople();
    void setFamilyTreeFramework(std::shared_ptr<FamilyTreeFramework>&);
    void showFamily(int, int personIndex);
    Family getFamily();

    void clearFamilyView();

    void resetCompleter();

private slots:
    void on_addPartner_clicked();
    void on_addKid_clicked();
    void on_peopleList_currentIndexChanged(int index);
    void on_buttonBox_accepted();
    void on_deleteFromFamily_clicked();
    void on_familyKidsMembersView_itemSelectionChanged();
    void on_familyPartnersMembersView_itemSelectionChanged();
    void on_addNewPartner_clicked();
    void on_addNewKid_clicked();
    void on_familyPartnersMembersView_itemDoubleClicked(QListWidgetItem *item);
    void on_familyKidsMembersView_itemDoubleClicked(QListWidgetItem *item);
    void on_editPersonButton_clicked();
    void on_NextFamily_clicked();
    void on_PrevFamily_clicked();
    void on_filterByPerson_clicked(bool checked);

private:
    void addPersonToFamily(auto&, auto&);
    void removePersonFromFamily(auto&, auto&);
    void editPerson(auto&, auto&);
    bool showAndExecDialog(auto&&);
    void addNewPerson(auto&&, auto&, auto&);
    void toggleMemberViewButtons();

    std::shared_ptr<FamilyTreeFramework> ftf;
    Ui::DialogAddFamily *ui;
    std::vector<int> addedPartnersIndexes;
    std::vector<int> addedKidsIndexes;
    int currentPersonIndex;
    Family family;
    int currentFamilyIndex = -1;
    void recreateFamily(const auto people, const auto partners);
    int currentPersonFilterIndex = -1;
    QStringList peopleDescriptionStringList;
    QCompleter *completer = nullptr;
};

#endif // DIALOGADDFAMILY_H
