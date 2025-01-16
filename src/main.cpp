#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <functional>

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // First prompt:
    std::cout << "$ ";

    // Defining input taken from user
    std::string input;

    // Exit flag for REPL
    bool exit = false;

    // Maps for storing the cmd-action pairs
    std::map<std::string, std::function<void(std::vector<std::string>&)>> cmds;

    // Exit action fn:
    cmds["exit 0"] = [&exit](std::vector<std::string>& args) {
        std::cout << "Exiting the shell..." << std::endl;
        exit = true;
        };

    // Echo action fn:
    cmds["echo"] = [](std::vector<std::string>& args) {
        for (const auto& arg : args) {
            std::cout << arg << " ";
        }
        std::cout << std::endl;
        };

    // REPL for shell:
    while (!exit) {
        // Getting the input from the user:
        std::getline(std::cin, input);

        // Stringstream for efficiently differentiating the cmds and args:
        std::stringstream ss(input);
        std::string cmd;
        std::vector<std::string> args;

        // Extracting the cmd through the stream:
        ss >> cmd;

        // Extracting the args from the stream:
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }

        // Valid cmd found => perform its action:
        if (cmds.find(cmd) != cmds.end()) {
            cmds[cmd](args);
        }
        else {  // Else invalid cmd => not found:
            std::cout << cmd << ": command not found" << std::endl;
        }

        std::cout << "$ ";
    }

    return 0;
}
