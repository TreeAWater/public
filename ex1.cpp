#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Class to represent a single term (monomial) of the polynomial
class Item
{
public:
    int coef;  // Coefficient of the term
    int power; // Exponent (power) of the term

    // Constructor with default values
    Item(int c = 0, int p = 0) : coef(c), power(p) {}
};

// Node structure for the linked list
struct Node
{
    Item term;     // The term stored in the node
    Node* next;    // Pointer to the next node

    // Constructor
    Node(Item t, Node* n = nullptr) : term(t), next(n) {}
};

// Class to represent a polynomial using a linked list
class Polynomial
{
private:
    Node* head;    // Head pointer of the linked list

public:
    // Constructor initializes the head to nullptr
    Polynomial() : head(nullptr) {}

    // Destructor to free the allocated memory
    ~Polynomial()
    {
        Node* current = head;
        while (current)
        {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
    }

    // Function to insert a term into the polynomial in order (from high power to low power)
    void insertTerm(Item term)
    {
        Node* newNode = new Node(term);
        // If the list is empty or the term has higher power than the head
        if (!head || term.power > head->term.power)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node* current = head;
            Node* prev = nullptr;
            // Find the correct position to insert the term
            while (current && term.power <= current->term.power)
            {
                if (term.power == current->term.power)
                {
                    // If same power, add the coefficients
                    current->term.coef += term.coef;
                    delete newNode;
                    return;
                }
                prev = current;
                current = current->next;
            }
            // Insert the new term into the list
            newNode->next = current;
            prev->next = newNode;
        }
    }

    // Function to display the polynomial from high power to low power
    void display()
    {
        Node* current = head;
        bool first = true; // Flag to handle the '+' sign for the first term
        while (current)
        {
            // Handle the sign of the coefficient
            if (current->term.coef >= 0 && !first)
                cout << "+";
            cout << current->term.coef << "*A^" << current->term.power;
            current = current->next;
            first = false;
        }
        cout << endl;
    }

    // Function to merge another polynomial into this one
    void merge(Polynomial& p)
    {
        Node* current = p.head;
        while (current)
        {
            // Insert each term of the other polynomial
            insertTerm(current->term);
            current = current->next;
        }
    }

    // Function to parse the input string and build the polynomial
    void parsePolynomial(const string& str)
    {
        istringstream ss(str);
        char op = '+'; // Current operator, default to '+'
        while (!ss.eof())
        {
            int coef = 0;
            int power = 0;
            char variable;
            char caret;
            // Read the operator if any
            if (ss.peek() == '+' || ss.peek() == '-')
                ss >> op;
            // Read the coefficient
            ss >> coef;
            // Read the variable part
            ss >> variable >> caret >> power;
            // Adjust the coefficient based on the operator
            if (op == '-')
                coef = -coef;
            // Insert the term into the polynomial
            insertTerm(Item(coef, power));
        }
    }
};

int main()
{
    // Polynomials to hold the input and the result
    Polynomial poly1, poly2, result;

    // Input strings for the polynomials
    string input1, input2;

    // Read and parse the first polynomial
    cout << "输入多项式1：";
    getline(cin, input1);
    poly1.parsePolynomial(input1);

    // Read and parse the second polynomial
    cout << "输入多项式2：";
    getline(cin, input2);
    poly2.parsePolynomial(input2);

    // Merge the two polynomials
    result = poly1;
    result.merge(poly2);

    // Display the polynomials
    cout << "多项式1：";
    poly1.display();

    cout << "多项式2：";
    poly2.display();

    cout << "合并后的多项式：";
    result.display();

    return 0;
}
