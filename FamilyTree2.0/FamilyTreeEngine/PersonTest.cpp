#include "gtest/gtest.h"
#include "Person.hpp"

using namespace testing;

class PersonTest : public Test
{
    
};

TEST_F(PersonTest, canMakePerson)
{
    Name name = "";
    Surname surname = "";
    Date birthdate = 0;
    Date deathdate = 0;
    
    Person person;
    
    ASSERT_EQ(name, person.getName());
    ASSERT_EQ(surname, person.getSurname());
    ASSERT_EQ(birthdate, person.getBirthdate());
    ASSERT_EQ(deathdate, person.getDeathdate());
}

TEST_F(PersonTest, canSetPerson)
{
    Name name = "Name";
    Surname surname = "Surname";
    Date birthdate = 23;
    Date deathdate = 45;
    Place actualPlace = "Place";
    IsAlive isAlive = true;
    
    Person person;
    person.setName(name);
    person.setSurname(surname);
    person.setBirthdate(birthdate);
    person.setDeathdate(deathdate);
    person.setPlace(actualPlace);
    person.setIsAlive(isAlive);
    
    ASSERT_EQ(name, person.getName());
    ASSERT_EQ(surname, person.getSurname());
    ASSERT_EQ(birthdate, person.getBirthdate());
    ASSERT_EQ(deathdate, person.getDeathdate());
    ASSERT_EQ(actualPlace, person.getPlace());
    ASSERT_EQ(isAlive, person.getIsAlive());
}

TEST_F(PersonTest, personIsDead)
{
    IsAlive isAlive = false;
    Person person;
    person.setIsAlive(isAlive);
    ASSERT_EQ(isAlive, person.getIsAlive());
}
