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

    if (!part.empty()) result.push_back(part); // Add last token
    return result;
}

// Parse a csv file into a map
Map parse_csv(std::string filepath) {
    std::ifstream file(filepath); // Replace with your file path
    std::string line;

    if (file.is_open()) {
        // Getting first line with map size
        std::getline(file, line);
        while(std::getline(file, line)){
            switch (line[0])
            {
            case ' ':
                // Ocean
                break;
            
            default:
                break;
            }
        }
        file.close();
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