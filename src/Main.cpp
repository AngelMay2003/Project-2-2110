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
        cout << "3. View campus graph" << endl;
        cout << "4. Display graph counts and degrees" << endl;
        cout << "5. Find reachable building (BFS)" << endl;
        cout << "6. Add department" << endl;
        cout << "7. Search department" << endl;
        cout << "8. Display department hierarchy" << endl;
        cout << "9. Run preorder traversal" << endl;
        cout << "10. Exit" << endl;
        
        cout << "Enter your choice: ";
        
        cin >> menu;
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
            //Add view campus graph
            graph->display(cout);
        }
        
        else if (menu == 4)
        {
            //Add display grpah count and degrees
            cout << "Buildings: " << graph->buildingCount() << endl;
            cout << "Connections: " << graph->edgeCount() << endl;
            cout << "Degrees: " << endl;
            
            graph->display(cout);
        }
        
        else if (menu == 5)
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
        
        else if (menu == 6)
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
        
        else if (menu == 7)
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
        
        else if (menu == 8)
        {
            departments->displayHierarchy();
        }
        
        else if (menu == 9)
        {
            departments->preorderTraversal();
        }
        
        else if (menu == 10)
        {
            cout << "Exit: Thanks for using our system" << endl;
        }
        
        else
        {
            cout << "Only use the menu with numbers 1-10." << endl;
        }
        
    } while (menu != 10);
    
    return 0;
}
