#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

struct Transition {
    int fromState;
    int toState;
    char inputSymbol;
    char stackTop;
    string pushString;
};

struct State {
    vector<Transition> transitions;
};

vector<State> states;
vector<string> inputStrings;

stack<char> faStack;
int initialState = 0;
int currentState;

void initStack() {
    while (!faStack.empty()) faStack.pop();
    faStack.push('Z'); // Stack bottom symbol
}

void printStack(stack<char> s) {
    vector<char> temp;
    while (!s.empty()) {
        temp.push_back(s.top());
        s.pop();
    }
    reverse(temp.begin(), temp.end());
    for (char c : temp) cout << c;
    cout << endl;
}

void pushSymbols(string symbols) {
    for (int i = symbols.size() - 1; i >= 0; i--) {
        if (symbols[i] != '~') {
            faStack.push(symbols[i]);
        }
    }
}

bool findTransition(State &s, char input, char top, Transition &found) {
    for (auto &t : s.transitions) {
        if (t.inputSymbol == input && t.stackTop == top) {
            found = t;
            return true;
        }
    }
    return false;
}

bool simulatePDA(string input) {
    initStack();
    currentState = initialState;

    cout << "Initial Stack: ";
    printStack(faStack);

    for (int i = 0; i < input.size(); ++i) {
        char currentInput = input[i];
        char stackTop = faStack.top();
        Transition t;

        cout << "Step " << i + 1 << ": Read '" << currentInput << "', Stack Top: '" << stackTop << "'\n";

        if (findTransition(states[currentState], currentInput, stackTop, t)) {
            cout << "  Applying Transition: (" << t.fromState << ", '" << t.inputSymbol << "', '" << t.stackTop
                 << "') -> (" << t.toState << ", push \"" << t.pushString << "\")\n";

            faStack.pop();          // Pop stack top
            pushSymbols(t.pushString);  // Push new symbols
            currentState = t.toState;

            cout << "  Stack after: ";
            printStack(faStack);
        } else {
            cout << "  No valid transition found. REJECTED.\n";
            return false;
        }
    }

    if (faStack.top() == 'Z') {
        cout << "Final Stack Top: Z (Bottom reached). ACCEPTED.\n";
        return true;
    } else {
        cout << "Stack not empty after input. REJECTED.\n";
        return false;
    }
}

int main() {
    int C;
    cout << "Enter number of input strings: ";
    cin >> C;

    int N, T;
    cout << "Enter number of states: ";
    cin >> N;
    cout << "Enter number of transitions: ";
    cin >> T;

    states.resize(N);

    cout << "\nEnter transitions in the format: fromState toState inputChar stackTop pushString\n";
    cout << "(Use '~' to indicate epsilon push)\nTransition 1: 0 0 a Z AZ \nTransition 2: 0 0 a A AA)\n";
    for (int i = 0; i < T; ++i) {
        int from, to;
        char input, stackTop;
        string pushStr;
        cout << "Transition " << i + 1 << ": ";
        cin >> from >> to >> input >> stackTop >> pushStr;
        states[from].transitions.push_back({from, to, input, stackTop, pushStr});
    }

    cin.ignore(); // Clear newline before reading input strings

    cout << "\nEnter " << C << " input strings:\n";
    for (int i = 0; i < C; ++i) {
        string line;
        cout << "String " << i + 1 << ": ";
        getline(cin, line);
        inputStrings.push_back(line + "~"); // Append ~ to mark end of input
    }

    // Simulate PDA for each input string
    for (int i = 0; i < C; ++i) {
        cout << "\n==============================\n";
        cout << "Case " << i + 1 << ": \"" << inputStrings[i].substr(0, inputStrings[i].size() - 1) << "\"\n";
        bool result = simulatePDA(inputStrings[i]);
        cout << (result ? "ACCEPTED ✅" : "REJECTED ❌") << endl;
        cout << "==============================\n";
    }

    return 0;
}

