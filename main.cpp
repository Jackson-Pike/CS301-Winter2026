#include <iostream>
#include <vector>
#include <fstream>
#include "Employee.h"
using namespace std;

//Prepare ifstream, initialize fields
ifstream ifs;
fstream output;
int numRecords = 0;
vector<Employee*> employees;
bool printIterative = false;
int numComparisons = 0;

// Helper method to print out the empId of each employee in the vector, all on one line.
void printVector() {
    for (auto employee : employees) {
        cout << employee->getId() << " ";
    }
    cout << endl;
}

int partition(int p, int r) {
    int x = employees[r]->id;
    int i = p-1;

    for (int j = p; j <= r-1; j++) {
        numComparisons++;
        if (employees[j]->id <= x) {
            i++;
            auto temp = employees[i];
            employees[i] = employees[j];
            employees[j] = temp;
        }
    }

    auto temp = employees[i+1];
    employees[i+1] = employees[r];
    employees[r] = temp;
    if (printIterative) printVector();
    return i+1;

}

void quicksort(int p, int r) {
    if (p < r) {
        int q = partition(p, r);
        quicksort(p, q-1);
        quicksort(q+1, r);
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
        Employee** sheng = new Employee* [100];

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
        ifs.close();

    }

    // **************** SORT VECTOR AND STORE COMPARISONS **********************//
    quicksort(0, employees.size()-1);


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

    cout << "Number of Comparisons: " << numComparisons << endl;

    return 0;
}





