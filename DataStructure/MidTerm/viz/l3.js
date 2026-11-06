// l3.js — 第 3 讲：单链表原地反转 与 约瑟夫环
(function () {
  const V = window.Viz;

  // ---------- 动画 1：原地反转 ----------
  (function reverse() {
    const vals = [1, 2, 3, 4, 5];
    const n = vals.length;
    const frames = [];

    const links = new Array(n).fill("fwd");
    let prev = -1, cur = -1, next = -1, headTo = 0;

    const name = (i) => (i < 0 ? "∅" : vals[i]);
    const push = (note, line, extra) =>
      frames.push({
        links: links.slice(), prev, cur, next, headTo, line, note,
        vars: Object.assign({
          "prev": name(prev), "cur": name(cur), "next": name(next),
          "head->link": name(headTo),
        }, extra || {}),
      });

    push("初始化：prev、cur、next 都还没有指向真结点", 1);
    cur = 0;
    push("cur 从第一个结点开始；prev 保持 ∅", 2);

    while (cur >= 0) {
      push(`while 判断：cur = ${vals[cur]}，非空，进入循环`, 3);
      next = cur + 1 < n ? cur + 1 : -1;
      push("先把后继存进 next，否则掉头后链表就断了", 4);
      links[cur] = prev === -1 ? "null" : "back";
      push(`cur->link 改成指向 ${name(prev)}`, 5);
      prev = cur;
      push("prev 前移一格", 6);
      cur = next;
      push("cur 前移一格", 7);
    }
    push("while 判断：cur 为空，循环结束", 8);
    headTo = prev;
    push("最后别忘了把 head->link 指向新的首结点（原来的尾）", 9);

    const W = 760, H = 285;
    const CW = 86, X0 = 130, Y = 110;

    function draw(g, f) {
      const nx = (i) => X0 + i * CW;

      V.box(g, 30, Y, 56, 44, "head", "hl");
      V.text(g, 58, Y - 12, "表头", "small");
      V.arrow(g, 86, Y + 22, nx(f.headTo) - 4, Y + 22, "acc");

      for (let i = 0; i < n; i++) {
        const cls = i === f.cur ? "hot" : i === f.prev ? "acc" : "";
        V.box(g, nx(i), Y, CW - 22, 44, String(vals[i]), cls);
        V.text(g, nx(i) + (CW - 22) / 2, Y + 62, "p" + (i + 1), "small");
      }

      for (let i = 0; i < n; i++) {
        const d = f.links[i];
        if (d === "fwd") {
          if (i + 1 < n) V.arrow(g, nx(i) + CW - 22, Y + 22, nx(i + 1) - 4, Y + 22);
          else {
            V.line(g, nx(i) + CW - 22, Y + 22, nx(i) + CW - 2, Y + 22, "");
            V.text(g, nx(i) + CW + 6, Y + 26, "∅", "small");
          }
        } else if (d === "back") {
          V.curve(g, nx(i) - 2, Y + 10, nx(i - 1) + CW - 18, Y + 10, 48, "acc");
        } else {
          const cx = nx(i) + (CW - 22) / 2;
          V.arrow(g, cx, Y + 44, cx, Y + 68, "");
          V.text(g, cx, Y + 82, "∅", "small");
        }
      }

      const mark = (idx, nm, col, dy) => {
        if (idx < 0 || idx >= n) return;
        const x = nx(idx) + (CW - 22) / 2;
        V.arrow(g, x, Y - 26 - dy, x, Y - 6, col);
        V.text(g, x, Y - 34 - dy, nm, "small " + col);
      };
      if (f.next === f.cur) {
        mark(f.cur, "cur·next", "hot", 0);
      } else {
        mark(f.cur, "cur", "hot", 0);
        mark(f.next, "next", "", 0);
      }
      mark(f.prev, "prev", "acc", 18);

      V.text(g, X0 - 100, Y + 150, f.note, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "单链表原地反转：一条语句一条语句地走",
      note: "反转的关键顺序：先存 next，再改 cur->link，最后才移动 prev 和 cur。任何一步颠倒都会丢结点。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "Node *prev = NULL;",
        "Node *cur = head->link;",
        "while (cur) {",
        "    Node *next = cur->link;   // 1. 记住后继",
        "    cur->link = prev;         // 2. 掉头",
        "    prev = cur;               // 3. prev 前进",
        "    cur = next;               // 4. cur 前进",
        "}",
        "head->link = prev;",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 2：约瑟夫环 ----------
  (function josephus() {
    const n = 8, m = 3;
    const frames = [];
    let alive = [];
    for (let i = 1; i <= n; i++) alive.push(i);
    let pos = 0, k = 1;
    const out = [];

    const push = (o) => frames.push(Object.assign({ out: out.slice() }, o));

    push({
      alive: alive.slice(), cur: alive[0], line: 1,
      vars: { "k": 0, "j": 0, "p": alive[0], "pre": alive[n - 1], "出列": "—" },
      note: "8 人围成单循环链表，从 1 号开始报数",
    });

    while (alive.length > 1) {
      const preIdx = (pos - 1 + alive.length) % alive.length;
      const who = alive[pos];
      push({
        alive: alive.slice(), cur: who, line: 3,
        vars: { "k": k, "j": k, "p": who, "pre": alive[preIdx], "出列": "—" },
        note: `${who} 号报 ${k}`,
      });
      if (k === m) {
        push({
          alive: alive.slice(), cur: who, dead: who, line: 5,
          vars: { "k": k, "j": k, "p": who, "pre": alive[preIdx], "出列": out.join(" ") || "—" },
          note: `${who} 号报到 ${m}，输出它`,
        });
        out.push(who);
        alive.splice(pos, 1);
        if (alive.length === 1) break;
        pos = pos % alive.length;
        const np = (pos - 1 + alive.length) % alive.length;
        push({
          alive: alive.slice(), cur: alive[pos], line: 7,
          vars: { "k": k, "j": k, "p": alive[pos], "pre": alive[np], "出列": out.join(" ") },
          note: "从环上摘掉，p 指向下一个",
        });
        k = 1;
      } else {
        k++;
        pos = (pos + 1) % alive.length;
      }
    }

    push({
      alive: alive.slice(), cur: alive[0], dead: alive[0], line: 9,
      vars: { "k": "—", "j": "—", "p": alive[0], "pre": "—", "出列": out.join(" ") },
      note: `幸存者：${alive[0]} 号`,
    });

    const W = 760, H = 340;
    const CX = 300, CY = 165, R = 110;

    function draw(g, f) {
      const posOf = (v) => {
        const a = -Math.PI / 2 + (2 * Math.PI * (v - 1)) / n;
        return [CX + R * Math.cos(a), CY + R * Math.sin(a)];
      };

      V.el("circle", { cx: CX, cy: CY, r: R, class: "ln", fill: "none", "stroke-dasharray": "3 5" }, g);

      for (let v = 1; v <= n; v++) {
        const [x, y] = posOf(v);
        const gone = !f.alive.includes(v);
        const isCur = v === f.cur && !gone;
        V.circle(g, x, y, 22, String(v), gone ? "dim" : isCur ? "hot" : "");
        if (gone) {
          V.line(g, x - 18, y - 18, x + 18, y + 18, "hot");
          V.line(g, x - 18, y + 18, x + 18, y - 18, "hot");
        }
        if (f.dead === v && gone) V.text(g, x, y - 30, "出列", "small hot");
      }

      if (f.line === 3) V.text(g, CX, CY + 6, `报数 ${f.vars.j}`, "ok");
      if (f.line === 9) V.text(g, CX, CY + 6, "幸存者", "ok");

      V.text(g, 500, 60, "出列顺序", "small", "start");
      V.text(g, 500, 88, f.out.length ? f.out.join(" → ") : "（还没有人出列）", "mono", "start");
      V.text(g, 500, 130, `n = ${n}　m = ${m}`, "small", "start");
      V.text(g, 500, 250, "公式：J(n) = (J(n-1) + m) mod n", "small", "start");
      V.text(g, 500, 274, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "约瑟夫环：报数 → 出列 → 重新报数",
      note: "用单循环链表实现时，pre 始终是 p 的前驱，摘结点就是 pre->link = p->link。也可以不用链表，直接推公式 J(n) = (J(n-1)+m) mod n。",
      width: W, height: H, frames, draw, interval: 560,
      code: [
        "for (k = 0; k < n - 1; ++k) {",
        "    for (j = 1; j < m; ++j) {",
        "        pre = p;  p = p->link;   // 报数",
        "    }",
        '    printf("%d ", p->data);      // 出列',
        "    pre->link = p->link;  delete p;",
        "    p = pre->link;               // 重新开始",
        "}",
        'printf("幸存者：%d", p->data);',
      ],
      caption: (f) => f.note,
    });
  })();
})();
