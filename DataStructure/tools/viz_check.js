#!/usr/bin/env node
// viz_check.js — 用 node 跑一遍 MidTerm/viz 下所有动画，检查两类问题：
//   1. 绘制元素超出 viewBox（会被裁掉，看起来像文字被切了）
//   2. 文字互相重叠，或文字大面积压在结点/方框上
//
// 用法：node tools/viz_check.js
// 说明：这里没有真实浏览器，DOM 和 SVG 都是桩。文字宽度按「ASCII 7px、其他 13px」估算，
//       所以它只能抓明显越界和明显重叠，抓不到字形细节。

const fs = require("fs");
const path = require("path");
const vm = require("vm");

const DIR = path.join(__dirname, "..", "MidTerm", "viz");

class El {
  constructor(tag) {
    this.tagName = tag;
    this.children = [];
    this.attrs = {};
    this._text = "";
    this._html = "";
    this.classList = {
      _s: new Set(),
      add(c) { this._s.add(c); },
      remove(c) { this._s.delete(c); },
      contains(c) { return this._s.has(c); },
      toggle(c, on) { if (on === undefined ? !this._s.has(c) : on) this._s.add(c); else this._s.delete(c); },
    };
  }
  setAttribute(k, v) { this.attrs[k] = v; }
  getAttribute(k) { return this.attrs[k]; }
  appendChild(c) { this.children.push(c); return c; }
  removeChild(c) { const i = this.children.indexOf(c); if (i >= 0) this.children.splice(i, 1); }
  set className(v) { this.attrs.class = v; }
  get className() { return this.attrs.class; }
  set textContent(v) { this._text = String(v); }
  get textContent() { return this._text; }
  set innerHTML(v) { this._html = String(v); if (v === "") this.children = []; }
  get innerHTML() { return this._html; }
  scrollIntoView() {}
}

const document = {
  createElementNS: (ns, tag) => new El(tag),
  createElement: (tag) => new El(tag),
  querySelector: () => new El("div"),
};

const sandbox = { window: {}, document, console, Math, Set, Number, String, Array, Object, JSON };
sandbox.window.document = document;
const ctx = vm.createContext(sandbox);
vm.runInContext(fs.readFileSync(path.join(DIR, "engine.js"), "utf8"), ctx, { filename: "engine.js" });
const Viz = sandbox.window.Viz;

let boxes = [];
let frames = 0;
let problems = 0;

function textWidth(str) {
  let w = 0;
  for (const ch of String(str)) w += ch.charCodeAt(0) > 255 ? 13 : 7;
  return w;
}

const origText = Viz.text;
Viz.text = function (g, x, y, str, cls, anchor) {
  const w = textWidth(str);
  let x0 = x - w / 2, x1 = x + w / 2;
  if (anchor === "start") { x0 = x; x1 = x + w; }
  if (anchor === "end") { x0 = x - w; x1 = x; }
  boxes.push({ t: "text", s: String(str), x0, x1, y0: y - 12, y1: y + 5 });
  return origText.apply(null, arguments);
};
const origBox = Viz.box;
Viz.box = function (g, x, y, w, h, str) {
  boxes.push({ t: "box", s: String(str === undefined ? "" : str), x0: x, x1: x + w, y0: y, y1: y + h });
  return origBox.apply(null, arguments);
};
const origCircle = Viz.circle;
Viz.circle = function (g, cx, cy, r, str) {
  boxes.push({ t: "circle", s: String(str), x0: cx - r, x1: cx + r, y0: cy - r, y1: cy + r });
  return origCircle.apply(null, arguments);
};
const origLine = Viz.line;
Viz.line = function (g, x1, y1, x2, y2) {
  boxes.push({ t: "line", s: "", x0: Math.min(x1, x2), x1: Math.max(x1, x2), y0: Math.min(y1, y2), y1: Math.max(y1, y2) });
  return origLine.apply(null, arguments);
};
const origArrow = Viz.arrow;
Viz.arrow = function (g, x1, y1, x2, y2) {
  boxes.push({ t: "arrow", s: "", x0: Math.min(x1, x2), x1: Math.max(x1, x2), y0: Math.min(y1, y2), y1: Math.max(y1, y2) });
  return origArrow.apply(null, arguments);
};

Viz.panel = function (opts) {
  const W = opts.width || 760;
  const H = opts.height || 300;
  const name = opts.title || "(untitled)";
  const seen = new Set();

  for (let i = 0; i < opts.frames.length; i++) {
    frames += 1;
    boxes = [];
    opts.draw(new El("g"), opts.frames[i], i, opts.frames);
    if (opts.caption) opts.caption(opts.frames[i], i, opts.frames);

    for (const b of boxes) {
      if (b.y0 < -1 || b.y1 > H + 1 || b.x0 < -1 || b.x1 > W + 1) {
        const key = `O|${b.t}|${b.s}|${Math.round(b.y1)}`;
        if (seen.has(key)) continue;
        seen.add(key);
        problems += 1;
        console.log(`[${name}] 帧${i} ${b.t} "${b.s}" 越界: ` +
          `x ${b.x0.toFixed(0)}..${b.x1.toFixed(0)} y ${b.y0.toFixed(0)}..${b.y1.toFixed(0)}  (${W}x${H})`);
      }
    }

    const texts = boxes.filter((b) => b.t === "text" && b.s.trim());
    const shapes = boxes.filter((b) => b.t === "circle" || b.t === "box");

    for (const T of texts) {
      for (const S of shapes) {
        const ox = Math.min(T.x1, S.x1) - Math.max(T.x0, S.x0);
        const oy = Math.min(T.y1, S.y1) - Math.max(T.y0, S.y0);
        if (ox <= 0 || oy <= 0) continue;
        const tArea = (T.x1 - T.x0) * (T.y1 - T.y0);
        if (ox * oy > tArea * 0.5) {
          const key = `S|${T.s}|${S.s}`;
          if (seen.has(key)) continue;
          seen.add(key);
          problems += 1;
          console.log(`[${name}] 帧${i} 文字压住图形: "${T.s}" 盖在 ${S.t} "${S.s}" 上`);
        }
      }
    }

    for (let a = 0; a < texts.length; a++) {
      for (let b2 = a + 1; b2 < texts.length; b2++) {
        const A = texts[a], B = texts[b2];
        const ox = Math.min(A.x1, B.x1) - Math.max(A.x0, B.x0);
        const oy = Math.min(A.y1, B.y1) - Math.max(A.y0, B.y0);
        if (ox <= 0 || oy <= 0) continue;
        const small = Math.min((A.x1 - A.x0) * (A.y1 - A.y0), (B.x1 - B.x0) * (B.y1 - B.y0));
        if (ox * oy > small * 0.45) {
          const key = `T|${A.s}|${B.s}`;
          if (seen.has(key)) continue;
          seen.add(key);
          problems += 1;
          console.log(`[${name}] 帧${i} 文字重叠: "${A.s}" 与 "${B.s}"`);
        }
      }
    }
  }
};

for (let n = 1; n <= 8; n++) {
  vm.runInContext(fs.readFileSync(path.join(DIR, `l${n}.js`), "utf8"), ctx, { filename: `l${n}.js` });
}

console.log(`\n共 ${frames} 帧，发现 ${problems} 类问题`);
process.exit(problems ? 1 : 0);
