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
int globalCollisions;
int partOneCollisions;
int n = 0; // number of records contained in the file
vector<Employee*>* part_one_hashtable;
ostringstream partOne, partTwo;


// Division hash
int hash_div(int key) {
    return key % m;
}

// Multiplication Method - Discussed and developed with Bro Draper in class.
int hash_mult(int key) {
    //multiplication method
    double A = (sqrt(5) - 1) / 2;
    double actual = key * A;
    int integerPortion = (int)actual;
    double decimalPortion = actual - integerPortion;
    return (int)(m * decimalPortion);
    //return (int)(m * ((key * A) % 1));
}


 /* hashes the emp id to get the index, using the division method.
// Checks the array of vectors (employee pointers) - adds
// the parameter e to the vector at the computed index.
*  If the employee pointer at index 0 of the vector is not
*  e, then we know there was a collision.
*/
void addElement(auto* &e, int hash) {

    part_one_hashtable[hash].push_back(e);

    // Collision Check
    int collisions = 0;

    //Collisions
    if (part_one_hashtable[hash].at(0) != e) {
        partOneCollisions++;
        collisions = part_one_hashtable[hash].size()-1;
    }

    partOne << "Adding " << e->name << " to table at index " << hash << " ("<< collisions << " collision";
    if (collisions != 1) partOne << "s";
    partOne << ")" << endl;

}

 /* My implementation to convert string to an int representation.
  * Runs through each char in string key, multiples its ASCII value by the first 6 digits of pi, adds
  * the result to an int hash
  */
int pike_stoi(string key) {
    int hash = 0;
    for (char c: key) {
        hash += c * 3.14159 - 2;
        // This truncates towards 0, so 5 * 3.14159 = 15.70795 = 15
    }
    return hash;
}


    /*
     * My lookup method. Takes in the searchKey string (the employees name in this case)
     * Keeps track of local collisions. Hashes the searchKey using the pike_stoi and hash_div
     * to get the bucket where our employee should be. ideally, they would be at index one of this bucket
     * (which is a vector of Employee* pointers.)
     *
     * However if it not the first element, that is a collision and we continue searching.
     *
     * Outputs the information to the partTwo StringStream
     */

void lookup(string searchKey) {
    int localCollisions = 0;
    int index = hash_div(pike_stoi(searchKey)); // Hash the search key to limit search to bucket
    auto& bucket = part_one_hashtable[index]; // Get the bucket where our employee is
    Employee* employeeRecord; // For printing the employees info later once we find them.
    for (auto employee : bucket) {
        if (employee->name != searchKey) {
            localCollisions++; // If the first or any subsequent index is not our employee, mark as a collision
        } else { // Found the employee, grab the object and break
            employeeRecord = employee;
            break;
        }
    }

    globalCollisions += localCollisions; // Increment our total collisions during lookup

    /** String format **/
    string afterCollisions = (localCollisions != 1) ? " collisions" : " collision";
    partTwo << "FOUND " << searchKey << " after " << localCollisions << afterCollisions << " at index " << index << " in the hashtable. " << endl;

    partTwo << "Complete Record: "
            << employeeRecord->name
            << "|" << employeeRecord->id
            << "|" << employeeRecord->age
            << "|" << employeeRecord->job
            << "|" << employeeRecord->year << endl << "*" << endl;
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

        part_one_hashtable = new vector<Employee*>[m];


        /* Parse the file, creating an employee object and adding them to the employees vector
            in each iteration
        */

        /**************************** HASHING ******************************************/
        partOne << "<------------------- PART ONE - DIVISION METHOD ----------------------->" << endl;

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
            int partOneIndex = hash_div(pike_stoi(empName));

            //Add Element to our hashtable
            addElement(e, partOneIndex);

        }

        // Discard the *** in the file
        string discard;
        getline(ifs, discard);

        // Get the number of queries (employees we'll be searching for)
        string str_queries;
        getline(ifs, str_queries);

        // Loop for the number of queries, calling lookup function each time with the name at that line.
        int numQueries = stoi(str_queries);
        for (int i = 0; i < numQueries; i++) {
            string name;
            getline(ifs, name);
            lookup(name);
        }

        // Add final global collision to partTwo string
        partTwo << "Total Collisions during lookup phase: " << globalCollisions << endl;


        // Print Part one and two outputs.
        cout << partOne.str();
        cout << partTwo.str();


        ifs.close();

    }

    return 0;
}





