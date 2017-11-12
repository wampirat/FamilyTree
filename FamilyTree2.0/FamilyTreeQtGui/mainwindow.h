#pragma once

#include <QMainWindow>
#include <QXmlStreamWriter>
#include <QCompleter>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <memory>
#include "dialogaddfamily.h"
#include "dialogaddperson.h"

class FamilyTreeFramework;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setFamilyTreeFramework(std::shared_ptr<FamilyTreeFramework>&);
    bool showAndExecDialog(auto&&);

    void saveToXmlFile();

    void loadFromXmlFile();

    void resetCompleter();

private slots:
    void on_addPerson_clicked();
    void on_addFamily_clicked();
    void on_drawTree_clicked();
    void on_printPdf_clicked();
    void on_editPerson_clicked();
    void on_deletePerson_clicked();
    void on_personsFamilies_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_peopleList_currentIndexChanged(int index);
    void keyPressEvent(QKeyEvent *event);
    void on_drawVertically_clicked();
    void on_drawHorizontally_clicked();
    void on_ancestorsCheckBox_clicked(bool checked);
    void on_posterityCheckBox_clicked(bool checked);
    void on_fontComboBox_currentIndexChanged(const QString &arg1);
    void on_fontSize_valueChanged(int arg1);
    void on_descriptionFont_currentIndexChanged(const QString &arg1);
    void on_descriptionFontSize_valueChanged(int arg1);
    void on_marginCheckBox_clicked(bool checked);

    void on_descriptionCheckBox_clicked(bool checked);

private:
    void xmlWirtePeople(QXmlStreamWriter&);
    void xmlWriteRelations(QXmlStreamWriter&);
    void xmlWriteFamily(QXmlStreamWriter&, const std::shared_ptr<Family>&);
    void xmlWritePartners(QXmlStreamWriter&, const std::vector<std::shared_ptr<SocialPerson>>&);
    void xmlWriteKids(QXmlStreamWriter&, const std::vector<std::shared_ptr<SocialPerson>>&);
    void xmlReadPeople(QXmlStreamReader&);
    void xmlReadRelations(QXmlStreamReader&);
    void xmlReadFamilyTree(QXmlStreamReader&);
    bool restorePeopleList();
    Ui::MainWindow *ui;
    std::shared_ptr<FamilyTreeFramework> ftf;
    QString fontName = "Monotype Corsiva";
    QString descriptionFontName = "Monotype Corsiva";
    int fontSize = 14;
    int descriptionFontSize = 72;
    int probandIndex = -1;
    bool vertically = true;
    bool drawAncestors = false;
    bool drawPosterity = true;
    QStringList peopleDescriptionStringList;
    QCompleter *completer = nullptr;
    bool margin = false;
    int baseWindowHeight = 180;
    int extendedWindowHeight = 310;
};
