#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FamilyTreeFramework.hpp"
#include "treeview.h"
#include "FamilyTreePainterPathsPreparator.hpp"
#include <iostream>
#include <QtWidgets>
#include <QtPrintSupport/QPrinter>


void MainWindow::resetCompleter()
{
    if (completer) delete completer;
    completer = new QCompleter(peopleDescriptionStringList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->peopleList->setCompleter(completer);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->peopleList->lineEdit()->setClearButtonEnabled(true);
    ui->descriptionFont->setVisible(false);
    ui->descriptionFontSize->setVisible(false);
    ui->descriptionText->setVisible(false);
    ui->descriptionLabel->setVisible(false);

    resetCompleter();
}

MainWindow::~MainWindow()
{
    if (completer) delete completer;
    delete ui;
}

void MainWindow::setFamilyTreeFramework(std::shared_ptr<FamilyTreeFramework>& _ftf)
{
    ftf = _ftf;
}

bool MainWindow::restorePeopleList()
{
    auto peopleCollection = ftf->getPeople();
    auto tmpIndex = ui->peopleList->currentIndex();
    ui->peopleList->clear();
    peopleDescriptionStringList.clear();
    for (const auto& p : peopleCollection)
    {
        peopleDescriptionStringList << QString::fromStdString((*p)->toString());
    }
    ui->peopleList->addItems(peopleDescriptionStringList);
    if (tmpIndex >= 0 && tmpIndex < ui->peopleList->count()) ui->peopleList->setCurrentIndex(tmpIndex);
    resetCompleter();
    return true;
}

bool MainWindow::showAndExecDialog(auto&& window)
{
    window.show();
    this->setEnabled(false);
    auto retval = (QDialog::Accepted == window.exec());
    this->setEnabled(true);
    return retval;
}

void MainWindow::on_addPerson_clicked()
{
    DialogAddPerson ap;
    if (showAndExecDialog(ap))
    {
        auto person = ftf->createPerson();
        person->setPerson(ap.getPerson());
        restorePeopleList();
    }
}

void MainWindow::on_addFamily_clicked()
{
    DialogAddFamily af;
    af.setFamilyTreeFramework(ftf);
    if (showAndExecDialog(af))
    {
        auto family = ftf->createFamily();
        auto familyAf = af.getFamily();
        for (const auto& partner : familyAf.getPartners())
        {
            family->addPartner(partner);
        }
        for (const auto& kid : familyAf.getKids())
        {
            family->addKid(kid);
        }
        restorePeopleList();
    }
}

void MainWindow::on_printPdf_clicked()
{
    if (probandIndex == -1) return;

    FamilyTreePainterPathsPreparator ppaths;
    auto proband = ftf->getPeople()[probandIndex];
    ppaths.setFont(fontName);
    ppaths.setFontSize(fontSize);
    ppaths.setAncestors(drawAncestors);
    ppaths.setPosterity(drawPosterity);
    ppaths.setDescriptionText(ui->descriptionText->toPlainText());
    ppaths.setDescriptionFont(descriptionFontName);
    ppaths.setDescriptionFontSize(descriptionFontSize);
    if (vertically) ppaths.setVertical();
    else ppaths.setHorizontal();
    ppaths.preparePaths(proband, ftf);

    auto location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0,
                                                    "Export PDF",
                                                    location+QString::fromStdString("/"+proband->getName()+proband->getSurname()),
                                                    "*.pdf");
    if (fileName.isEmpty()) return;
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    double scale = 1.0;
    int marginWidth = 25;
    double inch = 25.4;
    auto width = inch * (20+ppaths.getWidthInPx())/ printer.resolution();
    auto height = inch * ppaths.getHeightInPx() / printer.resolution();
    int pdfLimit = 5080; // PDF SIZE LIMIT 200" (5080mm)
    int a4Limit = 297; // A4 height 11.69" (297mm)
    int maxWidth = pdfLimit;
    if (margin)
    {
        maxWidth -= marginWidth;
    }

    if (height > a4Limit)
    {
        double heightScale = height / a4Limit;
        height = height / heightScale;
        width = width / heightScale;
        scale *= heightScale;
    }

    if (width > maxWidth)
    {
        double widthScale = width / maxWidth;
        width = width / widthScale;
        height = height / widthScale;
        scale *= widthScale;
    }

    if (margin) width += marginWidth;

    printer.setPaperSize(QSizeF(width, height), QPrinter::Millimeter);
    printer.setOutputFileName(fileName);

    QPainter p(&printer);
    p.scale(1/scale, 1/scale);
    if (margin) p.translate(marginWidth*printer.resolution()/inch, 0);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillPath(ppaths.getFramePaths(), QBrush(Qt::white));
    p.drawPath(ppaths.getFramePaths());
    p.drawPath(ppaths.getFramePaths());
    p.fillPath(ppaths.getDescriptionPaths(), QBrush(Qt::black));
    p.drawPath(ppaths.getBorderPaths());
    p.drawPath(ppaths.getBorderPaths());
}

void MainWindow::on_drawTree_clicked()
{
    if (probandIndex == -1) return;
    TreeView tv;
    tv.setFont(fontName);
    tv.setDescriptionFont(descriptionFontName);
    tv.setDescriptionText(ui->descriptionText->toPlainText());
    tv.setFontSize(fontSize);
    tv.setDescriptionFontSize(descriptionFontSize);
    tv.setFamilyTreeFramework(ftf);
    tv.setProband(probandIndex);
    tv.drawAncestors(drawAncestors);
    tv.drawPosterity(drawPosterity);
    this->setEnabled(false);
    if (vertically) tv.drawVertically();
    else tv.drawHorizontally();
    if (showAndExecDialog(tv))
    {};
    this->setEnabled(true);
}

void MainWindow::on_editPerson_clicked()
{
    if (!ftf->getPeople().size()) return;
    auto index = ui->peopleList->currentIndex();
    DialogAddPerson ap;
    ap.setPerson(ftf->getPeople()[index]);
    if (showAndExecDialog(ap))
    {
        ftf->getPeople()[index]->setPerson(ap.getPerson());
    }
    restorePeopleList();
    ui->peopleList->setCurrentIndex(index);
}

void MainWindow::on_deletePerson_clicked()
{
    if (!ftf->getPeople().size()) return;
    auto index = ui->peopleList->currentIndex();
    auto person = ftf->getPeople()[index];

    ftf->removePerson(person);

    restorePeopleList();
    auto newIndex = std::min(index, static_cast<int>(ftf->getPeople().size()) - 1);
    if (newIndex >= 0)  ui->peopleList->setCurrentIndex(newIndex);
}

void MainWindow::on_personsFamilies_clicked()
{
    DialogAddFamily af;
    af.setFamilyTreeFramework(ftf);
    unsigned familyIndex = ftf->getFamilies().size() - 1;
    for (unsigned i = 0; i < ftf->familiesSize(); ++i)
    {
        if (ftf->getFamilies()[i]->isMember(ftf->getPeople()[ui->peopleList->currentIndex()]))
        {
            familyIndex = i;
            break;
        }
    }

    af.showFamily(familyIndex, ui->peopleList->currentIndex());
    if (showAndExecDialog(af))
    {
        restorePeopleList();
    }

}

void MainWindow::xmlWritePartners(QXmlStreamWriter& xmlWriter, const std::vector<std::shared_ptr<SocialPerson>>& partners)
{
    const auto& people = ftf->getPeople();
    xmlWriter.writeStartElement("Partners");
    for (const auto& partner : partners)
    {
        auto i = 0;
        for (const auto& person : people)
        {
            if (*person == *partner)
            {
                xmlWriter.writeTextElement("Partner", QString::number(i));
                break;
            }
            ++i;
        }
    }
    xmlWriter.writeEndElement();
}

void MainWindow::xmlWriteKids(QXmlStreamWriter& xmlWriter, const std::vector<std::shared_ptr<SocialPerson>>& kids)
{
    const auto& people = ftf->getPeople();
    xmlWriter.writeStartElement("Kids");
    for (const auto& kid : kids)
    {
        auto i = 0;
        for (const auto& person : people)
        {
            if (*person == *kid)
            {
                xmlWriter.writeTextElement("Kid", QString::number(i));
                break;
            }
            ++i;
        }
    }
    xmlWriter.writeEndElement();
}

void MainWindow::xmlWriteFamily(QXmlStreamWriter& xmlWriter, const std::shared_ptr<Family>& family)
{
    xmlWriter.writeStartElement("Family");
    xmlWritePartners(xmlWriter, family->getPartners());
    xmlWriteKids(xmlWriter, family->getKids());
    xmlWriter.writeEndElement();
}

void MainWindow::xmlWriteRelations(QXmlStreamWriter& xmlWriter)
{
    auto families = ftf->getFamilies();
    if (families.size())
    {
        xmlWriter.writeStartElement("Families");
        for (const auto& family : families)
        {
            xmlWriteFamily(xmlWriter, family);
        }
        xmlWriter.writeEndElement();
    }
}

void MainWindow::xmlWirtePeople(QXmlStreamWriter& xmlWriter)
{
    auto people = ftf->getPeople();
    if (people.size())
    {
        xmlWriter.writeStartElement("People");
        for (const auto& person : people)
        {
            xmlWriter.writeStartElement("Person");
            xmlWriter.writeTextElement("Name", QString::fromStdString(person->getName()));
            xmlWriter.writeTextElement("Surname", QString::fromStdString(person->getSurname()));
            xmlWriter.writeTextElement("Birthdate", QString::number(person->getBirthdate()));
            xmlWriter.writeTextElement("Deathdate", QString::number(person->getDeathdate()));
            xmlWriter.writeTextElement("Place", QString::fromStdString(person->getPlace()));
            xmlWriter.writeTextElement("IsAlive", QString::number(person->getIsAlive()));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }
}

void MainWindow::saveToXmlFile()
{
    auto location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = QFileDialog::getSaveFileName((QWidget* )0, "Export Xml", location, "*.xml");
    if (filename.isEmpty()) return;
    if (QFileInfo(filename).suffix().isEmpty()) { filename.append(".xml"); }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("FamilyTree");
    xmlWirtePeople(xmlWriter);
    xmlWriteRelations(xmlWriter);
    xmlWriter.writeEndElement();

    file.close();
}

void MainWindow::on_saveButton_clicked()
{
    saveToXmlFile();
}
void MainWindow::xmlReadFamilyTree(QXmlStreamReader& xmlReader)
{
    if (xmlReader.readNextStartElement() && xmlReader.name() == "People")
    {
        xmlReadPeople(xmlReader);
    }

    if (xmlReader.readNextStartElement() && xmlReader.name() == "Families")
    {
        xmlReadRelations(xmlReader);
    }
}

void MainWindow::xmlReadRelations(QXmlStreamReader& xmlReader)
{
    while (xmlReader.readNextStartElement() && xmlReader.name() == "Family")
    {
        auto family = ftf->createFamily();
        xmlReader.readNextStartElement();
        if (xmlReader.name() == "Partners")
        {
            while (xmlReader.readNextStartElement() && xmlReader.name() == "Partner")
            {
                auto index = xmlReader.readElementText().toInt();
                family->addPartner(ftf->getPeople()[index]);
            }
            xmlReader.readNextStartElement();
        }
        if (xmlReader.name() == "Kids")
        {
            while (xmlReader.readNextStartElement() && xmlReader.name() == "Kid")
            {
                auto index = xmlReader.readElementText().toInt();
                family->addKid(ftf->getPeople()[index]);
            }
            xmlReader.readNextStartElement();
        }
    }
}

void MainWindow::xmlReadPeople(QXmlStreamReader& xmlReader)
{
    while (xmlReader.readNextStartElement() && xmlReader.name() == "Person")
    {
        xmlReader.readNextStartElement();
        auto name = xmlReader.readElementText().toStdString();
        xmlReader.readNextStartElement();
        auto surname = xmlReader.readElementText().toStdString();
        xmlReader.readNextStartElement();
        auto birthdate = xmlReader.readElementText().toInt();
        xmlReader.readNextStartElement();
        auto deathdate = xmlReader.readElementText().toInt();
        xmlReader.readNextStartElement();
        auto place = xmlReader.readElementText().toStdString();
        xmlReader.readNextStartElement();
        auto isAlive = xmlReader.readElementText().toInt();
        auto person = ftf->createPerson();
        Person p;
        p.setName(name);
        p.setSurname(surname);
        p.setBirthdate(birthdate);
        p.setDeathdate(deathdate);
        p.setPlace(place);
        p.setIsAlive(isAlive);
        person->setPerson(p);
        xmlReader.readNextStartElement();
    }
}

void MainWindow::loadFromXmlFile()
{
    auto location = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import Xml"), location, tr("Xml files (*.xml)"));
    if (filename.isEmpty()) return;
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;

    }
    ftf = std::make_shared<FamilyTreeFramework>();
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

    if (xmlReader.readNextStartElement())
    {
        if (xmlReader.name() == "FamilyTree")
        {
            xmlReadFamilyTree(xmlReader);
        }
    }

    file.close();

    if (xmlReader.hasError())
    {
       std::cerr << "Error: Failed to parse file "
                 << qPrintable(filename) << ": "
                 << qPrintable(xmlReader.errorString()) << std::endl;
    }
    else if (file.error() != QFile::NoError)
    {
        std::cerr << "Error: Cannot read file " << qPrintable(filename)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
    }
    restorePeopleList();
}

void MainWindow::on_loadButton_clicked()
{
    loadFromXmlFile();
}

void MainWindow::on_peopleList_currentIndexChanged(int index)
{
    probandIndex = (ftf->getPeople().size()) ? index : -1;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (QApplication::keyboardModifiers() && Qt::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_S : saveToXmlFile(); break;
            case Qt::Key_O : loadFromXmlFile(); break;
        }
    }
}

void MainWindow::on_drawVertically_clicked()
{
    vertically = true;
}

void MainWindow::on_drawHorizontally_clicked()
{
    vertically = false;
}

void MainWindow::on_ancestorsCheckBox_clicked(bool checked)
{
    drawAncestors = checked;
}

void MainWindow::on_posterityCheckBox_clicked(bool checked)
{
    drawPosterity = checked;
}

void MainWindow::on_fontComboBox_currentIndexChanged(const QString& name)
{
    fontName = name;
}

void MainWindow::on_fontSize_valueChanged(int size)
{
    fontSize = size;
}

void MainWindow::on_descriptionFont_currentIndexChanged(const QString& name)
{
    descriptionFontName = name;
}

void MainWindow::on_descriptionFontSize_valueChanged(int size)
{
    descriptionFontSize = size;
}

void MainWindow::on_marginCheckBox_clicked(bool checked)
{
    margin = checked;
}

void MainWindow::on_descriptionCheckBox_clicked(bool checked)
{
    auto height = (checked)?extendedWindowHeight:baseWindowHeight;
    ui->descriptionFont->setVisible(checked);
    ui->descriptionFontSize->setVisible(checked);
    ui->descriptionText->setVisible(checked);
    ui->descriptionLabel->setVisible(checked);
    auto geo = geometry();
    geo.setHeight(height);
    setGeometry(geo);
    if (!checked) ui->descriptionText->clear();
}
