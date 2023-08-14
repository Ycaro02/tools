#include <iostream>
#include <fstream>

void create_data_file(std::ofstream &file, std::string file_name, std::string data)
{
    file.open(file_name + ".output", std::fstream::in | std::fstream::out | std::fstream::trunc);
    file << data;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "1 argument expected : file name";
        return (1);
    }
    std::string str = argv[1];
    size_t find = str.find("01");
    size_t ext_pos = str.rfind('.');
    if (find > str.size() || ext_pos > str.size())
    {
        std::cerr << "Error wrong format detected can't found '01' or extension\n";
        return (1);
    }
    std::ofstream file1;
    std::ofstream file2;
    std::ofstream file3;
    std::string first = str.substr(0, find);
    std::string second = str.substr(find + 2);
    std::string ext = str.substr(ext_pos);
    create_data_file(file1, ".first", first);
    create_data_file(file2, ".second", second);
    create_data_file(file3, ".ext", ext);
    return (0);
}

