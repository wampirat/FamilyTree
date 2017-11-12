#include "treeview.h"
#include "ui_treeview.h"
#include <iostream>
#include <string>
#include <cmath>
#include <QDesktopWidget>
#include <QPrinter>
#include <QFileDialog>
#include <QMouseEvent>

TreeView::TreeView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeView)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMinMaxButtonsHint;
    setWindowFlags(flags);
    familyTree = new QImage(1000, 1000, QImage::Format_ARGB32);
}

TreeView::~TreeView()
{
    delete familyTree;
    delete ui;
}
QString TreeView::getPersonQString(const std::shared_ptr<SocialPerson>& person)
{
    auto name = person->getName();
    name.append(" ").append(person->getSurname());
    auto birthdate = person->getBirthdate();
    auto deathdate = person->getDeathdate();
    if (birthdate && deathdate)
    {
        name.append("\n").append(std::to_string(birthdate));
        name.append(" - ").append(std::to_string(deathdate));
    }
    else if (birthdate)
    {
        name.append("\n").append(std::to_string(birthdate));
    }
    else if (deathdate)
    {
        name.append("\n†").append(std::to_string(deathdate));
    }
    if (!deathdate) name.append("\nzam: ").append(person->getPlace());
    else name.append("\nmpc: ").append(person->getPlace());
    return QString::fromStdString(name);
}

void TreeView::mousePressEvent(QMouseEvent *event)
{
    mousePressPoint = event->pos();
}

void TreeView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos() == mousePressPoint) return;
    translation += mouseMovePoint - mousePressPoint;
    mouseMovePoint = {0, 0};
    mousePressPoint = {0, 0};
    repaint();
}

void TreeView::mouseMoveEvent(QMouseEvent *event)
{
    mouseMovePoint = event->pos();
    repaint();
}

void TreeView::wheelEvent(QWheelEvent *event)
{
    auto origScale = scale;
    if (event->angleDelta().y() < 0)
    {
        scale -= 0.1;
        scale = std::max(scale, 0.1);
    }
    else if (event->angleDelta().y() > 0)
    {
        scale += 0.1;
    }
    else return;
    QPointF origPos = event->posF() / origScale;
    QPointF newPos = origPos * scale;
    QPointF trans = event->pos() - newPos;
    translation += trans.toPoint();
    rescaleFamilyTreeImage();
}

void TreeView::rotate(int angle)
{
    rotation += angle;
    if (rotation > 360) rotation -= 360;
    if (rotation < 0) rotation += 360;
    repaint();
}

void TreeView::repaintFamilyTree()
{
    QPainter painter(familyTree);
    painter.scale(antialiasingFactor, antialiasingFactor);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillPath(ppaths.getFramePaths(), QBrush(Qt::white));
    painter.drawPath(ppaths.getFramePaths());
    painter.drawPath(ppaths.getFramePaths());
    painter.drawPath(ppaths.getBorderPaths());
    painter.drawPath(ppaths.getBorderPaths());
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawPath(ppaths.getDescriptionPaths());
    rescaleFamilyTreeImage();
}

void TreeView::rescaleFamilyTreeImage()
{
    int i = 0;
    do
    {
        scaledFamilyTree = std::make_unique<QImage>(familyTree->scaled(QSize(scale * familyTree->width() / antialiasingFactor,
                                                                             scale * familyTree->height() / antialiasingFactor),
                                                                       Qt::IgnoreAspectRatio,
                                                                       Qt::SmoothTransformation));
        i++;
    } while (scaledFamilyTree->isNull() && i < 10);
    if (i<10)
    {
        QRect scr = QApplication::desktop()->screenGeometry();
        auto wWidth = scaledFamilyTree->width() / antialiasingFactor;
        auto wHeight = scaledFamilyTree->height() / antialiasingFactor;
        if (wWidth < scr.width() && wHeight < scr.height())
        {
            auto geo = geometry();
            geo.setWidth(wWidth);
            geo.setHeight(wHeight);
            setGeometry(geo);

            move(scr.center() - rect().center());
        }
    }
    repaint();
}

void TreeView::keyPressEvent(QKeyEvent *event)
{
    if (QApplication::keyboardModifiers() && Qt::ControlModifier)
    {
        switch (event->key())
        {
            case Qt::Key_R : rotate(90); break;
            case Qt::Key_L : rotate(-90); break;
            case Qt::Key_Plus :
            case Qt::Key_Equal :
            {
                scale += 0.1;
                rescaleFamilyTreeImage();
                break;
            }
            case Qt::Key_Minus :
            case Qt::Key_Underscore:
            {
                scale -= 0.1;
                scale = std::max(scale, 0.1);
                rescaleFamilyTreeImage();
                break;
            }
            case Qt::Key_0 :
            {
                if (scale > 1.01 || scale < 0.99)
                {
                    scale = 1.0;
                    rescaleFamilyTreeImage();
                }
                break;
            }
        }
    }
}

void TreeView::resetFamilyTreeImage(QRectF rect)
{
    delete familyTree;
    familyTree = new QImage(antialiasingFactor * rect.width(),
                            antialiasingFactor * rect.height(),
                            QImage::Format_ARGB32);
    familyTree->fill(qRgba(0, 0, 0, 0));
}

void TreeView::redrawFamilyTree()
{
    resetFamilyTreeImage(ppaths.boundingRect());
    repaintFamilyTree();
}

void TreeView::setDescriptionText(QString text)
{
    ppaths.setDescriptionText(text);
}

void TreeView::drawHorizontally()
{
    ppaths.setHorizontal();
    ppaths.preparePaths(ftf->getPeople()[probandIndex], ftf);
    redrawFamilyTree();
}

void TreeView::drawVertically()
{
    ppaths.setVertical();
    ppaths.preparePaths(ftf->getPeople()[probandIndex], ftf);
    redrawFamilyTree();
}

void TreeView::drawAncestors(bool a)
{
    ppaths.setAncestors(a);
}

void TreeView::drawPosterity(bool b)
{
    ppaths.setPosterity(b);
}

void TreeView::setFont(QString fontName)
{
    ppaths.setFont(fontName);
}

void TreeView::setDescriptionFont(QString fontName)
{
    ppaths.setDescriptionFont(fontName);
}

void TreeView::setFontSize(int size)
{
    ppaths.setFontSize(size);
}

void TreeView::setDescriptionFontSize(int size)
{
    ppaths.setDescriptionFontSize(size);
}

void TreeView::paintEvent(QPaintEvent*)
{
    qp.begin(this);
    QTransform trans;
    trans.translate(width()/2, height()/2);
    trans.rotate(rotation);
    trans.translate(-width()/2, -height()/2);
    qp.setTransform(trans);
    QPoint p = translation + mouseMovePoint - mousePressPoint;
    if (rotation == 0 || rotation == 360) qp.translate(p.x(), p.y());
    else if (rotation == 90) qp.translate(p.y(), -p.x());
    else if (rotation == 180) qp.translate(-p.x(), -p.y());
    else if (rotation == 270) qp.translate(-p.y(), p.x());

    qp.setRenderHint(QPainter::Antialiasing);
    if (scaledFamilyTree->isNull())
    {
        qp.drawImage(0,
                     0,
                     familyTree->scaled(QSize(familyTree->width() / antialiasingFactor,
                                              familyTree->height() / antialiasingFactor),
                                        Qt::IgnoreAspectRatio,
                                        Qt::SmoothTransformation));
    }
    else
    {
        qp.drawImage(0,0, *scaledFamilyTree);
    }
    qp.end();
}

void TreeView::setFamilyTreeFramework(const std::shared_ptr<FamilyTreeFramework> &_ftf)
{
    ftf = _ftf;
}

void TreeView::setProband(int index)
{
    probandIndex = index;
}
