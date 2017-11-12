#pragma once

#include <memory>
#include "SocialPerson.hpp"
#include "FamilyTreeFramework.hpp"

#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>

class FamilyTreePainterPathsPreparator
{
public:
    FamilyTreePainterPathsPreparator();
    void preparePaths(const std::shared_ptr<SocialPerson>& proband, std::shared_ptr<FamilyTreeFramework>&);
    QPainterPath getFramePaths();
    QPainterPath getDescriptionPaths();
    QPainterPath getBorderPaths();
    void setHorizontal();
    void setVertical();
    void setAncestors(bool);
    void setPosterity(bool);
    void setDescriptionText(QString);
    int getWidthInPx();
    int getHeightInPx();
    void setFont(QString);
    void setDescriptionFont(QString);
    void setFontSize(int);
    void setDescriptionFontSize(int);
    void addBorder();
    QRect boundingRect();

    std::vector<QRect> addKids(const std::shared_ptr<SocialPerson> &person, const std::shared_ptr<SocialPerson> &partner, int &y, int x);

    int getFrameTextLength(const std::shared_ptr<SocialPerson>& person);
    
private:
    QRect preparePosterityTree(const std::shared_ptr<SocialPerson> &, int x = 0, int y = 0);
    QRect prepareAncestorsTree(const std::shared_ptr<SocialPerson> &, int x = 0, int y = 0, int parentNumber = 0);
    std::vector<QRect> addSiblings(int x, auto width, int y, const std::shared_ptr<SocialPerson>& person, int shift);
    QRect addPerson(int x, int y, auto width, const std::shared_ptr<SocialPerson>& person);
    std::vector<QRect> addParents(const std::shared_ptr<SocialPerson>& person, int x, int y);
    QRect addPartner(int x, int y, const std::shared_ptr<SocialPerson>& person, int width, int pIndex = 0);
    std::vector<QRect> addKids(const std::shared_ptr<SocialPerson>& person, int y, int x);
    void addConnectionLines(const std::vector<QRect> &parentsRects, const std::vector<QRect> &kidsRects);
    void addLine(QLine ul);
    void addText(auto person, int x, int y);
    void addVerticalText(auto partner, int x, int y);
    void addHorizontalText(auto person, int x, int y);
    QPainterPath addPersonDescription(auto partner, int x, int y);
    void calcFrameSize();
    void calcAncStringPixSize(const std::shared_ptr<SocialPerson>&);
    void calcPostStringPixSize(const std::shared_ptr<SocialPerson>&);
    int getLongestStringPixSize(auto people);
    void addDescriptionTextFrame();
    QPainterPath framePaths, descriptionPaths, borderPaths;
    int frameWidth = 0;
    int frameHeight = 0;
    int horizontalGap = 10;
    int verticalGap = 20;
    int frameHorizontalDistance = horizontalGap + frameWidth;
    int frameVerticalDistance = verticalGap + frameHeight;
    std::vector<int> framePosteritySizeVec;
    std::vector<int> frameAncestorsSizeVec;
    int div = 16;
    int mul1 = 7;
    int mul2 = 9;
    int x0;
    int y0;
    QRect probandsRect;
    bool shouldRelocateDrawing = false;
    QFont font;
    QFont descriptionFont;
    bool horizontal;
    bool ancestors;
    bool posterity;
    std::shared_ptr<FamilyTreeFramework> ftf;
    std::shared_ptr<SocialPerson> proband;
    std::vector<int> ancSizeVec;
    std::vector<int> postSizeVec;
    QString descriptionText;
};
