#include <iostream>
#include <stack>
#include <string>
#include <cctype> // 使用isdigit判断数字

using namespace std;

// 定义运算符的优先级
int level(char ch) {
    if (ch == '^') return 3;
    if (ch == '*' || ch == '/') return 2;
    if (ch == '+' || ch == '-') return 1;
    return 0; // 对于括号等其他字符返回0
}

// 判断运算符是否为幂运算符（右结合）
bool isPower(char op) {
    return op == '^';
}

// 判断字符是否为运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 将中缀表达式转换为后缀表达式
void infixToPostfix(string ss) {
    stack<char> st;
    for (char ch : ss) {
        if (isdigit(ch)) {
            // 如果是数字，直接输出
            cout << ch;
        } else if (ch == '(') {
            // 左括号直接入栈
            st.push(ch);
        } else if (ch == ')') {
            // 右括号处理：弹出栈顶直到遇到左括号
            while (!st.empty() && st.top() != '(') {
                cout << st.top();
                st.pop();
            }
            if (!st.empty()) st.pop(); // 弹出左括号
        } else if (isOperator(ch)) {
            // 处理运算符
            while (!st.empty() && level(st.top()) >= level(ch)) {
                // 幂运算符是右结合的，因此遇到同级别的右结合运算符时不能弹出
                if (isPower(ch) && isPower(st.top())) {
                    break;
                }
                cout << st.top();
                st.pop();
            }
            st.push(ch);
        }
    }

    // 将栈中剩余的运算符依次弹出
    while (!st.empty()) {
        cout << st.top();
        st.pop();
    }
}

int main() {
    string ss = "";
    getline(cin, ss); // 输入中缀表达式
    infixToPostfix(ss); // 转换为后缀表达式并输出
    return 0;
}
