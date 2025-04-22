#include "parser.hpp"

// Split a string based on split_char
std::vector<std::string> split(const std::string& str, char split_char) {
    std::vector<std::string> result;
    std::string part;

    for (char ch : str) {
        if (ch == split_char) {
            result.push_back(part);
            part = "";
        } else {
            part += ch;
        }
    }

    result.push_back(part);
    return result;
}

// Parse a csv file into a map
Map parse_csv(std::string filepath) {
    std::ifstream file(filepath);
    std::string line;
    std::vector<std::string> splitted_line;
    int height, width;
    if (file.is_open()) {
        // Getting first line with map size
        std::getline(file, line);
        splitted_line = split(line,',');

        height = std::stoi(splitted_line[0]);
        width = std::stoi(splitted_line[1]);

        Map result = Map(height,width);
        while(std::getline(file, line)){
            splitted_line = split(line,',');
            for(std::string tilestring : splitted_line){
                // Decomposing tilestring into a tile
            }
        }
        file.close();
        return result;
    } else {
        std::cerr << "Error : " << filepath << "could not be opened" << std::endl;
    }

}

int main(){
    for(std::string str : split("coucou",'c')){
        std::cout << str << std::endl;
    }
    return 0;
}