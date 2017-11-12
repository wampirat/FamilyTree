#include "Family.h"
#include "SocialPerson.hpp"

Family::Family(const std::shared_ptr<Family>& orig)
{
    update(orig);
}

void Family::addPartner(const std::shared_ptr<SocialPerson>& p)
{
    for (const auto& partner : m_partners)
    {
        if (partner == p) return;
    }
    m_partners.push_back(p);
    if (m_this.lock()) p->addRelatedFamily(m_this.lock());
}

void Family::addKid(const std::shared_ptr<SocialPerson>& p)
{
    for (const auto& kid : m_kids)
    {
        if (kid == p) return;
    }
    m_kids.push_back(p);
    std::sort(m_kids.begin(),
              m_kids.end(),
              [](const auto& a, const auto& b){return a->getBirthdate() < b->getBirthdate();});
    if (m_this.lock()) p->addRelatedFamily(m_this.lock());
}

int Family::size() const
{
    return m_partners.size() + m_kids.size();
}

void Family::update(Family f)
{
    m_partners = f.m_partners;
    m_kids = f.m_kids;
    ancestorsWidth = f.ancestorsWidth;
    posterityWidth = f.posterityWidth;
    for (const auto& partner : m_partners)
    {
        partner->addRelatedFamily(m_this.lock());
    }
    for (const auto& kid : m_kids)
    {
        kid->addRelatedFamily(m_this.lock());
    }
}

bool Family::isMember(const std::shared_ptr<SocialPerson> &p) const
{
    return p->isMember(m_this.lock());
}

bool Family::isPartner(const std::shared_ptr<SocialPerson>& p) const
{
    for (const auto& e : m_partners)
    {
        if (p == e) return true;
    }
    return false;
}

bool Family::isPartner(const SocialPerson& p) const
{
    for (const auto& e : m_partners)
    {
        if (p == *e) return true;
    }
    return false;
}

bool Family::isKid(const std::shared_ptr<SocialPerson>& p) const
{
    for (const auto& e : m_kids)
    {
        if (p == e) return true;
    }
    return false;
}

bool Family::isKid(const SocialPerson& p) const
{
    for (const auto& e : m_kids)
    {
        if (p == *e) return true;
    }
    return false;
}

void Family::removePerson(const std::shared_ptr<SocialPerson>& person)
{
    for (unsigned i = 0; i < m_partners.size(); ++i)
    {
        if (person == m_partners[i])
        {
            m_partners.erase(m_partners.begin() + i);
        }
    }
    for (unsigned i = 0; i < m_kids.size(); ++i)
    {
        if (person == m_kids[i])
        {
            m_kids.erase(m_kids.begin() + i);
        }
    }
}

void Family::removePerson(const SocialPerson& person)
{
    for (unsigned i = 0; i < m_partners.size(); ++i)
    {
        if (person == *m_partners[i])
        {
            m_partners.erase(m_partners.begin() + i);
        }
    }
    for (unsigned i = 0; i < m_kids.size(); ++i)
    {
        if (person == *m_kids[i])
        {
            m_kids.erase(m_kids.begin() + i);
        }
    }
}
