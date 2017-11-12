#pragma once
#include "Family.h"
#include <memory>

class FamilyFactory {
public:
    static std::shared_ptr<Family> create()
    {
        auto family = std::make_shared<Family>();
        family->setThis(family);
        return family;
    }
    
    static std::shared_ptr<Family> create(Family& f)
    {
        auto family = std::make_shared<Family>(f);
        family->setThis(family);
        return family;
    }
private:

};
