#ifndef INC_27_4_1_TREE_H
#define INC_27_4_1_TREE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
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
            cout << std::setw(indent) << "-> #" << node->getId();
            cout << " with name: " << node->getName() << " (from #";
            cout << std::to_string(node->getParent()->getId()) + ")";
        }
        if (node->getNumberOfChildren()) {
            cout << ": " << endl;
            for (const auto & child : node->getChildren()) { print(child); }
        }
        else { cout << endl; }
    }

    // searchMode указывает признак (name/id), по которому идёт поиск (param). Найденный узел передаётся в foundNode.
    // Вспомогательная для getNodeById и getNodeByName
    void findNodeBy(SearchMode searchMode, const string &param, Node* current, Node* &foundNode) {
        // Если узел, в котором ищем - пустой, или узел найден - выходим:
        if (current == nullptr || foundNode != nullptr) { return; }
        // По режиму SearchMode проверяем совпадение по name или по id
        if ((searchMode == SearchMode::NAME && current->getName() == param) ||
            (searchMode == SearchMode::ID && std::to_string(current->getId()) == param)) {
            foundNode = current;
            return;
        }

        if (current->getNumberOfChildren()) {
            for (const auto &child : current->getChildren()) { findNodeBy(searchMode, param, child, foundNode); }
        }
    }

    // Позволяет найти узел по id. Вспомогательная - findNodeBy
    Node* getNodeById(int id) {
        Node* foundNode = nullptr;
        findNodeBy(SearchMode::ID, std::to_string(id), core, foundNode);
        return foundNode;
    }

    // Позволяет найти узел по name. Вспомогательная - findNodeBy
    Node* getNodeByName(const string &str) {
        Node* foundNode = nullptr;
        findNodeBy(SearchMode::NAME, str, core, foundNode);
        return foundNode;
    }

    // Нужна для поиска по имени по всему дереву. Вспомогательная для hasNameOriginal
    void nameOriginalStatus(Node* node, const string &searchedName, bool &status) {
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
            for (const auto &child : node->getChildren()) { nameOriginalStatus(child, searchedName, status); }
        }
    }

    bool hasNameOriginal(const string &searchedName) {
        bool isOriginal = true;
        nameOriginalStatus(core, searchedName, isOriginal);
        return isOriginal;
    }

    void changeNameOfNode(Node* &node) {
        assert(node != nullptr);

        cout << "Do you want to set name?" << endl;
        if (selectMenuItem({"yes", "no"}) == 0) {
            while(true) {
                string name = putLineString("Enter name");
                capitalize(name);
                if (name == "None" || hasNameOriginal(name)) {
                    node->setName(name);
                    cout << "ACTION: name updated successfully" << endl;
                    break;
                }

                cout << "A similar name already exists. Try again." << endl;
            }
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
        // Находим узел по (id > 0 && id <= counter):
        Node* foundNode = getNodeById(putNumeric({ 1, counter }, {}, "number of node"));
        // условие в putNumeric всегда обеспечивает foundNode!= nullptr
        assert(foundNode != nullptr);
        cout << "Current name is " << foundNode->getName() << ". ";
        changeNameOfNode(foundNode);
    }

    void createNode() {
        cout << "ACTION: creating a node!" << endl;
        Node* childNode = new Node(++counter);
        changeNameOfNode(childNode);

        if (!core->getNumberOfChildren()) {
            cout << "ACTION: node (#1) added to -> (#0)!" << endl;
            core->setChild(childNode);
        } else {
            // Находим узел по (id == 0 && id <= (counter - 1)):
            Node* parentNode = getNodeById(putNumeric({ 0, counter - 1 }, {}, "number of parent node"));
            // условие в putNumeric всегда обеспечивает foundNode!= nullptr
            assert(parentNode != nullptr);

            parentNode->setChild(childNode);
            cout << "ACTION: node (#" << childNode->getId() << ") added to -> (#" << parentNode->getId() << ")" << endl;
        }
    }

    void generateNode() {
        cout << "ACTION: generating a node!" << endl;
        Node* childNode = new Node(++counter);

        // Если выпадет 0 (вероятность 33%), то автоматом будет присвоено None
        if (getRandomIntInRange(0, 2)) {
            while (true) {
                string name;
                name = "Elf";
                name += std::to_string(getRandomIntInRange(1, 1000));
                if (hasNameOriginal(name)) {
                    cout << "ACTION: name (" << name << ") updated successfully" << endl;
                    childNode->setName(name);
                    break;
                }
            }
        }

        // Ранее для объекта была уже выделена память. Перестрахуемся:
        assert(childNode != nullptr);

        if (!core->getNumberOfChildren()) {
            cout << "ACTION: node (#1) added to -> (#0)!" << endl;
            core->setChild(childNode);
        } else {
            // Находим узел по (id == 0 && id <= (counter - 1)):
            Node* parentNode = getNodeById(getRandomIntInRange(0, counter - 1));
            // условие в getRandomIntInRange всегда обеспечивает foundNode!= nullptr
            assert(parentNode != nullptr);

            parentNode->setChild(childNode);
            cout << "ACTION: node (#" << childNode->getId() << ") added to -> (#" << parentNode->getId() << ")" << endl;
        }
    }

    void findNeighbors() {
        string searchedName;
        while (true) {
            searchedName = putLineString("Enter name");
            capitalize(searchedName);
            // Если имя существует, то ок
            if (!hasNameOriginal(searchedName) && searchedName != "None") { break; }
            cout << "Name not found. Try again. ";
        }

        Node* childNode = getNodeByName(searchedName);
        // Цикл while с проверкой имени должен исключить возникновение пустого объекта. Проверим:
        assert(childNode != nullptr);
        // Совпадения с core быть не может, но перестрахуемся
        if (childNode->getId() == 0) { cout << "Cannot select core node" << endl; }
        Node* parent = childNode->getParent();
        if (!parent->getNumberOfChildren()) { cout << "No neighbors" << endl; }
        else {
            cout << parent->getNumberOfChildren() - 1 << " neighbors found: " << endl;
            for (const auto &child : parent->getChildren()) {
                string childName = child->getName();
                if (childName != searchedName) { cout << childName << endl; }
            }
        }
    }

    void printTree() {
        cout << "STATUS: print tree!" << endl;
        print(core);
    }
};

#endif //INC_27_4_1_TREE_H
