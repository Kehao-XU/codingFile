// l6.js — 第 6 讲：Hanoi 的移动过程、n 皇后的回溯
(function () {
  const V = window.Viz;

  // ---------- 动画 1：Hanoi ----------
  (function hanoi() {
    const N = 3;
    const moves = [];
    (function rec(n, a, b, c, depth) {
      if (n === 0) return;
      rec(n - 1, a, c, b, depth + 1);
      moves.push({ n, from: a, to: c, depth });
      rec(n - 1, b, a, c, depth + 1);
    })(N, "A", "B", "C", 1);

    const pegs = { A: [], B: [], C: [] };
    for (let d = N; d >= 1; d--) pegs.A.push(d);

    const frames = [{
      pegs: JSON.parse(JSON.stringify(pegs)), move: null, line: 1,
      vars: { "n": N, "from": "A", "via": "B", "to": "C", "递归深度": 1, "已移动": 0 },
      note: "初始：3 个盘都在 A 柱",
    }];

    moves.forEach((m, i) => {
      const d = pegs[m.from].pop();
      pegs[m.to].push(d);
      frames.push({
        pegs: JSON.parse(JSON.stringify(pegs)), move: m, line: 4,
        vars: {
          "n": m.n, "from": m.from, "to": m.to,
          "递归深度": m.depth, "已移动": i + 1,
        },
        note: `第 ${i + 1} 步：把 ${d} 号盘从 ${m.from} 移到 ${m.to}`,
      });
    });

    frames.push({
      pegs: JSON.parse(JSON.stringify(pegs)), move: null, line: 2,
      vars: {
        "n": 0, "from": "—", "to": "—",
        "递归深度": 0, "已移动": moves.length,
      },
      note: `全部移到 C，共 ${Math.pow(2, N) - 1} 步`,
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
          const hot = f.move && f.move.to === k && i === f.pegs[k].length - 1;
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
      width: W, height: H, frames, draw, interval: 760,
      code: [
        "void hanoi(int n, char from, char via, char to) {",
        "    if (n == 0) return;              // 出口",
        "    hanoi(n - 1, from, to, via);     // ① n-1 个挪到中转",
        "    move(n, from, to);               // ② 最大盘归位",
        "    hanoi(n - 1, via, from, to);     // ③ 再挪到目标",
        "}",
      ],
      caption: (f) => (f.move ? `把 ${f.move.n} 号盘 ${f.move.from} → ${f.move.to}` : f.note),
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

    const push = (note, line, hot, row, j) =>
      frames.push({
        queens: queens.map((q) => ({ ...q })), note, line, hot: hot || null,
        vars: {
          "row": row === undefined ? "—" : row,
          "j": j === undefined ? "—" : j,
          "已放皇后": queens.length,
          "解数": sols,
        },
      });

    push("从第 1 行开始，逐行放皇后", 1, null, 0, "—");

    (function solve(row) {
      if (row === n) {
        sols++;
        push(`放满 ${n} 行 → 找到第 ${sols} 个解`, 2, null, row, "—");
        return;
      }
      for (let j = 0; j < n; j++) {
        const m = n + row - j - 1;
        const s = row + j;
        if (col[j] || md[m] || sd[s]) continue;
        col[j] = md[m] = sd[s] = true;
        queens.push({ r: row, c: j });
        push(`第 ${row + 1} 行放第 ${j + 1} 列：列、两条对角线都空`, 5, { r: row, c: j }, row, j);
        solve(row + 1);
        queens.pop();
        col[j] = md[m] = sd[s] = false;
        push(`回溯：撤掉第 ${row + 1} 行第 ${j + 1} 列`, 7, null, row, j);
      }
    })(0);

    push(`搜索结束，n = ${n} 共有 ${sols} 个解`, 9, null, "—", "—");

    const W = 760, H = 360;
    const S = 52, X0 = 70, Y0 = 60;

    function draw(g, f) {
      for (let r = 0; r < n; r++) {
        for (let c = 0; c < n; c++) {
          V.box(g, X0 + c * S, Y0 + r * S, S, S, "", (r + c) % 2 === 0 ? "hl" : "");
        }
      }
      for (let i = 0; i <= n; i++) {
        V.line(g, X0, Y0 + i * S, X0 + n * S, Y0 + i * S, "");
        V.line(g, X0 + i * S, Y0, X0 + i * S, Y0 + n * S, "");
      }

      f.queens.forEach((q) => {
        const hot = f.hot && f.hot.r === q.r && f.hot.c === q.c;
        V.circle(g, X0 + q.c * S + S / 2, Y0 + q.r * S + S / 2, 17, "Q", hot ? "hot" : "ok");
      });

      V.text(g, 340, 70, "已找到解数", "small", "start");
      V.text(g, 340, 100, String(f.vars["解数"]), "ok", "start");
      V.text(g, 340, 150, "棋盘 4×4", "small", "start");
      V.text(g, 340, 176, "标记数组：", "small", "start");
      V.text(g, 340, 198, "col[j]      列占用", "small", "start");
      V.text(g, 340, 220, "md[n+r-j-1] 主对角线", "small", "start");
      V.text(g, 340, 242, "sd[r+j]     次对角线", "small", "start");
      V.text(g, 340, 290, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "n 皇后：放一个、递归、回来撤掉",
      note: "回溯的本质是「试错 + 撤销」。和 Hanoi 一样是递归，区别在于 Hanoi 每一步都是对的，n 皇后要判断这一步还能不能走，走不通就退回来。",
      width: W, height: H, frames, draw, interval: 520,
      code: [
        "void queen(int row) {",
        "    if (row == n) { ++solutions; return; }",
        "    for (int j = 0; j < n; ++j) {",
        "        if (col[j] || md[n+row-j-1] || sd[row+j]) continue;",
        "        col[j] = md[m] = sd[s] = 1;   // 落子",
        "        queen(row + 1);              // 下一行",
        "        col[j] = md[m] = sd[s] = 0;   // 撤销",
        "    }",
        "}",
      ],
      caption: (f) => `已找到 ${f.vars["解数"]} 个解`,
    });
  })();
})();
