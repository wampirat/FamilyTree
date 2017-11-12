#pragma once

#include <QDialog>
#include <memory>
#include "FamilyTreeFramework.hpp"
#include <QPainter>

#include "FamilyTreePainterPathsPreparator.hpp"

namespace Ui {
class TreeView;
}

class TreeView : public QDialog
{
    Q_OBJECT

public:
    explicit TreeView(QWidget *parent = 0);
    ~TreeView();
    void setFamilyTreeFramework(const std::shared_ptr<FamilyTreeFramework>&);
    void setProband(int);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void drawHorizontally();
    void drawVertically();
    void drawAncestors(bool a);
    void drawPosterity(bool p);
    void setFont(QString);
    void setDescriptionFont(QString);
    void setFontSize(int);
    void setDescriptionFontSize(int);
    void rotate(int angle);
    void resetFamilyTreeImage(QRectF rect);
    void repaintFamilyTree();
    void redrawFamilyTree();
    void setDescriptionText(QString);
    
    void rescaleFamilyTreeImage();

private:
    QString getPersonQString(const std::shared_ptr<SocialPerson>&);
    std::shared_ptr<FamilyTreeFramework> ftf;
    std::shared_ptr<SocialPerson> proband;
    Ui::TreeView *ui;
    QPainter qp;
    QImage* familyTree;
    std::unique_ptr<QImage> scaledFamilyTree;
    FamilyTreePainterPathsPreparator ppaths;
    QPoint mousePressPoint;
    QPoint mouseMovePoint;
    QPoint mouseReleasePoint;
    QPoint translation;
    double scale = 1;
    double rotation = 0;
    int probandIndex = -1;
    double antialiasingFactor = 1.0;
};
