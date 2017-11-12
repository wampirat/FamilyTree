#pragma once

#include "Person.hpp"
#include "Family.h"
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

using FamilyPtr = std::shared_ptr<Family>;
using Families = std::vector<FamilyPtr>;
        
class SocialPerson 
{
public:
    SocialPerson() noexcept 
        : person(std::make_shared<Person>())
    {
    }

    SocialPerson(const SocialPerson& orig) noexcept
    {
        posterityWidth = orig.posterityWidth;
        ancestorsWidth = orig.ancestorsWidth;
        person = orig.person;
        personsFamilies = orig.personsFamilies;
    }

    SocialPerson(Person p) noexcept
    {
        setPerson(p);
    }

    SocialPerson& operator=(const SocialPerson& orig)
    {
        posterityWidth = orig.posterityWidth;
        ancestorsWidth = orig.ancestorsWidth;
        person = orig.person;
        personsFamilies = orig.personsFamilies;
        return *this;
    }

    auto getName() const noexcept
    {
        return person->getName();
    }

    void setName(auto name) noexcept
    {
        return person->setName(name);
    }
    
    auto getSurname() const noexcept
    {
        return person->getSurname();
    }
    
    void setSurname(auto surname) noexcept
    {
        return person->setSurname(surname);
    }

    auto getBirthdate() const noexcept
    {
        return person->getBirthdate();
    }
    
    void setBirthdate(auto birthdate) noexcept
    {
        return person->setBirthdate(birthdate);
    }

    auto getDeathdate() const noexcept
    {
        return person->getDeathdate();
    }
    
    void setDeathdate(auto deathdate) noexcept
    {
        return person->setDeathdate(deathdate);
    }

    auto getIsAlive() const noexcept
    {
        return person->getIsAlive();
    }
    
    void setIsAlive(auto isAlive) noexcept
    {
        return person->setIsAlive(isAlive);
    }

    auto getPlace() const noexcept
    {
        return person->getPlace();
    }
    
    void setPlace(auto place) noexcept
    {
        return person->setPlace(place);
    }

    auto toString()
    {
        return person->toString();
    }

    auto getPerson() const noexcept
    {
        return person;
    }
    void setPerson(Person p) noexcept
    {
        person = std::make_shared<Person>(p);
    }
        
    auto operator*() const noexcept
    {
        return person;
    }
    
    std::shared_ptr<Person> operator->() const noexcept
    {
        return person;
    }
    
    std::shared_ptr<Person> get() const noexcept
    {
        return person;
    }
   
    auto getNumOfRelatedFamilies() const noexcept
    {
        return personsFamilies.size();
    }
    
    void addRelatedFamily(FamilyPtr f) noexcept
    {
        for (const auto& family : personsFamilies)
        {
            if (family == f) return;
        }
        personsFamilies.push_back(f);
    }
    
    bool isMember(FamilyPtr f) const noexcept
    {
        for (const auto& e : personsFamilies)
        {
            if (e == f) return true;
        }
        return false;
    }
    Families getRelatedFamilies() const noexcept
    {
        return personsFamilies;
    }

    auto getKids() noexcept
    {
        auto retval = People();
        for (const auto& family : personsFamilies)
        {
            if (family->isPartner(*this))
            {
                auto kids = family->getKids();
                retval.insert(retval.end(), kids.begin(), kids.end());
            }
        }
        return retval;
    }

    auto getParents() noexcept
    {
        auto retval = People();
        for (const auto& family : personsFamilies)
        {
            if (family->isKid(*this))
            {
                auto parents = family->getPartners();
                retval.insert(retval.end(), parents.begin(), parents.end());
            }
        }
        return retval;
    }
    
    auto getPartners() noexcept
    {
        auto retval = People();
        for (const auto& family : personsFamilies)
        {
            if (family->isPartner(*this))
            {
                const auto& partners = family->getPartners();
                for (const auto& p : partners)
                {
                    if (*p == person) continue;
                    retval.push_back(p);
                }

            }
        }
        return retval;
    }
    
    auto getAncestors() const noexcept
    {
        
    }
    
    auto getSiblings() const noexcept
    {
        People retval;
        for (const auto& family : personsFamilies)
        {
            if (family->isKid(*this))
            {
                const auto& partners = family->getKids();

                for (const auto& p : partners)
                {
                    if (*p == person) continue;
                    retval.push_back(p);
                }
                return retval;
            }
        }
        return retval;
    }
    
    bool operator==(const SocialPerson& p) const
    {
        return person == p.person;
    }
    
    bool operator==(const std::shared_ptr<Person>& p) const
    {
        return person == p;
    }

    int calcPosterityWidth()
    {
        int posterityWidth = 0;
        for (const std::shared_ptr<SocialPerson>& kid : this->getKids())
        {
            posterityWidth += kid->calcPosterityWidth();
        }
        posterityWidth = std::max(posterityWidth, 1 + static_cast<int>(getPartners().size()));
        this->posterityWidth = posterityWidth;
        return posterityWidth;
    }

    int calcAncestorsDepth(int p_depth = 0)
    {
        auto parents = getParents();
        int depth = 0;
        for (const auto& parent : parents)
        {
            depth = std::max(depth, parent->calcAncestorsDepth(p_depth + 1) + 1);
        }
        this->ancestorsDepth = p_depth;
        this->posterityDepth = 0;
        for (const auto& sibling : getSiblings())
        {
            sibling->setAncestorsDepth(this->ancestorsDepth);
        }
        for (const auto& partner : getPartners())
        {
            partner->setAncestorsDepth(this->ancestorsDepth);
        }
        return depth;
    }

    int calcPosterityDepth(int p_depth = 0)
    {
        auto kids = getKids();
        int depth = 0;
        for (const auto& sibling : getSiblings())
        {
            sibling->setPosterityDepth(p_depth);
        }
        for (const auto& partner : getPartners())
        {
            partner->setPosterityDepth(p_depth);
        }
        for (const auto& kid : kids)
        {
            depth = std::max(depth, kid->calcPosterityDepth(p_depth + 1) + 1);
        }
        this->ancestorsDepth = 0;
        this->posterityDepth = p_depth;
        return depth;
    }

    int getPosterityWidth()
    {
        return posterityWidth;
    }

    void setAncestorsWidth(int width)
    {
        ancestorsWidth = width;
    }

    int calcAncestorsWidth()
    {
        int ancestorsWidth = 0;
        for (const std::shared_ptr<SocialPerson>& parent : this->getParents())
        {
            ancestorsWidth += parent->calcAncestorsWidth();
        }
        ancestorsWidth = (std::max(ancestorsWidth, 1 + static_cast<int>(getSiblings().size())));
        this->ancestorsWidth = ancestorsWidth;
        return ancestorsWidth;
    }

    int getAncestorsWidth()
    {
        return ancestorsWidth;
    }

    ~SocialPerson()
    {
        for (const std::shared_ptr<Family>& family : personsFamilies)
        {
            family->removePerson(*this);
        }
    }

    void setAncestorsDepth(int depth)
    {
        ancestorsDepth = depth;
        posterityDepth = 0;
    }

    int getAncestorsDepth() const
    {
        return ancestorsDepth;
    }

    void setPosterityDepth(int depth)
    {
        posterityDepth = depth;
        ancestorsDepth = 0;
    }

    int getPosterityDepth() const
    {
        return posterityDepth;
    }

private:
    int posterityWidth = 0;
    int ancestorsWidth = 0;
    int ancestorsDepth = 0;
    int posterityDepth = 0;
    std::shared_ptr<Person> person;
    Families personsFamilies;
};
