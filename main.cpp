#include <iostream>
#include <vector>
#include <string>
// #include <cstdlib>
// #include <ctime>
#include "includes/utilities.h"
#include "includes/constants.h"
#include "includes/Tree.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

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