#include "Tree.h"
#include <cctype>

DepartmentNode::DepartmentNode(std::string departmentName, DepartmentNode* parentNode)
{
    name = departmentName;
    parent = parentNode;
}//Constructor

DepartmentTree::DepartmentTree()
{
    root = nullptr;
}//Constructor

DepartmentTree::~DepartmentTree()
{
    deleteTree(root);
}//Destructor

void DepartmentTree::deleteTree(DepartmentNode* node)
{
    if (node == nullptr)
    {
        return;
    }
    
    for (int i = 0; i < node->children.size(); i++)
    {
        deleteTree(node->children[i]);
    }
    
    delete node;
} //deletes nodes to clear memory usage

bool DepartmentTree::Empty() const
{
    return root == nullptr;
} //check if tree is empty

std::string DepartmentTree::normalize(std::string name) const
{
    size_t start = name.find_first_not_of("\t\r\n");
    
    if (start == std::string::npos)
    {
        return "";
    }
    
    size_t end = name.find_last_not_of(" \t\r\n");
    
    name = name.substr(start, end - start + 1);
    
    for (int i = 0; i < name.length(); i++)
    {
        name[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[i])));
    }
    
    return name;
} //Removes whitespace and converts characters to uppercase

DepartmentNode* DepartmentTree::searchRecursive(DepartmentNode* node, std::string name) const
{
    if (node == nullptr)
    {
        return nullptr;
    }
    
    if (normalize(node->name) == normalize(name))
    {
        return node;
    }
    
    for (int i = 0; i < node->children.size(); i++)
    {
        DepartmentNode* result = searchRecursive(node->children[i], name);
        
        if (result != nullptr) //Add to cell machine
        {
            return result;
        }
    }
    
    return nullptr;
} //Uses recursion to search

DepartmentNode* DepartmentTree::searchDepartment(std::string name) const
{
    return searchRecursive(root,name);
} //searches tree

bool DepartmentTree::existDepartment(std::string name) const
{
    return searchDepartment(name) != nullptr;
}//checks if department exists

bool DepartmentTree::addDepartment(std::string name, std::string parentName)
{
    name = name.substr(name.find_first_not_of(" \t\r\n"));
    
    name = name.substr(0, name.find_last_not_of(" \t\r\n") + 1);
    
    if (name.empty())
    {
        std::cout << "The Department name cannot be empty \n";
        return false;
    }
    
    if (existDepartment(name))
    {
        std::cout << "The Department already exists: " << name << '\n';
        return false;
    }
    
    if (Empty())
    {
        root = new DepartmentNode(name);
        
        std::cout << "A new root department has been added \n";
        return true;
    }
    
    DepartmentNode*parent = searchDepartment(parentName);
    
    if (parent == nullptr)
    {
        std::cout << "Parent department couldn't be found: " << parentName << '\n';
        return false;
    }
    
    DepartmentNode* newDepartment = new DepartmentNode(name,parent);
    
    parent->children.push_back(newDepartment);
    
    std::cout << "The department has been added.\n";
    
    return true;
} //Creates and adds a new department to the tree

void DepartmentTree::displayRecursive(DepartmentNode *node, int depth) const
{
    if (node == nullptr)
    {
        return;
    }
    
    for (int i = 0; i < depth; i++)
    {
        std::cout << "    ";
    }
    
    std::cout << node->name << '\n';
    
    for (int i = 0; i < node->children.size(); i++)
    {
        displayRecursive(node->children[i], depth + 1);
    }
} //Displays hierarchy

void DepartmentTree::displayHierarchy() const
{
    if (Empty())
    {
        std::cout << "Nothing was loaded.\n";
        return;
    }
    
    std::cout << "\nDepartment Hierarchy:\n";
    
    displayRecursive(root, 0);
} //Displays complete tree

void DepartmentTree::preorderRecursive(DepartmentNode *node) const
{
    if (node == nullptr)
    {
        return;
    }
    
    std::cout << node->name << '\n';
    
    for (int i = 0; i < node->children.size(); i++)
    {
        preorderRecursive(node->children[i]);
    }
}

void DepartmentTree::preorderTraversal() const
{
    if (Empty())
    {
        std::cout << "Nothing was loaded.\n";
        return;
    }
    
    std::cout << "\nPreorder Traversal:\n";
    
    preorderRecursive(root);
} // runs preorder traversal

int DepartmentTree::getDepth(DepartmentNode* node) const
{
    int depth = 0;
    
    while (node != nullptr && node->parent != nullptr)
    {
        depth++;
        
        node = node->parent;
    }
    
    return depth;
} //shows the depth of tree

void DepartmentTree::displayDepartmentInfo(std::string name) const
{
    DepartmentNode* department = searchDepartment(name);
    
    if (department == nullptr)
    {
        std::cout << "Department wasn't found: " << name << '\n';
        
        return;
    }
    
    if (department->parent == nullptr)
    {
        std::cout << "Parent: None\n";
    }
    else
    {
        std::cout << "Parent: " << department->parent->name << '\n';
    }
    
    std::cout << "Direct children: " << department->children.size() << '\n';
    
    std::cout << "Depth: " << getDepth(department) << '\n';
} //Displays info about specific tree node
