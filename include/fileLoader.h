#ifndef FILELOADER_H
#define FILELOADER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "Tree.h"
#include "CampusGraph.h"
#include "Facilities.h"

class fileLoader {
    
private:
    
    std::string folderPath;
    
    CampusGraph graph;
    
    Facilities facilities;
    
    DepartmentTree departments;
    
    std::string normalize(std::string text);
    
    std::string whitespace(std::string text);
    
    std::vector<std::string> fields(std::string line);
    
    bool commentCheck(std::string line);
    
    void printWarning(std::string filename, int lineNumber, std::string reason);
    
    void printSummary(std::string filename, int accepted, int rejected);
    
public:
    
    fileLoader(std::string folder);
    
    DepartmentTree* getDepartments();
    CampusGraph* getGraph();
    Facilities* getFacilities();
    
    void loadBuildings();
    void loadConnections();
    void loadFacilities();
    void loadDepartments();
    
    void loadData();
};

#endif
