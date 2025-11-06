#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Helper function: check if a string is a valid double
bool isValidDouble(const string& s) {
    if (s.empty()) return false;
    int i = 0;
    if (s[i] == '+' || s[i] == '-') i++; // optional sign
    if (i == s.size()) return false;

    bool hasDigitsBefore = false;
    bool hasDigitsAfter = false;
    bool hasDot = false;

    for (; i < (int)s.size(); i++) {
        char c = s[i];
        if (isdigit(c)) {
            if (!hasDot) hasDigitsBefore = true;
            else hasDigitsAfter = true;
        } else if (c == '.') {
            if (hasDot) return false; // only one dot allowed
            hasDot = true;
        } else {
            return false; // invalid character
        }
    }

    if (hasDot) return (hasDigitsBefore && hasDigitsAfter);
    return hasDigitsBefore;
}

// Remove leading zeros from integer part
string trimLeadingZeros(const string& s) {
    size_t start = 0;
    while (start < s.size() - 1 && s[start] == '0') start++;
    return s.substr(start);
}

// Remove trailing zeros from fractional part
string trimTrailingZeros(const string& s) {
    int end = s.size() - 1;
    while (end >= 0 && s[end] == '0') end--;
    return s.substr(0, end + 1);
}

// Add two positive integer strings (no decimals)
string addIntegerStrings(const string& a, const string& b) {
    string A = a, B = b;
    if (A.size() < B.size()) swap(A, B);
    int diff = A.size() - B.size();
    B.insert(0, diff, '0');

    string result = "";
    int carry = 0;
    for (int i = A.size() - 1; i >= 0; i--) {
        int sum = (A[i] - '0') + (B[i] - '0') + carry;
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    if (carry) result.push_back(carry + '0');
    reverse(result.begin(), result.end());
    return result;
}

// Subtract two positive integer strings (a >= b)
string subtractIntegerStrings(const string& a, const string& b) {
    string A = a, B = b;
    int diff = A.size() - B.size();
    B.insert(0, diff, '0');

    string result = "";
    int borrow = 0;
    for (int i = A.size() - 1; i >= 0; i--) {
        int digit = (A[i] - '0') - (B[i] - '0') - borrow;
        if (digit < 0) {
            digit += 10;
            borrow = 1;
        } else borrow = 0;
        result.push_back(digit + '0');
    }
    reverse(result.begin(), result.end());
    return trimLeadingZeros(result);
}

// Compare two positive integer strings: returns true if a >= b
bool greaterOrEqual(const string& a, const string& b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return a >= b;
}

// Add two decimal strings (both positive)
string addPositiveDecimals(string a, string b) {
    // Split into integer and fraction
    size_t dot1 = a.find('.');
    size_t dot2 = b.find('.');
    string int1 = (dot1 == string::npos) ? a : a.substr(0, dot1);
    string frac1 = (dot1 == string::npos) ? "" : a.substr(dot1 + 1);
    string int2 = (dot2 == string::npos) ? b : b.substr(0, dot2);
    string frac2 = (dot2 == string::npos) ? "" : b.substr(dot2 + 1);

    // Pad fractional parts
    int maxFrac = max(frac1.size(), frac2.size());
    frac1.append(maxFrac - frac1.size(), '0');
    frac2.append(maxFrac - frac2.size(), '0');

    string fracSum = addIntegerStrings(frac1, frac2);
    int carry = 0;
    if (fracSum.size() > maxFrac) {
        carry = 1;
        fracSum = fracSum.substr(1);
    }

    string intSum = addIntegerStrings(int1, int2);
    if (carry) intSum = addIntegerStrings(intSum, "1");

    fracSum = trimTrailingZeros(fracSum);

    return fracSum.empty() ? intSum : intSum + "." + fracSum;
}

// Subtract decimal strings: a - b, both positive, a >= b
string subtractPositiveDecimals(string a, string b) {
    size_t dot1 = a.find('.');
    size_t dot2 = b.find('.');
    string int1 = (dot1 == string::npos) ? a : a.substr(0, dot1);
    string frac1 = (dot1 == string::npos) ? "" : a.substr(dot1 + 1);
    string int2 = (dot2 == string::npos) ? b : b.substr(0, dot2 + 1);
    string frac2 = (dot2 == string::npos) ? "" : b.substr(dot2 + 1);

    int maxFrac = max(frac1.size(), frac2.size());
    frac1.append(maxFrac - frac1.size(), '0');
    frac2.append(maxFrac - frac2.size(), '0');

    string fracResult = "";
    int borrow = 0;
    for (int i = maxFrac - 1; i >= 0; i--) {
        int digit = (frac1[i] - '0') - (frac2[i] - '0') - borrow;
        if (digit < 0) {
            digit += 10;
            borrow = 1;
        } else borrow = 0;
        fracResult.push_back(digit + '0');
    }
    reverse(fracResult.begin(), fracResult.end());
    fracResult = trimTrailingZeros(fracResult);

    string intResult = subtractIntegerStrings(int1, int2);
    if (borrow) intResult = subtractIntegerStrings(intResult, "1");

    return fracResult.empty() ? intResult : intResult + "." + fracResult;
}

// Main addition handling signs
string addStrings(string a, string b) {
    bool negA = a[0] == '-';
    bool negB = b[0] == '-';
    if (a[0] == '+' || a[0] == '-') a = a.substr(1);
    if (b[0] == '+' || b[0] == '-') b = b.substr(1);

    if (!negA && !negB) return addPositiveDecimals(a, b);
    if (negA && negB) return "-" + addPositiveDecimals(a, b);
    if (negA && !negB) {
        if (greaterOrEqual(b, a)) return subtractPositiveDecimals(b, a);
        else return "-" + subtractPositiveDecimals(a, b);
    }
    // !negA && negB
    if (greaterOrEqual(a, b)) return subtractPositiveDecimals(a, b);
    else return "-" + subtractPositiveDecimals(b, a);
}

int main() {
    string filename;
    cout << "Enter input filename: ";
    cin >> filename;

    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Error: Cannot open file.\n";
        return 1;
    }

    string num1, num2;
    int lineNum = 1;

    while (infile >> num1 >> num2) {
        cout << "Line " << lineNum++ << ": " << num1 << " + " << num2 << endl;

        if (!isValidDouble(num1) || !isValidDouble(num2)) {
            cout << "  Error: invalid number format\n" << endl;
            continue;
        }

        string result = addStrings(num1, num2);
        cout << "  Result = " << result << "\n" << endl;
    }

    infile.close();
    return 0;
}
