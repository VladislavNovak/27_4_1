#ifndef INC_27_4_1_TREE_H
#define INC_27_4_1_TREE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include "utilities.h"
#include "Node.h"
#include "constants.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;


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

    void findNodeById(int targetId, Node* current, Node* &foundNode) {
        if (current == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }
        if (foundNode != nullptr) { return; }
        if (current->getId() == targetId) {
            foundNode = current;
            return;
        }
        else if (current->getNumberOfChildren()) {
            for (const auto &child : current->getChildren()) { findNodeById(targetId, child, foundNode); }
        }
    }

    // Добавляет новый узел. Вспомогательная для createNode/generateNode
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

    // Нужна для поиска по имени по всему дереву. Вспомогательная для hasOriginalityName
    void findByName(Node* node, const string &searchedName, bool &status) {
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
            for (const auto &child : node->getChildren()) { findByName(child, searchedName, status); }
        }
    }

    bool hasOriginalityName(const string &searchedName) {
        bool isOriginal = true;
        findByName(core, searchedName, isOriginal);
        return isOriginal;
    }

    // Нужна для поиска соседей. Вспомогательная для findNeighbors
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
    void changeName() {
        Node* foundNode = nullptr;
        int nodeId = putNumeric({1, counter}, {}, "number of node");
        findNodeById(nodeId, core, foundNode);
        if (foundNode == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }

        cout << "Current name is " << foundNode->getName() << ". ";
        cout << "Do you want to set name?" << endl;
        if (selectMenuItem({"yes", "no"}) == 0) {
            while(true) {
                string name = putLineString("Enter name");
                capitalize(name);
                if (name == "None" || hasOriginalityName(name)) {
                    foundNode->setName(name);
                    break;
                }

                cout << "A similar name already exists. Try again." << endl;
            }
        }
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
    void generateNode() {
        cout << "STATUS: generating a node!" << endl;
        Node*childNode = new Node(++counter);

        // Если выпадет 0 (вероятность 33%), то автоматом будет присвоено None
        if (getRandomIntInRange(0, 2)) {
            while (true) {
                string name;
                name = "Elf";
                name += std::to_string(getRandomIntInRange(1, 1000));
                if (hasOriginalityName(name)) {
                    cout << "Name is: " << name << endl;
                    childNode->setName(name);
                    break;
                }
            }
        }

        if (!core->getNumberOfChildren()) {
            cout << "STATUS: node (1) generate/added to (trunk of tree)!" << endl;
            core->setChild(childNode);
        } else {
            bool isFound = false;
            int targetId = getRandomIntInRange(0, counter - 1);
            cout << "target id: " << targetId << endl;
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

#endif //INC_27_4_1_TREE_H
