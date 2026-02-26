//
// Created by Jackson Pike on 1/23/26.
//

#include "Employee.h"

//Getters and Setters
string Employee::getName() const {
    return name;
}

int Employee::getId() const {
    return id;
}

int Employee::getAge() const {
    return age;
}

string Employee::getJob() const {
    return job;
}

int Employee::getYear() const {
    return year;
}

// Constructor
Employee::Employee(string n, int i, int a, string j, int y) {
    name = n;
    age = a;
    id = i;
    job = j;
    year = y;
}

bool Employee::operator<(const Employee& other) const {
    return id < other.id;
}

bool Employee::operator>(const Employee& other) const {
    return id > other.id;
}

bool Employee::operator==(const Employee& other) const {
    return id == other.id;
}



string Employee::say() {

}



