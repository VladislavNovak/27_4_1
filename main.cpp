#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
// #include <cstdlib>
// #include <ctime>
#include "includes/utilities.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

enum class Menu { ADD, PRINT, FIND, EXIT };

class Node {
    // Позволяет позиционировать и добавлять новые узлы к старым
    int id;
    // Нужен для позиционирования при распечатывании всего дерева
    int stage = 0;
    string name = "None";
    Node* parent = nullptr;
    vector<Node*> children;
public:
    explicit Node(int inId) { id = inId; }
    ~Node() {
        if (!children.empty()) {
            for (auto &child : children) {
                cout << "STATUS: delete " << child->id << endl;
                delete child;
            }
            this->children.clear();
        }
    }
    void setChild(Node* child) {
        child->parent = this;
        child->stage = this->stage + 1;
        children.emplace_back(child);
    }
    void setName(string inName) {
        if (!name.empty()) {
            if (name == "None") { name = inName; }
            else {
                cout << "Current name is " << name << ". Are we going to change the name?" << endl;
                if (selectMenuItem({"yes", "no"}) == 0) { name = std::move(inName); }
            }
        } else { name = inName; }
    }
    std::string getName() { return name; }
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] int getStage() const { return stage; }
    [[nodiscard]] Node* getParent() const { return parent; }
    int getNumberOfChildren() { return (children.empty() ? 0 : (int)children.size()); }
    vector<Node*> getChildren() { return children; }
};

class Tree {
    Node* core = nullptr;
    // Содержит текущий максимальный свободный id
    int counter = 0;
    void print(Node* node) {
        if (node == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }
        int indent = node->getStage() > 0 ? (node->getStage() * 2) + 4 : 4;
        if (node->getId() == 0) { cout << std::setw(indent) << "-> " << node->getId() << " is Core"; }
        else {
            cout << std::setw(indent) << "-> " << node->getId();
            cout << " with name: " << node->getName() << " (from " + std::to_string(node->getParent()->getId()) + ")";
        }
        if (node->getNumberOfChildren()) {
            cout << ": " << endl;
            for (const auto & child : node->getChildren()) { print(child); }
        }
        else { cout << endl; }
    }
    void push(Node* current, int targetId, Node* newNode, bool &isFound) {
        if (current == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }
        if (isFound) { return; }
        if (current->getId() == targetId) {
            cout << "STATUS: added " << newNode->getId() << " -> to " << targetId << endl;
            current->setChild(newNode);
            isFound = true;
        }
        else if (current->getNumberOfChildren()) {
            for (const auto &child : current->getChildren()) { push(child, targetId, newNode, isFound); }
        }
    }
    void findNameAmongNodes(Node* node, const string &searchedName, bool &status) {
        if (node == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }
        if (!status) { return; }
        if (node->getName() == searchedName) {
            // Если найдено совпадение, значит переданное имя - не оригинальное
            status = false;
            return;
        }
        if (node->getNumberOfChildren()) {
            for (const auto &child : node->getChildren()) { findNameAmongNodes(child, searchedName, status); }
        }
    }
    bool hasOriginalityName(const string &searchedName) {
        bool isOriginal = true;
        findNameAmongNodes(core, searchedName, isOriginal);
        return isOriginal;
    }
    void find(Node* node, const string &searchedName, bool &isFound) {
        if (node == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            // Если произойдёт зацикливание, присвоить значение переменной isFound
            return; }
        if (isFound) { return; }
        if (node->getName() == searchedName) {
            isFound = true;
            // Совпадения с core быть не может, но перестрахуемся
            if (node->getId() == 0) { cout << "Cannot select core node" << endl; }
            Node* parent = node->getParent();
            if (!parent->getNumberOfChildren()) { cout << "No neighbors" << endl; }
            else {
                cout << parent->getNumberOfChildren() - 1 << " neighbors found: " << endl;
                for (const auto &child : parent->getChildren()) {
                    string childName = child->getName();
                    if (childName != searchedName) { cout << childName << endl; }
                }
            }
        }
        else if (node->getNumberOfChildren()) {
            for (const auto &child : node->getChildren()) { find (child, searchedName, isFound); }
        }
    }
public:
    Tree() {
        core = new Node(0);
        cout << "STATUS: created a tree and a trunk node" << endl;
    }
    ~Tree() {
        cout << "STATUS: delete tree and children nodes" << endl;
        delete core;
    }
    void createNode() {
        cout << "STATUS: creating a node!" << endl;
        Node* childNode = new Node(++counter);

        cout << "Do you want to set name?" << endl;
        if (selectMenuItem({"yes", "no"}) == 0) {
            while(true) {
                string name = putLineString("Enter name");
                capitalize(name);
                if (name == "None" || hasOriginalityName(name)) {
                    childNode->setName(name);
                    break;
                }

                cout << "A similar name already exists. Try again." << endl;
            }
        }

        if (core == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            // Если произойдёт зацикливание, присвоить значение переменной isFound
            return; }
        else if (!core->getNumberOfChildren()) {
            cout << "STATUS: node (1) added to (trunk of tree)!" << endl;
            core->setChild(childNode);
        } else {
            bool isFound = false;
            int targetId = putNumeric({0, counter - 1}, {}, "number of parent node");
            push(core, targetId, childNode, isFound);
        }
    }
    void findNeighbors() {
        bool isFound = false;
        string searchedName = putLineString("Enter name");
        capitalize(searchedName);
        find(core, searchedName, isFound);
        if (!isFound) { cout << "Name not found" << endl; }
    }
    void printTree() {
        cout << "STATUS: print tree!" << endl;
        print(core);
    }
};

int main() {
    std::vector<std::string> mainMenu = { "add_branch", "print_tree", "find_neighbors", "exit" };

    Tree tree;
    while(true) {
        std::cout << "--- Main menu ---" << std::endl;
        int command = selectMenuItem(mainMenu);

        if (command == static_cast<int>(Menu::ADD)) {
            std::cout << "Menu --> add mode -->" << std::endl;
            tree.createNode();
        }
        else if (command == static_cast<int>(Menu::PRINT)) {
            std::cout << "Menu --> print mode -->" << std::endl;
            tree.printTree();
        }
        else if (command == static_cast<int>(Menu::FIND)) {
            std::cout << "Menu --> find mode -->" << std::endl;
            tree.findNeighbors();
        }
        else if (command == static_cast<int>(Menu::EXIT)) {
            std::cout << "Menu --> exit mode -->" << std::endl;
            break;
        }
    }
}