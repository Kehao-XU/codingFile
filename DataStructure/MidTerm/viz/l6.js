// l6.js — 第 6 讲：Hanoi 的移动过程、n 皇后的回溯
(function () {
  const V = window.Viz;

  // ---------- 动画 1：Hanoi ----------
  (function hanoi() {
    const N = 3;
    let moves = [];
    (function rec(n, a, b, c) {
      if (n === 0) return;
      rec(n - 1, a, c, b);
      moves.push({ n, from: a, to: c });
      rec(n - 1, b, a, c);
    })(N, "A", "B", "C");

    const pegs = { A: [], B: [], C: [] };
    for (let d = N; d >= 1; d--) pegs.A.push(d);

    const frames = [{ pegs: JSON.parse(JSON.stringify(pegs)), note: "初始：3 个盘都在 A 柱", move: null }];
    moves.forEach((m, i) => {
      const d = pegs[m.from].pop();
      pegs[m.to].push(d);
      frames.push({
        pegs: JSON.parse(JSON.stringify(pegs)), move: m,
        note: `第 ${i + 1} 步：把 ${d} 号盘从 ${m.from} 移到 ${m.to}（共需 2^${N}-1 = ${Math.pow(2, N) - 1} 步）`,
      });
    });

    const W = 760, H = 320;
    const PX = { A: 170, B: 380, C: 590 };
    const BASE = 250;

    function draw(g, f) {
      for (const k of ["A", "B", "C"]) {
        V.line(g, PX[k], 90, PX[k], BASE, "");
        V.text(g, PX[k], BASE + 26, k + " 柱", "small");
      }
      V.line(g, 90, BASE, 680, BASE, "");

      for (const k of ["A", "B", "C"]) {
        f.pegs[k].forEach((d, i) => {
          const w = 34 + d * 26;
          const y = BASE - (i + 1) * 26;
          const hot = f.move && ((f.move.to === k && i === f.pegs[k].length - 1));
          V.box(g, PX[k] - w / 2, y, w, 22, String(d), hot ? "hot" : "");
        });
      }

      V.text(g, 380, 40, f.note, "small");
      V.text(g, 380, 60, "移动次数 = 2ⁿ - 1；递归调用次数 = 2ⁿ⁺¹ - 1", "small");
    }

    V.panel({
      host: "#a1",
      title: "Hanoi：3 个盘要 7 步",
      note: "递归三句话：把上面 n-1 个挪到中转柱，把最大的挪到目标柱，再把那 n-1 个挪过去。每一层都在重复这三句话，所以移动次数一定是 2ⁿ-1。",
      width: W, height: H, frames, draw, interval: 800,
      caption: (f) => (f.move ? `把 ${f.move.n} 号盘 ${f.move.from} → ${f.move.to}` : "准备开始"),
    });
  })();

  // ---------- 动画 2：n 皇后回溯 ----------
  (function queens() {
    const n = 4;
    const frames = [];
    const col = new Array(n).fill(false);
    const md = new Array(2 * n).fill(false);
    const sd = new Array(2 * n).fill(false);
    const queens = [];
    let sols = 0;

    const push = (note, hot) =>
      frames.push({ queens: queens.map((q) => ({ ...q })), note, hot: hot || null, sols });

    push("从第 1 行开始，逐行放皇后");

    (function solve(row) {
      if (row === n) {
        sols++;
        push(`放满 ${n} 行 → 找到第 ${sols} 个解`, null);
        return;
      }
      for (let j = 0; j < n; j++) {
        const m = n + row - j - 1;
        const s = row + j;
        if (col[j] || md[m] || sd[s]) continue;
        col[j] = md[m] = sd[s] = true;
        queens.push({ r: row, c: j });
        push(`第 ${row + 1} 行放在第 ${j + 1} 列（列、两条对角线都空）`, { r: row, c: j });
        solve(row + 1);
        queens.pop();
        col[j] = md[m] = sd[s] = false;
        push(`回溯：撤掉第 ${row + 1} 行第 ${j + 1} 列，试下一列`, null);
      }
    })(0);

    push(`搜索结束，n = ${n} 共有 ${sols} 个解`, null);

    const W = 760, H = 360;
    const S = 52, X0 = 70, Y0 = 60;

    function draw(g, f) {
      for (let r = 0; r < n; r++) {
        for (let c = 0; c < n; c++) {
          const dark = (r + c) % 2 === 0;
          V.box(g, X0 + c * S, Y0 + r * S, S, S, "",
            (r + c) % 2 === 0 ? "hl" : "");
        }
      }
      // 网格线
      for (let i = 0; i <= n; i++) {
        V.line(g, X0, Y0 + i * S, X0 + n * S, Y0 + i * S, "");
        V.line(g, X0 + i * S, Y0, X0 + i * S, Y0 + n * S, "");
      }

      f.queens.forEach((q) => {
        const hot = f.hot && f.hot.r === q.r && f.hot.c === q.c;
        V.circle(g, X0 + q.c * S + S / 2, Y0 + q.r * S + S / 2, 17, "Q", hot ? "hot" : "ok");
      });

      V.text(g, 340, 70, "已找到解数", "small", "start");
      V.text(g, 340, 100, String(f.sols), "ok", "start");
      V.text(g, 340, 150, "棋盘 4×4", "small", "start");
      V.text(g, 340, 176, "标记数组：", "small", "start");
      V.text(g, 340, 198, "col[j] 列占用", "small", "start");
      V.text(g, 340, 220, "md[n+r-j-1] 主对角线", "small", "start");
      V.text(g, 340, 242, "sd[r+j] 次对角线", "small", "start");
      V.text(g, 340, 290, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "n 皇后：放一个、递归、回来撤掉",
      note: "回溯的本质是「试错 + 撤销」。和 Hanoi 一样是递归，区别在于 Hanoi 每一步都是对的，n 皇后要判断这一步是否还能走，走不通就退回来。",
      width: W, height: H, frames, draw, interval: 520,
      caption: (f) => `已找到 ${f.sols} 个解`,
    });
  })();
})();
