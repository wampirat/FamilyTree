#include "gtest/gtest.h"
#include "Family.h"
#include "FamilyFactory.hpp"
#include "Person.hpp"
#include "SocialPerson.hpp"
#include <vector>

using namespace testing;

class ScenarioTest : public Test
{
public:
    void SetUp() override
    {
        preparePeople();
        prepareFamilies();
    }
    
    void preparePeople()
    {
        prepareProbant();
        socialPeople.push_back(probant);
        socialPeople.push_back(zona);
        socialPeople.push_back(syn);
        socialPeople.push_back(corka);
        socialPeople.push_back(ojciec);
        socialPeople.push_back(wujek);
        socialPeople.push_back(ciocia);
        socialPeople.push_back(matka);
        socialPeople.push_back(dziadek);
        socialPeople.push_back(babcia);
        socialPeople.push_back(siostra);
        socialPeople.push_back(brat);
        socialPeople.push_back(szwagier);
        socialPeople.push_back(tesc);
        socialPeople.push_back(tesciowa);
    }
    
    void prepareProbant()
    {
        probant->setName("Tomek");
        probant->setSurname("");
        probant->setBirthdate(1874);
        probant->setDeathdate(1906);
    }
    void prepareFamilies()
    {
        auto l_rodzina = FamilyFactory::create();
        rodzina = l_rodzina;
        families.push_back(l_rodzina);
        
        l_rodzina->addPartner(probant);
        l_rodzina->addPartner(zona);
        l_rodzina->addKid(corka);
        l_rodzina->addKid(syn);

        auto l_rodzice = FamilyFactory::create();
        rodzice = l_rodzice;
        families.push_back(l_rodzice);
        l_rodzice->addPartner(ojciec);
        l_rodzice->addPartner(matka);
        l_rodzice->addKid(probant);
        l_rodzice->addKid(siostra);
        l_rodzice->addKid(brat);
        
        auto l_dziadkowie = FamilyFactory::create();
        dziadkowie = l_dziadkowie;
        families.push_back(l_dziadkowie);
        l_dziadkowie->addPartner(dziadek);
        l_dziadkowie->addPartner(babcia);
        l_dziadkowie->addKid(wujek);
        l_dziadkowie->addKid(ciocia);
        l_dziadkowie->addKid(ojciec);

        auto l_tesciowie = FamilyFactory::create();
        tesciowie = l_tesciowie;
        families.push_back(l_tesciowie);
        l_tesciowie->addPartner(tesc);
        l_tesciowie->addPartner(tesciowa);
        l_tesciowie->addKid(zona);
        l_tesciowie->addKid(szwagier);
    }
    std::shared_ptr<SocialPerson> probant = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> zona = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> syn = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> corka = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> ojciec = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> wujek = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> ciocia = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> matka = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> dziadek = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> babcia = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> siostra = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> brat = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> szwagier = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> tesc = std::make_shared<SocialPerson>();
    std::shared_ptr<SocialPerson> tesciowa = std::make_shared<SocialPerson>();

    std::vector<std::shared_ptr<SocialPerson>> socialPeople;
    std::vector<std::shared_ptr<Family>> families;
    
    FamilyPtr rodzina;
    FamilyPtr rodzice;
    FamilyPtr dziadkowie;
    FamilyPtr tesciowie;   
};

TEST_F(ScenarioTest, iloscRodzinOsoby)
{
    ASSERT_EQ(2, probant->getNumOfRelatedFamilies());
    ASSERT_EQ(1, corka->getNumOfRelatedFamilies());
}

TEST_F(ScenarioTest, wielkoscRodziny)
{
    ASSERT_EQ(4, rodzina->size());
    ASSERT_EQ(5, rodzice->size());
}

TEST_F(ScenarioTest, findAllFamilesContainingSpecificPerson)
{
    auto relatedFamilies = probant->getRelatedFamilies();
    ASSERT_EQ(2, relatedFamilies.size());
    ASSERT_EQ(relatedFamilies[0], rodzina);
    ASSERT_EQ(relatedFamilies[1], rodzice);
}

TEST_F(ScenarioTest, findAllKids)
{
    auto kids = dziadek->getKids();
    ASSERT_EQ(3, kids.size());
    ASSERT_EQ(kids[0], wujek);
    ASSERT_EQ(kids[1], ciocia);
    ASSERT_EQ(kids[2], ojciec);
}

TEST_F(ScenarioTest, findAllParents)
{
    auto parents = probant->getParents();
    ASSERT_EQ(2, parents.size());
    ASSERT_EQ(parents[0], ojciec);
    ASSERT_EQ(parents[1], matka);
}

TEST_F(ScenarioTest, grandfatherHasNoParents)
{
    auto parents = dziadek->getParents();
    ASSERT_EQ(0, parents.size());
}

TEST_F(ScenarioTest, uncleHasNoKids)
{
    auto kids = wujek->getKids();
    ASSERT_EQ(0, kids.size());
}

TEST_F(ScenarioTest, getPartners)
{
    auto partners = probant->getPartners();
    ASSERT_EQ(1, partners.size());
    ASSERT_EQ(partners[0], zona);
}

TEST_F(ScenarioTest, getSiblings)
{
    auto siblings = probant-> getSiblings();
    ASSERT_EQ(2, siblings.size());
    ASSERT_EQ(siblings[0], siostra);
    ASSERT_EQ(siblings[1], brat);
}
