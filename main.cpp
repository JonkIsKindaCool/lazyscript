#include <iostream>
#include <map>
#include <cstring>

#include <fstream>
#include <iomanip>
#include <filesystem>

#include "json.hpp"

namespace fs = std::filesystem;

void printDefaultMsg()
{
    std::cout << "Welcome to LazyScript, please use the --help command." << std::endl;
}

int main(int argc, char const *argv[])
{
    static const std::map<std::string, std::string> commandsList = {
        {"--help", "Displays a list of all the commands."},
        {"--run", "Compiles and run the current project."},
        {"--build", "Compiles and build the current project into an executable."},
        {"--new", "Makes a new project in a directory with the given name."},
        {"--info", "Gives all the info of the current project."}
    };

    if (argc <= 1)
    {
        printDefaultMsg();
        return 0;
    }

    const char *command = argv[1];

    if (std::strcmp(command, "--help") == 0)
    {
        std::cout << "LazyScript commands list:" << std::endl;

        for (const auto &[key, value] : commandsList)
        {
            std::cout << key << ": " << value << std::endl;
        }
    }
    else if (std::strcmp(command, "--new") == 0)
    {
        std::string path = std::string(fs::current_path().c_str());

        if (argc <= 2)
        {
            std::cout << "Expected a name for the project, without spaces." << std::endl;
        }
        else
        {
            std::string project_name = std::string(argv[2]);

            nlohmann::json project;

            project["project"] = project_name;
            project["version"] = "1.0.0";

            project["entry"] = "src/main.ls";
            project["output"] = "build/" + project_name;

            project["libraries"] = nlohmann::json::array();

            fs::create_directory(path.append("/").append(project_name));

            std::ofstream file(path.append("/lazy-config.json"));
            if (file.is_open())
            {
                file << project.dump(4);
                file.close();
                std::cout << "Project " << project_name << " created successfully!" << std::endl;
            }
            else
            {
                std::cerr << "Unable to create the project." << std::endl;
            }
        }
    }
    else if (std::strcmp(command, "--info") == 0)
    {
    }
    else
    {
        printDefaultMsg();
    }

    return 0;
}