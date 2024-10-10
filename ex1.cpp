#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// 类表示多项式中的一项（单项式）
class Item
{
public:
    int coef;  // 系数
    int power; // 指数（幂）

    // 构造函数，带默认值
    Item(int c = 0, int p = 0) : coef(c), power(p) {}
};

// 节点结构，用于构建链表
struct Node
{
    Item term;   // 存储在节点中的项
    Node* next;  // 指向下一个节点的指针

    // 构造函数
    Node(Item t, Node* n = nullptr) : term(t), next(n) {}
};

// 类表示多项式，使用链表实现
class Polynomial
{
private:
    Node* head; // 链表的头指针

public:
    // 构造函数，初始化头指针为 nullptr
    Polynomial() : head(nullptr) {}

    // 拷贝构造函数，实现深拷贝
    Polynomial(const Polynomial& other)
    {
        head = nullptr;
        Node* current = other.head;
        Node* tail = nullptr; // 用于跟踪新链表的最后一个节点
        while (current)
        {
            // 创建一个具有相同项的新节点
            Node* newNode = new Node(current->term);
            if (!head)
                head = newNode;
            else
                tail->next = newNode;
            tail = newNode;
            current = current->next;
        }
    }

    // 赋值运算符，实现深拷贝
    Polynomial& operator=(const Polynomial& other)
    {
        if (this == &other)
            return *this;

        // 首先，删除现有链表
        clear();

        // 然后，从其他多项式复制
        Node* current = other.head;
        Node* tail = nullptr;
        while (current)
        {
            Node* newNode = new Node(current->term);
            if (!head)
                head = newNode;
            else
                tail->next = newNode;
            tail = newNode;
            current = current->next;
        }
        return *this;
    }

    // 析构函数，释放分配的内存
    ~Polynomial()
    {
        clear();
    }

    // 清空多项式，释放内存
    void clear()
    {
        Node* current = head;
        while (current)
        {
            Node* temp = current->next;
            delete current;
            current = temp;
        }
        head = nullptr;
    }

    // 向多项式中插入一项，保持从高到低的顺序
    void insertTerm(Item term)
    {
        if (term.coef == 0)
            return; // 跳过系数为零的项

        Node* newNode = new Node(term);
        // 如果链表为空，或者该项的指数大于头节点
        if (!head || term.power > head->term.power)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node* current = head;
            Node* prev = nullptr;
            // 找到正确的位置插入该项
            while (current && term.power < current->term.power)
            {
                prev = current;
                current = current->next;
            }
            if (current && term.power == current->term.power)
            {
                // 如果指数相同，累加系数
                current->term.coef += term.coef;
                delete newNode;
                // 如果系数为零，删除该节点
                if (current->term.coef == 0)
                {
                    if (prev)
                        prev->next = current->next;
                    else
                        head = current->next;
                    delete current;
                }
            }
            else
            {
                // 插入新的节点
                newNode->next = current;
                if (prev)
                    prev->next = newNode;
                else
                    head = newNode;
            }
        }
    }

    // 显示多项式，从高次到低次
    void display()
    {
        Node* current = head;
        bool first = true; // 用于处理第一个项的正号
        while (current)
        {
            // 处理系数的符号
            if (current->term.coef >= 0 && !first)
                cout << "+";
            cout << current->term.coef << "*A^" << current->term.power;
            current = current->next;
            first = false;
        }
        cout << endl;
    }

    // 直接合并两个多项式，保持时间复杂度低
    static Polynomial mergePolynomials(const Polynomial& p1, const Polynomial& p2)
    {
        Polynomial result;
        Node* ptr1 = p1.head;
        Node* ptr2 = p2.head;
        Node* tail = nullptr; // 用于跟踪 result 链表的最后一个节点

        while (ptr1 && ptr2)
        {
            Item term;
            if (ptr1->term.power > ptr2->term.power)
            {
                term = ptr1->term;
                ptr1 = ptr1->next;
            }
            else if (ptr1->term.power < ptr2->term.power)
            {
                term = ptr2->term;
                ptr2 = ptr2->next;
            }
            else // 指数相同，系数相加
            {
                int coefSum = ptr1->term.coef + ptr2->term.coef;
                if (coefSum != 0)
                {
                    term = Item(coefSum, ptr1->term.power);
                }
                else
                {
                    // 系数和为零，跳过该项
                    ptr1 = ptr1->next;
                    ptr2 = ptr2->next;
                    continue;
                }
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }
            // 将新项添加到结果多项式
            Node* newNode = new Node(term);
            if (!result.head)
            {
                result.head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }

        // 将剩余的项添加到结果多项式
        while (ptr1)
        {
            Node* newNode = new Node(ptr1->term);
            if (!result.head)
            {
                result.head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            ptr1 = ptr1->next;
        }

        while (ptr2)
        {
            Node* newNode = new Node(ptr2->term);
            if (!result.head)
            {
                result.head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            ptr2 = ptr2->next;
        }

        return result;
    }

    // 解析输入的多项式字符串并构建多项式
    void parsePolynomial(const string& str)
    {
        string s = str;
        // 删除字符串中的所有空格
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        size_t index = 0;
        char op = '+'; // 当前操作符

        while (index < s.length())
        {
            // 读取操作符
            if (s[index] == '+' || s[index] == '-')
            {
                op = s[index];
                index++;
            }
            else if (index == 0)
            {
                op = '+';
            }

            // 读取系数
            int coef = 0;
            int sign = (op == '-') ? -1 : 1;
            size_t coef_start = index;

            // 处理省略系数的情况（默认为1）
            if (s[index] == 'A')
            {
                coef = 1 * sign;
            }
            else
            {
                // 读取系数的数字部分
                while (index < s.length() && isdigit(s[index]))
                    index++;

                string coef_str = s.substr(coef_start, index - coef_start);
                if (coef_str.empty())
                    coef = 1 * sign;
                else
                    coef = stoi(coef_str) * sign;
            }

            // 期望有一个 '*'
            if (index < s.length() && s[index] == '*')
                index++; // 跳过 '*'

            // 读取变量 'A'
            if (index < s.length() && s[index] == 'A')
                index++;
            else
            {
                cerr << "解析错误：缺少变量 'A'\n";
                return;
            }

            // 期望有一个 '^'
            if (index < s.length() && s[index] == '^')
                index++; // 跳过 '^'
            else
            {
                cerr << "解析错误：缺少 '^'\n";
                return;
            }

            // 读取指数
            size_t exp_start = index;
            while (index < s.length() && isdigit(s[index]))
                index++;

            if (exp_start == index)
            {
                cerr << "解析错误：缺少指数\n";
                return;
            }

            string exp_str = s.substr(exp_start, index - exp_start);
            int power = stoi(exp_str);

            // 将项插入多项式
            insertTerm(Item(coef, power));
        }
    }
};

int main()
{
    // 用于存储输入和结果的多项式
    Polynomial poly1, poly2, result;

    // 多项式的输入字符串
    string input1, input2;

    // 读取并解析第一个多项式
    cout << "输入多项式1：";
    getline(cin, input1);
    poly1.parsePolynomial(input1);

    // 读取并解析第二个多项式
    cout << "输入多项式2：";
    getline(cin, input2);
    poly2.parsePolynomial(input2);

    // 合并两个多项式，直接合并链表，时间复杂度 O(n)
    result = Polynomial::mergePolynomials(poly1, poly2);


    cout << "合并后的多项式：";
    result.display();

    return 0;
}
