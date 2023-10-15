#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "includes/utilities.h"
#include "includes/constants.h"
#include "includes/Tree.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

int main() {
    std::srand(std::time(nullptr)); // NOLINT(cert-msc51-cpp)
    std::vector<std::string> mainMenu = { "add_branch", "print_tree", "change_name", "find_neighbors", "exit" };
    // Содержит коллекцию всех имен. Нужно для анализа всех tree
    std::vector<std::string> namesCollection;
    string currentName;

    Tree tree;
    // Генерируем произвольные узлы для дерева
    for (int i = 0; i < 10; ++i) {
        currentName = tree.generateNode();
        if (!currentName.empty()) { namesCollection.emplace_back(currentName); }
    }

    cout << "LOG: Collection: " << endl;
    for (const string &name : namesCollection) { cout << name << ", "; }
    cout << endl;

    while(true) {
        std::cout << "--- Main menu ---" << std::endl;
        int command = selectMenuItem(mainMenu);

        if (command == static_cast<int>(Menu::ADD)) {
            std::cout << "Menu --> add mode -->" << std::endl;
            currentName = tree.createNode();
            if (!currentName.empty()) { namesCollection.emplace_back(currentName); }

            cout << "LOG: Collection: " << endl;
            for (const string &name : namesCollection) { cout << name << ", "; }
            cout << endl;
        }
        else if (command == static_cast<int>(Menu::PRINT)) {
            std::cout << "Menu --> print mode -->" << std::endl;
            tree.printTree();
        }
        else if (command == static_cast<int>(Menu::CHANGE)) {
            std::cout << "Menu --> change mode -->" << std::endl;
            auto names = tree.changeName();

            if (!names[0].empty()) { removeKeyFromVector(names[0], namesCollection); }
            if (!names[1].empty()) { namesCollection.emplace_back(names[1]); }

            cout << "LOG: Collection: " << endl;
            for (const string &name : namesCollection) { cout << name << ", "; }
            cout << endl;
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