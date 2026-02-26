//
// Created by Jackson Pike on 1/23/26.
// Simple Employee Class. An employee has a name, id, age, job, and year.
//

#ifndef INC_301_EMPLOYEE_H
#define INC_301_EMPLOYEE_H
#include <string>
using namespace std;

class Employee {
public:
    string name;
    int id;
    int age;
    string job;
    int year;

    Employee(string, int, int, string, int);


    string say();

    // getters
    string getName() const;
    int getId() const;
    int getAge() const;
    string getJob() const;
    int getYear() const;

    // comparisons (by id)
    bool operator<(const Employee& other) const;
    bool operator>(const Employee& other) const;
    bool operator==(const Employee& other) const;
};


#endif //INC_301_EMPLOYEE_H
