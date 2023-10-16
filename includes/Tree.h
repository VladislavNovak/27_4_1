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

    void print(Node* node) const {
        if (node == nullptr) {
            cout << "Something is wrong: node not found" << endl;
            return; }

        int indent = node->getStage() > 0 ? (node->getStage() * 2) + 8 : 4;
        if (node->getId() == 0) { cout << std::setw(indent) << "-> " << node->getId() << " is Core"; }
        else {
            cout << std::setw(indent) << "(from #" << std::to_string(node->getParent()->getId()) + ")";
            cout << "-> #" << node->getId() << " (with name: " << node->getName() << ")";
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

    // constraints - список имен, которые нельзя давать/изменять для узла
    string changeNameOfNode(Node* &node, const vector<std::string> &constraints) {
        assert(node != nullptr);

        cout << "Do you want to set name?" << endl;
        if (selectMenuItem({"yes", "no"}) == 0) {
            while(true) {
                string name = putLineString("Enter name");
                capitalize(name);

                // hasNameOriginal возвращает true если имя оригинально для своего дерева
                // isIncludes проверяет по именам всех деревьев (список constraints)
                if (name == "None" || (hasNameOriginal(name) && !isIncludes(constraints, name))) {
                    cout << "ACTION: name updated successfully" << endl;
                    node->setName(name);
                    return name;
                }

                cout << "A similar name already exists. Try again." << endl;
            }
        }

        return "";
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

    // Меняет имя указанного узла. Возвращает старое имя и новое.
    // constraints - список имен, которые нельзя давать/изменять для узла
    vector<string> changeName(const vector<std::string> &constraints) {
        // Находим узел по (id > 0 && id <= counter):
        Node* foundNode = getNodeById(putNumeric({ 1, counter }, {}, "number of node"));
        // условие в putNumeric всегда обеспечивает foundNode!= nullptr
        assert(foundNode != nullptr);
        string oldName = foundNode->getName();
        cout << "Current name is " << oldName << ". ";
        string newName = changeNameOfNode(foundNode, constraints);
        vector<string> names = {(!oldName.empty() && oldName != "None") ? oldName : "",
                                (!newName.empty() && newName != "None") ? newName : "",
        };
        return names;
    }

    // Возвращает новое имя (либо пустую строку)
    // constraints - список имен, которые нельзя давать/изменять для узла
    string createNode(const vector<std::string> &constraints) {
        cout << "ACTION: creating a node!" << endl;
        Node* childNode = new Node(++counter);
        string name = changeNameOfNode(childNode, constraints);

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

        return (!name.empty() && name != "None") ? name : "";
    }

    // Возвращает новое имя (либо пустую строку)
    // constraints - список имен, которые нельзя давать/изменять для узла
    string generateNode(const vector<std::string> &constraints) {
        cout << "ACTION: generating a node!" << endl;
        Node* childNode = new Node(++counter);

        string name;
        // Если выпадет 0 (вероятность 33%), то автоматом будет присвоено None
        if (getRandomIntInRange(0, 2)) {
            while (true) {
                name = "Elf";
                name += std::to_string(getRandomIntInRange(1, 1000));
                if (hasNameOriginal(name) && !isIncludes(constraints, name)) {
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

        return (!name.empty() && name != "None") ? name : "";
    }

    // Позволяет найти соседние узлы по введенному имени:
    bool findNeighbors(const std::string &searchedName) {
        if (hasNameOriginal(searchedName) || searchedName == "None") { return false; }

        Node* childNode = getNodeByName(searchedName);
        // Проверка имени в hasNameOriginal должна исключить возникновение пустого объекта. Проверим:
        assert(childNode != nullptr);
        // Совпадения с core быть не может, но перестрахуемся
        if (childNode->getId() == 0) {
            cout << "Cannot select core node" << endl;
            return false;
        }

        Node* parent = childNode->getParent();
        vector<string> names;
        for (const auto &child : parent->getChildren()) {
            string childName = child->getName();
            // Добавляем, если это не сам объект поиска и если сосед имеет имя:
            if (childName != searchedName && childName != "None") { names.emplace_back(childName); }
        }

        if (names.empty()) { cout << "No neighbors" << endl; }
        else {
            cout << names.size() << " neighbor"  << (names.size() > 1 ? "s" : "") << " found: " << endl;
            cout << joinListToStream(names).str() << endl;
        }

        return true;
    }

    void printTree() const {
        cout << "STATUS: print tree!" << endl;
        print(core);
    }
};

#endif //INC_27_4_1_TREE_H
