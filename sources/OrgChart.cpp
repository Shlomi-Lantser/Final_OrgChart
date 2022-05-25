#include <string>
#include <vector>
#include "Node.hpp"
#include "OrgChart.hpp"
#include <iostream>
#include <unordered_map>

using namespace std;
const int spacesToPrint = 5;


namespace ariel
{

    OrgChart::OrgChart()
    {
        this->root = nullptr; // initialize root to nullptr
        this->size = 0;       // initialize size to 0
    };

    OrgChart::~OrgChart(){
        // Node *current = this->root;
        // delete_tree(current);
    };

  
    OrgChart::OrgChart(OrgChart &&other) noexcept
    {
        this->root = other.root;
        this->size = other.size;
        other.root = nullptr;
        other.size = 0;
    };

    void OrgChart::delete_tree(Node *current){
        if(current == nullptr){
            return;
        }
        for (uint i = 0; i < current->children.size(); i++) {
            delete_tree(current->children[i]);
        }
        delete current;
    };

    
    OrgChart &OrgChart::add_root(const string &role) // Add or change the root of the OrgChart
    {
        if (this->root == nullptr)
        {
            this->root = new Node(role);
            this->size++;
        }
        else
        {
            Node *temp = this->root;
            this->root = new Node(role);
            for (uint i =0; i < temp->children.size(); i++)
            {
                this->root->children.push_back(temp->children[i]);
            }
        }
        return *this;
    }

    int OrgChart::getIndex(Node &node) const // Get the index of a node in parent vector for printing
    {
        int result = 0;
        if (node.getParent() != this->root)
        {
            for (Node *children : node.getParent()->getParent()->children)
            {
                if (children == node.getParent())
                {
                    break;
                }
                result++;
            }
        }
        return result;
    }

    Node *OrgChart::getRoot() const
    {
        return this->root;
    }

    Node *findNode(const string &name, Node *root) // Find a node by name/role
    {
        if (root->getRole() == name)
        {
            return root;
        }
        for (uint i = 0; i < root->children.size(); i++)
        {
            Node *node = findNode(name, root->children[i]); // Recursive call to find the node
            if (node != nullptr)
            {
                return node;
            }
        }
        return nullptr;
    }

    OrgChart &OrgChart::add_sub(const string &parent, const string &child) // Add a child to a parent.
    {
        if (root == nullptr)
        {
            throw "No root";
        }
        Node *parentNode = findNode(parent, root);
        if (parentNode == nullptr) // if parent doesn't exist.
        {
            throw "Parent does not exist";
        }
        Node *childNode = new Node(child);
        parentNode->children.push_back(childNode);
        childNode->parent = parentNode;
        this->size++;
        return *this;
    }

    // Helper function that returns 2 dimentional map of level and nodes on the level
    void level_order_helper(Node *node, int level, unordered_map<int, vector<Node *>> &map)
    {
        if (node == nullptr)
        {
            return;
        }
        map[level].push_back(node); // Add node to the level by its level
        for (uint i = 0; i < node->children.size(); i++)
        {
            level_order_helper(node->children[i], level + 1, map); // Adding children by their level to the next level
        }
    }

    void pre_order_helper(Node *node, vector<Node *> &vec) // Helper function that returns a vector of nodes in pre_order.
    {
        if (node == nullptr)
        {
            return;
        }
        vec.push_back(node);
        for (uint i = 0; i < node->children.size(); i++)
        {
            pre_order_helper(node->children[i], vec); // Adding children first in pre_order.
        }
    }

    ostream &operator<<(ostream &os, const OrgChart &org) // Printing OrgChart.
    {
        unordered_map<int, vector<Node *>> map;
        Node *root = org.root;
        level_order_helper(root, 0, map); // Get the map of level and nodes on the level
        string tempJ;

        for (int i = 0; !map[i].empty(); i++) // Printing nodes by level.
        {
            for (Node *j : map[i]) // Printing nodes on this level.
            {
                Node *node = findNode(j->getRole(), org.root);
                if (node == org.root)
                {
                    int size = org.size;
                    for (int i = 0; i < size * spacesToPrint / 2; i++) // Printing the root in the center.
                    {
                        os << " ";
                    }
                    os << node->getRole();
                }
                else if (node->children.size() > 1)
                {
                    for (int k = 0; k < (node->children.size() + 1 / 2) * spacesToPrint; k++) // Printing the parent in the center.
                    {
                        os << " ";
                    }
                    os << j->getRole();
                }
                else if (node->getParent() == org.root)
                {
                    os << "     " << j->getRole();
                }

                else
                {
                    if (node == node->parent->children[0])
                    { // Printing the first child in the center with the needed spaces.
                        int parentIndex = org.getIndex(*node);
                        if (parentIndex == 0)
                        {
                            parentIndex++;
                        }
                        for (int k = 0; k < (parentIndex * spacesToPrint) - parentIndex; k++)
                        {
                            os << " ";
                        }
                    }
                    os << " " << j->getRole();
                }
            }
            os << endl;
        }
        return os;
    }

    OrgChart::Iterator::Iterator()
    {
        current = nullptr;
        flag = "default"; // level_order
    }

    OrgChart::Iterator::Iterator(Node *root)
    {
        this->current = root;
        flag = "default"; // level_order
    }

    OrgChart::Iterator::Iterator(Node *root,const string &flag)
    {
        this->current = root;
        this->flag = flag;

        if (flag == "defualt")
        {                                           // level_order
            unordered_map<int, vector<Node *>> map; // Map of level and nodes on the level
            level_order_helper(root, 0, map);

            for (int i = 0; !map[i].empty(); i++)
            {
                for (Node *j : map[i])
                {
                    this->def_queue.push(j);
                }
            }
        }
        else if (flag == "reverse_level")
        {
            unordered_map<int, vector<Node *>> map; //
            level_order_helper(root, 0, map);

            for (int i = (int)map.size() - 1; i >= 0; i--) // From the end to the start.
            {
                for (Node *j : map[i])
                {
                    this->def_queue.push(j);
                }
            }
        }
        else if (flag == "pre_order")
        {
            vector<Node *> vec;
            pre_order_helper(root, vec);
            for (Node *j : vec)
            {
                this->def_queue.push(j); // Adding nodes to the queue in pre_order.
            }
        }
    }

    bool OrgChart::Iterator::operator==(const Iterator &other) const
    {
        return this->current == other.current;
    }

    bool OrgChart::Iterator::operator!=(const Iterator &other) const
    {
        if (this->def_queue.empty())
        {
            return false;
        }
        return !(*this == other);
    }

    OrgChart::Iterator &OrgChart::Iterator::operator++()
    {
        if (!def_queue.empty())
        {
            def_queue.pop();
        }
        else
        {
            *this = OrgChart::Iterator(nullptr);
        }
        return *this;
    }

    string OrgChart::Iterator::operator*()
    {
        return def_queue.front()->getRole();
    }

    string *OrgChart::Iterator::operator->()
    {
        return &def_queue.front()->role;
    }

    OrgChart::Iterator OrgChart::begin_level_order() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(this->root, "defualt");
    }

    OrgChart::Iterator OrgChart::end_level_order() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(nullptr);
    }

    OrgChart::Iterator OrgChart::reverse_order() const 
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(nullptr);
    }

    OrgChart::Iterator OrgChart::begin_preorder() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(this->root, "pre_order");
    }

    OrgChart::Iterator OrgChart::end_preorder() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(nullptr);
    }

    OrgChart::Iterator OrgChart::begin_reverse_order() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(this->root, "reverse_level");
    }

    

    OrgChart::Iterator OrgChart::begin() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(this->root, "defualt");
    }

    OrgChart::Iterator OrgChart::end() const
    {
        if (this->root == nullptr)
        {
            throw "Chart is empty!";
        }
        return OrgChart::Iterator(nullptr);
    }

    void initHelper_leve(Node *node, queue<Node *> &q)
    {
        if (node == nullptr)
        {
            return;
        }
        q.push(node);
        for (uint i = 0; i < node->children.size(); i++)
        {
            initHelper_leve(node->children[i], q);
        }
    }
}