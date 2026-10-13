// Task 1: expression conversion and evaluation 
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Simple stack of strings (linked list)
struct Node { string val; Node* next; };
class Stack {
    Node* top = nullptr;
public:
    void push(string v) { top = new Node{v, top}; }
    string pop() { Node* t = top; string v = t->val; top = t->next; delete t; return v; }
    string peek() { return top->val; }
    bool empty() { return top == nullptr; }
    ~Stack() { while (!empty()) pop(); }
};

bool isOp(const string& t) { return t == "+" || t == "-" || t == "*" || t == "/" || t == "^"; }
int prec(const string& o) { return (o == "^") ? 3 : (o == "*" || o == "/") ? 2 : 1; }

// Split a string into tokens: operands (letters/digits/.), operators, parentheses
int tokenize(const string& s, string out[]) {
    int n = 0;
    for (size_t i = 0; i < s.size();) {
        if (s[i] == ' ') { i++; continue; }
        if (isalnum(s[i]) || s[i] == '.') {
            string t;
            while (i < s.size() && (isalnum(s[i]) || s[i] == '.')) t += s[i++];
            out[n++] = t;
        } else out[n++] = string(1, s[i++]);
    }
    return n;
}

string infix_to_postfix(string expr) {
    string tok[200]; int n = tokenize(expr, tok);
    Stack st; string res;
    for (int i = 0; i < n; i++) {
        string t = tok[i];
        if (t == "(") st.push(t);
        else if (t == ")") {
            while (!st.empty() && st.peek() != "(") res += st.pop() + " ";
            if (!st.empty()) st.pop();
        } else if (isOp(t)) {
            while (!st.empty() && st.peek() != "(" &&
                   (prec(st.peek()) > prec(t) || (prec(st.peek()) == prec(t) && t != "^")))
                res += st.pop() + " ";
            st.push(t);
        } else res += t + " ";
    }
    while (!st.empty()) res += st.pop() + " ";
    if (!res.empty()) res.pop_back();
    return res;
}

// Postfix to prefix (helper), then used by infix_to_prefix
string postfix_to_prefix(string expr) {
    string tok[200]; int n = tokenize(expr, tok);
    Stack st;
    for (int i = 0; i < n; i++) {
        if (isOp(tok[i])) { string b = st.pop(), a = st.pop(); st.push(tok[i] + " " + a + " " + b); }
        else st.push(tok[i]);
    }
    return st.pop();
}

string infix_to_prefix(string expr) { return postfix_to_prefix(infix_to_postfix(expr)); }

string postfix_to_infix(string expr) {
    string tok[200]; int n = tokenize(expr, tok);
    Stack st;
    for (int i = 0; i < n; i++) {
        if (isOp(tok[i])) { string b = st.pop(), a = st.pop(); st.push("(" + a + " " + tok[i] + " " + b + ")"); }
        else st.push(tok[i]);
    }
    return st.pop();
}

double apply(string op, double a, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    return pow(a, b);
}

double evaluate_postfix(string expr) {
    string tok[200]; int n = tokenize(expr, tok);
    Stack st;
    for (int i = 0; i < n; i++) {
        if (isOp(tok[i])) { double b = stod(st.pop()), a = stod(st.pop()); st.push(to_string(apply(tok[i], a, b))); }
        else st.push(tok[i]);
    }
    return stod(st.pop());
}

double evaluate_prefix(string expr) {
    string tok[200]; int n = tokenize(expr, tok);
    Stack st;
    for (int i = n - 1; i >= 0; i--) {          // scan right to left
        if (isOp(tok[i])) { double a = stod(st.pop()), b = stod(st.pop()); st.push(to_string(apply(tok[i], a, b))); }
        else st.push(tok[i]);
    }
    return stod(st.pop());
}

int main() {
    string infix = "(3 + 4) * 2 - 6 / 3";
    string post = infix_to_postfix(infix);
    string pre = infix_to_prefix(infix);
    cout << "Infix   : " << infix << endl;
    cout << "Postfix : " << post << endl;
    cout << "Prefix  : " << pre << endl;
    cout << "Back to infix (from postfix): " << postfix_to_infix(post) << endl;
    cout << "Evaluate postfix: " << evaluate_postfix(post) << endl;
    cout << "Evaluate prefix : " << evaluate_prefix(pre) << endl;

    string i2 = "A + B * C ^ D";
    cout << "\nInfix   : " << i2 << endl;
    cout << "Postfix : " << infix_to_postfix(i2) << endl;
    cout << "Prefix  : " << infix_to_prefix(i2) << endl;
    return 0;
}