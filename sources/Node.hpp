#include <string>
#include <vector>

using namespace std;

#pragma once
namespace ariel
{
    class Node
    {
    public:
        Node(const string &role);
        Node(const Node &other);
        Node(Node &&other) noexcept;
        ~Node();
        Node &operator=(const Node &other);
        Node &operator=(Node &&other) noexcept;

        string getRole() const;
        Node *getParent() const;
        Node *parent;
        vector<Node *> children;
        string role;
        int size;
    };
}