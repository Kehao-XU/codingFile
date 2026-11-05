// l7.js — 第 7 讲：前序+中序建树、四种遍历
(function () {
  const V = window.Viz;
  const PRE = "ABHFDECKG";
  const INO = "HBDFAEKCG";

  // 先建好一棵完整的树，用来算坐标
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

  // 中序编号决定横坐标
  let counter = 0;
  (function place(nd) {
    if (!nd) return;
    place(nd.l);
    nd.x = 52 + counter++ * 78;
    nd.y = 70 + nd.depth * 78;
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
      created.push(r);
      frames.push({
        preStart, inoStart, len, root: r, k,
        created: created.slice(),
        note: `前序第一个 ${r} 是根；在中序里它左边有 ${k} 个结点，所以左子树 ${k} 个、右子树 ${len - k - 1} 个`,
      });
      rec(preStart + 1, inoStart, k, depth + 1);
      rec(preStart + k + 1, inoStart + k + 1, len - k - 1, depth + 1);
    }
    rec(0, 0, PRE.length, 0);

    const W = 760, H = 380;
    const CW = 56, X0 = 90;

    function draw(g, f) {
      // 前序 / 中序两行
      const drawSeq = (arr, y, start, len, label) => {
        V.text(g, X0 - 60, y + 24, label, "small", "start");
        for (let i = 0; i < arr.length; i++) {
          const inside = i >= start && i < start + len;
          V.box(g, X0 + i * CW, y, CW - 13, 32, arr[i], inside ? "acc" : "dim");
        }
      };
      drawSeq(PRE, 20, f.preStart, f.len, "前序");
      drawSeq(INO, 66, f.inoStart, f.len, "中序");

      // 已建出的树
      edges(g, new Set(f.created));
      for (const k in nodes) {
        const nd = nodes[k];
        if (!f.created.includes(k)) continue;
        const isNew = k === f.root;
        V.circle(g, nd.x, nd.y, 19, k, isNew ? "hot" : "ok");
      }

      V.text(g, 30, 330, f.note, "small", "start");
      V.text(g, 30, 356, `已建 ${f.created.length} / 9 个结点`, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "由前序 + 中序还原二叉树",
      note: "前序给出「谁是根」，中序给出「根左边有几个」。两句话就够了。注意前序和中序的切分长度必须一样：左子树用 [preStart+1, k 个]，右子树用 [preStart+k+1, len-k-1 个]。",
      width: W, height: H, frames, draw, interval: 850,
      caption: (f) => `当前根 ${f.root}`,
    });
  })();

  // ---------- 动画 2：四种遍历 ----------
  (function traversals() {
    const frames = [];
    // 手工收集四种顺序
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

    [["前序（根左右）", pre], ["中序（左根右）", ino], ["后序（左右根）", post], ["层序（队列）", lvl]]
      .forEach(([name, seq]) => {
        frames.push({ name, seq, k: -1, out: [], note: name + "：准备开始" });
        seq.forEach((v, i) => {
          frames.push({
            name, seq, k: i, out: seq.slice(0, i + 1),
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
        V.circle(g, nd.x, nd.y, 19, k,
          isCur ? "hot" : visited.has(k) ? "ok" : "");
      }

      V.text(g, 30, 330, f.note, "small", "start");
      V.text(g, 30, 356, "序列：" + f.out.join(" "), "mono", "start");
    }

    V.panel({
      host: "#a2",
      title: "四种遍历：同一棵树，四种顺序",
      note: "前序、中序、后序的区别只是「根」什么时候访问；层序完全换了一套机制，用队列，先进先出。",
      width: W, height: H, frames, draw, interval: 520,
      caption: (f) => `${f.name}　已访问 ${f.out.length} / 9`,
    });
  })();
})();
