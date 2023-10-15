#ifndef INC_27_4_1_NODE_H
#define INC_27_4_1_NODE_H

#include <iostream>
#include <vector>
#include "utilities.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

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
    void setName(string inName) { name = std::move(inName); }
    std::string getName() { return name; }
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] int getStage() const { return stage; }
    [[nodiscard]] Node* getParent() const { return parent; }
    int getNumberOfChildren() { return (children.empty() ? 0 : (int)children.size()); }
    vector<Node*> getChildren() { return children; }
};

#endif //INC_27_4_1_NODE_H
