#pragma once

#include "Person.hpp"
#include <vector>
#include <memory>

using PersonPtr = std::shared_ptr<Person>;
using People = std::vector<PersonPtr>;

class PeopleCollection
{
public:
    PersonPtr addPerson(Person);
    People findPeople(Name);
    People getPeople();
private:
    People people;
};
