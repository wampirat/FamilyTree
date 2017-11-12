#include "FamilyTreeFramework.hpp"
#include "gtest/gtest.h"

using namespace testing;

class FamilyTreeFrameworkTest : public Test
{
public:
    void SetUp() override
    {
        
    }
    FamilyTreeFramework ftf;
};

TEST_F(FamilyTreeFrameworkTest, canMakeFamily)
{
    auto family = ftf.createFamily();
    auto person1 = ftf.createPerson();
    auto person2 = ftf.createPerson();
    auto person3 = ftf.createPerson();
    auto person4 = ftf.createPerson();
    auto person1Name = "Qwerty23";
    person1->setName(person1Name);
    
    family->addPartner(person1);
    family->addPartner(person2);
    family->addKid(person3);
    family->addKid(person4);
    
    ASSERT_EQ(4, family->size());
    ASSERT_EQ(1, ftf.familiesSize());
    ASSERT_EQ(4, ftf.peopleSize());
    
    auto families = ftf.getFamilies();
    ASSERT_EQ(1, families.size());
    ASSERT_EQ(families[0], family);
    
    auto people = ftf.getPeople();
    ASSERT_EQ(4, people.size());
    ASSERT_EQ(person1.get(), people[0].get());
    ASSERT_EQ(person2.get(), people[1].get());
    ASSERT_EQ(person3.get(), people[2].get());
    ASSERT_EQ(person4.get(), people[3].get());
    
    ASSERT_EQ(person1->getName(), person1Name);
    ASSERT_EQ(people[0]->getName(), person1Name);
}

TEST_F(FamilyTreeFrameworkTest, canMakeHugeTree)
{
    std::shared_ptr<SocialPerson> probant = ftf.createPerson();
    std::shared_ptr<SocialPerson> zona = ftf.createPerson();
    auto syn = ftf.createPerson();
    auto corka = ftf.createPerson();
    auto synowa = ftf.createPerson();
    auto ziec = ftf.createPerson();
    auto wnuk = ftf.createPerson();
    auto wnuczka = ftf.createPerson();
    auto siostra = ftf.createPerson();
    auto brat = ftf.createPerson();
    auto ojciec = ftf.createPerson();
    auto matka = ftf.createPerson();
    auto wujek = ftf.createPerson();
    auto wujenka = ftf.createPerson();
    auto kuzyn = ftf.createPerson();
    auto kuzynka = ftf.createPerson();
    auto ciocia = ftf.createPerson();
    auto babcia = ftf.createPerson();
    auto dziadek = ftf.createPerson();
    auto tesc = ftf.createPerson();
    auto tesciowa = ftf.createPerson();
    auto szwagier = ftf.createPerson();
    auto szwagierka = ftf.createPerson();
    auto siostrzenica = ftf.createPerson();
    auto bratanica = ftf.createPerson();
    auto siostrzeniec = ftf.createPerson();
    auto bratanek = ftf.createPerson();
    auto bratowa = ftf.createPerson();
    auto prababcia = ftf.createPerson();
    auto pradziadek = ftf.createPerson();
    auto kochanka = ftf.createPerson();
    auto bekart = ftf.createPerson();
    
    std::shared_ptr<Family> baza = ftf.createFamily();
    baza->addPartner(probant);
    baza->addPartner(zona);
    baza->addKid(syn);
    baza->addKid(corka);
    
    std::shared_ptr<Family> rodzice = ftf.createFamily();
    rodzice->addPartner(ojciec);
    rodzice->addPartner(matka);
    rodzice->addKid(probant);
    rodzice->addKid(brat);
    rodzice->addKid(siostra);
    
    std::shared_ptr<Family> dziadkowie = ftf.createFamily();
    dziadkowie->addPartner(dziadek);
    dziadkowie->addPartner(babcia);
    dziadkowie->addKid(ojciec);
    dziadkowie->addKid(wujek);
    dziadkowie->addKid(ciocia);
    
    std::shared_ptr<Family> pradziadkowie = ftf.createFamily();
    pradziadkowie->addPartner(pradziadek);
    pradziadkowie->addPartner(prababcia);
    pradziadkowie->addKid(dziadek);
    
    std::shared_ptr<Family> wujostwo = ftf.createFamily();
    wujostwo->addPartner(wujek);
    wujostwo->addPartner(wujenka);
    wujostwo->addKid(kuzyn);
    wujostwo->addKid(kuzynka);
    
    std::shared_ptr<Family> tesciowie = ftf.createFamily();
    tesciowie->addPartner(tesc);
    tesciowie->addPartner(tesciowa);
    tesciowie->addKid(zona);
    tesciowie->addKid(szwagier);
    tesciowie->addKid(szwagierka);
    
    std::shared_ptr<Family> odBrata = ftf.createFamily();
    odBrata->addPartner(brat);
    odBrata->addPartner(bratowa);
    odBrata->addKid(bratanek);
    odBrata->addKid(bratanica);
    
    std::shared_ptr<Family> odSiostry = ftf.createFamily();
    odSiostry->addPartner(siostra);
    odSiostry->addKid(siostrzeniec);
    odSiostry->addKid(siostrzenica);
    
    std::shared_ptr<Family> poMieczu = ftf.createFamily();
    poMieczu->addPartner(syn);
    poMieczu->addPartner(synowa);
    poMieczu->addKid(wnuk);
    poMieczu->addKid(wnuczka);
    
    std::shared_ptr<Family> poKadzieli = ftf.createFamily();
    poKadzieli->addPartner(corka);
    poKadzieli->addPartner(ziec);
    
    std::shared_ptr<Family> konkubinat = ftf.createFamily();
    konkubinat->addPartner(probant);
    konkubinat->addPartner(kochanka);
    konkubinat->addKid(bekart);
    
    const auto probantName = "QWERT";
    probant->setName(probantName);
    
    ASSERT_EQ(probantName, ftf.getPeople()[0]->getName());
    
    ASSERT_EQ(3, probant->getNumOfRelatedFamilies());
    ASSERT_EQ(2, zona->getNumOfRelatedFamilies());
    
    auto probantKids = probant->getKids();
    auto zonaKids = zona->getKids();
    auto probantParents = probant->getParents();
    auto probantSiblings = probant->getSiblings();
    auto probantPartners = probant->getPartners();
    auto probantFamilies = probant->getRelatedFamilies();
    
    ASSERT_EQ(3, probantKids.size());
    ASSERT_EQ(syn, probantKids[0]);
    ASSERT_EQ(corka, probantKids[1]);
    ASSERT_EQ(bekart, probantKids[2]);
    
    ASSERT_EQ(2, zonaKids.size());
    ASSERT_EQ(syn, zonaKids[0]);
    ASSERT_EQ(corka, zonaKids[1]);
    ASSERT_EQ(2, probant->getPartners().size());
    ASSERT_EQ(1, zona->getPartners().size());
    
    ASSERT_EQ(2, probantParents.size());
    ASSERT_EQ(ojciec, probantParents[0]);
    ASSERT_EQ(matka, probantParents[1]);
    
    ASSERT_EQ(2, probantSiblings.size());
    ASSERT_EQ(brat, probantSiblings[0]);
    ASSERT_EQ(siostra, probantSiblings[1]);
    
    ASSERT_EQ(2, probantPartners.size());
    ASSERT_EQ(zona, probantPartners[0]);
    ASSERT_EQ(kochanka, probantPartners[1]);
    
    ASSERT_EQ(3, probantFamilies.size());
    ASSERT_EQ(baza, probantFamilies[0]);
    
    ASSERT_EQ(32, ftf.peopleSize());
    ASSERT_EQ(11, ftf.familiesSize());
}