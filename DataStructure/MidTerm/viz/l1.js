// l1.js — 第 1 讲：增长阶 与 程序步
(function () {
  const V = window.Viz;

  // ---------- 动画 1：四种增长阶 ----------
  (function growth() {
    const frames = [];
    for (let n = 1; n <= 64; n += 2) {
      const val = Math.pow(2, n);
      frames.push({
        n,
        vars: {
          "n": n,
          "n log₂n": Math.round(n * Math.log2(n)),
          "n²": n * n,
          "2ⁿ": val > 1e9 ? val.toExponential(2) : val,
        },
      });
    }

    const W = 760, H = 300;
    const L = 60, R = 560, T = 24, B = 250;
    const YMAX = 4096;

    function pts(f, upto) {
      const out = [];
      for (let k = 1; k <= upto; k += 1) {
        out.push([L + (k / 64) * (R - L), B - Math.min(f(k) / YMAX, 1.04) * (B - T)]);
      }
      return out;
    }
    const fs = {
      n: (k) => k,
      nlogn: (k) => k * Math.log2(k),
      n2: (k) => k * k,
      exp: (k) => Math.pow(2, k),
    };
    const colors = { n: "k1", nlogn: "k2", n2: "k3", exp: "k4" };

    function draw(g, f) {
      V.line(g, L, T, L, B, "");
      V.line(g, L, B, R, B, "");
      for (let k = 0; k <= 64; k += 16) {
        const x = L + (k / 64) * (R - L);
        V.line(g, x, B, x, B + 5, "");
        V.text(g, x, B + 20, String(k), "small");
      }
      for (let v = 0; v <= 4096; v += 1024) {
        const y = B - (v / YMAX) * (B - T);
        V.line(g, L - 5, y, L, y, "");
        V.text(g, L - 10, y + 4, String(v), "small", "end");
      }
      V.text(g, (L + R) / 2, B + 38, "输入规模 n", "small");

      for (const key of ["n", "nlogn", "n2", "exp"]) {
        const ps = pts(fs[key], f.n);
        const d = ps.map((p, i) => (i ? "L" : "M") + p[0].toFixed(1) + "," + p[1].toFixed(1)).join(" ");
        V.el("path", { d, fill: "none", class: "ln " + colors[key], "stroke-width": 2.2 }, g);
      }

      const x = L + (f.n / 64) * (R - L);
      V.line(g, x, T, x, B, "hot");
      V.text(g, x, T - 6, "n = " + f.n, "small hot");

      V.text(g, 600, T + 10, "四条曲线", "small", "start");
      V.text(g, 600, T + 38, "n       线性", "small k1", "start");
      V.text(g, 600, T + 60, "n log n 线性对数", "small k2", "start");
      V.text(g, 600, T + 82, "n²      平方", "small k3", "start");
      V.text(g, 600, T + 104, "2ⁿ      指数", "small k4", "start");
      V.text(g, 600, T + 140, "2ⁿ 在 n≈12 就", "small", "start");
      V.text(g, 600, T + 158, "冲出坐标轴", "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "四种增长阶（同一坐标轴，线性刻度）",
      note: "拖动或用按钮改变 n。注意 2ⁿ 在 n≈12 时就超出 0..4096 的范围，这不是画错，而是它真实的样子。",
      width: W, height: H, frames, draw, interval: 260,
      caption: (f) => `n = ${f.n}　|　n² 是 n 的 ${f.n} 倍，n log n 只有 ${Math.round(Math.log2(f.n))} 倍`,
    });
  })();

  // ---------- 动画 2：程序步计数 ----------
  (function steps() {
    const N = 3;
    const frames = [];
    let s = 0;
    const push = (o) => frames.push(Object.assign({ steps: s, filled: 0, line: 1, vars: {} }, o));

    push({ i: 0, j: 0, line: 1, filled: 0 });
    for (let i = 1; i <= N; i++) {
      s++; push({ i, j: 0, line: 1, filled: (i - 1) * N });
      for (let j = 1; j <= N; j++) {
        s++; push({ i, j, line: 2, filled: (i - 1) * N + (j - 1) });
        s++; push({ i, j, line: 3, filled: (i - 1) * N + j });
      }
      s++; push({ i, j: N + 1, line: 2, filled: i * N });
    }
    s++; push({ i: N + 1, j: 0, line: 1, filled: N * N });

    // 给每帧补上变量表
    frames.forEach((f) => {
      f.vars = { "i": f.i, "j": f.j, "已执行步数": f.steps };
    });

    const W = 760, H = 280;
    const code = [
      "for (i = 1; i <= n; i++)",
      "  for (j = 1; j <= n; j++)",
      "    sum += a[i][j];",
    ];

    function draw(g, f) {
      V.text(g, 30, 36, "n = 3 时的执行过程", "small", "start");
      V.text(g, 30, 58, "每格代表一次赋值，", "small", "start");
      V.text(g, 30, 78, "代码和当前行见右侧。", "small", "start");

      const x0 = 320, y0 = 50, cw = 92, ch = 62;
      for (let i = 1; i <= N; i++) {
        for (let j = 1; j <= N; j++) {
          const idx = (i - 1) * N + (j - 1);
          const done = idx < f.filled;
          const cur = f.line === 3 && i === f.i && j === f.j;
          V.box(g, x0 + (j - 1) * cw, y0 + (i - 1) * ch, cw - 8, ch - 8,
            done ? "✓" : "", cur ? "hot" : done ? "ok" : "");
        }
      }
      V.text(g, x0 + 1.5 * cw, y0 - 14, "a[i][j]", "small");

      V.box(g, 30, 210, 120, 34, "i = " + (f.i || "-"), f.line === 1 ? "hot" : "");
      V.box(g, 165, 210, 120, 34, "j = " + (f.j || "-"), f.line === 2 ? "hot" : "");
      V.box(g, 300, 210, 150, 34, "已执行 " + f.steps + " 步", "acc");
    }

    V.panel({
      host: "#a2",
      title: "程序步：把一次执行拆成一格一格数",
      note: "程序步不是「一行代码一步」，而是按基本操作的执行次数计。三个 for 判断加一次赋值，n=3 时总共 25 步。",
      width: W, height: H, frames, draw, interval: 420,
      code: code,
      caption: (f) => `已执行 ${f.steps} 步　i=${f.i}　j=${f.j}`,
    });
  })();
})();
