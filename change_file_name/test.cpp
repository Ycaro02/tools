#include <sstream>
#include <iomanip>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

static bool check_file(std::string file)
{
    if (access(file.c_str(), F_OK) != 0)
        return (false);
    return (true);
}

static bool check_path(std::string &str, std::string &new_str)
{
    if (check_file(str) == false)
    {
        std::cout << "Error file : " << str << "not found\n";
        return false;
    }
   if (check_file(new_str) == true)
    {
        std::cout << "Error file : " << new_str << " already exist please select another new name\n";
        return (false);
    }
    if (new_str == str)
    {
        std::cout << "Error same name detected please enter another new name\n";
        return (false);
    }
    return (true);
}

static bool rename_file(std::string str, std::string new_name, char **envp, int nb)
{
    std::string cmd = "/usr/bin/mv";
    size_t ext_pos = str.rfind('.');
    std::string ext = str.substr(ext_pos);
    std::string number;

    if (nb < 10)
        number = "0" +  std::to_string(nb);
    else
        number =  std::to_string(nb);

    std::string new_str = new_name + number + ext;
    if (check_path(str, new_str) == false)
        return (false);

    std::rename(str.c_str(), new_str.c_str());
    return (true);
}

static std::string find_file_name(std::string directory, std::string number)
{
        struct dirent *my_dir;
        std::string all_name;
        DIR *dir = opendir(directory.c_str());
        std::string str;
        while ((my_dir = readdir(dir)) != NULL)
        {
                str = my_dir->d_name;
                if (str.find(number) <= str.length())
                    break;
                str.clear();
        }
        closedir(dir);
        return (str);
}

int main(int argc, char **argv, char **envp)
{
    if (argc != 3)
    {
        std::cout << "Error 1 argument expected : nb_file\n";
        std::cout << "Error 2 argument expected : new name\n";
        return (1);
    }
    std::string nb_file = argv[1];
    std::string new_name = argv[2];
    std::string file_name;

    int max = stoi(nb_file);
    int i = 1;
    while (i <= max)
    {
        file_name = find_file_name(".", std::to_string(i));
        if (file_name.length() == 0)
        {
            std::cout << "Error file_name number : " << i << " not found\n";
            return (1);
        }
        if (rename_file(file_name, new_name, envp, i) == false)
            return (1);
        i++;
    }
    return (0);
}