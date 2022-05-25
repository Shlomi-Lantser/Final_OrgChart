#include <string>
#include <vector>
#include "Node.hpp"
#include <stack>
#include <queue>
#include <iostream>


using namespace std;

namespace ariel
{
    class OrgChart
    {

        class Iterator
        {
            public:
                Iterator();
                Iterator(Node *root); // constructor
                Iterator(Node *root,const string &flag); // constructor
                Iterator &operator++();
                bool operator==(const Iterator &other) const; 
                bool operator!=(const Iterator &other) const;
                string operator*();
                string *operator->();
                void initialize();
                string flag; // Flag of iterator (default, reverse_level, pre_order)
                Node *current; // current node
                queue<Node *> def_queue; // default queue
                stack<Node *> reverse_def; // reverse default queue
        };

    public:
        OrgChart();
        ~OrgChart();
        OrgChart(const OrgChart &other); // copy constructor
        OrgChart &operator=(const OrgChart &other); // copy assignment operator
        OrgChart &add_root(const string &role); // Add or change the root of the OrgChart
        OrgChart &add_sub(const string &parent,const string &child); // Add a child to a parent
        void delete_tree(Node *current); // Delete the OrgChart
        Node *getRoot() const; // Get the root of the OrgChart
        int size; // Size of OrgChart
        friend ostream &operator<<(ostream &os, const OrgChart &org); // Print OrgChart (friend because the left operand)
        int getIndex(Node &node) const ; // Get the index of a node in parent vector
        Iterator begin() const ; // Get the begin iterator
        Iterator end() const ; // Get the end iterator
        Iterator begin_level_order() const; // Get the begin iterator of level_order
        Iterator end_level_order() const ;// Get the end iterator of level_order
        Iterator begin_reverse_order() const ; // Get the begin iterator of reverse_order
        Iterator reverse_order() const ;// Get the end iterator of reverse_order
        Iterator begin_preorder() const ;// Get the begin iterator of pre_order
        Iterator end_preorder() const ;  // Get the end iterator of pre_order
        OrgChart &operator=(OrgChart &&other) noexcept;
        OrgChart(OrgChart &&other) noexcept;
        Node *root;
    };
}