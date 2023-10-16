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
    std::vector<std::string> mainMenu = { "add", "print_tree", "print_all", "change", "find", "names", "exit" };
    std::vector<std::string> mainMenuTitles = {
            " -> " + mainMenu[0] + ":             create a new node and attach it to the tree",
            " -> " + mainMenu[1] + ":      will print the specified tree",
            " -> " + mainMenu[2] + ":       will print all trees",
            " -> " + mainMenu[3] + ":          will change the name of the specified node",
            " -> " + mainMenu[4] + ":            will find all neighboring nodes by name",
            " -> " + mainMenu[5] + ":           shows all reserved names",
            " -> " + mainMenu[6] + ":            to exit (all trees and nodes will be destroyed)",
    };
    // Содержит список исключений: коллекцию всех имен. Нужно для анализа всех tree
    std::vector<std::string> namesCollection;
    string currentName;

    std::cout << "--- Initialization ---" << std::endl;
    int numOfTrees = putNumeric({1, 5}, {}, "number of trees");

    // --->
    vector<Tree*> trees;
    trees.reserve(numOfTrees);

    for (int j = 0; j < numOfTrees; ++j) {
        Tree* tree = new Tree();
        // Генерируем произвольные узлы для дерева
        for (int i = 0; i < 10; ++i) {
            // Для конкретного узла даём имя
            currentName = tree->generateNode(namesCollection);
            // Если имя оригинальное, добавляем его в список исключений
            if (!currentName.empty()) { namesCollection.emplace_back(currentName); }
        }
        trees.emplace_back(tree);
    }

    while(true) {
        std::cout << "--- Main menu ---" << std::endl;
        cout << joinListToStream(mainMenuTitles, "\n").str() << endl;
        int command = selectMenuItem(mainMenu);

        if (command == static_cast<int>(Menu::ADD)) {
            std::cout << "Menu --> add mode -->" << std::endl;

            int item = putNumeric({ 0, numOfTrees - 1 }, {}, "number of tree");
            // nameCollection содержит имена, которые нельзя давать/изменять для узлов
            currentName = trees[item]->createNode(namesCollection);
            if (!currentName.empty()) { namesCollection.emplace_back(currentName); }
        }
        else if (command == static_cast<int>(Menu::PRINT)) {
            std::cout << "Menu --> print tree mode -->" << std::endl;

            int item = putNumeric({ 0, numOfTrees - 1 }, {}, "number of tree");
            trees[item]->printTree();
        }
        else if (command == static_cast<int>(Menu::PRINT_ALL)) {
            std::cout << "Menu --> print trees mode -->" << std::endl;

            for (int i = 0; i < numOfTrees; ++i) {
                cout << "Tree #" << i << ":" << endl;
                trees[i]->printTree();
            }
        }
        else if (command == static_cast<int>(Menu::CHANGE)) {
            std::cout << "Menu --> change mode -->" << std::endl;

            int item = putNumeric({ 0, numOfTrees - 1 }, {}, "number of tree");
            auto names = trees[item]->changeName(namesCollection);

            if (!names[0].empty()) { removeKeyFromVector(names[0], namesCollection); }
            if (!names[1].empty()) { namesCollection.emplace_back(names[1]); }
        }
        else if (command == static_cast<int>(Menu::FIND)) {
            std::cout << "Menu --> find mode -->" << std::endl;
            string searchedName;
            while (true) {
                searchedName = putLineString("Enter name");
                capitalize(searchedName);
                if (searchedName != "None" && isIncludes(namesCollection, searchedName)) { break; }
                cout << "Name not found. Try again. ";
            }

            for (auto &tree : trees) {
                auto found = tree->findNeighbors(searchedName);
                if (found) { break; }
            }
        }
        else if (command == static_cast<int>(Menu::NAMES)) {
            std::cout << "Menu --> names print mode -->" << std::endl;
            cout << joinListToStream(namesCollection).str() << endl;
            cout << endl;
        }
        else if (command == static_cast<int>(Menu::EXIT)) {
            std::cout << "Menu --> exit mode -->" << std::endl;
            for (auto &tree : trees) {
                delete tree;
                tree = nullptr;
            }
            trees.clear();

            break;
        }
    }
}