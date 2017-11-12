#include "Person.hpp"

void Person::setName(Name n)       {name = n;}
Name Person::getName() const       {return name;}

void Person::setSurname(Surname s) {surname = s;}
Surname Person::getSurname() const {return surname;}

void Person::setBirthdate(Date d)  {birthdate = d;}
Date Person::getBirthdate() const  {return birthdate;}

void Person::setDeathdate(Date d)  {deathdate = d;}
Date Person::getDeathdate() const  {return deathdate;}

void Person::setPlace(Place p)     {place = p;}
Place Person::getPlace() const     {return place;}

void Person::setIsAlive(IsAlive a)   {isAlive = a;}
IsAlive Person::getIsAlive() const   {return isAlive;}

std::string Person::toString() const
{
    std::string retString = "";
    if (name != "") retString += name + " ";
    if (surname != "")retString += surname + " ";
    if (birthdate != 0)
    {
        retString += "*" + std::to_string(birthdate) + " ";
    }
    if (deathdate != 0) retString += "+" + std::to_string(deathdate) + " ";
    if (place != "") retString += place;
    while (retString.size() && retString[0] == ' ') retString.erase();
    return retString;
}

bool Person::operator==(const Person& p) const
{
    return p.name      == name
        && p.surname   == surname
        && p.birthdate == birthdate
        && p.deathdate == deathdate
        && p.place     == place
        && p.isAlive   == isAlive;
}

bool Person::operator==(const PersonWeakPtr& p) const
{
    if (auto person = p.lock()) return operator==(*person);
    else return false;
}