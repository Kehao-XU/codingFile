// engine.js — 极简动画播放器，只依赖 DOM 和 SVG，没有第三方库。
// 每个动画 = 一串帧（frame）+ 一个把帧画到 SVG 上的函数。
// 用法见各 lN.js 末尾的 Viz.panel(...) 调用。

(function () {
  const NS = "http://www.w3.org/2000/svg";

  function el(tag, attrs, parent) {
    const n = document.createElementNS(NS, tag);
    if (attrs) for (const k in attrs) n.setAttribute(k, attrs[k]);
    if (parent) parent.appendChild(n);
    return n;
  }

  // ---------- 基础图元 ----------
  function box(g, x, y, w, h, text, cls) {
    const r = el("rect", { x, y, width: w, height: h, rx: 6, class: "bx " + (cls || "") }, g);
    if (text !== undefined && text !== null && text !== "") {
      el("text", { x: x + w / 2, y: y + h / 2 + 5, class: "tx" }, g).textContent = text;
    }
    return r;
  }

  function text(g, x, y, str, cls, anchor) {
    const t = el("text", { x, y, class: "tx " + (cls || ""), "text-anchor": anchor || "middle" }, g);
    t.textContent = str;
    return t;
  }

  function circle(g, cx, cy, r, str, cls) {
    el("circle", { cx, cy, r, class: "nd " + (cls || "") }, g);
    if (str !== undefined) text(g, cx, cy + 5, str, cls || "");
  }

  function line(g, x1, y1, x2, y2, cls) {
    return el("line", { x1, y1, x2, y2, class: "ln " + (cls || "") }, g);
  }

  function arrow(g, x1, y1, x2, y2, cls) {
    const a = el("line", {
      x1, y1, x2, y2, class: "ln ar " + (cls || ""), "marker-end": "url(#ah)",
    }, g);
    return a;
  }

  // 圆弧形式的箭头，用来画链表里的"掉头"指针
  function curve(g, x1, y1, x2, y2, bend, cls) {
    const mx = (x1 + x2) / 2;
    const my = (y1 + y2) / 2 - (bend || 30);
    return el("path", {
      d: `M${x1},${y1} Q${mx},${my} ${x2},${y2}`,
      class: "ln ar " + (cls || ""), fill: "none", "marker-end": "url(#ah)",
    }, g);
  }

  function defs(svg) {
    const d = el("defs", {}, svg);
    const m = el("marker", {
      id: "ah", viewBox: "0 0 10 10", refX: 9, refY: 5,
      markerWidth: 7, markerHeight: 7, orient: "auto-start-reverse",
    }, d);
    el("path", { d: "M0,0 L10,5 L0,10 z", class: "arhead" }, m);
  }

  // ---------- 播放器 ----------
  function panel(opts) {
    const host = typeof opts.host === "string" ? document.querySelector(opts.host) : opts.host;
    if (!host) return;

    const wrap = document.createElement("section");
    wrap.className = "viz";
    const h = document.createElement("h3");
    h.textContent = opts.title || "";
    wrap.appendChild(h);
    if (opts.note) {
      const p = document.createElement("p");
      p.className = "note";
      p.innerHTML = opts.note;
      wrap.appendChild(p);
    }

    const W = opts.width || 760;
    const H = opts.height || 300;
    const svg = el("svg", { viewBox: `0 0 ${W} ${H}`, class: "stage" });
    svg.setAttribute("preserveAspectRatio", "xMidYMid meet");
    defs(svg);
    wrap.appendChild(svg);

    const bar = document.createElement("div");
    bar.className = "bar";
    const bPrev = document.createElement("button");
    bPrev.textContent = "◀ 上一步";
    const bPlay = document.createElement("button");
    bPlay.textContent = "▶ 播放";
    bPlay.className = "primary";
    const bNext = document.createElement("button");
    bNext.textContent = "下一步 ▶";
    const bReset = document.createElement("button");
    bReset.textContent = "⟲ 重置";
    const spd = document.createElement("input");
    spd.type = "range";
    spd.min = "0.5";
    spd.max = "3";
    spd.step = "0.5";
    spd.value = "1";
    const spdLab = document.createElement("span");
    spdLab.className = "small";
    spdLab.textContent = "速度 1×";
    [bPrev, bPlay, bNext, bReset, spdLab, spd].forEach((x) => bar.appendChild(x));
    wrap.appendChild(bar);

    const cap = document.createElement("div");
    cap.className = "cap";
    wrap.appendChild(cap);
    host.appendChild(wrap);

    const frames = opts.frames || [];
    let idx = 0;
    let timer = null;
    let speed = 1;

    function render() {
      // 清掉除 defs 之外的全部内容
      Array.from(svg.children).forEach((c) => {
        if (c.tagName.toLowerCase() !== "defs") svg.removeChild(c);
      });
      const g = el("g", {}, svg);
      const f = frames[idx];
      if (f !== undefined && opts.draw) opts.draw(g, f, idx, frames);
      cap.innerHTML = opts.caption ? opts.caption(f, idx, frames) : "";
      bPlay.textContent = timer ? "⏸ 暂停" : "▶ 播放";
    }

    function go(n) {
      idx = Math.max(0, Math.min(frames.length - 1, n));
      render();
    }

    function stop() {
      if (timer) { clearInterval(timer); timer = null; }
      bPlay.textContent = "▶ 播放";
    }

    function play() {
      if (timer) { stop(); render(); return; }
      if (idx >= frames.length - 1) idx = 0;
      timer = setInterval(() => {
        if (idx >= frames.length - 1) { stop(); return; }
        idx += 1;
        render();
      }, (opts.interval || 700) / speed);
      render();
    }

    bPrev.onclick = () => { stop(); go(idx - 1); };
    bNext.onclick = () => { stop(); go(idx + 1); };
    bReset.onclick = () => { stop(); go(0); };
    bPlay.onclick = play;
    spd.oninput = () => {
      speed = Number(spd.value);
      spdLab.textContent = "速度 " + speed + "×";
      if (timer) { stop(); play(); }
    };

    // 键盘：面板获得焦点后可用空格/方向键
    wrap.tabIndex = 0;
    wrap.onkeydown = (e) => {
      if (e.key === " ") { e.preventDefault(); play(); }
      if (e.key === "ArrowRight") { e.preventDefault(); stop(); go(idx + 1); }
      if (e.key === "ArrowLeft") { e.preventDefault(); stop(); go(idx - 1); }
    };

    render();
  }

  window.Viz = { el, box, text, circle, line, arrow, curve, panel, NS };
})();
