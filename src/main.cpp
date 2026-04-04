#include <iostream>
#include <map>
#include <cstring>

#include <fstream>
#include <iomanip>
#include <filesystem>

#include "json.hpp"
#include "utils.hpp"
#include "lazyscript.hpp"

namespace fs = std::filesystem;

void printDefaultMsg()
{
    std::cout << "Welcome to LazyScript, please use the --help command." << std::endl;
}

void compile_project(nlohmann::json data)
{
    std::string source = data["source"];
    std::string entry = data["entry"];

    std::string path = fs::current_path().string().append("/").append(source.c_str());
    std::string source_path = fs::current_path().string().append("/").append(source.c_str());
    std::string main = fs::current_path().string().append("/").append(source.c_str()).append("/").append(entry.c_str());

    Lexer lex(read_file(main), entry);
    
    while (!lex.maybe(EOF_TOKEN))
    {
        Token tok = lex.next();
        std::cout << "Kind: " << tok.kind << " | Value: " << tok.value << std::endl;
    }
    
}

nlohmann::json get_config_file()
{
    std::string path = fs::current_path().string().append("/lazy-config.json");

    if (std::filesystem::exists(path))
    {
        std::ifstream f(path);

        nlohmann::json data = nlohmann::json::parse(f);

        return data;
    }

    throw std::runtime_error("No lazy-config.json found.");
}

int main(int argc, char const *argv[])
{
    static const std::map<std::string, std::string> commandsList = {
        {"--help", "Displays a list of all the commands."},
        {"--run", "Compiles and run the current project."},
        {"--build", "Compiles and build the current project into an executable."},
        {"--new", "Makes a new project in a directory with the given name."},
        {"--info", "Gives all the info of the current project."}};

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
        std::string path = fs::current_path().string();

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

            project["source"] = "main.ls";
            project["entry"] = "main.ls";
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
        nlohmann::json data = get_config_file();

        std::string project = data["project"];
        std::string version = data["version"];
        std::string output = data["output"];
        std::string source = data["source"];
        std::string entry = data["entry"];

        nlohmann::basic_json libraries = data["libraries"];

        std::cout << "Project Name: " << project << std::endl;
        std::cout << "Project Version: " << version << std::endl;
        std::cout << "Project Output: " << output << std::endl;
        std::cout << "Project Entry: " << entry << std::endl;
        std::cout << "Project Source: " << source << std::endl;
    }
    else if (std::strcmp(command, "--run") == 0)
    {
        nlohmann::json data = get_config_file();
        compile_project(data);
    }
    else
    {
        printDefaultMsg();
    }

    return 0;
}