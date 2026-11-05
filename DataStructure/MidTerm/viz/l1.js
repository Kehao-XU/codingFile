// l1.js — 第 1 讲：增长阶 与 程序步
(function () {
  const V = window.Viz;

  // ---------- 动画 1：四种增长阶 ----------
  (function growth() {
    const frames = [];
    for (let n = 1; n <= 64; n += 2) frames.push({ n });

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
    const names = { n: "n", nlogn: "n log n", n2: "n²", exp: "2ⁿ" };

    function draw(g, f) {
      // 坐标轴
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
      V.text(g, 18, T + 8, "运行量", "small");

      // 四条曲线
      for (const key of ["n", "nlogn", "n2", "exp"]) {
        const ps = pts(fs[key], f.n);
        const d = ps.map((p, i) => (i ? "L" : "M") + p[0].toFixed(1) + "," + p[1].toFixed(1)).join(" ");
        V.el("path", { d, fill: "none", class: "ln " + colors[key], "stroke-width": 2.2 }, g);
        // 曲线末端标注
        const last = ps[ps.length - 1];
        V.text(g, Math.min(last[0] + 6, R + 4), Math.max(last[1] - 4, T + 8), names[key], "small " + colors[key], "start");
      }

      // 当前 n 的竖线
      const x = L + (f.n / 64) * (R - L);
      V.line(g, x, T, x, B, "hot");
      V.text(g, x, T - 6, "n = " + f.n, "small hot");

      // 右侧数值表
      let y = T + 10;
      V.text(g, 600, y, "n = " + f.n, "ok", "start");
      y += 24;
      V.text(g, 600, y, "n      " + fs.n(f.n), "mono", "start"); y += 22;
      V.text(g, 600, y, "n logn " + Math.round(fs.nlogn(f.n)), "mono", "start"); y += 22;
      V.text(g, 600, y, "n²     " + fs.n2(f.n), "mono", "start"); y += 22;
      const e = Math.pow(2, f.n);
      V.text(g, 600, y, "2ⁿ     " + (e > 1e9 ? e.toExponential(1) : e), "mono", "start"); y += 30;
      V.text(g, 600, y, "2ⁿ 在 n≈12", "small", "start"); y += 16;
      V.text(g, 600, y, "就冲出坐标轴", "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "四种增长阶（同一坐标轴，线性刻度）",
      note: "拖动或用按钮改变 n。注意 2ⁿ 在 n≈12 时就超出 0..4096 的范围，这不是画错，而是它真实的样子。",
      width: W, height: H, frames, draw, interval: 260,
      caption: (f) => `n = ${f.n}　|　n² 已经是 n 的 ${(f.n * f.n / f.n).toFixed(0)} 倍，n log n 只有 ${Math.round(Math.log2(f.n))} 倍`,
    });
  })();

  // ---------- 动画 2：程序步计数 ----------
  (function steps() {
    const N = 3;
    const frames = [];
    let s = 0;
    const push = (o) => frames.push(Object.assign({ steps: s, filled: 0 }, o));

    push({ i: 0, j: 0, line: 0, filled: 0 });
    for (let i = 1; i <= N; i++) {
      s++; push({ i, j: 0, line: 0, filled: (i - 1) * N });
      for (let j = 1; j <= N; j++) {
        s++; push({ i, j, line: 1, filled: (i - 1) * N + (j - 1) });
        s++; push({ i, j, line: 2, filled: (i - 1) * N + j });
      }
      s++; push({ i, j: N + 1, line: 1, filled: i * N });
    }
    s++; push({ i: N + 1, j: 0, line: 0, filled: N * N });

    const W = 760, H = 280;
    const code = [
      "for (i = 1; i <= n; i++)",
      "  for (j = 1; j <= n; j++)",
      "    sum += a[i][j];",
    ];

    function draw(g, f) {
      // 左侧代码
      V.text(g, 16, 40, "n = 3 时的执行过程", "small", "start");
      code.forEach((ln, k) => {
        const y = 66 + k * 30;
        if (k === f.line) V.box(g, 10, y - 18, 330, 26, "", "hot");
        V.text(g, 18, y, ln, "mono " + (k === f.line ? "hot" : ""), "start");
      });

      // 右侧 3×3 矩阵
      const x0 = 400, y0 = 60, cw = 64, ch = 44;
      for (let i = 1; i <= N; i++) {
        for (let j = 1; j <= N; j++) {
          const idx = (i - 1) * N + (j - 1);
          const done = idx < f.filled;
          const cur = f.line === 2 && i === f.i && j === f.j;
          V.box(g, x0 + (j - 1) * cw, y0 + (i - 1) * ch, cw - 6, ch - 6,
            done ? "✓" : "", cur ? "hot" : done ? "ok" : "");
        }
      }
      V.text(g, x0 + 1.5 * cw, y0 - 12, "a[i][j]", "small");

      // 计数器
      const y = 230;
      V.box(g, 400, y - 22, 110, 34, "i = " + (f.i || "-"), f.line === 0 ? "hot" : "");
      V.box(g, 520, y - 22, 110, 34, "j = " + (f.j || "-"), f.line === 1 ? "hot" : "");
      V.box(g, 640, y - 22, 110, 34, "步数 " + f.steps, "acc");
    }

    V.panel({
      host: "#a2",
      title: "程序步：把一次执行拆成一格一格数",
      note: "程序步不是\"一行代码一步\"，而是按基本操作的执行次数计。三个 for 判断 + 赋值，n=3 时总共 25 步；n 一般时是 2n²+3n+2 量级。",
      width: W, height: H, frames, draw, interval: 420,
      caption: (f) => `已执行 ${f.steps} 步　i=${f.i}　j=${f.j}`,
    });
  })();
})();
