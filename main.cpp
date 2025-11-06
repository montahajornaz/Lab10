#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

bool isValidNumber(const string& s) {
    bool decimalFound = false;
    bool digitBeforeDecimal = false;
    bool digitAfterDecimal = false;
    int start = 0;

    if (s.empty()) return false;
    if (s[0] == '+' || s[0] == '-') start = 1;
    if (start == (int)s.size()) return false;

    for (int i = start; i < (int)s.size(); ++i) {
        if (s[i] == '.') {
            if (decimalFound) return false;
            decimalFound = true;
        } else if (isdigit(s[i])) {
            if (!decimalFound) digitBeforeDecimal = true;
            else digitAfterDecimal = true;
        } else {
            return false;
        }
    }
    if (decimalFound && (!digitBeforeDecimal || !digitAfterDecimal))
        return false;
    return digitBeforeDecimal || digitAfterDecimal;
}

int main() {
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: could not open file " << filename << endl;
        return 1;
    }

    string a, b;
    int lineNum = 1;

    while (infile >> a >> b) {
        cout << "Line " << lineNum << ": " << a << " + " << b << endl;

        if (!isValidNumber(a) || !isValidNumber(b)) {
            cout << "  Error: invalid number format\n" << endl;
            lineNum++;
            continue;
        }

        long double x, y;
        stringstream sa(a), sb(b);
        sa >> x;
        sb >> y;
        long double sum = x + y;

        cout << fixed << setprecision(6);
        cout << "  Result = " << sum << "\n" << endl;

        lineNum++;
    }

    infile.close();
    return 0;
}
