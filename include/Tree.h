#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <string>
#include <vector>

struct DepartmentNode {
    
    std::string name; //Stores department name
    
    DepartmentNode* parent; //Stores pointer to parent node
    
    std::vector<DepartmentNode*> children; //Stores parent node pointer to their children
    
    DepartmentNode(std::string departmentName, DepartmentNode* parentNode = nullptr); //Constructor
};

class DepartmentTree {
    
private:
    
    DepartmentNode* root; //Stores the root pointer
    
    DepartmentNode* searchRecursive(DepartmentNode* node, std::string name) const; //Searches for department via name
    
    void displayRecursive(DepartmentNode* node, int depth) const; //Displays the tree hierarchy
    
    void preorderRecursive(DepartmentNode* node) const; //Performs preorder on the tree
    
    std::string normalize(std::string name) const; //makes it follow a specific format (like all caps or lowercase)
    
    void deleteTree(DepartmentNode* node); //prevents copying
    
public:
    
    DepartmentTree(); // Constructor
    ~DepartmentTree(); //Destructor
    
    bool Empty() const; //checks if empty
    
    DepartmentTree(const DepartmentTree&) = delete; //prevents copyinh
    
    DepartmentTree& operator = (const DepartmentTree&) = delete; 
    
    bool addDepartment(std::string name, std::string parentName); //Adds a new department
    
    DepartmentNode* searchDepartment(std::string name) const; //searches tree for specific department
    
    void displayDepartmentInfo(std::string name) const; //displays info about department
    
    bool existDepartment(std::string name) const; //checks if department exists
    
    void displayHierarchy() const; //Setting to display tree
    
    void preorderTraversal() const; //displays preorder traversal
    
    int getDepth(DepartmentNode* node) const; //displays tree depth
};

#endif
