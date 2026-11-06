// l5.js — 第 5 讲：括号匹配的栈、中缀转后缀的 isp/icp
(function () {
  const V = window.Viz;

  // ---------- 动画 1：括号匹配 ----------
  (function brackets() {
    const s = "{[()]}";
    const frames = [];
    const stack = [];
    const push = (o) => frames.push(Object.assign({ stack: stack.slice(), line: 2, vars: {} }, o));

    push({ i: -1, note: "栈空开始扫描", ok: true, line: 2,
      vars: { "当前字符": "—", "栈": "（空）", "结论": "继续扫描" } });

    for (let i = 0; i < s.length; i++) {
      const c = s[i];
      if ("([{".includes(c)) {
        stack.push(c);
        push({
          i, note: `${c} 是左括号，入栈`, ok: true, line: 5,
          vars: { "当前字符": c, "栈": stack.join(" ") || "（空）", "结论": "push" },
        });
      } else {
        const top = stack.pop();
        const want = { ")": "(", "]": "[", "}": "{" }[c];
        const ok = top === want;
        push({
          i, note: ok ? `${c} 与栈顶 ${want} 配对，出栈` : `${c} 与栈顶不配对`,
          ok, line: 9,
          vars: { "当前字符": c, "栈": stack.join(" ") || "（空）", "结论": ok ? "配对成功" : "配对失败" },
        });
      }
    }
    const ok = stack.length === 0;
    push({
      i: s.length, note: ok ? "扫描结束，栈空 → 匹配" : "扫描结束但栈非空 → 左括号多了", ok, line: 12,
      vars: { "当前字符": "结束", "栈": stack.join(" ") || "（空）", "结论": ok ? "匹配" : "不匹配" },
    });

    const W = 760, H = 300;
    const CW = 56, X0 = 60;
    const SY = 200, SW = 52;

    function draw(g, f) {
      V.text(g, X0 - 14, 48, "输入", "small", "end");
      for (let i = 0; i < s.length; i++) {
        V.box(g, X0 + i * CW, 62, CW - 8, 40, s[i], i === f.i ? "hot" : "");
      }

      V.text(g, 420, 48, "栈（左边是栈顶）", "small", "start");
      for (let d = 0; d < f.stack.length; d++) {
        const c = f.stack[f.stack.length - 1 - d];
        V.box(g, 420 + d * SW, 62, SW - 6, 40, c, d === 0 ? "acc" : "");
      }
      if (!f.stack.length) V.text(g, 440, 88, "（空）", "small", "start");

      if (f.i >= 0 && f.i < s.length) {
        V.arrow(g, X0 + f.i * CW + (CW - 8) / 2, 130, X0 + f.i * CW + (CW - 8) / 2, 106, "acc");
      }

      V.text(g, X0, SY, f.note, f.ok ? "ok" : "hot", "start");
      V.text(g, X0, SY + 28, `栈内元素 ${f.stack.length} 个`, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "括号匹配：左括号入栈，右括号和栈顶比",
      note: "只有三种失败：右括号多出来（栈空时弹不出）、类型不对、扫完栈非空。它们分别对应左括号少、错配、左括号多。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "bool bracketMatch(const char *s) {",
        "    for (const char *p = s; *p; ++p) {",
        "        char c = *p;",
        "        if (c == '(' || c == '[' || c == '{') {",
        "            st.push(c);",
        "        } else {",
        "            char top;",
        "            if (!st.pop(top)) return false;",
        "            if (top != want[c]) return false;",
        "        }",
        "    }",
        "    return st.empty();",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 2：中缀转后缀 ----------
  (function infix() {
    const s = "A+B*(C-D)-E/F";
    const isp = { "*": 5, "/": 5, "+": 3, "-": 3, "(": 1, ")": 6 };
    const icp = { "*": 4, "/": 4, "+": 2, "-": 2, "(": 6, ")": 1 };

    const frames = [];
    const st = [];
    let out = "";
    const push = (o) => frames.push(Object.assign({ st: st.slice(), out, vars: {} }, o));

    push({ i: -1, line: 1, note: "初始化：运算符栈空，输出为空",
      vars: { "当前字符": "—", "栈": "（空）", "输出": "（空）" } });

    for (let i = 0; i < s.length; i++) {
      const c = s[i];
      if (c === "(") {
        st.push(c);
        push({ i, line: 4, note: "遇到 (，直接入栈（它的 icp 最大）",
          vars: { "当前字符": c, "栈": st.join(" "), "输出": out || "（空）" } });
      } else if (c === ")") {
        let popped = "";
        while (st.length) {
          const t = st.pop();
          if (t === "(") break;
          out += t;
          popped += t;
        }
        push({ i, line: 6, note: `遇到 )：一直弹到 (，输出 ${popped || "无"}，左括号丢弃`,
          vars: { "当前字符": c, "栈": st.join(" ") || "（空）", "输出": out } });
      } else if (isp[c] !== undefined) {
        let popped = "";
        while (st.length && isp[st[st.length - 1]] >= icp[c]) {
          const t = st.pop();
          out += t;
          popped += t;
        }
        st.push(c);
        push({
          i, line: 8,
          note: popped
            ? `运算符 ${c}：栈顶 isp ≥ icp(${c}) = ${icp[c]}，先弹出 ${popped}`
            : `运算符 ${c}：栈顶优先级不足，直接入栈`,
          vars: { "当前字符": c, "栈": st.join(" "), "输出": out || "（空）" },
        });
      } else {
        out += c;
        push({ i, line: 11, note: `操作数 ${c}：直接输出`,
          vars: { "当前字符": c, "栈": st.join(" ") || "（空）", "输出": out } });
      }
    }
    let rest = "";
    while (st.length) rest += st.pop();
    out += rest;
    push({ i: s.length, line: 14, note: `扫描结束，把栈里剩余的 ${rest} 全部弹出`,
      vars: { "当前字符": "结束", "栈": "（空）", "输出": out } });

    const W = 760, H = 320;
    const CW = 46, X0 = 40;
    const SW = 42;

    function draw(g, f) {
      V.text(g, X0 - 12, 40, "中缀", "small", "end");
      for (let i = 0; i < s.length; i++) {
        V.box(g, X0 + i * CW, 54, CW - 8, 34, s[i], i === f.i ? "hot" : "");
      }

      V.text(g, X0 - 12, 140, "栈", "small", "end");
      f.st.forEach((c, d) => {
        V.box(g, X0 + d * SW, 122, SW - 6, 34, c, d === f.st.length - 1 ? "acc" : "");
      });
      if (!f.st.length) V.text(g, X0 + 20, 146, "（空）", "small", "start");

      V.text(g, X0 - 12, 220, "后缀", "small", "end");
      for (let i = 0; i < f.out.length; i++) {
        V.box(g, X0 + i * (CW - 8), 204, CW - 14, 34, f.out[i], "ok");
      }

      V.text(g, X0, 268, f.note, "small", "start");
      V.text(g, X0, 292, "isp：栈内 ( 1  +- 3  */ 5  ) 0　|　icp：栈外 ( 6  +- 2  */ 4  ) 1", "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "中缀转后缀：栈内 isp 与栈外 icp",
      note: "规则只有一句：新运算符 c 进栈前，把栈顶所有 isp ≥ icp(c) 的运算符弹出去。左右括号的 isp/icp 故意设成反的，就是为了让 ( 压得住、) 一路弹到底。",
      width: W, height: H, frames, draw, interval: 680,
      code: [
        "for (const char *p = s; *p; ++p) {",
        "    char c = *p;",
        "    if (c == '(') {",
        "        st.push(c);",
        "    } else if (c == ')') {",
        "        while (st.pop(op)) { if (op=='(') break; out[k++] = op; }",
        "    } else if (isp(c) >= 0) {",
        "        while (st.peek(t) && isp(t) >= icp(c)) out[k++] = pop();",
        "        st.push(c);",
        "    } else {",
        "        out[k++] = c;",
        "    }",
        "}",
        "while (st.pop(op)) out[k++] = op;",
      ],
      caption: (f) => (f.out ? "当前输出：" + f.out : "还没有输出"),
    });
  })();
})();
