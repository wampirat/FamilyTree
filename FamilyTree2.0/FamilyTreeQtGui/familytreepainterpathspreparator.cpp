#include "FamilyTreePainterPathsPreparator.hpp"

void FamilyTreePainterPathsPreparator::setHorizontal()
{
    horizontal = true;
}

void FamilyTreePainterPathsPreparator::setVertical()
{
    horizontal = false;
}

void FamilyTreePainterPathsPreparator::setAncestors(bool a)
{
    ancestors = a;
}

void FamilyTreePainterPathsPreparator::setPosterity(bool p)
{
    posterity = p;
}

void FamilyTreePainterPathsPreparator::setDescriptionText(QString text)
{
    descriptionText = text;
}

std::vector<QRect> FamilyTreePainterPathsPreparator::addSiblings(int x, auto width, int y, const std::shared_ptr<SocialPerson>& person, int shift)
{
    std::vector<QRect> retval;
    int rectY = x*frameVerticalDistance+x0;
    auto rectHeight = frameHeight;
    for (const auto& sibling : person->getSiblings())
    {
        if (!horizontal)
        {
            rectHeight = (person->getPosterityDepth()>=person->getAncestorsDepth())?framePosteritySizeVec[person->getPosterityDepth()]:frameAncestorsSizeVec[person->getAncestorsDepth()];
            rectY = x0;
            if (x < 0)
            {
                for (int i = 1; i <= std::abs(x); ++i)
                {
                    rectY -= frameAncestorsSizeVec[i] + verticalGap;
                }
            }
            else
            {
                for (int i = 0; i < x; ++i)
                {
                    rectY += framePosteritySizeVec[i] + verticalGap;
                }
            }
        }
        auto rect = QRect((y + width + shift++)*frameHorizontalDistance+y0,
                          rectY,
                          frameWidth,
                          rectHeight);
        retval.push_back(rect);
        framePaths.addRect(rect);
        addText(sibling, rect.x(), rect.y());
    }
    return retval;
}

QRect FamilyTreePainterPathsPreparator::addPerson(int x, int y, auto width, const std::shared_ptr<SocialPerson>& person)
{
    QRect rect;
    if (shouldRelocateDrawing)
    {
        if (ancestors)
        {
            if (!horizontal)
            {
                int offset = 0;
                if (x < 0)
                {
                    for (int i = 1; i <= std::abs(x); ++i)
                    {
                        offset -= frameAncestorsSizeVec[i] + verticalGap;
                    }
                }
                else
                {
                    for (int i = 0; i < x; ++i)
                    {
                        offset += framePosteritySizeVec[i] + verticalGap;
                    }
                }
                x0 = probandsRect.y() - offset;
                y0 = probandsRect.x() - (y+width)*frameHorizontalDistance;
            }
            else
            {
                x0 = probandsRect.y() - x*frameVerticalDistance;
                y0 = probandsRect.x() - (y+width)*frameHorizontalDistance;
            }
        }
        else
        {
            x0 = verticalGap;
            y0 = horizontalGap;
        }
    }

    auto rectWidth = frameWidth;
    auto rectHeight = frameHeight;

    int rectY = 0;
    int rectX = 0;
    if (!horizontal)
    {
        rectY = (y+width)*frameHorizontalDistance+y0;
        auto totalXDistance = 0;
        if (x < 0)
        {
            for (int i = 1; i <= std::abs(x); i++)
            {
                totalXDistance -= frameAncestorsSizeVec[i] + verticalGap;
            }
        }
        else
        {
            for (int i = 0; i < x; i++)
            {
                totalXDistance += framePosteritySizeVec[i] + verticalGap;
            }
        }
        rectX = totalXDistance + x0;
        if (person->getAncestorsDepth() == person->getPosterityDepth())
        {
            rectHeight = std::max(framePosteritySizeVec[person->getPosterityDepth()], frameAncestorsSizeVec[person->getAncestorsDepth()]);
        }
        else
        {
            rectHeight = (person->getPosterityDepth()>person->getAncestorsDepth())?framePosteritySizeVec[person->getPosterityDepth()]:frameAncestorsSizeVec[person->getAncestorsDepth()];
        }
    }
    else
    {
        rectY = (y+width)*frameHorizontalDistance+y0;
        rectX = x*frameVerticalDistance+x0;
        rectHeight = frameHeight;
        rectWidth = frameWidth;
    }
    rect = QRect(rectY, rectX, rectWidth, rectHeight);
    if (shouldRelocateDrawing)
    {
        shouldRelocateDrawing = false;
        if (person == proband && ancestors) return rect;
    }
    framePaths.addRect(rect);

    addText(person, rect.x(), rect.y());
    return rect;
}

std::vector<QRect> FamilyTreePainterPathsPreparator::addKids(const std::shared_ptr<SocialPerson>& person, const std::shared_ptr<SocialPerson>& partner, int& y, int x)
{
    std::vector<QRect> kidsRects;
    for (const auto& kid : person->getKids())
    {
        for (const auto& pKid : partner->getKids())
        {
            if (kid == pKid)
            {
                kidsRects.push_back(preparePosterityTree(kid, x, y));
                y += kid->getPosterityWidth();
            }
        }
    }
    return kidsRects;
}

std::vector<QRect> FamilyTreePainterPathsPreparator::addKids(const std::shared_ptr<SocialPerson>& person, int y, int x)
{
    std::vector<QRect> kidsRects;
    for (const auto& kid : person->getKids())
    {
        kidsRects.push_back(preparePosterityTree(kid, x, y));
        y += kid->getPosterityWidth();
    }
    return kidsRects;
}

std::vector<QRect> FamilyTreePainterPathsPreparator::addParents(const std::shared_ptr<SocialPerson>& person, int x, int y)
{
    std::vector<QRect> parentsRects;
    int parentNo = 0;
    for (const auto& parent : person->getParents())
    {
        parentsRects.push_back(prepareAncestorsTree(parent, x, y, parentNo++));
        y += parent->getAncestorsWidth();
    }
    return parentsRects;
}

QPainterPath FamilyTreePainterPathsPreparator::addPersonDescription(auto partner, int x, int y)
{
    QPainterPath text;
    QFontMetrics qfm(font);

    const auto bdate = partner->getBirthdate();
    const auto ddate = partner->getDeathdate();

    const auto name = QString::fromStdString(partner->getName()+" "+partner->getSurname());
    std::string date;
    if (bdate && ddate)
    {
        date = "*"+ std::to_string(bdate) + " †" + std::to_string(ddate);
    }
    else if (bdate && !partner->getIsAlive())
    {
        date = "*" + std::to_string(bdate) + " - zmarły";
    }
    else if (bdate)
    {
        date = "*" + std::to_string(bdate);
    }
    else if (ddate)
    {
        date = "†" + std::to_string(ddate);
    }
    else if (!partner->getIsAlive())
    {
        date = "zmarły";
    }
    const auto place = QString::fromStdString(partner->getPlace());

    text.addText(x, y, font, name);
    text.addText(x, y + qfm.height(), font, QString::fromStdString(date));
    text.addText(x, y + 2 * qfm.height(), font, place);

    return text;
}

void FamilyTreePainterPathsPreparator::addText(auto person, int x, int y)
{
    if (!horizontal)
    {
        addVerticalText(person, x, y);
    }
    else
    {
        addHorizontalText(person, x, y);
    }
}

void FamilyTreePainterPathsPreparator::addHorizontalText(auto person, int x, int y)
{
    QFontMetrics qfm(font);
    QPainterPath text = addPersonDescription(person, x, y);

    text.translate(qfm.averageCharWidth(), qfm.height());

    descriptionPaths.addPath(text);
}

void FamilyTreePainterPathsPreparator::addVerticalText(auto partner, int x, int y)
{
    QMatrix mat;
    mat.rotate(270);

    QFontMetrics qfm(font);

    const auto textX = -y;
    const auto textY = x;

    QPainterPath text = addPersonDescription(partner, textX, textY);

    text = mat.map(text);
    auto rectHeight = (partner->getPosterityDepth()>=partner->getAncestorsDepth())?framePosteritySizeVec[partner->getPosterityDepth()]:frameAncestorsSizeVec[partner->getAncestorsDepth()];
    if (partner->getAncestorsDepth() == partner->getPosterityDepth())
    {
        rectHeight = std::max(framePosteritySizeVec[partner->getPosterityDepth()], frameAncestorsSizeVec[partner->getAncestorsDepth()]);
    }

    text.translate(qfm.height(), rectHeight - qfm.averageCharWidth());

    descriptionPaths.addPath(text);
}

QRect FamilyTreePainterPathsPreparator::addPartner(int x, int y, const std::shared_ptr<SocialPerson>& person, int width, int pIndex)
{
    auto partner = person->getPartners()[pIndex];
    auto rectHeight = frameHeight;
    auto rectWidth = frameWidth;
    auto rectY = (y + width + 1)*frameHorizontalDistance+y0;
    auto rectX = x*frameVerticalDistance+x0;

    if (!horizontal)
    {
        if (person->getAncestorsDepth() == person->getPosterityDepth())
        {
            rectHeight = std::max(framePosteritySizeVec[person->getPosterityDepth()], frameAncestorsSizeVec[person->getAncestorsDepth()]);
        }

        rectHeight = (person->getPosterityDepth()>=person->getAncestorsDepth())?framePosteritySizeVec[person->getPosterityDepth()]:frameAncestorsSizeVec[person->getAncestorsDepth()];
        auto totalXDistance = 0;
        if (x<0)
        {
            for (int i = 1; i <= std::abs(x); i++)
            {
                totalXDistance -= frameAncestorsSizeVec[i] + verticalGap;
            }
        }
        else
        {
            for (int i = 0; i < x; i++)
            {
                totalXDistance += framePosteritySizeVec[i] + verticalGap;
            }
        }
        rectX = totalXDistance + x0;
    }

    auto rect = QRect(rectY,
                      rectX,
                      rectWidth,
                      rectHeight);
    framePaths.addRect(rect);
    addText(partner, rect.x(), rect.y());
    return rect;
}

void FamilyTreePainterPathsPreparator::addLine(QLine ul)
{
    framePaths.moveTo(ul.p1());
    framePaths.lineTo(ul.p2());
}

void FamilyTreePainterPathsPreparator::addConnectionLines(const std::vector<QRect>& parentsRects, const std::vector<QRect>& kidsRects)
{
    if (!parentsRects.size()) return;
    int kidsConnectionX = 0;
    int kidsConnectionY = 0;

    for (unsigned i = 1; i < parentsRects.size(); ++i)
    {
        const QRect p1 = parentsRects[i - 1];
        const QRect p2 = parentsRects[i];
        QLine ul(p1.x()+p1.width(),
                 p1.y()+p1.height()*mul1/div,
                 p2.x(),
                 p2.y()+p2.height()*mul1/div);
        QLine dl(p1.x()+p1.width(),
                 p1.y()+p1.height()*mul2/div,
                 p2.x(),
                 p2.y()+p2.height()*mul2/div);
        kidsConnectionX = (p1.x()+p1.width()+p2.x())/2;
        kidsConnectionY = p1.y()+p1.height()+verticalGap/2;
        addLine(ul);
        addLine(dl);
        if (kidsRects.size())
        {
            QLine vl((p1.x()+p1.width()+p2.x())/2,
                     p1.y()+p1.height()*mul2/div,
                     kidsConnectionX,
                     kidsConnectionY);
            addLine(vl);
        }
    }
    if (parentsRects.size() == 1 && kidsRects.size())
    {
        auto p1 = parentsRects[0];
        kidsConnectionX = p1.x()+p1.width()/2;
        kidsConnectionY = p1.y()+p1.height()+verticalGap/2;
        QLine vl(kidsConnectionX,
                 p1.y()+p1.height(),
                 kidsConnectionX,
                 kidsConnectionY);
        addLine(vl);
    }
    for (const auto& r : kidsRects)
    {
        QLine line(r.x() + r.width()/2,
                   r.y() - verticalGap/2,
                   r.x() + r.width()/2,
                   r.y());
        QLine connectionLine(r.x() + r.width()/2,
                             r.y() - verticalGap/2,
                             kidsConnectionX,
                             kidsConnectionY);
        addLine(line);
        addLine(connectionLine);
    }
}

QRect FamilyTreePainterPathsPreparator::prepareAncestorsTree(const std::shared_ptr<SocialPerson>& person, int x, int y, int parentNumber)
{
    auto width = (person->getAncestorsWidth() - 1)/(person->getSiblings().size()+1);
    QRect rect;
    std::vector<QRect> kidsRects;
    std::vector<QRect> parentsRects;

    if (parentNumber)
    {
        rect = addPerson(x, y, width, person);
        kidsRects = addSiblings(x, width, y, person, 1);
    }
    else
    {
        int shift = 0;
        if (posterity && person == proband && person->getPartners().size() > 1) shift = -1;
        kidsRects = addSiblings(x, width, y, person, shift);
        rect = addPerson(x, y + person->getSiblings().size(), width, person);
    }

    kidsRects.push_back(rect);
    parentsRects = addParents(person, x - 1, y);
    addConnectionLines(parentsRects, kidsRects);
    return rect;
}

QRect FamilyTreePainterPathsPreparator::preparePosterityTree(const std::shared_ptr<SocialPerson>& person, int x, int y)
{
    auto width = (person->getPosterityWidth() - 1)/2;
    QRect rect;
    if (person->getPartners().size() < 2)
    {
        std::vector<QRect> partnersRects;
        rect = addPerson(x, y, width, person);
        partnersRects.push_back(rect);
        if (person->getPartners().size()) partnersRects.push_back(addPartner(x, y, person, width));
        std::vector<QRect> rects = addKids(person, y, x + 1);
        addConnectionLines(partnersRects, rects);
    }
    if (person->getPartners().size() == 2)
    {
        std::vector<QRect> partnersRects1;
        std::vector<QRect> partnersRects2;
        rect = addPerson(x, y, width, person);
        partnersRects1.push_back(addPartner(x, y - 2, person, width));
        partnersRects1.push_back(rect);
        partnersRects2.push_back(rect);
        partnersRects2.push_back(addPartner(x, y, person, width, 1));
        auto partner1 = person->getPartners()[0];
        auto partner2 = person->getPartners()[1];
        std::vector<QRect> kids1 = addKids(person, partner1, y, x + 1);
        std::vector<QRect> kids2 = addKids(person, partner2, y, x + 1);

        addConnectionLines(partnersRects1, kids1);
        addConnectionLines(partnersRects2, kids2);
    }
    return rect;
}

int FamilyTreePainterPathsPreparator::getLongestStringPixSize(auto people)
{
    int length = 0;
    for (const auto& person : people)
    {
        length = std::max(length, getFrameTextLength(person));
    }
    return length;
}

QPainterPath FamilyTreePainterPathsPreparator::getFramePaths()
{
    return framePaths;
}

QPainterPath FamilyTreePainterPathsPreparator::getDescriptionPaths()
{
    return descriptionPaths;
}

QPainterPath FamilyTreePainterPathsPreparator::getBorderPaths()
{
    return borderPaths;
}

int FamilyTreePainterPathsPreparator::getWidthInPx()
{
    auto frameCellsWidth = std::max(ancestors?proband->getAncestorsWidth():0,
                                    posterity?(y0-horizontalGap)/frameHorizontalDistance + proband->getPosterityWidth():0);
    return frameHorizontalDistance * frameCellsWidth + 2 * horizontalGap;
}

int FamilyTreePainterPathsPreparator::getHeightInPx()
{
    if (!horizontal)
    {
        auto totalFrameDistance = 0;
        if (ancestors) for (const auto& i : frameAncestorsSizeVec)
        {
            totalFrameDistance += i + verticalGap;
        }
        if (posterity) for (const auto& i : framePosteritySizeVec)
        {
            totalFrameDistance += i + verticalGap;
        }
        if (ancestors && posterity) totalFrameDistance -= std::min(frameAncestorsSizeVec[0], framePosteritySizeVec[0]) + verticalGap;
        return totalFrameDistance + 2 * verticalGap;
    }
    else
    {
        auto frameCellsHeight = (ancestors?proband->calcAncestorsDepth():0) + (posterity?proband->calcPosterityDepth():0) + 1;
        return frameVerticalDistance * frameCellsHeight + 2 * verticalGap;
    }
}

void FamilyTreePainterPathsPreparator::setFont(QString fontName)
{
    font.setFamily(fontName);
}

void FamilyTreePainterPathsPreparator::setDescriptionFont(QString fontName)
{
    descriptionFont.setFamily(fontName);
}

void FamilyTreePainterPathsPreparator::setFontSize(int size)
{
    font.setPixelSize(size);
}

void FamilyTreePainterPathsPreparator::setDescriptionFontSize(int size)
{
    descriptionFont.setPixelSize(size);
}

void FamilyTreePainterPathsPreparator::calcFrameSize()
{
    QFontMetrics qfm(font);
    int ancestorsDepth = proband->calcAncestorsDepth();
    int posterityDepth = proband->calcPosterityDepth();
    frameAncestorsSizeVec.clear();
    framePosteritySizeVec.clear();
    frameAncestorsSizeVec.resize(ancestorsDepth + 1, 0);
    framePosteritySizeVec.resize(posterityDepth + 1, 0);

    if (ancestors) calcAncStringPixSize(proband);
    if (posterity) calcPostStringPixSize(proband);
    frameAncestorsSizeVec[0] = std::max(frameAncestorsSizeVec[0], framePosteritySizeVec[0]);
    framePosteritySizeVec[0] = frameAncestorsSizeVec[0];

    for (auto& frameSize : frameAncestorsSizeVec)
    {
        frameSize += 2 * qfm.averageCharWidth();
    }

    for (auto& frameSize : framePosteritySizeVec)
    {
        frameSize += 2 * qfm.averageCharWidth();
    }

    if (!horizontal)
    {
        frameHeight  = 0;
        for (const auto& fs : frameAncestorsSizeVec)
        {
            frameHeight = std::max(frameHeight, fs);
        }

        for (const auto& fs : framePosteritySizeVec)
        {
            frameHeight = std::max(frameHeight, fs);
        }
        frameHeight += 2 * qfm.averageCharWidth();
        frameVerticalDistance = frameHeight + verticalGap;
        frameWidth = 3.5 * qfm.height();
        frameHorizontalDistance = frameWidth + horizontalGap;
    }
    else
    {
        frameWidth = 0;
        for (const auto& fs : frameAncestorsSizeVec)
        {
            frameWidth = std::max(frameWidth, fs);
        }

        for (const auto& fs : framePosteritySizeVec)
        {
            frameWidth = std::max(frameWidth, fs);
        }

        frameWidth += 2 * qfm.averageCharWidth();
        frameHorizontalDistance = frameWidth + horizontalGap;
        frameHeight = 3.5 * qfm.height();
        frameVerticalDistance = frameHeight + verticalGap;
    }
}

int FamilyTreePainterPathsPreparator::getFrameTextLength(const std::shared_ptr<SocialPerson>& person)
{
    QFontMetrics qfm(font);
    return std::max(qfm.size(Qt::TextSingleLine,
                             QString::fromStdString(person->getName() + " " + person->getSurname())).width(),
                    qfm.size(Qt::TextSingleLine,
                             QString::fromStdString(person->getPlace())).width());
}

void FamilyTreePainterPathsPreparator::calcAncStringPixSize(const std::shared_ptr<SocialPerson>& person)
{
    int length = getFrameTextLength(person);

    for (const auto& parent : person->getParents())
    {
        calcAncStringPixSize(parent);
    }
    for (const auto& sibling : person->getSiblings())
    {
        length = std::max(length, getFrameTextLength(sibling));
    }
    if (frameAncestorsSizeVec[person->getAncestorsDepth()] < length)
    {
        frameAncestorsSizeVec[person->getAncestorsDepth()] = length;
    }
}

void FamilyTreePainterPathsPreparator::calcPostStringPixSize(const std::shared_ptr<SocialPerson>& person)
{
    int length = getFrameTextLength(person);
    for (const auto& partner : person->getPartners())
    {
        length = std::max(length, getFrameTextLength(partner));
        for (const auto& kid : partner->getKids())
        {
            calcPostStringPixSize(kid);
        }
    }
    if (framePosteritySizeVec[person->getPosterityDepth()] < length)
    {
        framePosteritySizeVec[person->getPosterityDepth()] = length;
    }
    for (const auto& kid : person->getKids())
    {
        calcPostStringPixSize(kid);
    }
}

FamilyTreePainterPathsPreparator::FamilyTreePainterPathsPreparator()
{
    font = QFont("Monotype Corsiva", 14);
}

void FamilyTreePainterPathsPreparator::addDescriptionTextFrame()
{
    if (descriptionText.size())
    {
        QFontMetrics qfm(descriptionFont);
        auto descriptionLines = descriptionText.split('\n');
        auto linesNo = descriptionLines.size();
        auto textHeight = qfm.tightBoundingRect(descriptionText).height();
        auto textWidth = 0;
        for (const auto& line : descriptionLines)
        {
            textWidth = std::max(textWidth, qfm.width(line));
        }
        auto rectWidth = textWidth + 2*qfm.averageCharWidth();
        auto rect = QRect(getWidthInPx()-1.5*horizontalGap-verticalGap/2-rectWidth,
                     verticalGap,
                     rectWidth,
                     linesNo * textHeight+2*qfm.descent());
        framePaths.addRect(rect);

        for (int i = 0; i < linesNo; ++i)
        {
            descriptionPaths.addText(rect.x() + qfm.averageCharWidth(),
                                     rect.y() + qfm.descent() + (1+i)*textHeight,
                                     descriptionFont,
                                     descriptionLines[i]);
        }
    }
}

void FamilyTreePainterPathsPreparator::addBorder()
{
    auto rect = QRect(horizontalGap/2,
                      verticalGap/2,
                      getWidthInPx()-2*horizontalGap,
                      getHeightInPx()-2*verticalGap);
    borderPaths.addRect(rect);
}

QRect FamilyTreePainterPathsPreparator::boundingRect()
{
    QRect boundingRect(0,
                       0,
                       static_cast<int>(borderPaths.boundingRect().width() + horizontalGap),
                       static_cast<int>(borderPaths.boundingRect().height()+ verticalGap));
    return boundingRect;
}

void FamilyTreePainterPathsPreparator::preparePaths(const std::shared_ptr<SocialPerson>& proband, std::shared_ptr<FamilyTreeFramework>& _ftf)
{
    framePaths = QPainterPath();
    descriptionPaths = QPainterPath();
    borderPaths = QPainterPath();
    this->proband = proband;
    auto ancestorsDepth = proband->calcAncestorsDepth();
    ftf = _ftf;
    calcFrameSize();

    if (ancestors)
    {
        if (!horizontal)
        {
            x0 = verticalGap;
            for (int i = 1; i <= ancestorsDepth; ++i)
            {
                x0 += frameAncestorsSizeVec[i] + verticalGap;
            }
        }
        else
        {
            x0 = ancestorsDepth * frameVerticalDistance + verticalGap;
        }
    }
    else x0 = verticalGap;
    y0 = 0;
    proband->calcAncestorsWidth();
    if (proband->getPartners().size() > 1) proband->setAncestorsWidth(proband->getAncestorsWidth()+1);
    unsigned postY = (posterity) ? (proband->calcPosterityWidth() - 1)/2*frameHorizontalDistance : 0;
    unsigned ancY = (ancestors) ? (proband->getSiblings().size()+(proband->getAncestorsWidth() - 1)/(proband->getSiblings().size()+1))*frameHorizontalDistance : 0;

    if (postY > ancY) y0 = postY - ancY;

    y0 += horizontalGap;

    if (ancestors) probandsRect = prepareAncestorsTree(proband);
    if (posterity)
    {
        shouldRelocateDrawing = true;
        probandsRect = preparePosterityTree(proband);
        shouldRelocateDrawing = false;
    }
    addDescriptionTextFrame();
    addBorder();
}
