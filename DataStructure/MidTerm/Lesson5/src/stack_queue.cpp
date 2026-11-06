// Lesson5/src/stack_queue.cpp
// 栈与队列：括号匹配、中缀转后缀并求值、循环队列容量验证。
// 对应学案 05 第四节。
// 构建：g++ -O2 -std=c++17 src/stack_queue.cpp -o output/stack_queue.exe
//
// 练习骨架：TODO 处自己补，参考答案在 solution/stack_queue.cpp。

#include <cstdio>
#include <cstring>

// ---------------- 顺序栈（已给，直接用） ----------------
template <class T> class Stack {
  public:
    explicit Stack(int cap) : cap(cap), top(-1), data(new T[cap]) {}
    ~Stack() { delete[] data; }

    bool empty() const { return top == -1; }
    bool full() const { return top == cap - 1; }

    bool push(const T &x) {
        if (full())
            return false;
        data[++top] = x; // 先加下标，再放元素
        return true;
    }
    bool pop(T &x) {
        if (empty())
            return false;
        x = data[top--]; // 先取元素，再退下标
        return true;
    }
    bool peek(T &x) const {
        if (empty())
            return false;
        x = data[top];
        return true;
    }

  private:
    int cap;
    int top; // 栈顶元素下标，-1 表示空
    T *data;
};

// ---------------- 括号匹配 ----------------
// TODO:
// 用栈判断括号是否配对。遇到左括号入栈；遇到右括号时弹栈并检查类型是否对应，
//       栈空（右括号多出来）立刻返回 false；扫完后栈必须为空。
//       自测："((a+b)*c)-d" 匹配、"(()" 不匹配、")()" 不匹配、"{[()]}" 匹配。
bool bracketMatch(const char *s) {
    // TODO
    Stack<char> brackets(256);
    size_t i = 0;
    char x;

    while (s[i] != 0) {
        if (s[i] == '(') {
            brackets.push(s[i]);
        } else if (s[i] == ')') {
            if (brackets.empty())
                return false;
            brackets.pop(x);
        }
    }

    if (brackets.empty())
        return true;
    else
        return false;
}

// ---------------- 中缀转后缀 ----------------
// TODO: 返回运算符的栈内优先级 isp。约定：';' 为 0，'(' 为 1，'*' '/' 为 5，
//       '+' '-' 为 3，')' 为 6；不是运算符返回 -1。
//       对照学案 04 那张操作符优先级表。
int isp(char c) {
    // TODO
    switch (c) {
    case (';'):
        return 0;
    case ('('):
        return 1;
    case ('*'):
    case ('/'):
        return 5;
    case ('+'):
    case ('-'):
        return 3;
    case (')'):
        return 6;
    default:
        return -1;
    }
}

// TODO: 返回运算符的栈外优先级 icp。约定：';' 为 0，'(' 为 6，'*' '/' 为 4，
//       '+' '-' 为 2，')' 为 1；不是运算符返回 -1。
int icp(char c) {
    // TODO
    switch (c) {
    case ';':
        return 0;
    case '(':
        return 6;
    case '*':
    case '/':
        return 4;
    case '+':
    case '-':
        return 2;
    case ')':
        return 1;
    default:
        return -1;
    }
}

// TODO: 中缀表达式转后缀，结果写进 out（以 '\0' 结尾）。
//       操作数直接输出；运算符按 isp/icp 比较后入栈或弹出；
//       遇到右括号一直弹到左括号，左括号丢弃不输出；扫完后把栈里剩下的全部弹出。
//       自测：A+B*(C-D)-E/F 应得到 ABCD-*+EF/-。
void infixToPostfix(const char *s, char *out) {
    // TODO
    Stack<char> st(256);
    int k = 0;
    for (const char *p = s; *p; ++p) {
        char c = *p;
        if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            char op;
            while (st.pop(op)) {
                if (op == '(')
                    break; // 弹出左括号但不输出
                out[k++] = op;
            }
        } else if (isp(c) >= 0) { // 是运算符
            char top;
            while (st.peek(top) && isp(top) >= icp(c)) {
                st.pop(top);
                out[k++] = top;
            }
            st.push(c);
        } else {
            out[k++] = c; // 操作数直接输出
        }
    }
    char op;
    while (st.pop(op))
        out[k++] = op;
    out[k] = '\0';
}

// ---------------- 后缀求值 ----------------
// TODO: 求后缀表达式的值。大写字母是变量，取值 val[c -
// 'A']；遇到运算符弹出两个操作数，
//       注意先弹出的是右操作数、后弹出的是左操作数，做减法除法和顺序正好相反。
//       自测：A=1 B=2 C=3 D=4 E=6 F=2 时，1+2*(3-4)-6/2 = -4。
int evalPostfix(const char *post, const int *val) {
    // TODO
    Stack<int> st(256);
    int ans = 0;
    for (const char *p = post; *p; ++p) {
        char c = *p;
        if (c <= 'Z' && c >= 'A')
            st.push(val[(unsigned char)(c - 'A')]);
        else {
            int rightNum, leftNum;
            st.pop(rightNum);
            st.pop(leftNum);

            switch (c) {
            case '+':
                /* code */
                st.push(leftNum + rightNum);
                break;
            case '-':
                /* code */
                st.push(leftNum - rightNum);
                break;
            case '*':
                /* code */
                st.push(leftNum * rightNum);
                break;
            case '/':
                /* code */
                st.push(leftNum / rightNum);
                break;

            default:
                break;
            }
        }
    }
    st.pop(ans);
    return ans;
}

// ---------------- 循环队列 ----------------
class CircleQueue {
  public:
    explicit CircleQueue(int s)
        : size(s), front(0), rear(0), count(0), data(new int[s]) {}
    ~CircleQueue() { delete[] data; }

    // TODO: 判空 / 判满。判满采用"牺牲一个单元"的方案。
    bool empty() const {
        // TODO
        return front == rear;
    }
    bool full() const {
        // TODO
        return (rear + 1) % size == front;
    }

    // TODO: 入队。满则返回 false；否则 rear 先循环后移一格，再放元素，并维护
    // count。
    bool enQueue(int x) {
        // TODO
        if (full())
            return false;
        else {
            rear = (rear + 1) % size;
            data[rear] = x;
            ++count;
            return true;
        }
    }
    // TODO: 出队。空则返回 false；否则 front 先循环后移一格，再取元素，并维护
    // count。
    bool deQueue(int &x) {
        // TODO
        if (empty())
            return false;
        else {
            front = (front + 1) % size;
            x = data[front];
            --count;
            return true;
        }
    }

    int formulaCount() const { return (rear - front + size) % size; }
    int realCount() const { return count; } // 真实元素个数，用来和公式对照

  private:
    int size, front, rear;
    int count;
    int *data;
};

int main() {
    std::printf("=== 任务 1：括号匹配 ===\n");
    const char *cases[] = {"((a+b)*c)-d", "(()", ")()", "{[()]}"};
    for (int i = 0; i < 4; ++i)
        std::printf("  %-14s -> %s\n", cases[i],
                    bracketMatch(cases[i]) ? "匹配" : "不匹配");

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
    std::printf("  A=1 B=2 C=3 D=4 E=6 F=2 -> 1+2*(3-4)-6/2 = %d\n",
                evalPostfix(post, val));
    std::printf("  手算：1 + 2*(3-4) - 6/2 = 1 - 2 - 3 = -4\n");

    std::printf("\n=== 任务 3：循环队列（size = 8）===\n");
    CircleQueue q(8);
    int pushed = 0;
    for (int i = 1; i <= 8; ++i) {
        if (q.enQueue(i)) {
            ++pushed;
            std::printf("  进队 %d 成功，当前元素个数 = %d\n", i,
                        q.formulaCount());
        } else {
            std::printf("  进队 %d 失败（判满），当前元素个数 = %d\n", i,
                        q.formulaCount());
        }
    }
    std::printf("  容量 size=8，实际最多存 %d 个\n", pushed);

    int out;
    q.deQueue(out);
    q.deQueue(out);
    q.enQueue(99);
    std::printf("  出队两个再进队一个：公式算得 %d，真实计数 %d\n",
                q.formulaCount(), q.realCount());
    return 0;
}
