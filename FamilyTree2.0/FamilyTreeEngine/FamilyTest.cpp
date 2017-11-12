#include "gtest/gtest.h"
#include "Family.h"
#include "FamilyFactory.hpp"
#include "SocialPerson.hpp"

using namespace testing;

class FamilyTest : public Test
{
};

TEST_F(FamilyTest, canCreateFamily)
{
    Family f();
}

TEST_F(FamilyTest, canAddPartnerToFamily)
{
    Family f;
    auto p = std::make_shared<SocialPerson>();
    f.addPartner(p);
    ASSERT_EQ(1, f.size());
}

TEST_F(FamilyTest, canAddPartnerToFamily_FamilyFactory)
{
    auto f = FamilyFactory::create();
    auto p = std::make_shared<SocialPerson>();
    f->addPartner(p);
    ASSERT_EQ(1, f->size());
}

TEST_F(FamilyTest, canAddKid)
{
    Family f;
    auto p = std::make_shared<SocialPerson>();
    f.addKid(p);
    ASSERT_EQ(1, f.size());
}

TEST_F(FamilyTest, emptyFamilyHasSize0)
{
    Family f;
    ASSERT_EQ(0, f.size());
}

TEST_F(FamilyTest, canCreateWithFamilyFactoryFromRawFamily)
{
    Family f;
    FamilyFactory::create(f);
}

TEST_F(FamilyTest, isMemberTest)
{
    auto f1 = FamilyFactory::create();
    auto f2 = FamilyFactory::create();
    
    auto member = std::make_shared<SocialPerson>();
    auto notMember = std::make_shared<SocialPerson>();
    f1->addKid(member);
    f2->addPartner(member);
    
    ASSERT_TRUE(f1->isMember(member));
    ASSERT_FALSE(f1->isMember(notMember));
    
    ASSERT_TRUE(f2->isMember(member));
    ASSERT_FALSE(f2->isMember(notMember));
}

TEST_F(FamilyTest, isPartnerOrKidTest)
{
    auto f1 = FamilyFactory::create();
    auto f2 = FamilyFactory::create();
    
    auto member = std::make_shared<SocialPerson>();
    auto notMember = std::make_shared<SocialPerson>();
    f1->addKid(member);
    f2->addPartner(member);
    
    ASSERT_TRUE(f1->isKid(member));
    ASSERT_FALSE(f1->isKid(notMember));
    ASSERT_FALSE(f1->isPartner(member));
    ASSERT_FALSE(f1->isPartner(notMember));
    
    ASSERT_TRUE(f2->isPartner(member));
    ASSERT_FALSE(f2->isPartner(notMember));
    ASSERT_FALSE(f2->isKid(member));
    ASSERT_FALSE(f2->isKid(notMember));
}