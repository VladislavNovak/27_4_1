#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
// #include <cstdlib>
// #include <ctime>
#include <iterator>

using std::cout;
using std::endl;
using std::vector;
using std::string;

template<typename T, typename N>
bool isIncludes(const T &range, const N &item) {
    return std::any_of(range.begin(),
                       range.end(),
                       [&item](const N &c) { return c == item; });
}

void resetBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int putInput() {
    int input;
    while (true) {
        std::cin >> input;
        if (std::cin.fail()) {
            std::cout << "Input is invalid. Please try again:";
            resetBuffer();
            continue;
        }
        break;
    }
    resetBuffer();
    return input;
}

template<typename T>
std::stringstream joinListToStream(const std::vector<T> &list, const char* delim = ", ") {
    std::stringstream ss;
    std::copy(list.begin(), std::prev(list.end()), std::ostream_iterator<T>(ss, delim));
    if (!list.empty()) { ss << list.back(); }
    return ss;
}

// Любое число - если ничего не передать в массиве.
// В диапазоне - если передать два числа (меньшее и большее) в массиве.
// В перечне - если передать массив НЕ из двух элементов или из двух, в котором первый элемент больше второго
// За исключением - если добавить перечень во второй аргумент

int putNumeric(const std::vector<int> &list = {}, const std::vector<int> &excludedList = {}, const std::string &msg = "") {
    bool isRange = (list.size() == 2) && (list[0] < list[1]);
    bool isList = !list.empty() && (list.size() != 2 || ((list.size() == 2) && (list[0] > list[1])));
    bool isExcluded = !excludedList.empty();

    std::cout << "Enter";
    if (isRange) std::cout << " (in the range "  << joinListToStream(list, " - ").str() << ")";
    else if (isList) std::cout << " (in a list of " << joinListToStream(list).str() << ")";
    if (isExcluded) std::cout << " (excluded " << joinListToStream(excludedList).str() << ")";
    std::cout << (msg.length() ? " " + msg + ":" : ":");

    int userInput;

    while (true) {
        userInput = putInput();

        bool isTrouble = false;
        if (isRange && (userInput < list[0] || userInput > list[1])) isTrouble = true;
        if (isList && !isIncludes(list, userInput)) isTrouble = true;
        if (isExcluded && isIncludes(excludedList, userInput)) isTrouble = true;

        if (!isTrouble) { break; }
        std::cout << "Error.Try again:";
    }
    return userInput;
}

struct Node {
    int id;
    int stage = 0;
    Node* parent = nullptr;
    vector<Node*> children;
    explicit Node(int inId) { id = inId; }
    ~Node() {
        if (!children.empty()) { for (auto &child : children) { delete child; } }
        this->children.clear();
    }
    void setChild(Node* child) {
        child->parent = this;
        child->stage = this->stage + 1;
        children.emplace_back(child);
    }
};

struct Tree {
    Node* core = nullptr;
    int counter = 0;
    Tree() {
        core = new Node(0);
        cout << "STATUS: created a tree and a trunk node" << endl;
    }
    void push(Node* current, int targetId, Node* newNode, bool &isFound) {
        if (isFound) { return; }
        if (current->id == targetId) {
            cout << "STATUS: node (" << newNode->id << ") added to (" << targetId <<")!" << endl;
            current->setChild(newNode);
            isFound = true;
        }
        else if (!current->children.empty()) {
            for (const auto &child : current->children) { push(child, targetId, newNode, isFound); }
        }
    }
    void createNode() {
        cout << "STATUS: creating a node!" << endl;
        Node* childNode = new Node(++counter);
        if (core->children.empty()) {
            cout << "STATUS: node (1) added to (trunk of tree)!" << endl;
            core->setChild(childNode);
        } else {
            bool isFound = false;
            int targetId = putNumeric({0, counter - 1}, {}, "number of parent node");
            push(core, targetId, childNode, isFound);
        }
    }
    void print(Node* node) {
        int indent = node->stage > 0 ? (node->stage * 2) + 4 : 2;
        cout << std::setw(indent) << "-" << node->id;
        cout << " (from " << ((node->parent != nullptr) ? std::to_string(node->parent->id) : "") << ")";
        if (!node->children.empty()) {
            cout << ": " << endl;
            for (const auto & child : node->children) { print(child); }
        }
        else { cout << endl; }
    }
    void printTree() {
        cout << "STATUS: print tree!" << endl;
        print(core);
    }
};

int main() {
    Tree tree;
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.createNode();
    tree.printTree();
}