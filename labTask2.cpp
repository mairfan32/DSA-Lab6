// Task 2: bracket validator and HTML/XML tag validator using a stack
#include <iostream>
#include <string>
#include <vector>
#include <utility>
using namespace std;

// Part A: stack of chars implemented with an array
bool isValidBrackets(string s) {
    char st[1000]; int top = -1;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') st[++top] = c;
        else if (c == ')' || c == '}' || c == ']') {
            if (top < 0) return false;
            char o = st[top--];
            if ((c == ')' && o != '(') || (c == '}' && o != '{') || (c == ']' && o != '[')) return false;
        }
    }
    return top == -1;
}

// Part B: stack of tag names
pair<bool, vector<string>> validate_markup(string s) {
    vector<string> errors;
    string st[500]; int top = -1;
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] != '<') { i++; continue; }
        size_t j = s.find('>', i);
        if (j == string::npos) { errors.push_back("Unclosed '<' at position " + to_string(i)); break; }
        string inside = s.substr(i + 1, j - i - 1);
        i = j + 1;
        if (inside.empty()) { errors.push_back("Empty tag"); continue; }
        if (inside.back() == '/') continue;                      // self-closing e.g. <br/>
        if (inside[0] == '/') {                                  // closing tag
            string name = inside.substr(1);
            if (top < 0) errors.push_back("Unexpected closing tag </" + name + ">");
            else if (st[top] != name) {
                errors.push_back("Mismatch: expected </" + st[top] + "> but found </" + name + ">");
                top--;                                           // recover by dropping the open tag
            } else top--;
        } else {                                                 // opening tag (ignore attributes)
            string name = inside.substr(0, inside.find(' '));
            st[++top] = name;
        }
    }
    while (top >= 0) errors.push_back("Tag <" + st[top--] + "> was never closed");
    return {errors.empty(), errors};
}

int main() {
    string b[] = {"{[()]}", "([)]", "((", "a(b)c{d}[e]"};
    for (string x : b) cout << x << " -> " << (isValidBrackets(x) ? "Valid" : "Invalid") << endl;

    cout << endl;
    string m[] = {"<html><body><p>Hi</p><br/></body></html>",
                  "<div><p>text</div></p>",
                  "<ul><li>one</ul>",
                  "</b>"};
    for (string x : m) {
        auto r = validate_markup(x);
        cout << x << " -> " << (r.first ? "Valid" : "Invalid") << endl;
        for (string e : r.second) cout << "   - " << e << endl;
    }
    return 0;
}