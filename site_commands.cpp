#include <iostream>
#include <cstdarg>
#include <string>
#include <fstream>
#include <memory>
#include <cstdio>
#include <sstream>
#include <cstring>

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

int scan_site_updates(const char* site_path) {
    std::ofstream output;
    std::string result;

    char cmd[256];
    strcat(cmd, "cd "); strcat(cmd, site_path); strcat(cmd, "; composer outdated");
    std::string str = exec(cmd);
    std::istringstream iss(str);

    output.open("log.txt");
    
    std::string log;
    for (std::string line; std::getline(iss, line); )
    {
        std::cout << line.substr(0, 41) << '\n';
        if ((line[50] == '~' || line[50] == '!') && line.substr(0, 7) == "drupal/") {
            std::cout << "Found must-have update for: " << line.substr(0, 41) << '\n';
            log += line + "\n";
        }
    }
    if (output.is_open())
        output << log;
    output.close();
    return 0;
}


int main(int argc, char const *argv[]) {
    const char* site_path = "/home/dan/www/droopler-project1";
    scan_site_updates(site_path);
}