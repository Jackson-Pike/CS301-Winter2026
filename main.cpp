#include <iostream>
#include <vector>
#include <fstream>
#include "Employee.h"
#include <random>
using namespace std;

//Prepare ifstream, initialize fields
ifstream ifs;
fstream output;
int numRecords = 0;
vector<Employee*> employees, employeesRandom, employeesTRE;
bool printIterative = false;
int numComparisons = 0;

// Prep for random number generation, used later
random_device rd;
mt19937 gen(rd());

// Helper method to print out the empId of each employee in the vector, all on one line.
void printVector() {
    for (auto employee : employees) {
        cout << employee->getId() << " ";
    }
    cout << endl;
}

// Helper method of my own creation, to streamline swapping vector elements
void exchange(auto &left, auto &right) {
    auto temp = left;
    left = right;
    right = temp;
}

// Partition sub-routine, as seen in the textbook pseudocode. I did end up passing in the vector as a field, in order to have all three implementations on the same program
int partition(int p, int r, auto &desiredVector) {
    int partitionValue = desiredVector[r]->id;
    int i = p-1;

    for (int j = p; j <= r-1; j++) { // go all the way to r-1, because 0 based array
        numComparisons++;
        if (desiredVector[j]->id <= partitionValue) {
            i++;
            exchange(desiredVector[i], desiredVector[j]);
            if (printIterative) printVector();
        }
    }
    exchange(desiredVector[i+1], desiredVector[r]);

    if (printIterative) printVector();
    return i+1;

}

void quicksort(int p, int r) {
    if (p < r) {
        int q = partition(p, r, employees);
        quicksort(p, q-1);
        quicksort(q+1, r);
    }
}

int randomizedPartition(int p, int r) {

    uniform_int_distribution<> dist(p, r);
    int i = dist(gen); // get a random index for our partition index

    exchange(employeesRandom[r], employeesRandom[i]); // make the employee at random index become last element in vector
    return partition(p, r, employeesRandom);
}


void randomizedQuickSort(int p, int r) {
    if (p < r) {
        int q = randomizedPartition(p, r);
        randomizedQuickSort(p, q-1);
        randomizedQuickSort(q+1, r);
    }
}

void treQuickSort(int p, int r) {
    while (p < r) {
        int q = partition(p, r, employeesTRE);
        treQuickSort(p, q-1);
        p = q + 1;
    }
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

        if (argc == 3 && (stoi(argv[2]) == 1)) {
            printIterative = true;
        }
    }

    // **************************** FILE PARSING ****************************** //
    {
        // Begin parsing file
        string firstLine;
        getline(ifs, firstLine); // Get the number of records using first line
        numRecords = stoi(firstLine); // cast to int


        /* Parse the file, creating an employee object and adding them to the employees vector
            in each iteration
        */

        for (int i = 0; i < numRecords; i++) {
            string empName, empID, empAge, empJob, empYear;
            getline(ifs,empName, '|');
            getline(ifs, empID, '|');
            getline(ifs,empAge, '|');
            getline(ifs,empJob, '|');
            getline(ifs,empYear);
            Employee* e = new Employee(empName, stoi(empID), stoi(empAge), empJob, stoi(empYear));
            employees.push_back(e);

        }
        employeesRandom = employees;
        employeesTRE = employees;
        ifs.close();

    }

    // **************** SORT VECTOR AND STORE COMPARISONS **********************//

    // Normal Quicksort on the employees vector
    quicksort(0, employees.size()-1);
    cout << "Standard Quick - Number of Comparisons: " << numComparisons << endl;
    numComparisons = 0; // reset comparisons to 0 in prepartion to rerun

    // Randomized Pivot Value, quicksort on employeesRandom vector
    randomizedQuickSort(0, employeesRandom.size()-1);
    cout << "Randomized QuickSort - Number of Comparisons: " << numComparisons << endl;
    numComparisons = 0; // reset comparisons to 0 again

    // TRE Quicksort
    treQuickSort(0, employeesTRE.size()-1);
    cout << "TRE Quicksort Required Comparisons: " << numComparisons << endl;





    //******************* FILE OUTPUT **************************//
    {
        string outputFilename = "output_" + string(argv[1]);
        output.open(outputFilename, ios::out);

        if (!output) {
            cerr << "Error while opening file." << endl;
            exit(1);
        }

        for (auto employee : employees) {
            output << employee->getName();
            output << "|";
            output << employee->getId();
            output << "|";
            output << employee->getAge();
            output << "|";
            output << employee->getJob();
            output << "|";
            output << employee->getYear() << endl;
        }

        output.close();

    }



    return 0;
}





