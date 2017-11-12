#pragma once
#include <string>
#include <memory>

using Name = std::string;
using Surname = std::string;
using Date = int;
using Place = std::string;
using PersonId = unsigned long long int;
class Person;
using PersonWeakPtr = std::weak_ptr<Person>;
using IsAlive = bool;

class Person
{
public:
    void setName(Name);
    Name getName() const;
    void setSurname(Surname);
    Surname getSurname() const;
    void setPlace(Place);
    Place getPlace() const;
    bool operator==(const Person&) const;
    bool operator==(const PersonWeakPtr&) const;
    void setBirthdate(Date);
    Date getBirthdate() const;
    void setDeathdate(Date);
    Date getDeathdate() const;
    void setIsAlive(IsAlive);
    IsAlive getIsAlive() const;
    std::string toString() const;

private:
    Name name = "";
    Name surname = "";
    Date birthdate = 0;
    Date deathdate = 0;
    Place place = "";
    IsAlive isAlive = true;
};
