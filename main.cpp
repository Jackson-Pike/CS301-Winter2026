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
int heapSize = 0;

// Helper method to print out the empId of each employee in the vector, all on one line.
void printVector() {
    for (auto employee : employees) {
        cout << employee->getId() << " ";
    }
    cout << endl;
}

// Implements textbook merge sort psuedocode
void merge(int p, int q, int r) {
    int lenL = q - p + 1; // Length of the left half
    int lenR = r-q; // Length of the right half
    vector<Employee*> left; // create the left vector (local)
    vector<Employee*> right; // create the right vector (local)
    for (int i = 0; i < lenL; i++) {
        left.push_back(employees[p+i]); // add the left half of employees subvector to the left vector
    }
    for (int i = 0; i < lenR; i++) {
        right.push_back(employees[q+i+1]); // add the right half of this subvector to the right vector
    }
    int i = 0; // Indexes smallest remaining element in left
    int j = 0; // smallest remaining element in right
    int k = p; // k indexes the location in employees vector to fill

    while (i < lenL && j < lenR) {
        //numComparisons++;
        if (left[i]->id <= right[j]->id) {
            employees[k] = left[i];
            i++;
        } else {
            employees[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < lenL) {
        //numComparisons++;
        employees[k] = left[i];
        i++;
        k++;
    }
    while (j < lenR) {
        //numComparisons++;
        employees[k] = right[j];
        j = j+1;
        k = k+1;
    }
    if (printIterative) {
        printVector();
    }


}

// What we actually call to sort the array (or vector)
// Recursively calls itself
void mergeSort(int p, int r) {
    if (p >= r) {
        return;
    }
    int q = (p+r)/2;
    mergeSort(p, q);
    mergeSort(q+1, r);

    merge(p, q, r);
}

// Helper methods get  the indexes
int parent(int i) {
    return (i-1)/2;
}

int left(int i) {
    return 2*i+1;
}

int right (int i) {
    return 2*(i+1);
}


void maxHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int largest;

    numComparisons++;
    if (l <= heapSize && employees[l]->id > employees[i]->id) {
        largest = l;
    } else largest = i;

    numComparisons++;
    if (r <= heapSize && employees[r]->id > employees[largest]->id) {
        largest = r;
    }

    if (largest != i) {
        auto temp = employees[i];
        employees[i] = employees[largest];
        employees[largest] = temp;
        maxHeapify(largest);
    }
}

void buildMaxHeap() {
    heapSize = employees.size()-1;
    for (int i = employees.size()/2; i >= 0; i--) {
        maxHeapify(i);
    }
}

void heapsort() {
    if (printIterative) printVector();
    buildMaxHeap();
    if (printIterative) printVector();

    for (int i = employees.size()-1; i > 0; i--) {
        auto temp = employees[0];
        employees[0] = employees[i];
        employees[i] = temp;
        heapSize--;
        maxHeapify(0);
        if (printIterative) printVector();
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
        heapSize = employees.size();
        ifs.close();

        {
            // while (true) {
            //     cout << "Please enter an employee's ID: (or -1 to quit): ";
            //     int x;
            //     cin >> x;
            //     if (x == -1) break;
            //     bool found = false;
            //     //This is tika's version - sequential search
            //     for (int i = 0; i < employees.size(); i++) {
            //         if (employees[i]->id == x) {
            //             cout << "Found it at index " << i << endl;
            //             cout << "Name: " << employees[i]->getName() << endl;
            //             cout << "Age: " << employees[i]->getAge() << endl;
            //             found = true;
            //         }
            //     }
            //     if (!found) {
            //         cout << "Employee not found..." << endl;
            //     }
            //     cout << "Found it at index " << i << endl;
            //
            //     cout << "Name: " << employees[x]->getName() << endl;
            //     cout << "Age: " << employees[x]->getAge() << endl;
            //
            // }
        }
    }

    // **************** SORT VECTOR AND STORE COMPARISONS **********************//
    //mergeSort(0, employees.size()-1);

    heapsort();


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





