#include <iostream>
#include <vector>
#include <fstream>
#include "Employee.h"
#include <random>
#include <sstream>

using namespace std;

//Prepare ifstream, initialize fields
ifstream ifs;
fstream output;

int m = 0; // size of the array
int divCollisions = 0;
int multCollisions = 0;
int partThreeCollisions = 0;
int n = 0; // number of records contained in the file
vector<Employee*>* hashtable;
ostringstream partOne, partTwo, partThree;



int hash_div(int key) {
    return key % m;
}

int hash_mult(int key) {
    //multiplication method
    double A = (sqrt(5) - 1) / 2;
    double actual = key * A;
    int integerPortion = (int)actual;
    double decimalPortion = actual - integerPortion;
    return (int)(m * decimalPortion);
    //return (int)(m * ((key * A) % 1));
}

void addSimpleElement(auto* &emp, auto &array, auto &builder, int index, int &colCounter) {
    if (array[index] == nullptr) {
        builder << "Position at index " << index << " was empty. Adding " << emp->name
        << "with ID " << emp->id << " at that position" << endl;
        array[index] = emp;
    } else {
        builder << "Collision. Position at index " << index << " is already occupied by the employee "
              << emp->name << " with ID " << emp->id << endl;
        colCounter++;
    }
}

void addComplexElement(auto* &e) {
    // Collision Check
    int index = hash1(e->id);
    vector<Employee*> &employees = hashtable[index];
    employees.push_back(e);
    int collisions = 0;

    //Collisions
    if (employees.at(0) != e) {
        partThreeCollisions++;
        collisions = employees.size()-1;
    }

    partThree << "Adding " << e->name << " to table at index " << index << " ("<< collisions << " collison";
    if (collisions != 1) partThree << "s";
    partThree << ")" << endl;

}

int main(int argc, char* argv[]) {
    // **************************** FILE INPUT *******************************//
    {
        // Close program if a filename was not provided.
        if (argc < 2) {
            cout << "Ooops! You forgot to provide a filename on the command line." << endl;
            exit(1);
        }

        // Proceed to open ifs with commandline argument [1] (the filename)
        ifs.open(argv[1], ios::in);

        // If there was an issue or the file did not exist, print error and exit.
        if (!ifs) {
            cout << "ERROR occurred when reading file: " << argv[1] << endl;
            exit(1);
        }

    }

    // **************************** FILE PARSING ****************************** //
    {
        // Begin parsing file
        string firstLine;
        getline(ifs, firstLine); // Get the number of records using first line
        m = stoi(firstLine); // cast to int
        string secondLine;
        getline(ifs, secondLine);
        n = stoi(secondLine);

        Employee** simple_hash_div = new Employee*[m];
        Employee** simple_hash_mult = new Employee*[m];
        hashtable = new vector<Employee*>[m];


        /* Parse the file, creating an employee object and adding them to the employees vector
            in each iteration
        */

        /**************************** HASHING ******************************************/

        partOne << "<------------------- PART ONE - DIVISION METHOD ----------------------->" << endl;
        partTwo << "<------------------- PART TWO - MULTIPLICATION METHOD ----------------->" << endl;
        partThree << "<------------------- PART THREE - MULTIPLICATION METHOD ----------------->" << endl;

        // Loop through the file, creating the employee object from each line, and adding employee to the specified hashtable
        // Using addSimpleElement or addComplexElement (external methods which handle hashing, as well as collisions.)
        for (int i = 0; i < n; i++) {

            // Create Employee Object
            string empName, empID, empAge, empJob, empYear;
            getline(ifs,empName, '|');
            getline(ifs, empID, '|');
            getline(ifs,empAge, '|');
            getline(ifs,empJob, '|');
            getline(ifs,empYear);
            Employee* e = new Employee(empName, stoi(empID), stoi(empAge), empJob, stoi(empYear));


            // Get Indexes from hash
            int partOneIndex = hash_div(e->id);
            int partTwoIndex = hash_mult(e->id);



            if (argc == 3) {
                addComplexElement(e);
            } else {
                addSimpleElement(e, simple_hash_div, partOne, partOneIndex, divCollisions);
                addSimpleElement(e, simple_hash_mult, partTwo, partTwoIndex, multCollisions);
            }



        }

        if (argc == 3) {
            partThree << "Total Collisions needing resolving: " << partThreeCollisions;
            cout << partThree.str();
        } else {
            partOne << endl << "Total Collisions: " << divCollisions;
            partOne << endl << endl;
            partTwo << endl << "Total Collisions: " << multCollisions << endl << endl;

            cout << partOne.str();
            cout << partTwo.str();
        }

        ifs.close();

    }

    return 0;
}





