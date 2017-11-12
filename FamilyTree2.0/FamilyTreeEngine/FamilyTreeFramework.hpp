#pragma once

#include "FamilyFactory.hpp"
#include "SocialPerson.hpp"
#include <vector>
#include <memory>

class FamilyTreeFramework
{
public:
    auto createFamily()
    {
        auto family = FamilyFactory::create();
        families.push_back(family);
        return family;
    }

    auto createFamily(Family f)
    {
        auto family = FamilyFactory::create(f);
        families.push_back(family);
        return family;
    }
    
    auto createPerson()
    {
        auto person = std::make_shared<SocialPerson>();
        socialPeople.push_back(person);
        return person;
    }

    void removePerson(const std::shared_ptr<SocialPerson>& person)
    {
        for (unsigned i=0; i < socialPeople.size(); ++i)
        {
            if (socialPeople[i] == person)
            {
                for (const auto& family : person->getRelatedFamilies())
                {
                    family->removePerson(person);
                }
                socialPeople.erase(socialPeople.begin() + i);

            }
        }
    }
    
    auto familiesSize() const
    {
        return families.size();
    }
    
    auto peopleSize() const
    {
        return socialPeople.size();
    }
    
    auto getFamilies() const
    {
        return families;
    }
    
    auto getPeople() const
    {
        return socialPeople;
    }
private:
    std::vector<std::shared_ptr<Family>> families;
    std::vector<std::shared_ptr<SocialPerson>> socialPeople;
};
