// l8.js — 第 8 讲：最小堆的上浮、堆排序
(function () {
  const V = window.Viz;

  const CX = 380;
  function pos(i) {
    const d = Math.floor(Math.log2(i + 1));
    const off = i - (Math.pow(2, d) - 1);
    const total = Math.pow(2, d);
    const span = 620 / total;
    return [CX + (off - (total - 1) / 2) * span, 70 + d * 78];
  }

  function drawHeap(g, arr, hot, sortedFrom) {
    // 边
    for (let i = 1; i < arr.length; i++) {
      const p = Math.floor((i - 1) / 2);
      if (arr[i] === null) continue;
      const a = pos(p), b = pos(i);
      V.line(g, a[0], a[1], b[0], b[1], "");
    }
    arr.forEach((v, i) => {
      if (v === null) return;
      const [x, y] = pos(i);
      const dead = sortedFrom !== undefined && i >= sortedFrom;
      V.circle(g, x, y, 20, String(v), hot && hot.includes(i) ? "hot" : dead ? "dim" : "");
    });
  }

  // ---------- 动画 1：最小堆插入 ----------
  (function heapInsert() {
    const data = [4, 2, 5, 8, 3, 6, 10, 14];
    const heap = [];
    const frames = [{ heap: [], hot: [], note: "空堆" }];

    data.forEach((x) => {
      heap.push(x);
      frames.push({ heap: heap.slice(), hot: [heap.length - 1], note: `插入 ${x}，先放到数组末尾` });
      let j = heap.length - 1;
      while (j > 0) {
        const p = Math.floor((j - 1) / 2);
        if (heap[p] <= heap[j]) break;
        [heap[p], heap[j]] = [heap[j], heap[p]];
        frames.push({ heap: heap.slice(), hot: [p, j], note: `父结点 ${heap[j]} > 子结点 ${heap[p]}，交换` });
        j = p;
      }
      frames.push({ heap: heap.slice(), hot: [j], note: `${x} 落到位置 ${j}，堆序恢复` });
    });

    const W = 760, H = 400;

    function draw(g, f) {
      drawHeap(g, f.heap, f.hot);

      // 数组视图
      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 330, 38, 30, String(v), f.hot.includes(i) ? "hot" : "");
      });
      V.text(g, 40, 322, "数组表示", "small", "start");
      V.text(g, 40, 380, f.note, "small", "start");
      V.text(g, 520, 380, `共 ${f.heap.length} 个元素`, "small", "start");
    }

    V.panel({
      host: "#a1",
      title: "最小堆插入：新元素一路上浮",
      note: "插入只在数组末尾加一个元素，然后和父结点比，比父小就换。树高 ⌊log₂n⌋，所以最多交换这么多次——插入是 O(log n)，而不是 O(n)。",
      width: W, height: H, frames, draw, interval: 620,
      caption: (f) => f.note,
    });
  })();

  // ---------- 动画 2：堆排序 ----------
  (function heapSort() {
    const arr = [53, 17, 78, 9, 45, 65, 87, 23];
    const frames = [];
    let heap = arr.slice();
    let sortedFrom = heap.length;

    const push = (note, hot) =>
      frames.push({ heap: heap.map((v, i) => (i < sortedFrom ? v : null)), hot: hot || [], sortedFrom, note });

    push("原始数组，先建最大堆");

    function siftDown(i, n) {
      while (true) {
        let big = i;
        const l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && heap[l] > heap[big]) big = l;
        if (r < n && heap[r] > heap[big]) big = r;
        if (big === i) break;
        [heap[i], heap[big]] = [heap[big], heap[i]];
        push(`下沉：${heap[big]} 与 ${heap[i]} 交换`, [i, big]);
        i = big;
      }
    }

    for (let i = Math.floor((heap.length - 2) / 2); i >= 0; i--) {
      push(`从最后一个非叶结点 ${heap[i]} 开始下沉`, [i]);
      siftDown(i, heap.length);
    }
    push("建堆完成：a[i] ≥ a[2i+1] 且 ≥ a[2i+2]", []);

    for (let end = heap.length - 1; end > 0; end--) {
      [heap[0], heap[end]] = [heap[end], heap[0]];
      sortedFrom = end;
      push(`堆顶 ${heap[end]} 换到末尾，已排好 ${heap.length - end} 个`, [0, end]);
      siftDown(0, end);
    }
    sortedFrom = 0;
    push("全部排好：升序", []);

    const W = 760, H = 400;

    function draw(g, f) {
      drawHeap(g, f.heap.filter((v) => v !== null), f.hot, f.sortedFrom);

      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 330, 38, 30, v === null ? "" : String(v),
          f.hot.includes(i) ? "hot" : v === null ? "dim" : "");
      });
      V.text(g, 40, 322, "数组（灰色是已排好的部分）", "small", "start");
      V.text(g, 40, 380, f.note, "small", "start");
    }

    V.panel({
      host: "#a2",
      title: "堆排序：先建堆，再反复取堆顶",
      note: "建堆从最后一个非叶结点往前下沉，是 O(n)；之后每取一次堆顶要 O(log n)，共 n 次。注意建堆是自底向上下沉，不是一个个插入上浮——后者是 O(n log n)。",
      width: W, height: H, frames, draw, interval: 620,
      caption: (f) => f.note,
    });
  })();
})();
