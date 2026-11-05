// Lesson5/src/stack_queue.cpp
// 栈与队列：括号匹配、中缀转后缀并求值、循环队列容量验证。
// 对应学案 05 第四节。
// 构建：g++ -O2 -std=c++17 src/stack_queue.cpp -o output/stack_queue.exe

#include <cstdio>
#include <cstring>

// ---------------- 顺序栈 ----------------
template <class T>
class Stack {
public:
    explicit Stack(int cap) : cap(cap), top(-1), data(new T[cap]) {}
    ~Stack() { delete[] data; }

    bool empty() const { return top == -1; }
    bool full() const { return top == cap - 1; }

    bool push(const T &x) {
        if (full()) return false;
        data[++top] = x;                 // 先加下标，再放元素
        return true;
    }
    bool pop(T &x) {
        if (empty()) return false;
        x = data[top--];                 // 先取元素，再退下标
        return true;
    }
    bool peek(T &x) const {
        if (empty()) return false;
        x = data[top];
        return true;
    }

private:
    int cap;
    int top;                             // 栈顶元素下标，-1 表示空
    T *data;
};

// ---------------- 括号匹配 ----------------
bool bracketMatch(const char *s) {
    Stack<char> st(256);
    for (const char *p = s; *p; ++p) {
        char c = *p;
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            char top;
            if (!st.pop(top)) return false;        // 右括号多出来
            char want = (c == ')') ? '(' : (c == ']') ? '[' : '{';
            if (top != want) return false;         // 类型不配对
        }
    }
    return st.empty();                             // 左括号必须全部消化
}

// ---------------- 中缀转后缀 ----------------
// 找运算符的栈内/栈外优先级；返回 -1 表示不是运算符
int isp(char c) {
    switch (c) {
        case ';': return 0;
        case '(': return 1;
        case '*': case '/': return 5;
        case '+': case '-': return 3;
        case ')': return 6;
        default: return -1;
    }
}
int icp(char c) {
    switch (c) {
        case ';': return 0;
        case '(': return 6;
        case '*': case '/': return 4;
        case '+': case '-': return 2;
        case ')': return 1;
        default: return -1;
    }
}

void infixToPostfix(const char *s, char *out) {
    Stack<char> st(256);
    int k = 0;
    for (const char *p = s; *p; ++p) {
        char c = *p;
        if (c == '(' ) {
            st.push(c);
        } else if (c == ')') {
            char op;
            while (st.pop(op)) {
                if (op == '(') break;          // 弹出左括号但不输出
                out[k++] = op;
            }
        } else if (isp(c) >= 0) {              // 是运算符
            char top;
            while (st.peek(top) && isp(top) >= icp(c)) {
                st.pop(top);
                out[k++] = top;
            }
            st.push(c);
        } else {
            out[k++] = c;                      // 操作数直接输出
        }
    }
    char op;
    while (st.pop(op)) out[k++] = op;
    out[k] = '\0';
}

// ---------------- 后缀求值 ----------------
int evalPostfix(const char *post, const int *val) {
    Stack<int> st(256);
    for (const char *p = post; *p; ++p) {
        char c = *p;
        if (c >= 'A' && c <= 'Z') {
            st.push(val[c - 'A']);
        } else {
            int y = 0, x = 0;
            st.pop(y);                              // 先出的是右操作数
            st.pop(x);                              // 后出的是左操作数
            switch (c) {
                case '+': st.push(x + y); break;
                case '-': st.push(x - y); break;    // 注意是 x - y
                case '*': st.push(x * y); break;
                case '/': st.push(x / y); break;
            }
        }
    }
    int r = 0;
    st.pop(r);
    return r;
}

// ---------------- 循环队列 ----------------
class CircleQueue {
public:
    explicit CircleQueue(int s) : size(s), front(0), rear(0), count(0), data(new int[s]) {}
    ~CircleQueue() { delete[] data; }

    bool empty() const { return front == rear; }
    bool full() const { return (rear + 1) % size == front; }   // 牺牲一个单元

    bool enQueue(int x) {
        if (full()) return false;
        rear = (rear + 1) % size;
        data[rear] = x;
        ++count;
        return true;
    }
    bool deQueue(int &x) {
        if (empty()) return false;
        front = (front + 1) % size;
        x = data[front];
        --count;
        return true;
    }
    int formulaCount() const { return (rear - front + size) % size; }
    int realCount() const { return count; }   // 真实元素个数，用来和公式对照

private:
    int size, front, rear;
    int count;
    int *data;
};

int main() {
    std::printf("=== 任务 1：括号匹配 ===\n");
    const char *cases[] = {"((a+b)*c)-d", "(()", ")()", "{[()]}"};
    for (int i = 0; i < 4; ++i)
        std::printf("  %-14s -> %s\n", cases[i], bracketMatch(cases[i]) ? "匹配" : "不匹配");

    std::printf("\n=== 任务 2：中缀转后缀并求值 ===\n");
    const char *infix = "A+B*(C-D)-E/F";
    char post[128];
    infixToPostfix(infix, post);
    std::printf("  中缀：%s\n  后缀：%s\n", infix, post);

    int val[26] = {0};
    val['A' - 'A'] = 1;
    val['B' - 'A'] = 2;
    val['C' - 'A'] = 3;
    val['D' - 'A'] = 4;
    val['E' - 'A'] = 6;
    val['F' - 'A'] = 2;
    std::printf("  A=1 B=2 C=3 D=4 E=6 F=2 -> 1+2*(3-4)-6/2 = %d\n", evalPostfix(post, val));
    std::printf("  手算：1 + 2*(3-4) - 6/2 = 1 - 2 - 3 = -4\n");

    std::printf("\n=== 任务 3：循环队列（size = 8）===\n");
    CircleQueue q(8);
    int pushed = 0;
    for (int i = 1; i <= 8; ++i) {
        if (q.enQueue(i)) {
            ++pushed;
            std::printf("  进队 %d 成功，当前元素个数 = %d\n", i, q.formulaCount());
        } else {
            std::printf("  进队 %d 失败（判满），当前元素个数 = %d\n", i, q.formulaCount());
        }
    }
    std::printf("  容量 size=8，实际最多存 %d 个\n", pushed);

    int out;
    q.deQueue(out);
    q.deQueue(out);
    q.enQueue(99);
    std::printf("  出队两个再进队一个：公式算得 %d，真实计数 %d\n", q.formulaCount(), q.realCount());
    return 0;
}
