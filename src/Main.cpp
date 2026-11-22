#include <iostream>
#include <string>
#include <vector>
#include "fileLoader.h"
#include "Tree.h"
#include "CampusGraph.h"
#include "Facilities.h"

using namespace std;

int main()
{
    fileLoader loader("data");
    
    loader.loadData();
    
    DepartmentTree* departments = loader.getDepartments();
    CampusGraph* graph = loader.getGraph();
    Facilities* facilities = loader.getFacilities();
    
    
    int menu;
    
    do {
        
        cout << "==== MENU ====" << endl;
        
        cout << "1. View all facilities" << endl;
        cout << "2. Search facility by ID" << endl;
        cout << "3. Search facility by name" << endl;
        cout << "4. Search facility by category" << endl;
        cout << "5. View campus graph" << endl;
        cout << "6. Display graph counts and degrees" << endl;
        cout << "7. Find reachable building (BFS)" << endl;
        cout << "8. Find shortest path (BFS)" << endl; 
        cout << "9. Add Department" << endl;
        cout << "10. Search Department" << endl;
        cout << "11. Display Department Hierarchy" << endl;
        cout << "12. Run Preorder Traversal" << endl;
        cout << "13. Run Postorder Traversal" << endl;
        cout << "14. Run Level-order Traversal" << endl;
        cout << "15. Display Department Statistics" << endl;
        cout << "16. Facility Category Report" << endl;
        cout << "17. Graph Connectivity Report" << endl;
        cout << "0. Exit" << endl;
        
        cout << "Enter your choice: ";
        
        if (!(cin >> menu))
        {
            cout << "Only enter a number." << endl;
            
            cin.clear();
            cin.ignore(10000, '\n');
            
            menu = -1;
            continue;
        }
        
        cin.ignore(10000, '\n');
        
        if (menu == 1)
        {
            //Add view facilities
            facilities->display(cout);
        
        }
        
        else if (menu == 2)
        {
            // Add search by id
            string id;

            cout << "Enter facility ID: ";
            getline(cin, id);

            Facilities::SearchResult result = facilities->search(id);

            if (result.status == Facilities::SearchStatus::Invalid)
            {
                cout << "Invalid facility ID." << endl;
            }

            else if (result.status == Facilities::SearchStatus::NotFound)
            {
                cout << "No facility found with ID " << result.normalizedId << "." << endl;
            }

            else
            {
                cout << "Facility found:" << endl;
                cout << " ID:       " << result.facility->id << endl;
                cout << " Name:     " << result.facility->name << endl;
                cout << " Building: " << result.facility->building << endl;
                cout << " Category: " << result.facility->category << endl;
            }
        }
        
        else if (menu == 3)
        {
            // Add Search facility by name
            string name;

            cout << "Enter facility name: ";
            getline(cin, name);

            if (name.find_first_not_of(" \t\r\n") == string::npos)
            {
                cout << "Name cann't be empty. " << endl;
            }
            else
            {
                facilities->searchByName(name, cout);
            }
        }
        
        else if (menu == 4)
        {
            //Add Search facility by category
            string category;
            cout << "Enter category: ";
            getline(cin, category);

            if (category.find_first_not_of(" \t\r\n") == string::npos)
            {
                cout << "Category can't be empty. " << endl;
            }
            else
            {
                facilities->searchByCategory(category, cout);
            }
        }
        
        else if (menu == 5)
        {
            //Add view campus graph
            graph->display(cout);
        }
        
        else if (menu == 6)
        {
            //Add display grpah count and degrees
            cout << "Buildings: " << graph->buildingCount() << endl;
            cout << "Connections: " << graph->edgeCount() << endl;
            cout << "Degrees: " << endl;
            
            graph->display(cout);
        }
        
        else if (menu == 7)
        {
            //Add BFS
            string input;
            string start;

            cout << "Enter starting building: ";
            getline(cin, input);

            if (input.find_first_not_of(" \t\r\n" ) == string::npos)
            {
                cout << "Building name can't be empty." << endl;
            }

            else if (!graph->canonicalBuilding(input, start))
            {
                cout << "Building not found: " << input << endl;
            }

            else
            {
                vector<CampusGraph::Visit> visits;
                
                if (!graph->breadthFirst(start, visits))
                {
                    cout << "Building not found: " << input << endl;
                }
                
                else
                {
                    cout << "Buildings reachable from " << start << " (BFS order):" << endl;
                    
                    for (size_t i = 0; i < visits.size(); i++)
                    {
                        cout << " " << visits[i].building
                        << " (distance " << visits[i].distance << ")" << endl;
                    }
                    
                    cout << "Total reachable: " << visits.size() << endl;
                }
            }
        }
        
        else if (menu == 8)
        {
            // Add Find shortest path (BFS)
            string fromInput;
            string toInput;

            cout << "Enter starting building: ";
            getline(cin, fromInput);

            cout << "Enter destination buidling: ";
            getline(cin, toInput);

            if (fromInput.find_first_not_of(" \t\r\n") == string::npos ||
                toInput.find_first_not_of(" \t\r\n") == string::npos)
            {
                cout << "Building names can't be empty." << endl;
            }
            else
            {
                vector<string> path;
                
                if (graph->shortestPath(fromInput, toInput, path))
                {
                    cout << "Shortest route (" << path.size() - 1 << " step(s)):" << endl;

                    for (size_t i = 0; i < path.size(); i++)
                        {
                            if (i > 0)
                            {
                                cout << " -> ";
                            }
                            cout << path[i];
                        }
                    cout << endl;
                }
            }
        }
        
        else if (menu == 9)
        {
            string name;
            string parentName;
            
            cout << "Enter a department name: ";
            getline(cin, name);
            
            if (name.find_first_not_of(" \t\r\n") == string::npos)
            {
                cout << "Name can't be empty." << endl;
            }
            
            else if (departments->Empty())
            {
                departments->addDepartment(name, "");
            }
            
            else
            {
                cout << "Enter department name for parent: ";
                getline(cin, parentName);
                
                if (parentName.find_first_not_of(" \t\r\n") ==string::npos)
                {
                    cout << "Parent name can't be empty." << endl;
                }
                
                else
                {
                    departments-> addDepartment(name,parentName);
                }
            }
        }
        
        else if (menu == 10)
        {
            string name;
            
            cout << "Enter name to search: ";
            getline(cin, name);
            
            if (name.find_first_not_of(" \t\r\n") == string::npos)
            {
                cout << "Can't be empty." << endl;
            }
            
            else
            {
                departments->displayDepartmentInfo(name);
            }
        }
        
        else if (menu == 11)
        {
            departments->displayHierarchy();
        }
        
        else if (menu == 12)
        {
            departments->preorderTraversal();
        }
        
        else if (menu == 13)
        {
            departments->postorderTraversal();
        }
        
        else if (menu == 14)
        {
            departments->levelorderTraversal();
        }
        
        else if (menu == 15)
        {
            departments->displayStats();
        }
        
        else if (menu == 16)
        {
            facilities->categoryReport(cout);
        }
        
        else if (menu == 17)
        {
            graph->connectivityReport(cout);
        }
        
        else if (menu == 0)
        {
            cout << "Exit: Thanks for using our system" << endl;
        }
        
        else
        {
            cout << "Only use the menu with numbers 1-17." << endl;
        }
        
    } while (menu != 0);
    
    return 0;
}
