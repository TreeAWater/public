#include <iostream>
#include <stack>
#include <string>
#include <cctype>  // For isdigit function
#include <unordered_map>

// 定义运算符优先级
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0; // 非运算符（如括号）
}

// 判断是否为右结合运算符
bool isRightAssociative(char op) {
    return op == '^';
}

// 判断是否为运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 中缀表达式转后缀表达式
std::string infixToPostfix(const std::string& infix) {
    std::stack<char> s;  // 操作符栈
    std::string postfix; // 存储后缀表达式

    for (char ch : infix) {
        if (isdigit(ch)) {
            // 如果是数字，直接添加到后缀表达式中
            postfix += ch;
        } else if (ch == '(') {
            // 左括号直接入栈
            s.push(ch);
        } else if (ch == ')') {
            // 遇到右括号时，弹出栈顶直到左括号为止
            while (!s.empty() && s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop(); // 弹出左括号
        } else if (isOperator(ch)) {
            // 遇到运算符时，处理优先级
            while (!s.empty() && precedence(s.top()) >= precedence(ch)) {
                // 特殊处理：如果是右结合运算符且优先级相等，不弹栈
                if (isRightAssociative(ch) && precedence(s.top()) == precedence(ch)) {
                    break;
                }
                postfix += s.top();
                s.pop();
            }
            // 将当前运算符入栈
            s.push(ch);
        }
    }

    // 将栈中剩余的运算符依次弹出
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

int main() {
    std::string infix = "5*6+(5+4*3)^2";  // 示例输入
    std::string postfix = infixToPostfix(infix);
    std::cout << "中缀表达式: " << infix << std::endl;
    std::cout << "后缀表达式: " << postfix << std::endl;
    return 0;
}
