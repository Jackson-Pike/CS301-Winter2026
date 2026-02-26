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

int partitionCount = 0;

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
    partitionCount++;
    int pivotValue = desiredVector[r]->id;
    int i = p-1;

    for (int j = p; j <= r-1; j++) { //Start running through the array, starting at p (start index) all the way to the last index
        numComparisons++;
        if (desiredVector[j]->id <= pivotValue) {
            i++; // Increase the 'size' of our less then or equal to portion (the left size).

            //The value now at vector[i] is now actually greater than our pivot and needs to be exchanged with the current (j) element.
            exchange(desiredVector[i], desiredVector[j]); // put j in left portion
            if (printIterative) printVector();
        }
    }
    exchange(desiredVector[i+1], desiredVector[r]); // Move the pivot into its final position.

    if (printIterative) printVector();
    return i+1; // Return i+1, which will be used in quicksort calls

}

// Standard Quicksort, using partition subroutine and recursive calls to itself.
void quicksort(int p, int r) {
    if (p < r) {
        int q = partition(p, r, employees);
        quicksort(p, q-1); // Recursively partition the left side
        quicksort(q+1, r); // Right side, also recursive
    }
}

// Same as regular partition, except our index of our pivot is chosen randomly. This element is moved to the end of the array.
// This routine modifies the employeesRandom vector, which is a direct copy of the employees vector after initial file read.
int randomizedPartition(int p, int r) {

    uniform_int_distribution<> dist(p, r);
    int i = dist(gen); // get a random index for our partition index

    exchange(employeesRandom[r], employeesRandom[i]); // make the employee at random index become last element in vector
    return partition(p, r, employeesRandom);
}

// The randomized quicksort. Utilizes randomizedPartition as a subroutine.
void randomizedQuickSort(int p, int r) {
    if (p < r) {
        int q = randomizedPartition(p, r);
        randomizedQuickSort(p, q-1);
        randomizedQuickSort(q+1, r);
    }
}

// Different implementation of quicksort. Modifies the employeesTRE vector which is a direct copy of employees vector as it was after initial file read
// Uses a while loop to eliminate one recursive call in each call to treQuickSort.
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





