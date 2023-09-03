#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

static void fill_str(char *dest, const char* from)
{
    int i = 0;
    while (from  && from[i])
    {
        dest[i] = from[i];
        i++;
    }
    dest[i] = '\0';
}

static char** build_cmd(std::string &cmd, std::string &str, std::string &new_str)
{
    char **moove = NULL;
    moove = (char **)malloc(sizeof(char*) * 4);
    moove[0] = (char *)malloc(sizeof(char) * cmd.length() + 1);
    fill_str(moove[0], cmd.c_str());
    moove[1] = (char *)malloc(sizeof(char) * str.length() + 1);
    fill_str(moove[1], str.c_str());
    moove[2] = (char *)malloc(sizeof(char) * new_str.length() + 1);
    fill_str(moove[2], new_str.c_str());
    moove[3] = NULL;
    return (moove);
}

static void free_all(char **moove, int nb)
{
    for (int i = 0; i <= nb; i++)
        free(moove[i]);
    free(moove);
}

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

    char **moove = NULL;
    moove = build_cmd(cmd, str, new_str);
    if (moove == NULL)
    {
        std::cout << "Malloc error\n";
        return false;
    }

    int pid = fork();
    if (pid == 0)
         execve(cmd.c_str(), moove, envp);
    free_all(moove, 3);
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
