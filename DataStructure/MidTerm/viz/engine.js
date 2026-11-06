// engine.js — 极简动画播放器 + 类调试器视图，只依赖 DOM 和 SVG。
//
// 每个动画 = 一串帧（frame）+ 一个把帧画到 SVG 上的函数。
// 帧里还可以带两部分调试信息：
//   line: 当前执行到第几行（1 起算）。单代码块时是数字，多代码块时是数组。
//   vars: 当前变量表。单代码块时是对象，多代码块时是对象数组。
// 代码块用 opts.code 声明：[ "第1行", "第2行", ... ] 或 [ {title, lines}, ... ]。

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
    return el("line", {
      x1, y1, x2, y2, class: "ln ar " + (cls || ""), "marker-end": "url(#ah)",
    }, g);
  }

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

  // ---------- 调试视图 ----------
  function normalizeCode(code) {
    if (!code) return [];
    if (typeof code[0] === "string") return [{ title: "", lines: code }];
    return code;
  }

  function pickLine(f, i, many) {
    if (f.line === undefined) return -1;
    if (many) return Array.isArray(f.line) ? f.line[i] : i === 0 ? f.line : -1;
    return Array.isArray(f.line) ? f.line[i] || -1 : f.line;
  }

  function pickVars(f, i, many) {
    if (!f.vars) return {};
    if (many) return (Array.isArray(f.vars) ? f.vars[i] : i === 0 ? f.vars : null) || {};
    return Array.isArray(f.vars) ? f.vars[i] || {} : f.vars;
  }

  function renderVars(tbody, cur, prev) {
    tbody.innerHTML = "";
    const keys = Object.keys(cur);
    if (!keys.length) {
      const tr = document.createElement("tr");
      const td = document.createElement("td");
      td.colSpan = 2;
      td.className = "k";
      td.textContent = "（无变量）";
      tr.appendChild(td);
      tbody.appendChild(tr);
      return;
    }
    keys.forEach((k) => {
      const tr = document.createElement("tr");
      const a = document.createElement("td");
      a.className = "k";
      a.textContent = k;
      const b = document.createElement("td");
      b.className = "v";
      const v = cur[k];
      b.textContent = v === undefined || v === null ? "—" : String(v);
      if (prev && String(prev[k]) !== String(v)) tr.className = "chg";
      tr.appendChild(a);
      tr.appendChild(b);
      tbody.appendChild(tr);
    });
  }

  // ---------- 播放器 ----------
  function panel(opts) {
    const host = typeof opts.host === "string" ? document.querySelector(opts.host) : opts.host;
    if (!host) return;

    const wrap = document.createElement("section");
    wrap.className = "viz";
    wrap.tabIndex = 0;

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

    // 左边：执行现场；右边：代码 + 变量
    const stagewrap = document.createElement("div");
    stagewrap.className = "stagewrap";
    const canvas = document.createElement("div");
    canvas.className = "canvas";
    canvas.appendChild(svg);
    stagewrap.appendChild(canvas);
    wrap.appendChild(stagewrap);

    // 调试视图：每列 = 标题 + 代码 + 变量
    const cols = normalizeCode(opts.code);
    // 没有代码但有变量时，仍然给一个变量栏
    if (!cols.length && (opts.frames || []).some((f) => f && f.vars)) cols.push({ title: "", lines: [] });
    const many = cols.length > 1;
    const dbg = document.createElement("div");
    dbg.className = "dbg";
    const view = cols.map((c) => {
      const col = document.createElement("div");
      col.className = "col";
      if (c.title) {
        const t = document.createElement("div");
        t.className = "ctitle";
        t.textContent = c.title;
        col.appendChild(t);
      }
      const pre = document.createElement("pre");
      pre.className = "code";
      const rows = (c.lines || []).map((src, i) => {
        const r = document.createElement("div");
        r.className = "cl";
        const n = document.createElement("span");
        n.className = "lnum";
        n.textContent = String(i + 1);
        const t = document.createElement("span");
        t.className = "ct";
        t.textContent = src === "" ? " " : src;
        r.appendChild(n);
        r.appendChild(t);
        pre.appendChild(r);
        return r;
      });
      col.appendChild(pre);
      const tbl = document.createElement("table");
      tbl.className = "vars";
      const tb = document.createElement("tbody");
      tbl.appendChild(tb);
      col.appendChild(tbl);
      dbg.appendChild(col);
      return { rows, tb };
    });
    if (cols.length) stagewrap.appendChild(dbg);

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
      Array.from(svg.children).forEach((c) => {
        if (c.tagName.toLowerCase() !== "defs") svg.removeChild(c);
      });
      const g = el("g", {}, svg);
      const f = frames[idx];
      if (f !== undefined && opts.draw) opts.draw(g, f, idx, frames);

      if (cols.length) {
        const prev = idx > 0 ? frames[idx - 1] : null;
        view.forEach((v, i) => {
          const active = pickLine(f || {}, i, many);
          v.rows.forEach((r, k) => {
            if (k + 1 === active) r.classList.add("on");
            else r.classList.remove("on");
          });
          renderVars(v.tb, pickVars(f || {}, i, many), prev ? pickVars(prev, i, many) : null);
        });
      }

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

    wrap.onkeydown = (e) => {
      if (e.key === " ") { e.preventDefault(); play(); }
      if (e.key === "ArrowRight") { e.preventDefault(); stop(); go(idx + 1); }
      if (e.key === "ArrowLeft") { e.preventDefault(); stop(); go(idx - 1); }
    };

    // 自动滚动到当前行，代码块较长时也能跟上
    const origGo = go;
    go = function (n) {
      origGo(n);
      view.forEach((v) => {
        const on = v.rows.find((r) => r.classList.contains("on"));
        if (on && on.scrollIntoView) {
          try { on.scrollIntoView({ block: "nearest" }); } catch (e) { /* 忽略 */ }
        }
      });
    };

    render();
  }

  window.Viz = { el, box, text, circle, line, arrow, curve, panel, NS };
})();
