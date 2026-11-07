// l8.js — 第 8 讲：最小堆的上浮、堆排序
(function () {
  const V = window.Viz;

  const CX = 380;
  function pos(i) {
    const d = Math.floor(Math.log2(i + 1));
    const off = i - (Math.pow(2, d) - 1);
    const total = Math.pow(2, d);
    const span = 620 / total;
    return [CX + (off - (total - 1) / 2) * span, 62 + d * 78];
  }

  function drawHeap(g, arr, hot, sortedFrom) {
    const deadAt = (i) => sortedFrom !== undefined && i >= sortedFrom;
    for (let i = 1; i < arr.length; i++) {
      const p = Math.floor((i - 1) / 2);
      if (arr[i] === null) continue;
      const a = pos(p), b = pos(i);
      V.line(g, a[0], a[1], b[0], b[1], deadAt(i) || deadAt(p) ? "dim" : "");
    }
    arr.forEach((v, i) => {
      if (v === null) return;
      const [x, y] = pos(i);
      const dead = deadAt(i);
      V.circle(g, x, y, 20, String(v), hot && hot.includes(i) ? "hot" : dead ? "dim" : "");
    });
  }

  // ---------- 动画 1：最小堆插入 ----------
  (function heapInsert() {
    const data = [4, 2, 5, 8, 3, 6, 10, 14];
    const heap = [];
    const frames = [];

    const vars = (x, j, i, temp) => ({
      "插入值 x": x === undefined ? "—" : x,
      "size": heap.length,
      "j": j === undefined ? "—" : j,
      "i（父结点）": i === undefined ? "—" : i,
      "temp": temp === undefined ? "—" : temp,
      "堆": heap.join(" ") || "（空）",
    });
    const push = (line, note, hot, x, j, i, temp) =>
      frames.push({ heap: heap.slice(), note, line, hot: hot || [], vars: vars(x, j, i, temp) });

    push(1, "空堆", [], "—", "—", "—", "—");

    data.forEach((x) => {
      heap.push(x);
      push(2, `把 ${x} 放到数组末尾`, [heap.length - 1], x, heap.length - 1, Math.floor((heap.length - 2) / 2), x);

      let j = heap.length - 1;
      let temp = x;
      while (j > 0) {
        const p = Math.floor((j - 1) / 2);
        push(8, `比较 a[${p}] = ${heap[p]} 与 temp = ${temp}`, [p, j], x, j, p, temp);
        if (heap[p] <= temp) break;
        heap[j] = heap[p];
        push(9, `父结点 ${heap[p]} 下沉到位置 ${j}`, [p, j], x, j, p, temp);
        j = p;
        push(10, `j 上移到 ${j}，继续和新的父结点比`, [j], x, j, Math.floor((j - 1) / 2), temp);
      }
      heap[j] = temp;
      push(12, `${temp} 落在位置 ${j}，堆序恢复`, [j], x, j, Math.floor((j - 1) / 2), temp);
    });

    const W = 760, H = 420;

    function draw(g, f) {
      drawHeap(g, f.heap, f.hot);

      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 344, 38, 30, String(v), f.hot.includes(i) ? "hot" : "");
      });
      V.text(g, 40, 336, "数组表示", "small", "start");
      V.text(g, 40, 400, f.note, "small", "start");
      V.text(g, 520, 400, `共 ${f.heap.length} 个元素`, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "最小堆插入：新元素一路上浮",
      note: "插入只在数组末尾加一个元素，然后和父结点比，比父小就上移。树高 ⌊log₂n⌋，所以最多走这么多次——插入是 O(log n)，不是 O(n)。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "void insert(int x) {",
        "    a[size] = x;",
        "    siftUp(size);",
        "    ++size;",
        "}",
        "void siftUp(int j) {",
        "    int temp = a[j], i = (j - 1) / 2;",
        "    while (j > 0 && a[i] > temp) {",
        "        a[j] = a[i];",
        "        j = i;  i = (i - 1) / 2;",
        "    }",
        "    a[j] = temp;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 2：堆排序 ----------
  (function heapSort() {
    const arr = [53, 17, 78, 9, 45, 65, 87, 23];
    const frames = [];
    let heap = arr.slice();
    let sortedFrom = heap.length;

    const push = (line, note, hot, extra) =>
      frames.push({
        heap: heap.slice(),
        sortedFrom,
        line, note, hot: hot || [],
        vars: Object.assign({ "已排好": heap.length - sortedFrom, "堆大小": sortedFrom }, extra || {}),
      });

    push(1, "原始数组，先自底向上建最大堆", []);
    for (let i = Math.floor((heap.length - 2) / 2); i >= 0; i--) {
      push(1, `从最后一个非叶结点 a[${i}] = ${heap[i]} 开始下沉`, [i], { "i": i });
      siftDown(i, heap.length);
    }
    push(2, "建堆完成：a[i] ≥ a[2i+1] 且 ≥ a[2i+2]", []);

    for (let end = heap.length - 1; end > 0; end--) {
      [heap[0], heap[end]] = [heap[end], heap[0]];
      sortedFrom = end;
      push(4, `堆顶 ${heap[end]} 换到末尾 a[${end}]，锁定一个元素`, [0, end], { "end": end });
      siftDown(0, end);
    }
    // 最后一帧：全部排好，不再有"灰掉"的部分，直接展示完整数组
    sortedFrom = heap.length;
    push(3, "全部排好：升序", []);
    const lastFrame = frames[frames.length - 1];
    lastFrame.vars["已排好"] = heap.length;
    lastFrame.vars["堆大小"] = 0;

    function siftDown(i, n) {
      let cmp = 0;
      while (true) {
        let big = i;
        const l = 2 * i + 1, r = 2 * i + 2;
        push(11, `比较 a[${i}]=${heap[i]} 的左右子女`, [i, l, r].filter((k) => k < n),
          { "i": i, "j": l < n ? l : "—", "temp": heap[i] });
        if (l < n && heap[l] > heap[big]) big = l;
        if (r < n && heap[r] > heap[big]) big = r;
        if (big === i) {
          push(12, "父结点已经是三者最大，停", [i], { "i": i, "temp": heap[i] });
          break;
        }
        [heap[i], heap[big]] = [heap[big], heap[i]];
        push(13, `交换后 ${heap[i]} 上浮，${heap[big]} 继续下沉`, [i, big], { "i": big, "temp": heap[i] });
        i = big;
        if (++cmp > 8) break;
      }
    }

    const W = 760, H = 420;

    function draw(g, f) {
      drawHeap(g, f.heap, f.hot, f.sortedFrom);
      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 344, 38, 30, String(v),
          f.hot.includes(i) ? "hot" : i >= f.sortedFrom ? "dim" : "");
      });
      V.text(g, 40, 336, "数组（灰掉的是已排好的部分）", "small", "start");
      V.text(g, 40, 400, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "堆排序：先建堆，再反复取堆顶",
      note: "建堆从最后一个非叶结点往前下沉，总代价 O(n)；之后每取一次堆顶 O(log n)，共 n-1 次。注意建堆不是「一个个插入上浮」，那样是 O(n log n)。",
      width: W, height: H, frames, draw, interval: 560,
      code: [
        "for (int i = (n - 2) / 2; i >= 0; --i)   // 建堆",
        "    siftDown(a, i, n);",
        "for (int end = n - 1; end > 0; --end) {",
        "    swap(a[0], a[end]);",
        "    siftDown(a, 0, end);",
        "}",
        "",
        "void siftDown(int *a, int i, int n) {",
        "    int temp = a[i], j = 2 * i + 1;",
        "    while (j < n) {",
        "        if (j + 1 < n && a[j + 1] > a[j]) ++j;",
        "        if (temp >= a[j]) break;",
        "        a[i] = a[j];  i = j;  j = 2 * j + 1;",
        "    }",
        "    a[i] = temp;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 3：中序线索化 ----------
  (function threadedAnim() {
    const PRE = "ABHFDECKG", INO = "HBDFAEKCG";
    const nodes = {};
    function build(ps, is, len, depth) {
      if (len === 0) return null;
      const r = PRE[ps];
      const k = INO.indexOf(r, is) - is;
      const nd = { v: r, depth, l: null, r: null, x: 0, y: 0 };
      nd.l = build(ps + 1, is, k, depth + 1);
      nd.r = build(ps + k + 1, is + k + 1, len - k - 1, depth + 1);
      nodes[r] = nd;
      return nd;
    }
    const root = build(0, 0, PRE.length, 0);
    let counter = 0;
    (function place(nd) {
      if (!nd) return;
      place(nd.l);
      nd.x = 70 + counter++ * 74;
      nd.y = 55 + nd.depth * 66;
      place(nd.r);
    })(root);

    const order = [];
    (function ino(nd) { if (!nd) return; ino(nd.l); order.push(nd); ino(nd.r); })(root);

    const lt = {}, rt = {};
    order.forEach((nd) => { lt[nd.v] = 0; rt[nd.v] = 0; });
    let threads = [];

    const count = () => order.reduce((s, nd) => s + lt[nd.v] + rt[nd.v], 0);
    const firstNode = (nd) => {
      while (nd && !lt[nd.v] && nd.l) nd = nd.l;
      return nd;
    };
    const lastNode = (nd) => {
      while (nd && !rt[nd.v] && nd.r) nd = nd.r;
      return nd;
    };

    const frames = [];
    const snap = (extra) => Object.assign({
      lt: Object.assign({}, lt), rt: Object.assign({}, rt),
      threads: threads.slice(), line: 3, note: "", vars: {},
    }, extra || {});

    frames.push(snap({
      note: "初始：所有 ltag = rtag = 0，指针都指向真子女（或为空）。",
      vars: { "阶段": "准备", "线索数": 0 },
    }));

    let pre = null;
    order.forEach((nd) => {
      const name = nd.v;
      frames.push(snap({
        cur: name, pre: pre ? pre.v : "—",
        note: `处理 ${name}，pre = ${pre ? pre.v : "NULL"}`,
        line: 3,
        vars: { "阶段": "线索化", "当前 p": name, "pre": pre ? pre.v : "—", "线索数": count() },
      }));

      const msgs = [];
      if (!nd.l) {
        lt[name] = 1;
        if (pre) threads.push({ a: name, b: pre.v, kind: "pred" });
        msgs.push(`${name} 无左子女：left 指向前驱 ${pre ? pre.v : "NULL"}`);
      }
      if (pre && !pre.r) {
        rt[pre.v] = 1;
        threads.push({ a: pre.v, b: name, kind: "succ" });
        msgs.push(`${pre.v} 无右子女：right 指向后继 ${name}`);
      }
      pre = nd;

      frames.push(snap({
        cur: name, pre: pre ? pre.v : "—",
        note: msgs.length ? msgs.join("；") : `${name} 暂不新建线索`,
        line: msgs.length ? 4 : 3,
        vars: { "阶段": "线索化", "当前 p": name, "pre": pre ? pre.v : "—", "线索数": count() },
      }));
    });

    if (pre && !pre.r) rt[pre.v] = 1;
    frames.push(snap({
      cur: "—", pre: pre ? pre.v : "—",
      note: `线索化完成：共 ${count()} 条线索（n+1 = 10）。`,
      line: 7,
      vars: { "阶段": "完成", "线索数": count(), "第一个": firstNode(root).v, "最后": lastNode(root).v },
    }));

    function threadSucc(v) {
      const t = threads.find((x) => x.a === v && x.kind === "succ");
      return t ? nodes[t.b] : null;
    }
    function nextNode(nd) {
      if (!nd) return null;
      if (rt[nd.v] === 1) return threadSucc(nd.v);
      return firstNode(nd.r);
    }

    const out = [];
    let p = firstNode(root);
    while (p) {
      out.push(p.v);
      frames.push(snap({
        cur: p.v, out: out.slice(), phase: "walk",
        note: `沿 next 走到 ${p.v}`,
        line: 12,
        vars: { "阶段": "无栈遍历", "当前 p": p.v, "输出": out.join(" "), "已访问": out.length },
      }));
      p = nextNode(p);
    }

    const W = 760, H = 430;

    function draw(g, f) {
      for (const k in nodes) {
        const nd = nodes[k];
        if (nd.l) V.line(g, nd.x, nd.y, nd.l.x, nd.l.y, "");
        if (nd.r) V.line(g, nd.x, nd.y, nd.r.x, nd.r.y, "");
      }

      f.threads.forEach((t) => {
        const a = nodes[t.a], b = nodes[t.b];
        const cls = t.kind === "succ" ? "thread ok" : "thread hot";
        V.curve(g, a.x, a.y, b.x, b.y, t.kind === "succ" ? 26 : -26, cls);
      });

      const visited = new Set(f.out || []);
      for (const k in nodes) {
        const nd = nodes[k];
        let cls = "";
        if (f.cur === k) cls = "hot";
        else if (f.pre === k) cls = "ok";
        else if (visited.has(k)) cls = "acc";
        V.circle(g, nd.x, nd.y, 18, k, cls);
        V.text(g, nd.x, nd.y + 30, `${f.lt[k]}/${f.rt[k]}`, "small mono");
      }

      V.text(g, 40, 350, "虚线：橙 = 前驱线索，绿 = 后继线索；结点下方是 ltag/rtag。", "small", "start");
      V.text(g, 40, 376, f.note, "small", "start");
      V.text(g, 40, 402, f.out ? "输出：" + f.out.join(" ") : "", "mono", "start");
    }

    V.panel({
      host: "#a3",
      title: "中序线索化：把空指针变成前驱/后继",
      note: "中序遍历一遍，用 pre 记住上一个访问的结点：p 左空就指 pre，pre 右空就指 p。线索化后可以不用栈做中序遍历。",
      width: W, height: H, frames, draw, interval: 700,
      code: [
        "void inThread(ThreadNode *p, ThreadNode *&pre) {",
        "    if (!p) return;",
        "    inThread(p->left, pre);",
        "    if (!p->left) { p->left = pre; p->ltag = 1; }",
        "    if (pre && !pre->right) { pre->right = p; pre->rtag = 1; }",
        "    pre = p;",
        "    inThread(p->right, pre);",
        "}",
        "",
        "// 无栈中序遍历",
        "for (ThreadNode *p = first(root); p; p = next(p))",
        "    visit(p);",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 4：删除堆顶 ----------
  (function heapDelete() {
    let heap = [2, 3, 5, 8, 4, 6, 10, 14];
    const out = [];
    const frames = [];

    const push = (line, note, hot, extra) => frames.push({
      heap: heap.slice(), out: out.slice(), hot: hot || [], line, note,
      vars: Object.assign({
        "堆大小": heap.length, "已弹出": out.length,
        "输出": out.join(" ") || "（空）",
      }, extra || {}),
    });

    push(1, "初始最小堆：堆顶 2 是最小值", []);
    while (heap.length) {
      const x = heap[0];
      push(2, `取出堆顶 ${x}`, [0], { "取出的值": x });
      const last = heap.pop();
      if (heap.length) {
        heap[0] = last;
        push(3, `把末尾 ${last} 搬到堆顶，堆大小减 1`, [0], { "取出的值": x });
        let i = 0;
        while (true) {
          const l = 2 * i + 1, r = 2 * i + 2;
          push(10, `比较 a[${i}]=${heap[i]} 与子女`, [i, l, r].filter((k) => k < heap.length), { "i": i });
          let small = i;
          if (l < heap.length && heap[l] < heap[small]) small = l;
          if (r < heap.length && heap[r] < heap[small]) small = r;
          if (small === i) {
            push(12, `a[${i}] 已经不大于子女，停`, [i], { "i": i });
            break;
          }
          const tmp = heap[i];
          heap[i] = heap[small];
          heap[small] = tmp;
          push(13, `交换 ${heap[small]} 与 ${heap[i]}，继续下沉`, [i, small], { "i": small });
          i = small;
        }
      }
      out.push(x);
      push(5, `输出 ${x}`, [], { "取出的值": x });
    }
    push(6, `全部弹出：${out.join(" ")}（升序）`, []);

    const W = 760, H = 440;

    function draw(g, f) {
      drawHeap(g, f.heap, f.hot);
      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 344, 38, 30, String(v), f.hot.includes(i) ? "hot" : "");
      });
      V.text(g, 40, 336, "数组表示", "small", "start");
      V.text(g, 40, 390, "输出：" + (f.out.length ? f.out.join(" ") : "（空）"), "mono", "start");
      V.text(g, 40, 416, f.note, "small", "start");
      V.text(g, 600, 416, `堆大小 ${f.heap.length}`, "small", "start");
    }

    V.panel({
      host: "#a4",
      title: "删除堆顶：末尾搬到根，再一路下沉",
      note: "删除最小值只动根和末尾：末尾补到根，堆大小减 1，然后和较小的子女交换。每次 O(log n)，连续弹出就是升序。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "int deleteMin(int *a, int &n) {",
        "    int top = a[0];",
        "    a[0] = a[--n];",
        "    siftDown(a, 0, n);",
        "    return top;",
        "}",
        "",
        "void siftDown(int *a, int i, int n) {",
        "    int temp = a[i], j = 2 * i + 1;",
        "    while (j < n) {",
        "        if (j + 1 < n && a[j + 1] < a[j]) ++j;",
        "        if (temp <= a[j]) break;",
        "        a[i] = a[j]; i = j; j = 2 * i + 1;",
        "    }",
        "    a[i] = temp;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 5：建堆与判堆 ----------
  (function heapBuildJudge() {
    const a = [53, 17, 78, 9, 45, 65, 87, 23];
    const frames = [];
    const push = (line, note, hot, extra) => frames.push({
      heap: a.slice(), hot: hot || [], line, note,
      vars: Object.assign({ "堆": a.join(" "), "n": a.length }, extra || {}),
    });

    push(1, "原始数组：还不是堆，从最后一个非叶结点开始下沉", []);
    for (let i = Math.floor((a.length - 2) / 2); i >= 0; i--) {
      push(2, `从 a[${i}] = ${a[i]} 开始 SiftDown`, [i], { "i": i });
      let k = i;
      while (true) {
        const l = 2 * k + 1, r = 2 * k + 2;
        push(3, `比较 a[${k}]=${a[k]} 与子女`, [k, l, r].filter((x) => x < a.length), { "i": k });
        let small = k;
        if (l < a.length && a[l] < a[small]) small = l;
        if (r < a.length && a[r] < a[small]) small = r;
        if (small === k) {
          push(3, `a[${k}] 不大于子女，停`, [k], { "i": k });
          break;
        }
        const t = a[k];
        a[k] = a[small];
        a[small] = t;
        push(3, `交换 ${a[small]} 与 ${a[k]}`, [k, small], { "i": small });
        k = small;
      }
    }
    push(4, "建堆完成，检查每个结点与子女的关系", []);

    let ok = true;
    for (let i = 0; i < a.length; i++) {
      const l = 2 * i + 1, r = 2 * i + 2;
      if (l >= a.length && r >= a.length) continue;
      const good = (l >= a.length || a[l] >= a[i]) && (r >= a.length || a[r] >= a[i]);
      ok = ok && good;
      push(good ? 7 : 8, `检查 a[${i}]=${a[i]}：${good ? "满足" : "不满足"}`, [i, l, r].filter((x) => x < a.length), { "i": i, "判定": good ? "✓" : "✗" });
    }
    push(11, `判定结果：${ok ? "是最小堆" : "不是最小堆"}`, []);

    const W = 760, H = 440;

    function draw(g, f) {
      drawHeap(g, f.heap, f.hot);
      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 344, 38, 30, String(v), f.hot.includes(i) ? "hot" : "");
      });
      V.text(g, 40, 336, "数组表示", "small", "start");
      V.text(g, 40, 390, f.note, "small", "start");
      V.text(g, 40, 416, "建堆：从最后一个非叶结点往前 SiftDown，总代价 O(n)", "small", "start");
    }

    V.panel({
      host: "#a5",
      title: "建堆与判堆：自底向上 O(n)",
      note: "建堆从最后一个非叶结点 (n-2)/2 开始，倒着对每个结点 SiftDown；越靠下的结点越多、能沉的距离越短，求和是 O(n)。判堆只需检查每个结点是否不大于自己的子女。",
      width: W, height: H, frames, draw, interval: 620,
      code: [
        "void buildMinHeap(int *a, int n) {",
        "    for (int i = (n - 2) / 2; i >= 0; --i)",
        "        siftDown(a, i, n);",
        "}",
        "",
        "bool isMinHeap(const int *a, int n) {",
        "    for (int i = 0; i < n; ++i) {",
        "        if (2*i+1 < n && a[2*i+1] < a[i]) return false;",
        "        if (2*i+2 < n && a[2*i+2] < a[i]) return false;",
        "    }",
        "    return true;",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 6：霍夫曼编码 ----------
  (function huffmanAnim() {
    const chars = ["A", "B", "C", "D", "E", "F", "G", "H"];
    const wt = [15, 3, 14, 2, 6, 9, 16, 17, 5, 11, 20, 29, 33, 49, 82];
    const kids = { 8: [3, 1], 9: [8, 4], 10: [5, 9], 11: [2, 0], 12: [6, 7], 13: [10, 11], 14: [12, 13] };
    const parent = {};
    for (const k in kids) {
      parent[kids[k][0]] = Number(k);
      parent[kids[k][1]] = Number(k);
    }

    const x = {}, depth = {};
    const leafOrder = [6, 7, 5, 3, 1, 4, 2, 0]; // G H F D B E C A
    leafOrder.forEach((id, i) => { x[id] = 80 + i * 80; });
    function setDepth(id, d) {
      depth[id] = d;
      if (id >= 8) {
        setDepth(kids[id][0], d + 1);
        setDepth(kids[id][1], d + 1);
      }
    }
    setDepth(14, 0);
    Object.keys(kids).forEach((k) => {
      const id = Number(k);
      x[id] = (x[kids[id][0]] + x[kids[id][1]]) / 2;
    });
    const Y = (id) => 42 + depth[id] * 58;

    const shown = new Set([0, 1, 2, 3, 4, 5, 6, 7]);
    const frames = [];
    const merges = [[3, 1, 8], [8, 4, 9], [5, 9, 10], [2, 0, 11], [6, 7, 12], [10, 11, 13], [12, 13, 14]];
    let wpl = 0;

    frames.push({
      shown: [...shown], hot: [], line: 1,
      note: "初始森林：8 个叶子，权值就是频度",
      vars: { "阶段": "合并", "已合并": 0, "WPL": "—" },
    });

    merges.forEach(([a, b, k], i) => {
      shown.add(k);
      wpl += wt[k];
      frames.push({
        shown: [...shown], hot: [a, b, k], line: 3,
        note: `取最小的两个根 ${wt[a]} 与 ${wt[b]}，合并成 ${wt[k]}`,
        vars: { "阶段": "合并", "已合并": i + 1, "WPL": wpl },
      });
    });

    frames.push({
      shown: [...shown], hot: [], line: 7,
      note: "合并完成：WPL = 5+11+20+29+33+49+82 = 229",
      vars: { "阶段": "完成", "WPL": wpl, "内部结点和": wpl },
    });

    function codeOf(id) {
      let p = id, s = "";
      while (parent[p] !== undefined) {
        const par = parent[p];
        s = (kids[par][0] === p ? "0" : "1") + s;
        p = par;
      }
      return s;
    }

    const allCodes = [];
    for (let i = 0; i < 8; i++) {
      const code = codeOf(i);
      allCodes.push(`${chars[i]}:${code}`);
      const path = [];
      let p = i;
      while (parent[p] !== undefined) { path.push(p); p = parent[p]; }
      path.push(14);
      path.reverse();
      frames.push({
        shown: [...shown], hot: path, codeLeaf: i, line: 13,
        note: `${chars[i]}：从根走到叶子，编码 ${code}（码长 ${code.length}）`,
        vars: { "阶段": "编码", "字符": chars[i], "频度": wt[i], "编码": code, "码长": code.length, "WPL": wpl },
      });
    }

    frames.push({
      shown: [...shown], hot: [], line: 18,
      note: "WPL 唯一，但 0/1 可以左右互换，所以编码不唯一。",
      vars: { "WPL": wpl, "编码表": allCodes.join(" ") },
    });

    const W = 760, H = 430;

    function draw(g, f) {
      const on = new Set(f.shown);
      for (const k in kids) {
        const id = Number(k);
        if (!on.has(id)) continue;
        const [a, b] = kids[id];
        V.line(g, x[id], Y(id), x[a], Y(a), "");
        V.line(g, x[id], Y(id), x[b], Y(b), "");
      }

      if (f.codeLeaf !== undefined) {
        let p = f.codeLeaf;
        const path = [p];
        while (parent[p] !== undefined) { p = parent[p]; path.push(p); }
        path.reverse();
        for (let i = 0; i < path.length - 1; i++) {
          const a = path[i], b = path[i + 1];
          const bit = kids[a][0] === b ? "0" : "1";
          V.line(g, x[a], Y(a), x[b], Y(b), "hot");
          V.text(g, (x[a] + x[b]) / 2 + (bit === "0" ? -10 : 10), (Y(a) + Y(b)) / 2, bit, "hot");
        }
      } else if (f.hot && f.hot.length === 3) {
        const [a, b, k] = f.hot;
        V.line(g, x[k], Y(k), x[a], Y(a), "hot");
        V.line(g, x[k], Y(k), x[b], Y(b), "hot");
      }

      const hot = new Set(f.hot || []);
      for (const id of on) {
        const leaf = id < 8;
        V.circle(g, x[id], Y(id), 17, String(wt[id]), hot.has(id) ? "hot" : leaf ? "" : "acc");
        if (leaf) V.text(g, x[id], Y(id) + 30, chars[id], "small");
      }

      V.text(g, 40, 390, "橙色边：当前合并或当前字符的编码路径；叶子下方是字符。", "small", "start");
      V.text(g, 40, 414, f.note, "small", "start");
    }

    V.panel({
      host: "#a6",
      title: "霍夫曼编码：每次合并最小的两个",
      note: "权值越大的叶子离根越近，编码越短。WPL 可以逐叶算「权值 × 深度」，也可以直接累加每次合并出来的内部结点权值。",
      width: W, height: H, frames, draw, interval: 850,
      code: [
        "for (int k = n; k < 2*n-1; ++k) {",
        "    findTwoMin(t, k, m1, m2);",
        "    t[k].weight = t[m1].weight + t[m2].weight;",
        "    t[k].left = m1;  t[k].right = m2;",
        "    t[m1].parent = t[m2].parent = k;",
        "    wpl += t[k].weight;",
        "}",
        "",
        "void buildCodes(const HNode *t, int n, char code[][MAXN], int *len) {",
        "    for (int i = 0; i < n; ++i) {",
        "        int p = i, k = 0;  char tmp[MAXN];",
        "        while (t[p].parent != -1) {",
        "            tmp[k++] = (t[t[p].parent].left == p) ? '0' : '1';",
        "            p = t[p].parent;",
        "        }",
        "        for (int j = 0; j < k; ++j) code[i][j] = tmp[k-1-j];",
        "        code[i][k] = '\\0';  len[i] = k;",
        "    }",
        "}",
      ],
      caption: (f) => f.note,
    });
  })();
})();
