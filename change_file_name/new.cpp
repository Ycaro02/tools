#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

bool reverse_search(std::string rev)
{
        if (rev == "--reverse")
            return true;
        else
        {
            std::cout << "Error invalid option, --reverse is the only one acepted\n";
            return (false);
        }
}

std::string find_final(std::string directory)
{
        struct dirent *my_dir;
        std::string all_name;
        DIR *dir = opendir(directory.c_str());
        std::string str;
        while ((my_dir = readdir(dir)) != NULL)
        {
                str = my_dir->d_name;
                if (str.find("FINAL") <= str.length())
                        return str;
        }
        closedir(dir);
        str.clear();
        return (str);
}


void fill_str(char *dest, const char* from)
{
    int i = 0;
    while (from  && from[i])
    {
        dest[i] = from[i];
        i++;
    }
    dest[i] = '\0';
}

void rename_file(std::string str, std::string new_name, size_t find, char **envp, int nb, bool finale)
{
    std::string cmd = "/usr/bin/mv";
    std::string first = str.substr(0, find);
    std::string second = str.substr(find + 2);
    size_t ext_pos = str.rfind('.');
    std::string ext = str.substr(ext_pos);

    std::string number;
    if (nb < 10)
        number = "0" +  std::to_string(nb);
    else
        number =  std::to_string(nb);

    std::string old_str = first + number + second;
    std::string new_str = new_name + number + ext;
    char **moove;
    int i = 1;
    if (finale == true)
        old_str = str;

    moove = (char **)malloc(sizeof(char*) * 4);
    moove[0] = (char *)malloc(sizeof(char) * cmd.length() + 1);
    fill_str(moove[0], cmd.c_str());
    moove[1] = (char *)malloc(sizeof(char) * old_str.length() + 1);
    fill_str(moove[1], old_str.c_str());
    moove[2] = (char *)malloc(sizeof(char) * new_str.length() + 1);
    fill_str(moove[2], new_str.c_str());
    moove[3] = NULL;
  
    int pid = fork();
    if (pid == 0)
         execve(cmd.c_str(), moove, envp);
    free(moove[0]);
    free(moove[1]);
    free(moove[2]);
    free(moove[3]);
    free(moove);
}

int main(int argc, char **argv, char **envp)
{
    if (argc <= 3 || argc > 4)
    {
        std::cout << "Error 1 argument expected : nb_file\n";
        std::cout << "Error 2 argument expected : 1st file name\n";
        std::cout << "Error 3 argument expected : new name\n";
        std::cout << "Error 4 argument option --reverse\n";
        return (1);
    }
    bool reverse = false;
    if (argc == 3 )
    {
        if (reverse_search(argv[4]) == true);
            reverse = true;    
        return (1);
    }
   
    std::string nb_file = argv[1];
    std::string str = argv[2];

    size_t find = str.find("01");
    if (reverse == true)
         find = str.rfind("01");

    size_t ext_pos = str.rfind('.');

    if (find > str.size() || ext_pos > str.size())
    {
        std::cerr << "Error wrong format detected can't found '01' or extension\n";
        return (1);
    }
    std::string final_file;
    final_file = find_final(".");
    int max = stoi(nb_file);
    int i = 1;

    while (i <= max)
    {
        if (i == max && final_file.length() > 0)
            rename_file(final_file, argv[3], find, envp, i, true);
        else
            rename_file(str, argv[3], find, envp, i, false);
        i++;
    }
    return (0);
}