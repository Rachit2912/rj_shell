#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <functional>
#include<filesystem>

std::string get_path(std::string command) {
    std::string path_env = std::getenv("PATH");
    if (path_env == "")
    {
        return "";
    }
    std::stringstream ss(path_env);
    std::string path; 

    while (!ss.eof()) {
        std::getline(ss, path, ':');
        std::string abs_path = path + '/' + command;    
        if (std::filesystem::exists(abs_path)) {
            return abs_path;
        }
    }
    return "";
}

int main() {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // Defining input taken from user
    std::string input;

    // Exit flag for REPL
    bool exit = false;

    // Maps for storing the cmd-action pairs
    std::map<std::string, std::function<void(std::vector<std::string>&)>> cmds;

    // exit action fn:
    cmds["exit"] = [&exit](std::vector<std::string>& args) {
        if (args.size() == 1 && args[0] == "0") {
            exit = true;
            std::exit(0); // Exit program iff "exit 0" is typed
        }
        else {
            std::cout << "exit "<<args[0]<<" : command not found." << std::endl;
        }
        };

    // echo action fn:
    cmds["echo"] = [&input](std::vector<std::string>& args) {
        if (input[5] == '\'') {
            std::cout << input.substr(6, input.length() - 7) << std::endl;
        }
        else if (input[5] == '\"') {
            std::cout << input.substr(6, input.length() - 7) << std::endl;
        }
        else {
            for (int i = 1; i < args.size(); i++) {
                if (!args[i].empty()) std::cout << args[i] << " ";
            }
            std::cout << std::endl;
        }
        };


    // type action fn. : 
    cmds["type"] = [&cmds](std::vector<std::string>& args) {
        if (args.size() == 1 && cmds.find(args[0]) != cmds.end()) {
            std::cout << args[0] << " is a shell builtin" << std::endl;
        }
        else {
            std::string path = get_path(args[0]);
            if (path.empty()) {
                std::cout << args[0] << " not found\n";
            }
            else {
                std::cout << args[0]<< " is " << path << std::endl;
            }
        }
        };

    // pwd action fn. : 
    cmds["pwd"] = [](std::vector<std::string>& args) {
        std::string cmd_path = std::filesystem::current_path();
        std::cout << cmd_path << "\n";
        };


    //cd action fn. : 
    cmds["cd"] = [](std::vector<std::string>& args) {
        std::string target = args[0];
        if (target[0] == '/') {
            if (std::filesystem::exists(target)) {
                std::filesystem::current_path(target);
            }
            if (chdir(target.c_str()) == -1) {
                std::cout << "cd: " << target << ": No such file or directory" << std::endl;
            }
        }

        else if(target[0] == '.'){
            std::string cwd = std::filesystem::current_path().string();
            std::string dir = cwd + '/' + target;
            cwd = std::filesystem::canonical(dir);
            if (chdir(cwd.c_str()) == -1)
                std::cout << "cd: " << target<< ": No such file or directory" << std::endl;
        }

        else if (target[0] == '~') {
            chdir(getenv("HOME"));
        }
        };  

    // cat action fn. :
    cmds["cat"] = [](std::vector<std::string>& args) {
        for (auto& arg : args)
        {
            if (!arg.empty() && arg[0] == '\'') {
                arg = arg.substr(1, arg.size() - 2);
            }
        }

        std::string final_cmd = "cat";
        for (const auto& arg : args) {
            final_cmd += " " + arg;
        }
        system(final_cmd.c_str());
        };

    // REPL for shell:
    while (!exit) {

        // prompt sign :
        std::cout << "$ ";


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
            std::string cmdPath = get_path(cmd); // checking for exe files 
            if (cmdPath == "") {
            std::cout << cmd << ": command not found" << std::endl;
            }
            else {
                std::string command_with_full_path = cmdPath;
                std::filesystem::path path(command_with_full_path);
                command_with_full_path = path.filename();

                for (int argn = 0; argn < args.size(); argn++)
                {
                    command_with_full_path += " ";
                    command_with_full_path += args[argn];
                }
                const char* command_ptr = command_with_full_path.c_str();
                system(command_ptr);
                continue;
            }
        }

    }

    return 0;
}
