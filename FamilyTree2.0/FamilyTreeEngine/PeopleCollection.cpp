#include "PeopleCollection.hpp"
#include <algorithm>
PersonPtr PeopleCollection::addPerson(Person p)
{
    for (const auto& e : people) if (*e == p) return e;
    auto person = std::make_shared<Person>(p);
    people.push_back(person);
    return person;
}

People PeopleCollection::findPeople(Name name)
{
    People matchingPeople;
    for (const auto& e : people)
    {
        if (name == e->getName()) matchingPeople.push_back(e);
    }
    return matchingPeople;
}

People PeopleCollection::getPeople()
{
    People matchingPeople;
    for (const auto& e : people) matchingPeople.push_back(e);
    return matchingPeople;
}
