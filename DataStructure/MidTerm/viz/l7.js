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
})();
