// l2.js — 第 2 讲：顺序表的搬移 与 三对角矩阵的压缩映射
(function () {
  const V = window.Viz;

  // ---------- 动画 1：插入 + 删除的搬移 ----------
  (function shift() {
    const CAP = 8;
    const frames = [];
    let arr = [10, 20, 30, 40, 50, null, null, null];
    let last = 4;
    let moves = 0;

    const push = (tag, hot) =>
      frames.push({ arr: arr.slice(), last, moves, tag, hot: hot || [] });

    push("初始：n = 5，容量 8。要在第 2 个位置（下标 2）插入 99", []);

    for (let j = last; j >= 2; j--) {
      arr[j + 1] = arr[j];
      arr[j] = null;
      moves++;
      push(`a[${j}] 搬到 a[${j + 1}]：从后往前，才不会覆盖还没搬的元素`, [j, j + 1]);
    }
    arr[2] = 99;
    last = 5;
    push("空出位置后放入 99，n 变为 6。插入共搬 3 次", [2]);

    for (let j = 2; j < last; j++) {
      arr[j] = arr[j + 1];
      arr[j + 1] = null;
      moves++;
      push(`删除 99：a[${j + 1}] 搬到 a[${j}]，这次是从前往后`, [j, j + 1]);
    }
    last = 4;
    push("删除完成，n 回到 5。删除也搬了 3 次", []);

    const W = 760, H = 230, CW = 70, X0 = 30, Y0 = 70;

    function draw(g, f) {
      V.text(g, X0, 36, "顺序表（连续内存）", "small", "start");

      for (let i = 0; i < CAP; i++) {
        const x = X0 + i * CW;
        const v = f.arr[i];
        const cls = f.hot.includes(i) ? "hot" : v === null ? "dim" : "";
        V.box(g, x, Y0, CW - 8, 46, v === null ? "" : String(v), cls);
        V.text(g, x + (CW - 8) / 2, Y0 + 66, String(i), "small");
      }

      // 下标 last 指针
      const lx = X0 + f.last * CW + (CW - 8) / 2;
      V.arrow(g, lx, Y0 + 96, lx, Y0 + 52, "acc");
      V.text(g, lx, Y0 + 112, "last = " + f.last, "small acc");

      // 说明 + 计数器
      V.text(g, X0, Y0 + 150, "搬移次数 moves = " + f.moves, "ok", "start");
      V.text(g, X0, Y0 + 178, f.tag, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "插入与删除：为什么要从后往前搬",
      note: "插入时若是从前往后搬，a[i] 被写到 a[i+1] 之后，下一步再读 a[i+1] 拿到的就是刚写进去的副本。删除反过来，要从前往后。",
      width: W, height: H, frames, draw, interval: 650,
      caption: (f) => `moves = ${f.moves}`,
    });
  })();

  // ---------- 动画 2：三对角矩阵 ----------
  (function tridiagonal() {
    const n = 6;
    const cells = [];
    for (let i = 0; i < n; i++)
      for (let j = i - 1; j <= i + 1; j++)
        if (j >= 0 && j < n) cells.push({ i, j, k: 2 * i + j });
    const frames = cells.map((c) => ({ cur: c, done: cells.filter((x) => x.k < c.k) }));

    const W = 760, H = 330;
    const GS = 42, GX = 40, GY = 50;
    const AX = 420, AW = 34, AY = 60;

    function draw(g, f) {
      V.text(g, GX, 28, `三对角矩阵 n = ${n}（非零元 3n-2 = 16 个）`, "small", "start");

      // 网格
      for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
          const keep = Math.abs(i - j) <= 1;
          const isCur = f.cur.i === i && f.cur.j === j;
          const isDone = f.done.some((d) => d.i === i && d.j === j);
          V.box(g, GX + j * GS, GY + i * GS, GS - 4, GS - 4, "0",
            isCur ? "hot" : keep && isDone ? "ok" : keep ? "acc" : "dim");
        }
      }
      V.text(g, GX + 2.5 * GS, GY - 10, "j →", "small");
      V.text(g, GX - 12, GY + 3 * GS, "i", "small");

      // 一维数组
      V.text(g, AX - 12, 28, "一维数组 b[0..15]", "small", "start");
      for (let k = 0; k < 3 * n - 2; k++) {
        const isCur = f.cur.k === k;
        V.box(g, AX + (k % 8) * AW, AY + Math.floor(k / 8) * (AW + 14), AW - 4, 26,
          String(k), isCur ? "hot" : "");
      }

      // 公式
      const { i, j, k } = f.cur;
      V.text(g, AX, AY + 110, `(${i}, ${j})  →  k = 2i + j = ${k}`, "ok", "start");
      V.text(g, AX, AY + 138, `反解：i = ⌊(k+1)/3⌋ = ${Math.floor((k + 1) / 3)}`, "small", "start");
      V.text(g, AX, AY + 162, `j = k - 2i = ${k - 2 * Math.floor((k + 1) / 3)}`, "small", "start");
      V.text(g, AX, AY + 196, "每行 3 个（首尾行只有 2 个），", "small", "start");
      V.text(g, AX, AY + 218, "所以 k 不是 3i+j，而是 2i+j。", "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "三对角矩阵：二维下标 ↔ 一维下标",
      note: "按行优先压缩，k = 2i + j。把 16 个非零元依次放进 b[0..15]，每个 k 都是满的、不浪费。",
      width: W, height: H, frames, draw, interval: 420,
      caption: (f) => `正在处理 (${f.cur.i}, ${f.cur.j})，对应 k = ${f.cur.k}`,
    });
  })();
})();
