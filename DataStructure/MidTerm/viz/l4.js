// l4.js — 第 4 讲：BF 与 KMP 的指针轨迹、next 数组的构造
(function () {
  const V = window.Viz;
  const T = "ababcabcacbab";
  const P = "abcac";

  function buildNext(p) {
    const m = p.length;
    const next = new Array(m).fill(0);
    next[0] = -1;
    let j = 0, k = -1;
    while (j < m - 1) {
      if (k === -1 || p[j] === p[k]) { j++; k++; next[j] = k; }
      else k = next[k];
    }
    return next;
  }

  const BF_CODE = [
    "int bf(const char *t, const char *p, long long &cmp) {",
    "    int n = strlen(t), m = strlen(p);",
    "    for (int s = 0; s + m <= n; ++s) {",
    "        int j = 0;",
    "        while (j < m && t[s+j] == p[j]) { ++j; ++cmp; }",
    "        if (j == m) return s;",
    "    }",
    "    return -1;",
    "}",
  ];

  const KMP_CODE = [
    "int kmp(const char *t, const char *p, const int *next, long long &cmp) {",
    "    int n = strlen(t), m = strlen(p);",
    "    int i = 0, j = 0;",
    "    while (i < n && j < m) {",
    "        ++cmp;",
    "        if (j == -1 || t[i] == p[j]) { ++i; ++j; }",
    "        else j = next[j];",
    "    }",
    "    return (j == m) ? i - m : -1;",
    "}",
  ];

  // ---------- 动画 1：BF 与 KMP 同步对比 ----------
  (function compare() {
    const n = T.length, m = P.length;
    const next = buildNext(P);

    const bf = { s: 0, j: 0, cmp: 0, done: false, res: -1 };
    const kmp = { i: 0, j: 0, cmp: 0, done: false, res: -1 };
    const snapshot = (bfLine, kmpLine, note) => ({
      bf: { ...bf }, kmp: { ...kmp },
      line: [bfLine || 3, kmpLine || 4],
      vars: [
        { "s": bf.s, "j": bf.j, "比较次数": bf.cmp },
        { "i": kmp.i, "j": kmp.j, "比较次数": kmp.cmp, "next[j]": kmp.j >= 0 ? next[kmp.j] : "—" },
      ],
      note: note || "",
    });

    const frames = [snapshot(3, 4, "开始")];

    function stepBF() {
      if (bf.done) return 3;
      if (bf.s + m > n) { bf.done = true; bf.res = -1; return 3; }
      bf.cmp++;
      if (T[bf.s + bf.j] === P[bf.j]) {
        bf.j++;
        if (bf.j === m) { bf.done = true; bf.res = bf.s; return 6; }
        return 5;
      }
      bf.s++; bf.j = 0;
      return 5;
    }
    function stepKMP() {
      if (kmp.done) return 4;
      if (kmp.i >= n || kmp.j >= m) { kmp.done = true; return 4; }
      kmp.cmp++;
      if (kmp.j === -1 || T[kmp.i] === P[kmp.j]) {
        kmp.i++; kmp.j++;
        if (kmp.j === m) { kmp.done = true; kmp.res = kmp.i - m; return 9; }
        return 6;
      }
      kmp.j = next[kmp.j];
      return 7;
    }

    let guard = 0;
    while ((!bf.done || !kmp.done) && guard++ < 200) {
      const bl = stepBF();
      const kl = stepKMP();
      frames.push(snapshot(bl, kl));
    }
    frames.push(snapshot(3, 4, `BF 用了 ${bf.cmp} 次比较，KMP 用了 ${kmp.cmp} 次`));

    const W = 760, H = 340;
    const CW = 26, X0 = 40;
    const pos = (i) => X0 + i * CW;

    function row(g, y, patOff, cmp, hotText, hotPat, label) {
      V.text(g, X0 - 12, y + 4, label, "small", "end");
      for (let i = 0; i < n; i++) {
        V.box(g, pos(i), y, CW - 3, 30, T[i],
          i === hotText ? "hot" : i >= patOff && i < patOff + m ? "acc" : "");
      }
      for (let j = 0; j < m; j++) {
        const x = pos(patOff) + j * CW;
        if (x < X0 - 1 || x + CW > 700) continue;
        V.box(g, x, y + 44, CW - 3, 30, P[j], j === hotPat ? "hot" : "");
      }
      V.text(g, 686, y + 6, "比较 " + cmp + " 次", "small", "start");
    }

    function draw(g, f) {
      const b = f.bf, k = f.kmp;
      row(g, 60, b.s, b.cmp, b.done ? -1 : b.s + b.j, b.done ? -1 : b.j, "BF");

      const align = Math.max(0, k.i - Math.max(k.j, 0));
      row(g, 190, align, k.cmp, k.done ? -1 : k.i, k.done ? -1 : k.j, "KMP");

      V.text(g, X0, 278, `BF 结果 ${b.res}    KMP 结果 ${k.res}`, "ok", "start");
      V.text(g, X0, 302, "KMP 的 i 只增不减；BF 每次失配都 s++ 并且 j 归零", "small", "start");
      V.text(g, X0, 322, f.note, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "BF 与 KMP：两个算法同时单步",
      note: "文本 <code>ababcabcacbab</code>，模式 <code>abcac</code>。左边是 BF 的代码，右边是 KMP 的代码，各自高亮当前执行到的那一行。",
      width: W, height: H, frames, draw, interval: 420,
      code: [{ title: "BF", lines: BF_CODE }, { title: "KMP", lines: KMP_CODE }],
      caption: (f) => f.note || `BF 第 ${f.bf.cmp} 次比较　KMP 第 ${f.kmp.cmp} 次比较`,
    });
  })();

  // ---------- 动画 2：next 数组的构造 ----------
  (function nextTable() {
    const m = P.length;
    const next = new Array(m).fill(null);
    next[0] = -1;
    const frames = [];
    let j = 0, k = -1;

    const push = (line, note) =>
      frames.push({
        j, k, next: next.slice(), line, note,
        vars: {
          "j": j, "k": k,
          "p[j]": P[j] || "—", "p[k]": k >= 0 ? P[k] : "—",
          "next": next.map((v) => (v === null ? "·" : v)).join(" "),
        },
      });

    push(1, "next[0] = -1 是约定");
    push(2, "j 扫模式，k 记录「已匹配的前缀长度」");

    while (j < m - 1) {
      const match = k === -1 || P[j] === P[k];
      push(4, match ? (k === -1 ? "k = -1，直接前进" : `p[${j}] = p[${k}] = '${P[j]}'，可以前进`) : `p[${j}] = '${P[j]}' ≠ p[${k}] = '${P[k]}'`);
      if (match) {
        j++; k++; next[j] = k;
        push(5, `写入 next[${j}] = ${k}`);
      } else {
        k = next[k];
        push(7, `k ← next[${k}]，回退`);
      }
    }
    push(4, "j 到 m-1，构造结束");

    const W = 760, H = 250;
    const CW = 84, X0 = 86;

    function draw(g, f) {
      const rows = [
        { label: "j", vals: Array.from({ length: m }, (_, i) => i) },
        { label: "p[j]", vals: P.split("") },
        { label: "next[j]", vals: f.next },
      ];
      rows.forEach((r, ri) => {
        const y = 70 + ri * 52;
        V.text(g, X0 - 18, y + 22, r.label, "small", "end");
        r.vals.forEach((v, i) => {
          V.box(g, X0 + i * CW, y, CW - 8, 34, v === null ? "" : String(v), i === f.j ? "hot" : "");
        });
      });

      if (f.k >= 0 && f.k < m) {
        const x = X0 + f.k * CW + (CW - 8) / 2;
        V.arrow(g, x, 150, x, 210, "acc");
        V.text(g, x, 226, "k = " + f.k, "small acc");
      }
      V.text(g, X0 - 18, 44, `模式 p = ${P}`, "ok", "start");
      V.text(g, X0, 236, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "next 数组：j 前进、k 回退",
      note: "next[j] 是 p[0..j-1] 的最长相等真前缀与真后缀长度，约定 next[0] = -1。构造是线性的：j 只增，k 的回退总量不超过 j 的增量。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "next[0] = -1;",
        "int j = 0, k = -1;",
        "while (j < m - 1) {",
        "    if (k == -1 || p[j] == p[k]) {",
        "        ++j;  ++k;  next[j] = k;",
        "    } else {",
        "        k = next[k];",
        "    }",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();
})();
