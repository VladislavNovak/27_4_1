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

struct Node {
    // Позволяет позиционировать и добавлять новые узлы к старым
    int id;
    // Нужен для позиционирования при распечатывании всего дерева
    int stage = 0;
    string name = "None";
    Node* parent = nullptr;
    vector<Node*> children;
    explicit Node(int inId) { id = inId; }
    ~Node() {
        if (!children.empty()) {
            for (auto &child : children) {
                // cout << "STATUS: delete " << child->id << endl;
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
};

struct Tree {
    Node* core = nullptr;
    // Содержит текущий максимальный свободный id
    int counter = 0;
    Tree() {
        core = new Node(0);
        cout << "STATUS: created a tree and a trunk node" << endl;
    }
    ~Tree() {
        cout << "STATUS: delete tree and children nodes" << endl;
        delete core;
    }
    void print(Node* node) {
        int indent = node->stage > 0 ? (node->stage * 2) + 4 : 4;
        cout << std::setw(indent) << "-" << node->id << " with name: " << node->name;
        cout << ((node->parent != nullptr) ? " (from " + std::to_string(node->parent->id) + ")": " is CORE");
        // cout << " (from " << ((node->parent != nullptr) ? std::to_string(node->parent->id) : "") << ")";
        if (!node->children.empty()) {
            cout << ": " << endl;
            for (const auto & child : node->children) { print(child); }
        }
        else { cout << endl; }
    }
    void push(Node* current, int targetId, Node* newNode, bool &isFound) {
        if (isFound) { return; }
        if (current->id == targetId) {
            cout << "STATUS: added " << newNode->id << " -> to " << targetId << endl;
            current->setChild(newNode);
            isFound = true;
        }
        else if (!current->children.empty()) {
            for (const auto &child : current->children) { push(child, targetId, newNode, isFound); }
        }
    }
    void findNameAmongNodes(Node* node, const string &newName, bool &status) {
        if (!status) { return; }
        if (node->name == newName) {
            // Если найдено совпадение, значит переданное имя - не оригинальное
            status = false;
            return;
        }
        if (!node->children.empty()) {
            for (const auto &child : node->children) { findNameAmongNodes(child, newName, status); }
        }
    }
    bool hasOriginalityName(const string &newName) {
        bool isOriginal = true;
        findNameAmongNodes(core, newName, isOriginal);
        return isOriginal;
    }
    void createNode() {
        cout << "STATUS: creating a node!" << endl;
        Node* childNode = new Node(++counter);

        cout << "Do you want to set name?" << endl;
        if (selectMenuItem({"yes", "no"}) == 0) {
            while(true) {
                string name = putLineString("Enter name");
                capitalize(name);
                // Здесь: поднимем первую букву в верхний регистр
                if (name == "None" || hasOriginalityName(name)) {
                    childNode->setName(name);
                    break;
                }

                cout << "A similar name already exists. Try again." << endl;
            }
        }

        if (core->children.empty()) {
            cout << "STATUS: node (1) added to (trunk of tree)!" << endl;
            core->setChild(childNode);
        } else {
            bool isFound = false;
            int targetId = putNumeric({0, counter - 1}, {}, "number of parent node");
            push(core, targetId, childNode, isFound);
        }
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