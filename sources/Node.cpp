#include <string>
#include <vector>
#include "Node.hpp"

using namespace std;

namespace ariel{
    Node::Node(const string &role){ // Node for every role in the OrgChart
        if (role.empty()){
            throw "Role cannot be empty";
        }
        this->role = role; // Role of the node
        this->children = vector<Node *>(); // Children of the node
        this->parent = nullptr; 
        this->size = 0;
    }

    Node::Node(const Node &other){ // Copy constructor
        this->role = other.role;
        this->children = other.children;
        this->parent = other.parent;
        this->size = other.size;
    }

    Node::Node(Node &&other) noexcept{ // Move constructor
        this->role = other.role;
        this->children = other.children;
        this->parent = other.parent;
        this->size = other.size;
    }

    Node::~Node(){ // Destructor
        for(uint i = 0; i < this->children.size(); i++){
            delete this->children[i];
        }
    }
    // Copy assignment operator
    Node &Node::operator=(const Node &other){
        if(this != &other){
            this->role = other.role;
            this->children = other.children;
            this->parent = other.parent;
            this->size = other.size;
        }
        return *this;
    }
    // Move assignment operator
    Node &Node::operator=(Node &&other) noexcept{
        if(this != &other){
            this->role = other.role;
            this->children = other.children;
            this->parent = other.parent;
            this->size = other.size;
        }
        return *this;
    }
    string Node::getRole() const{
        return this->role; // Get the role of the node
    }

    Node* Node::getParent() const{
        return this->parent; // Get the parent of the node
    }
    
}