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
        heap: heap.map((v, i) => (i < sortedFrom ? v : null)),
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
    sortedFrom = 0;
    push(3, "全部排好：升序", []);

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
      drawHeap(g, f.heap.filter((v) => v !== null), f.hot, f.sortedFrom);
      f.heap.forEach((v, i) => {
        V.box(g, 40 + i * 44, 344, 38, 30, v === null ? "" : String(v),
          f.hot.includes(i) ? "hot" : v === null ? "dim" : "");
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
})();
