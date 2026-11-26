#include "Tree.h"
#include <cassert>
#include <iostream>
#include <sstream>

int main()
{
   DepartmentTree tree;
   assert(tree.addDepartment("University", "NONE")); 
   assert(tree.addDepartment("College of Engineering", "University")); 
   assert(tree.addDepartment("College of Science", "University")); 
   assert(tree.addDepartment("Computer Science", "College of Engineering")); 
   assert(tree.addDepartment("Electrical Engineering", "College of Engineering"));
   assert(tree.addDepartment("Mathematics", "College of Science"));
   assert(tree.addDepartment("Physics", "College of Science"));
   assert(!tree.addDepartment("University", "NONE"));
   assert(!tree.addDepartment(" computer science ", "University"));
   assert(!tree.addDepartment("", "University"));
   assert(!tree.addDepartment("Chemistry", "Biology"));
   assert(tree.existDepartment("University"));
   assert(tree.existDepartment("College of Engineering"));
   assert(tree.existDepartment("Computer Science"));
   assert(tree.existDepartment(" physics "));
   assert(!tree.existDepartment("Chemistry"));
   assert(tree.existDepartment(" university"));
   assert(tree.existDepartment("UNIVERSITY "));
   assert(tree.existDepartment("  computer science  "));

   DepartmentNode* University = tree.searchDepartment("University");
   DepartmentNode* Engineering = tree.searchDepartment("College of Engineering");
   DepartmentNode* Science = tree.searchDepartment("College of Science");

   assert(University->parent == nullptr);
   assert(Engineering->parent == University);
   assert(Science->parent == University);

   assert(University->children.size() == 2);
   assert(Engineering->children.size() == 2);
   assert(Science->children.size() == 2);

   assert(tree.getDepth(University) == 0);
   assert(tree.getDepth(Engineering) == 1);
   assert(tree.getDepth(tree.searchDepartment("Computer Science")) == 2);

   std::ostringstream output;
   std::streambuf* original = std::cout.rdbuf(output.rdbuf());

   tree.preorderTraversal();

   std::cout.rdbuf(original);

   std::string result = output.str();

   assert(result.find("University") < result.find("College of Engineering"));
   assert(result.find("College of Engineering") < result.find("Computer Science"));
   assert(result.find("Computer Science") < result.find("Electrical Engineering"));

   output.str("");
   output.clear();

   original = std::cout.rdbuf(output.rdbuf());

   tree.postorderTraversal();

   std::cout.rdbuf(original);

   result = output.str();

   assert(result.find("Computer Science") < result.find("College of Engineering"));
   assert(result.find("College of Engineering") < result.find("University"));
   assert(result.find("Physics") < result.find("College of Science"));

   output.str("");
   output.clear();

   original = std::cout.rdbuf(output.rdbuf());

   tree.levelorderTraversal();

   std::cout.rdbuf(original);

   result = output.str();

   assert(result.find("University") < result.find("College of Engineering"));
   assert(result.find("College of Engineering") < result.find("Computer Science"));
   assert(result.find("College of Science") < result.find("Mathematics"));
   assert(result.find("Mathematics") < result.find("Physics"));
    
   output.str("");
   output.clear();

   original = std::cout.rdbuf(output.rdbuf());

   tree.displayStats();

   std::cout.rdbuf(original);

   assert(output.str().find("Total departments: 7") != std::string::npos);
   assert(output.str().find("Leaves: 4") != std::string::npos);
   assert(output.str().find("Tree height: 2") != std::string::npos);

   output.str("");
   output.clear();

   original = std::cout.rdbuf(output.rdbuf());

   tree.displayHierarchy();

   std::cout.rdbuf(original);

   assert(output.str().find("University") != std::string::npos);
   assert(output.str().find("Physics") != std::string::npos);

    std::cout << "SUCCESS: All the following tests have sucessfully passed\n";
   return 0;
}
