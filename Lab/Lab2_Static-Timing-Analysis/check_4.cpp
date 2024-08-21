#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

bool compare_files(const string& file1, const string& file2) {
    ifstream f1(file1);
    ifstream f2(file2);

    if (!f1.is_open()) {
        cerr << "Error opening file: " << file1 << endl;
        return false;
    }
    if (!f2.is_open()) {
        cerr << "Error opening file: " << file2 << endl;
        return false;
    }

    string line1, line2;
    int line_number = 1;

    while (getline(f1, line1) && getline(f2, line2)) {
        if (line1 != line2) {
            cout << "Difference found at line " << line_number << ":\n";
            cout << "File 1: " << line1 << "\n";
            cout << "File 2: " << line2 << "\n";
            return false;
        }
        line_number++;
    }

    // Check if one file has more lines than the other
    if (getline(f1, line1) || getline(f2, line2)) {
        cout << "Files have different lengths.\n";
        return false;
    }

    cout << "Files are identical.\n";
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <file1> <file2>\n";
        return 1;
    }

    string file1 = argv[1];
    string file2 = argv[2];

    if (!compare_files(file1, file2)) {
        cout << "Files are not identical.\n";
        return 1;
    }

    cout << "Files match perfectly.\n";
    return 0;
}
