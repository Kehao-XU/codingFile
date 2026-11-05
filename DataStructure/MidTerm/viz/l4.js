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

  // ---------- 动画 1：BF 与 KMP 同步对比 ----------
  (function compare() {
    const n = T.length, m = P.length;
    const next = buildNext(P);

    const bf = { s: 0, j: 0, cmp: 0, done: false, res: -1 };
    const kmp = { i: 0, j: 0, cmp: 0, done: false, res: -1 };
    const frames = [{ bf: { ...bf }, kmp: { ...kmp }, note: "开始" }];

    function stepBF() {
      if (bf.done) return;
      if (bf.s + m > n) { bf.done = true; bf.res = -1; return; }
      bf.cmp++;
      if (T[bf.s + bf.j] === P[bf.j]) {
        bf.j++;
        if (bf.j === m) { bf.done = true; bf.res = bf.s; }
      } else { bf.s++; bf.j = 0; }
    }
    function stepKMP() {
      if (kmp.done) return;
      if (kmp.i >= n || kmp.j >= m) { kmp.done = true; return; }
      kmp.cmp++;
      if (kmp.j === -1 || T[kmp.i] === P[kmp.j]) {
        kmp.i++; kmp.j++;
        if (kmp.j === m) { kmp.done = true; kmp.res = kmp.i - m; }
      } else kmp.j = next[kmp.j];
    }

    let guard = 0;
    while ((!bf.done || !kmp.done) && guard++ < 200) {
      stepBF(); stepKMP();
      frames.push({ bf: { ...bf }, kmp: { ...kmp }, note: "" });
    }
    frames.push({
      bf: { ...bf }, kmp: { ...kmp },
      note: `BF 用了 ${bf.cmp} 次比较，KMP 用了 ${kmp.cmp} 次`,
    });

    const W = 760, H = 340;
    const CW = 26, X0 = 40;
    const pos = (i) => X0 + i * CW;

    function row(g, y, textOff, patOff, cmp, hotText, hotPat, label) {
      V.text(g, X0 - 12, y + 4, label, "small", "end");
      for (let i = 0; i < n; i++) {
        V.box(g, pos(i), y, CW - 3, 30, T[i],
          i === hotText ? "hot" : i >= textOff && i < textOff + m ? "acc" : "");
      }
      for (let j = 0; j < m; j++) {
        const x = pos(patOff) + j * CW;
        if (x < X0 - 1 || x + CW > 700) continue;
        V.box(g, x, y + 44, CW - 3, 30, P[j], j === hotPat ? "hot" : "");
      }
      V.text(g, 710, y + 6, "比较 " + cmp + " 次", "small", "start");
    }

    function draw(g, f) {
      const b = f.bf, k = f.kmp;

      row(g, 60, b.s, b.s, b.cmp, b.done ? -1 : b.s + b.j, b.done ? -1 : b.j, "BF");

      const align = Math.max(0, k.i - Math.max(k.j, 0));
      row(g, 190, align, align, k.cmp, k.done ? -1 : k.i, k.done ? -1 : k.j, "KMP");

      // 结果
      V.text(g, X0, 278, `BF  结果 ${b.res}    KMP 结果 ${k.res}`, "ok", "start");
      V.text(g, X0, 302, "KMP 的 i 只增不减；BF 每次失配都 s++ 并且 j 归零", "small", "start");
      V.text(g, X0, 322, f.note, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "BF 与 KMP：同一组输入下的指针轨迹",
      note: "文本 <code>ababcabcacbab</code>，模式 <code>abcac</code>。两行同步推进，每格一次比较。注意 BF 失配时 s 前进、j 归零；KMP 的 i 从不后退，只把 j 沿 next 往后拨。",
      width: W, height: H, frames, draw, interval: 420,
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
    frames.push({ j, k, next: next.slice(), note: "next[0] = -1，j = 0，k = -1" });

    while (j < m - 1) {
      const match = k === -1 || P[j] === P[k];
      if (match) {
        frames.push({ j, k, next: next.slice(), note: k === -1 ? `k = -1，直接前进` : `p[${j}] = p[${k}] = '${P[j]}'，相等` });
        j++; k++;
        next[j] = k;
        frames.push({ j: j - 1, k, next: next.slice(), note: `写入 next[${j}] = ${k}` });
      } else {
        frames.push({ j, k, next: next.slice(), note: `p[${j}] = '${P[j]}' ≠ p[${k}] = '${P[k]}'，k 回退` });
        const oldK = k;
        k = next[k];
        frames.push({ j, k, next: next.slice(), note: `k ← next[${oldK}] = ${k}` });
      }
    }
    frames.push({ j: m - 1, k, next: next.slice(), note: "构造完成" });

    const W = 760, H = 250;
    const CW = 84, X0 = 60;

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
          const cls = i === f.j ? "hot" : "";
          V.box(g, X0 + i * CW, y, CW - 8, 34, v === null ? "" : String(v), cls);
        });
      });

      // k 指针
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
      note: "next[j] 的定义是 p[0..j-1] 的最长相等真前缀与真后缀长度；约定 next[0] = -1。构造过程是线性时间，因为 j 只增、k 的回退总量不超过 j 的增量。",
      width: W, height: H, frames, draw, interval: 700,
      caption: (f) => f.note,
    });
  })();
})();
