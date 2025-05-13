#include "mainwindow.h"
#include <QVBoxLayout>
#include <QString>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct transition {
    int O;
    int F;
    char L;
    char E;
    string D;
};

struct state {
    vector<transition> transitions;
};

stack<char> faStack;
vector<state> states;
int initialState = 0;
int currentState;

// GUI constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    inputEdit = new QTextEdit(this);
    inputEdit->setPlaceholderText("Enter input string here...");
    processButton = new QPushButton("Test String", this);
    outputBrowser = new QTextBrowser(this);

    layout->addWidget(inputEdit);
    layout->addWidget(processButton);
    layout->addWidget(outputBrowser);

    setCentralWidget(central);
    setWindowTitle("PDA Simulator");

    connect(processButton, &QPushButton::clicked, this, &MainWindow::processInput);

    setupAutomaton(); // initialize FA transitions
}

MainWindow::~MainWindow() {}

// Initialize transitions (example: simple PDA)
void MainWindow::setupAutomaton()
{
    states.clear();
    for (int i = 0; i < 2; ++i) {
        state s;
        states.push_back(s);
    }

    // Transition: q0 --a/Z-> q0 with AZ (push A)
    states[0].transitions.push_back({0, 0, 'a', 'Z', "AZ"});

    // Transition: q0 --a/A-> q0 with AA (push A)
    states[0].transitions.push_back({0, 0, 'a', 'A', "AA"});

    // Transition: q0 --b/A-> q1 with ~ (pop A)
    states[0].transitions.push_back({0, 1, 'b', 'A', "~"});

    // Transition: q1 --b/A-> q1 with ~ (continue popping)
    states[1].transitions.push_back({1, 1, 'b', 'A', "~"});
}

// Stack functions
void initStack() {
    while (!faStack.empty()) faStack.pop();
    faStack.push('Z');
}

void applyTransition(transition t) {
    faStack.pop();
    for (int i = t.D.size() - 1; i >= 0; --i) {
        if (t.D[i] != '~') faStack.push(t.D[i]);
    }
    currentState = t.F;
}

bool testDefinedTransition(state s, char c, char t, int &index) {
    for (int i = 0; i < s.transitions.size(); ++i) {
        if (c == s.transitions[i].L && t == s.transitions[i].E) {
            index = i;
            return true;
        }
    }
    return false;
}

bool MainWindow::testInputString(const string &input)
{
    initStack();
    currentState = initialState;
    int index;
    char stackTop = faStack.top();

    for (int i = 0; i < input.size(); ++i) {
        if (testDefinedTransition(states[currentState], input[i], stackTop, index)) {
            transition t = states[currentState].transitions[index];
            applyTransition(t);
            stackTop = faStack.top();
        } else {
            return false;
        }
    }

    // Accept if at final state (e.g., q1) and only Z remains
    return currentState == 1 && faStack.top() == 'Z';
}

// GUI event
void MainWindow::processInput()
{
    QString qstr = inputEdit->toPlainText().trimmed();
    std::string input = qstr.toStdString();

    bool accepted = testInputString(input);
    outputBrowser->clear();
    outputBrowser->append("Input: " + qstr);
    outputBrowser->append(accepted ? "ACCEPTED ✅" : "REJECTED ❌");
}

