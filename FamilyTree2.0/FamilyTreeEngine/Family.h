#pragma once

#include <vector>
#include <memory>
#include <iostream>

class SocialPerson;

class Person;

using People = std::vector<std::shared_ptr<SocialPerson>>;

class Family
{
public:
    Family() = default;
    Family(const std::shared_ptr<Family> &);
    void addPartner(const std::shared_ptr<SocialPerson>&);
    void addKid(const std::shared_ptr<SocialPerson>&);
    bool isMember(const std::shared_ptr<SocialPerson>&) const;
    bool isPartner(const std::shared_ptr<SocialPerson>&) const;
    bool isPartner(const SocialPerson&) const;
    bool isKid(const std::shared_ptr<SocialPerson> &) const;
    bool isKid(const SocialPerson&) const;
    void removePerson(const SocialPerson&);
    void removePerson(const std::shared_ptr<SocialPerson>&);
    int size() const;
    void setThis(const std::shared_ptr<Family>& f) noexcept
    {
        m_this = f;
    }

    auto getPartners() const noexcept
    {
        return m_partners;
    }
    
    auto getKids() const noexcept
    {
        return m_kids;
    }

    void update(Family f);
    
    bool operator==(const Family& f) const noexcept
    {
        return m_this.lock() == f.m_this.lock();
    }
    
    bool operator==(const std::shared_ptr<Family>& f) const noexcept
    {
        return *this == *f;
    }

private:
    std::weak_ptr<Family> m_this;
    People m_partners;
    People m_kids;
    int ancestorsWidth = 0;
    int posterityWidth = 0;
};
