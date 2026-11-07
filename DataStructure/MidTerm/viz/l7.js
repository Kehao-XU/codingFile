// l7.js — 第 7 讲：前序+中序建树、四种遍历
(function () {
  const V = window.Viz;
  const PRE = "ABHFDECKG";
  const INO = "HBDFAEKCG";

  const nodes = {};
  function build(preStart, inoStart, len, depth) {
    if (len === 0) return null;
    const root = PRE[preStart];
    const k = INO.indexOf(root, inoStart) - inoStart;
    const node = { v: root, depth, x: 0, y: 0, l: null, r: null };
    node.l = build(preStart + 1, inoStart, k, depth + 1);
    node.r = build(preStart + k + 1, inoStart + k + 1, len - k - 1, depth + 1);
    nodes[root] = node;
    return node;
  }
  const root = build(0, 0, PRE.length, 0);

  let counter = 0;
  (function place(nd) {
    if (!nd) return;
    place(nd.l);
    nd.x = 52 + counter++ * 78;
    nd.y = 118 + nd.depth * 62;
    place(nd.r);
  })(root);

  function edges(g, allowed) {
    for (const k in nodes) {
      if (allowed && !allowed.has(k)) continue;
      const nd = nodes[k];
      if (nd.l && (!allowed || allowed.has(nd.l.v))) V.line(g, nd.x, nd.y, nd.l.x, nd.l.y, "");
      if (nd.r && (!allowed || allowed.has(nd.r.v))) V.line(g, nd.x, nd.y, nd.r.x, nd.r.y, "");
    }
  }

  // ---------- 动画 1：建树 ----------
  (function buildAnim() {
    const frames = [];
    const created = [];

    function rec(preStart, inoStart, len, depth) {
      if (len === 0) return;
      const r = PRE[preStart];
      const k = INO.indexOf(r, inoStart) - inoStart;

      const vars = () => ({
        "前序区间": `[${preStart}, ${preStart + len - 1}]`,
        "中序区间": `[${inoStart}, ${inoStart + len - 1}]`,
        "本子树结点数": len,
        "root": r,
        "k（左子树大小）": k,
      });

      frames.push({ preStart, inoStart, len, root: r, k, created: created.slice(), line: 3, vars: vars(),
        note: `取前序第一个 ${r} 作为根` });
      frames.push({ preStart, inoStart, len, root: r, k, created: created.slice(), line: 4, vars: vars(),
        note: `在中序里找 ${r}，它左边有 ${k} 个结点` });
      created.push(r);
      frames.push({ preStart, inoStart, len, root: r, k, created: created.slice(), line: 5, vars: vars(),
        note: `建结点 ${r}：左子树 ${k} 个，右子树 ${len - k - 1} 个` });

      rec(preStart + 1, inoStart, k, depth + 1);
      rec(preStart + k + 1, inoStart + k + 1, len - k - 1, depth + 1);
    }
    rec(0, 0, PRE.length, 0);

    const W = 760, H = 380;
    const CW = 56, X0 = 90;

    function draw(g, f) {
      const drawSeq = (arr, y, start, len, label) => {
        V.text(g, X0 - 60, y + 24, label, "small", "start");
        for (let i = 0; i < arr.length; i++) {
          const inside = i >= start && i < start + len;
          V.box(g, X0 + i * CW, y, CW - 13, 32, arr[i], inside ? "acc" : "dim");
        }
      };
      drawSeq(PRE, 20, f.preStart, f.len, "前序");
      drawSeq(INO, 66, f.inoStart, f.len, "中序");

      edges(g, new Set(f.created));
      for (const k in nodes) {
        const nd = nodes[k];
        if (!f.created.includes(k)) continue;
        V.circle(g, nd.x, nd.y, 19, k, k === f.root ? "hot" : "ok");
      }

      V.text(g, 30, 344, f.note, "small", "start");
      V.text(g, 30, 368, `已建 ${f.created.length} / 9 个结点`, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "由前序 + 中序还原二叉树",
      note: "前序给出「谁是根」，中序给出「根左边有几个」。切分时两边的长度必须一致：左子树取 k 个，右子树取 n-k-1 个。",
      width: W, height: H, frames, draw, interval: 760,
      code: [
        "Node *buildFromPreIn(const char *pre, const char *in, int n) {",
        "    if (n == 0) return NULL;",
        "    char root = pre[0];",
        "    int k = 0; while (in[k] != root) ++k;",
        "    Node *p = new Node(root);",
        "    p->left  = buildFromPreIn(pre + 1,     in,     k);",
        "    p->right = buildFromPreIn(pre + k + 1, in + k + 1, n - k - 1);",
        "    return p;",
        "}",
      ],
      caption: (f) => `当前根 ${f.root}`,
    });
  })();

  // ---------- 动画 2：四种遍历 ----------
  (function traversals() {
    const frames = [];
    const pre = [], ino = [], post = [], lvl = [];
    (function p(nd) { if (!nd) return; pre.push(nd.v); p(nd.l); p(nd.r); })(root);
    (function i(nd) { if (!nd) return; i(nd.l); ino.push(nd.v); i(nd.r); })(root);
    (function o(nd) { if (!nd) return; o(nd.l); o(nd.r); post.push(nd.v); })(root);
    (function l() {
      const q = [root];
      while (q.length) {
        const nd = q.shift();
        lvl.push(nd.v);
        if (nd.l) q.push(nd.l);
        if (nd.r) q.push(nd.r);
      }
    })();

    [["前序（根左右）", pre, 4], ["中序（左根右）", ino, 12], ["后序（左右根）", post, 20], ["层序（队列）", lvl, 25]]
      .forEach(([name, seq, line]) => {
        frames.push({
          name, seq, k: -1, out: [], line,
          vars: { "遍历方式": name, "已访问": 0, "输出": "（空）" },
          note: name + "：准备开始",
        });
        seq.forEach((v, i) => {
          frames.push({
            name, seq, k: i, out: seq.slice(0, i + 1), line,
            vars: { "遍历方式": name, "已访问": i + 1, "输出": seq.slice(0, i + 1).join(" ") },
            note: `访问 ${v}（第 ${i + 1} 个）`,
          });
        });
      });

    const W = 760, H = 380;

    function draw(g, f) {
      edges(g, null);
      const visited = new Set(f.out);
      for (const k in nodes) {
        const nd = nodes[k];
        const isCur = f.k >= 0 && f.seq[f.k] === k;
        V.circle(g, nd.x, nd.y, 19, k, isCur ? "hot" : visited.has(k) ? "ok" : "");
      }

      V.text(g, 30, 344, f.note, "small", "start");
      V.text(g, 30, 368, "序列：" + f.out.join(" "), "mono", "start");
    }

    V.panel({
      host: "#a2",
      title: "四种遍历：同一棵树，四种顺序",
      note: "前序、中序、后序的差别只是「根」什么时候访问；层序换了一套机制，用队列，先进先出。",
      width: W, height: H, frames, draw, interval: 480,
      code: [
        "// 前序：根 左 右",
        "void preOrder(Node *p) {",
        "    if (!p) return;",
        "    visit(p);",
        "    preOrder(p->left);",
        "    preOrder(p->right);",
        "}",
        "// 中序：左 根 右",
        "void inOrder(Node *p) {",
        "    if (!p) return;",
        "    inOrder(p->left);",
        "    visit(p);",
        "    inOrder(p->right);",
        "}",
        "// 后序：左 右 根",
        "void postOrder(Node *p) {",
        "    if (!p) return;",
        "    postOrder(p->left);",
        "    postOrder(p->right);",
        "    visit(p);",
        "}",
        "// 层序：队列",
        "void levelOrder(Node *p) {",
        "    queue<Node*> q; q.push(p);",
        "    while (!q.empty()) { Node *c = q.front(); q.pop(); visit(c); }",
        "}",
      ],
      caption: (f) => `${f.name}　已访问 ${f.out.length} / 9`,
    });
  })();

  // ---------- 动画 3：树 -> 二叉树 ----------
  (function treeToBinaryAnim() {
    const T = {
      A: { x: 150, y: 55, kids: ["B", "C", "D"] },
      B: { x: 75, y: 135, kids: ["E", "F"] },
      C: { x: 150, y: 135, kids: [] },
      D: { x: 225, y: 135, kids: ["G"] },
      E: { x: 45, y: 215, kids: [] },
      F: { x: 105, y: 215, kids: [] },
      G: { x: 225, y: 215, kids: [] },
    };
    const B = {
      A: { x: 535, y: 45 },
      B: { x: 470, y: 115 },
      E: { x: 405, y: 185 },
      F: { x: 485, y: 185 },
      C: { x: 540, y: 185 },
      D: { x: 615, y: 255 },
      G: { x: 575, y: 325 },
    };
    const steps = [
      { from: "A", to: "B", side: "left", why: "A 的第一个孩子 B -> A.left" },
      { from: "B", to: "E", side: "left", why: "B 的第一个孩子 E -> B.left" },
      { from: "B", to: "C", side: "right", why: "B 的下一个兄弟 C -> B.right" },
      { from: "E", to: "F", side: "right", why: "E 的下一个兄弟 F -> E.right" },
      { from: "C", to: "D", side: "right", why: "C 的下一个兄弟 D -> C.right" },
      { from: "D", to: "G", side: "left", why: "D 的第一个孩子 G -> D.left" },
    ];

    const frames = [];
    frames.push({
      n: 0, note: "先看左边的树：每个结点有 firstChild 和 nextSibling 两个指针。",
      line: 1, vars: { "已建立边": 0 },
    });
    steps.forEach((s, i) => {
      frames.push({
        n: i + 1, hot: [s.from, s.to], note: s.why,
        line: s.side === "left" ? 4 : 5,
        vars: {
          "当前结点": s.from,
          "关系": s.side === "left" ? "第一个孩子 -> 左子女" : "下一个兄弟 -> 右子女",
          "已建立边": i + 1,
        },
      });
    });

    const W = 760, H = 390;

    function draw(g, f) {
      V.text(g, 150, 24, "树（firstChild / nextSibling）", "small");
      for (const p in T)
        for (const c of T[p].kids) V.line(g, T[p].x, T[p].y, T[c].x, T[c].y, "");
      for (const k in T) {
        const nd = T[k];
        const hot = f.hot && f.hot.includes(k);
        V.circle(g, nd.x, nd.y, 18, k, hot ? "hot" : "");
      }

      V.text(g, 535, 24, "二叉树（left / right）", "small");
      const lit = new Set();
      for (let i = 0; i < f.n; i++) {
        lit.add(steps[i].from);
        lit.add(steps[i].to);
      }
      for (let i = 0; i < f.n; i++) {
        const s = steps[i];
        const a = B[s.from], b = B[s.to];
        V.line(g, a.x, a.y, b.x, b.y, s.side === "left" ? "acc" : "ok");
      }
      for (const k in B) {
        if (!lit.has(k)) continue;
        V.circle(g, B[k].x, B[k].y, 18, k, f.hot && f.hot.includes(k) ? "hot" : "");
      }

      V.text(g, 40, 310, "左孩子管「往下」，右孩子管「往右」；根 A 没有兄弟，所以没有右子树。", "small", "start");
      V.text(g, 40, 338, f.note, "small", "start");
      V.text(g, 40, 366, `已建立 ${f.n} / 6 条转换边`, "small", "start");
    }

    V.panel({
      host: "#a3",
      title: "树 -> 二叉树：左孩子-右兄弟",
      note: "每个结点只保留「第一个孩子」和「下一个兄弟」：前者变左子女，后者变右子女。",
      width: W, height: H, frames, draw, interval: 900,
      code: [
        "BinNode *treeToBinary(TreeNode *p) {",
        "    if (!p) return NULL;",
        "    BinNode *q = new BinNode(p->data);",
        "    q->left  = treeToBinary(p->firstChild);",
        "    q->right = treeToBinary(p->nextSibling);",
        "    return q;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 4：森林 -> 二叉树 ----------
  (function forestToBinaryAnim() {
    const trees = [
      { nodes: { A: { x: 90, y: 60 }, B: { x: 55, y: 130 }, C: { x: 125, y: 130 } }, edges: [["A", "B"], ["A", "C"]] },
      { nodes: { D: { x: 370, y: 60 }, E: { x: 370, y: 130 } }, edges: [["D", "E"]] },
      { nodes: { F: { x: 630, y: 60 }, G: { x: 590, y: 130 }, H: { x: 670, y: 130 } }, edges: [["F", "G"], ["F", "H"]] },
    ];
    const B = {
      A: { x: 360, y: 205 }, B: { x: 240, y: 275 }, C: { x: 320, y: 345 },
      D: { x: 480, y: 275 }, E: { x: 400, y: 345 }, F: { x: 560, y: 345 },
      G: { x: 510, y: 415 }, H: { x: 590, y: 415 },
    };
    const steps = [
      { edges: [["A", "B", "left"], ["B", "C", "right"]], note: "T1：A 的第一个孩子 B 变左子女；B 的下一个兄弟 C 变右子女。" },
      { edges: [["D", "E", "left"]], note: "T2：D 的第一个孩子 E 变左子女。" },
      { edges: [["F", "G", "left"], ["G", "H", "right"]], note: "T3：F 的第一个孩子 G 变左子女；G 的下一个兄弟 H 变右子女。" },
      { edges: [["A", "D", "right"]], note: "串森林：第 2 棵树的根 D 作为 A 的右子女。" },
      { edges: [["D", "F", "right"]], note: "第 3 棵树的根 F 作为 D 的右子女；根的右链就是各棵树的根。" },
    ];

    const frames = [];
    let revealed = [];
    frames.push({
      n: 0, revealed: [], note: "森林 = 三棵互不相交的树。先各自转换，再把根串起来。",
      line: 9, vars: { "已转换": "0 / 3 棵" },
    });
    steps.forEach((s, i) => {
      revealed = revealed.concat(s.edges);
      frames.push({
        n: i + 1, revealed: revealed.slice(), hot: s.edges.map((e) => [e[0], e[1]]),
        note: s.note, line: i < 3 ? 3 : 11,
        vars: {
          "已转换": i < 3 ? `${i + 1} / 3 棵` : "3 / 3 棵",
          "森林先根": "A B C D E F G H",
          "森林后根": "B C A E D G H F",
        },
      });
    });

    const W = 760, H = 490;

    function draw(g, f) {
      V.text(g, 40, 24, "森林", "small", "start");
      trees.forEach((t, ti) => {
        for (const [a, b] of t.edges)
          V.line(g, t.nodes[a].x, t.nodes[a].y, t.nodes[b].x, t.nodes[b].y, "");
        for (const k in t.nodes) V.circle(g, t.nodes[k].x, t.nodes[k].y, 16, k, "");
        V.text(g, t.nodes[Object.keys(t.nodes)[0]].x, 168, `T${ti + 1}`, "small");
      });

      V.text(g, 360, 185, "二叉树", "small", "start");
      const lit = new Set();
      (f.revealed || []).forEach(([a, b]) => { lit.add(a); lit.add(b); });
      (f.revealed || []).forEach(([a, b, side]) => {
        V.line(g, B[a].x, B[a].y, B[b].x, B[b].y, side === "left" ? "acc" : "ok");
      });
      const hot = new Set();
      (f.hot || []).forEach(([a, b]) => { hot.add(a); hot.add(b); });
      for (const k in B) {
        if (!lit.has(k)) continue;
        V.circle(g, B[k].x, B[k].y, 17, k, hot.has(k) ? "hot" : "");
      }

      V.text(g, 40, 455, "森林先根 = 二叉树前序：A B C D E F G H", "small", "start");
      V.text(g, 40, 477, "森林后根 = 二叉树中序：B C A E D G H F", "small", "start");
    }

    V.panel({
      host: "#a4",
      title: "森林 -> 二叉树：把各棵树的根串成右链",
      note: "先各自用「左孩子-右兄弟」转换，再把第 2、3… 棵树的根依次挂到前一根的右指针上。",
      width: W, height: H, frames, draw, interval: 1000,
      code: [
        "BinNode *treeToBinary(TreeNode *p) {",
        "    if (!p) return NULL;",
        "    BinNode *q = new BinNode(p->data);",
        "    q->left  = treeToBinary(p->firstChild);",
        "    q->right = treeToBinary(p->nextSibling);",
        "    return q;",
        "}",
        "",
        "BinNode *forestToBinary(TreeNode **r, int n) {",
        "    BinNode *first = treeToBinary(r[0]), *cur = first;",
        "    for (int i = 1; i < n; ++i) {",
        "        cur->right = treeToBinary(r[i]);",
        "        cur = cur->right;",
        "    }",
        "    return first;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();
})();
