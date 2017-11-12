#include "gtest/gtest.h"
#include "PeopleCollection.hpp"
#include "Person.hpp"

using namespace testing;

class PeopleCollectionTest : public Test
{
public:
    PeopleCollection p;
};


TEST_F(PeopleCollectionTest, canAddPersonToCollection)
{
    Person person;
    p.addPerson(person);
}

TEST_F(PeopleCollectionTest, canAddTwoDifferentPeople)
{
    Person person, person2;
    person.setName("Name");
    person2.setName("Name2");
    ASSERT_NE(p.addPerson(person), p.addPerson(person2));
}

TEST_F(PeopleCollectionTest, whenAddedTwoSamePeopleThenOnlyOneWillBeAdded)
{
    Person person, person2;
    ASSERT_EQ(p.addPerson(person), p.addPerson(person2));
}

TEST_F(PeopleCollectionTest, canFindPersonByName)
{
    Person person;
    Name name = "NameToBeFound";
    person.setName(name);
    p.addPerson(person);
    auto matchingPeople = p.findPeople(name);
    ASSERT_EQ(1, matchingPeople.size());
    ASSERT_EQ(person, matchingPeople[0]);
}

TEST_F(PeopleCollectionTest, cannotFindByNameInEmptyCollection)
{
    Person person;
    Name name = "NameToNotBeFound";
    person.setName(name);
    auto matchingPeople = p.findPeople(name);
    ASSERT_EQ(0, matchingPeople.size());
}

TEST_F(PeopleCollectionTest, cannotFindByNameWhenPersonWithThisNameIsNotAdded)
{
    Person person, person2;
    Name name = "NameToNotBeFound";
    person.setName(name);
    person2.setName("Jack");
    p.addPerson(person2);
    auto matchingPeople = p.findPeople(name);
    ASSERT_EQ(0, matchingPeople.size());
}

TEST_F(PeopleCollectionTest, willFindAllPeopleWithTheSameName)
{
    constexpr int numberOfPeople = 100;
    Person people[numberOfPeople];
    Name name = "Johnny";
    Name nameNotToBeFound = "Kenny";
    for (int i = 0; i < numberOfPeople; ++i)
    {
        people[i].setName((i % 2) ? name : nameNotToBeFound);
        people[i].setSurname(std::to_string(i));
        p.addPerson(people[i]);
    }
    auto matchingPeople = p.findPeople(name);
    ASSERT_EQ(numberOfPeople / 2, matchingPeople.size());
}