#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Transition structure
struct Transition {
    int fromState;
    int toState;
    char inputSymbol;
    char stackTop;
    string pushString;
};

// State structure
struct State {
    vector<Transition> transitions;
};

//Global Variables
vector<State> states;
vector<string> inputStrings;
stack<char> faStack;
int initialState = 0;
int currentState;

// Initialize the stack
void initStack() {
    while (!faStack.empty()) faStack.pop();
    faStack.push('Z'); // Stack bottom symbol
}

// Print stack content
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

// Push a string onto the stack (in reverse)
void pushSymbols(string symbols) {
    for (int i = symbols.size() - 1; i >= 0; i--) {
        if (symbols[i] != '~') {
            faStack.push(symbols[i]);
        }
    }
}

// Find a matching transition for current state, input symbol, and stack top
bool findTransition(State &s, char input, char top, Transition &found) {
    for (auto &t : s.transitions) {
        if (t.inputSymbol == input && t.stackTop == top) {
            found = t;
            return true;
        }
    }
    return false;
}

// Simulate the PDA for a given input string
bool simulatePDA(string input) {
    initStack();
    currentState = initialState;

    cout << "📦 Initial Stack: ";
    printStack(faStack);

    for (int i = 0; i < input.size(); ++i) {
        if (faStack.empty()) {
            cout << "❌ Stack underflow. REJECTED.\n";
            return false;
        }

        char currentInput = input[i];
        char stackTop = faStack.top();
        Transition t;

        cout << "\nStep " << i + 1 << ": Reading '" << currentInput << "', Stack Top: '" << stackTop << "'\n";

        if (findTransition(states[currentState], currentInput, stackTop, t)) {
            cout << "  ✅ Applying Transition: (q" << t.fromState << ", '" << t.inputSymbol << "', '" << t.stackTop
                 << "') -> (q" << t.toState << ", push \"" << t.pushString << "\")\n";

            faStack.pop();
            pushSymbols(t.pushString);
            currentState = t.toState;

            cout << "  📦 Stack after: ";
            printStack(faStack);
        } else {
            cout << "  ❌ No valid transition found. REJECTED.\n";
            return false;
        }
    }

    // ✅ Accept if stack top is Z and nothing else is above it
    if (faStack.size() == 1 && faStack.top() == 'Z') {
        cout << "\n✅ Stack reduced to base symbol 'Z'. ACCEPTED.\n";
        return true;
    } else {
        cout << "\n❌ Stack not reduced to 'Z'. REJECTED.\n";
        return false;
    }
}


int main() {
    int C, N, T;

    cout << "🔢 Enter number of input strings: ";
    cin >> C;

    cout << "🔢 Enter number of states: ";
    cin >> N;

    cout << "🔢 Enter number of transitions: ";
    cin >> T;

    states.resize(N);

    cout << "\n📜 Enter transitions in the format:\n";
    cout << "fromState toState inputSymbol stackTop pushString\n";
    cout << "(Example: 0 0 a Z AZ)\n";

    for (int i = 0; i < T; ++i) {
        int from, to;
        char input, stackTop;
        string pushStr;
        cout << "Transition " << i + 1 << ": ";
        cin >> from >> to >> input >> stackTop >> pushStr;
        states[from].transitions.push_back({from, to, input, stackTop, pushStr});
    }

    cin.ignore(); // Clear newline

    cout << "\n✍️ Enter " << C << " input strings:\n";
    for (int i = 0; i < C; ++i) {
        string line;
        cout << "String " << i + 1 << ": ";
        getline(cin, line);
        inputStrings.push_back(line); // No '~' needed
    }

    // Simulate PDA for each string
    for (int i = 0; i < C; ++i) {
        cout << "\n==============================\n";
        cout << "🧪 Test Case " << i + 1 << ": \"" << inputStrings[i] << "\"\n";
        bool result = simulatePDA(inputStrings[i]);
        cout << (result ? "✅ ACCEPTED" : "❌ REJECTED") << endl;
        cout << "==============================\n";
    }

    return 0;
}
