#include "fileLoader.h"
#include <cctype>
#include <sstream>

fileLoader::fileLoader(std::string folder) : folderPath(folder), facilities(graph)
{
    
}

DepartmentTree* fileLoader::getDepartments()
{
    return &departments;
}

CampusGraph* fileLoader::getGraph()
{
    return &graph;
}

Facilities* fileLoader::getFacilities()
{
    return &facilities;
}

std::string fileLoader::whitespace(std::string text)
{
    size_t start = text.find_first_not_of(" \t\r\n");
    
    if (start == std::string::npos)
    {
        return "";
    }
    
    size_t end = text.find_last_not_of(" \t\r\n");
    
    return text.substr(start, end - start + 1);
}

std::vector<std::string> fileLoader::fields(std::string line)
{
    std::vector<std::string> data;
    
    size_t start = 0;
    size_t position;
    
    while (true)
    {
        position = line.find('|', start);
        
        if (position == std::string::npos)
        {
            data.push_back(whitespace(line.substr(start)));
            break;
        }
        
        data.push_back(whitespace(line.substr(start, position - start)));
        
        start = position + 1;
    }
    
    return data;
        
}
    

bool fileLoader::commentCheck(std::string line)
{
    line = whitespace(line);
    
    return line.empty() || line[0] == '#';
}

std::string fileLoader::normalize(std::string text)
{
    text = whitespace(text);
    
    for (int i = 0; i < text.length(); i++)
    {
        text[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(text[i])));
    }
    
    return text;
}

void fileLoader::printWarning(std::string filename, int lineNumber, std::string reason)
{
    std::cout << "Warning:" << filename << " line " << lineNumber << ": " << reason << std::endl;
}

void fileLoader::printSummary(std::string filename, int accepted, int rejected)
{
    std::cout << "\n" << filename << std::endl;
    std::cout << "Accepted: " << accepted << std::endl;
    std::cout << "Rejected: " << rejected << std::endl;
}

void fileLoader::loadBuildings()
{
    graph.loadBuildings(folderPath + "/buildings.txt", std::cout);
}

void fileLoader::loadConnections()
{
    graph.loadConnections(folderPath + "/connections.txt", std::cout);
}

void fileLoader::loadFacilities()
{
    facilities.load(folderPath + "/facilities.txt", std::cout);
}

void fileLoader::loadDepartments()
{
    std::ifstream departmentText(folderPath + "/departments.txt");
    
    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;
    
    std::string line;
    
    if(!departmentText.is_open())
    {
        std::cout << "departments.txt can't be opened\n";
        printSummary("departments.txt", 0, 0);
        return;
    }
    
    while (std::getline(departmentText, line))
    {
        lineNumber++;
        
        if (commentCheck(line))
        {
            continue;
        }
        
        std::vector<std::string> data = fields(line);
        
        if (data.size() != 2 || data[0].empty() || data[1].empty())
        {
            printWarning("departments.txt", lineNumber, "The record is invalid");
            
            rejected++;
            continue;
        }
        
        if (departments.existDepartment(data[0]))
        {
            printWarning("departments.txt", lineNumber, "The department is a duplicate");
            
            rejected++;
            continue;
        }
        
        if (departments.Empty())
        {
            if (normalize(data[1]) != "NONE")
            {
                printWarning("departments.txt", lineNumber, "The first department needs to have None as a parent");
                
                rejected++;
                continue;
            }
        }
        
        else
        {
            if (normalize(data[1]) == "NONE" || !departments.existDepartment(data[1]))
            {
                printWarning("departments.txt",lineNumber, "Invalid parent.");
                
                rejected++;
                continue;
            }
        }
        
        departments.addDepartment(data[0], data[1]);
        
        accepted++;
    }
    
    departmentText.close();
    
    printSummary("departments.txt",accepted,rejected);
}

void fileLoader::loadData()
{
    loadBuildings();
    loadConnections();
    loadFacilities();
    loadDepartments();
    
    std::cout << "File loading is completed\n";
}
