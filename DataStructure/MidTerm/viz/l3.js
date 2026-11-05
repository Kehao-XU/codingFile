// l3.js — 第 3 讲：单链表原地反转 与 约瑟夫环
(function () {
  const V = window.Viz;

  // ---------- 动画 1：原地反转 ----------
  (function reverse() {
    const vals = [1, 2, 3, 4, 5];
    const n = vals.length;
    const frames = [];
    let dirs = new Array(n).fill("fwd");
    let headTo = 0;

    frames.push({ dirs: dirs.slice(), headTo, prev: -1, cur: 0, next: 1, note: "初始：head → 1 → 2 → 3 → 4 → 5" });

    let prev = -1;
    for (let cur = 0; cur < n; cur++) {
      const next = cur + 1 < n ? cur + 1 : -1;
      dirs[cur] = prev === -1 ? "null" : "back";
      if (cur === 0) headTo = cur;
      frames.push({
        dirs: dirs.slice(), headTo, prev, cur, next,
        note: `cur = ${vals[cur]}：把它的 link 指向 ${prev === -1 ? "null" : vals[prev]}（掉头），然后 prev、cur 各前移一格`,
      });
      prev = cur;
    }
    headTo = n - 1;
    frames.push({
      dirs: dirs.slice(), headTo, prev, cur: -1, next: -1,
      note: "循环结束：head->link 指向原来的尾结点 5，反转完成（只改了指针，没有新建结点）",
    });

    const W = 760, H = 260;
    const CW = 86, X0 = 130, Y = 110;

    function draw(g, f) {
      const nx = (i) => X0 + i * CW;

      // 表头结点
      V.box(g, 30, Y, 56, 44, "head", "hl");
      V.text(g, 58, Y - 12, "表头", "small");
      // head 的 link
      V.arrow(g, 86, Y + 22, nx(f.headTo) - 4, Y + 22, "acc");

      for (let i = 0; i < n; i++) {
        const cls = i === f.cur ? "hot" : i === f.prev ? "acc" : "";
        V.box(g, nx(i), Y, CW - 22, 44, String(vals[i]), cls);
        V.text(g, nx(i) + (CW - 22) / 2, Y + 62, "p" + (i + 1), "small");
      }

      // 结点之间的 link
      for (let i = 0; i < n; i++) {
        const d = f.dirs[i];
        if (d === "fwd") {
          V.arrow(g, nx(i) + CW - 22, Y + 22, nx(i + 1) - 4, Y + 22);
        } else if (d === "back") {
          V.curve(g, nx(i) - 2, Y + 10, nx(i - 1) + CW - 18, Y + 10, 48, "acc");
        } else if (d === "null") {
          // 反转后的第一个结点，link 指向 null：画在结点下方，避免和 head 的箭头重叠
          const cx = nx(i) + (CW - 22) / 2;
          V.arrow(g, cx, Y + 44, cx, Y + 68, "");
          V.text(g, cx, Y + 82, "∅", "small");
        }
      }
      // 尾结点的 link（原来是 null）
      if (f.dirs[n - 1] === "fwd") {
        V.line(g, nx(n - 1) + CW - 22, Y + 22, nx(n - 1) + CW - 2, Y + 22, "");
        V.text(g, nx(n - 1) + CW + 6, Y + 26, "∅", "small");
      }

      // 三个指针
      const mark = (idx, name, col, dy) => {
        if (idx < 0 || idx >= n) return;
        const x = nx(idx) + (CW - 22) / 2;
        V.arrow(g, x, Y - 26 - dy, x, Y - 6, col);
        V.text(g, x, Y - 34 - dy, name, "small " + col);
      };
      mark(f.prev, "prev", "acc", 18);
      mark(f.cur, "cur", "hot", 0);
      mark(f.next, "next", "", 0);

      V.text(g, X0 - 100, Y + 150, f.note, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "单链表原地反转：三个指针走一遍",
      note: "必须先用 next 记住后继，再让 cur->link 掉头，否则链表断开、后面的结点全丢。反转后 head->link 要指向原尾结点。",
      width: W, height: H, frames, draw, interval: 900,
      caption: (f) => `prev = ${f.prev < 0 ? "∅" : vals[f.prev]}　cur = ${f.cur < 0 ? "∅" : vals[f.cur]}　next = ${f.next < 0 ? "∅" : vals[f.next]}`,
    });
  })();

  // ---------- 动画 2：约瑟夫环 ----------
  (function josephus() {
    const n = 8, m = 3;
    const frames = [];
    let alive = [];
    for (let i = 1; i <= n; i++) alive.push(i);
    let pos = 0, k = 1;
    const out = [];

    frames.push({ alive: alive.slice(), cur: alive[0], k: 0, out: [], note: "8 人围成单循环链表，从 1 号开始报数，报到 3 的出列" });

    while (alive.length > 1) {
      const who = alive[pos];
      frames.push({
        alive: alive.slice(), cur: who, k,
        out: out.slice(),
        note: `${who} 号报 ${k}`,
      });
      if (k === m) {
        out.push(who);
        frames.push({
          alive: alive.slice(), cur: who, k, out: out.slice(), dead: who,
          note: `${who} 号报到 ${m}，出列（从环上摘掉）`,
        });
        alive.splice(pos, 1);
        if (alive.length === 1) break;
        pos = pos % alive.length;
        k = 1;
        frames.push({
          alive: alive.slice(), cur: alive[pos], k: 0, out: out.slice(),
          note: `从 ${alive[pos]} 号重新从 1 开始报数`,
        });
      } else {
        k++;
        pos = (pos + 1) % alive.length;
      }
    }

    frames.push({
      alive: alive.slice(), cur: alive[0], k: 0, out: out.slice(), dead: alive[0],
      note: `幸存者：${alive[0]} 号`,
    });

    const W = 760, H = 340;
    const CX = 300, CY = 165, R = 110;

    function draw(g, f) {
      const N = n;
      const posOf = (v) => {
        const idx = v - 1;
        const a = -Math.PI / 2 + (2 * Math.PI * idx) / N;
        return [CX + R * Math.cos(a), CY + R * Math.sin(a)];
      };

      // 环
      V.el("circle", { cx: CX, cy: CY, r: R, class: "ln", fill: "none", "stroke-dasharray": "3 5" }, g);

      for (let v = 1; v <= N; v++) {
        const [x, y] = posOf(v);
        const gone = !f.alive.includes(v);
        const isCur = v === f.cur && !gone;
        const isDead = v === f.dead;
        let cls = "";
        if (gone) cls = "dim";
        else if (isCur) cls = "hot";
        V.circle(g, x, y, 22, String(v), cls);
        if (gone) {
          V.line(g, x - 18, y - 18, x + 18, y + 18, "hot");
          V.line(g, x - 18, y + 18, x + 18, y - 18, "hot");
        }
        if (isDead && gone) V.text(g, x, y - 30, "出列", "small hot");
      }

      // 报数
      if (f.k > 0) V.text(g, CX, CY + 6, `报数 ${f.k}`, "ok");

      // 出列序列
      V.text(g, 500, 60, "出列顺序", "small", "start");
      V.text(g, 500, 88, f.out.length ? f.out.join(" → ") : "（还没有人出列）", "mono", "start");

      V.text(g, 500, 130, `n = ${n}　m = ${m}`, "small", "start");
      V.text(g, 500, 250, f.note, "small", "start");
      V.text(g, 500, 274, "公式：J(n) = (J(n-1) + m) mod n", "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "约瑟夫环：报数 → 出列 → 重新报数",
      note: "用单循环链表实现时，pre 始终指向 p 的前驱，摘结点就是 pre->link = p->link。也可以不用链表，直接推公式 J(n) = (J(n-1)+m) mod n。",
      width: W, height: H, frames, draw, interval: 620,
      caption: (f) => f.note,
    });
  })();
})();
