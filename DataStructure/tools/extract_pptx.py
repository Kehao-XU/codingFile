#!/usr/bin/env python3
"""extract_pptx.py — 把 pptx 正文抽成按页分节的纯文本，只回报一份可跳转的目录。

用法：
    python tools/extract_pptx.py 课件/chapter-9-full.pptx
    python tools/extract_pptx.py 课件/*.pptx --brief
    python tools/extract_pptx.py 课件/chapter-9-full.pptx --out material --stdout

设计意图：
    * 正文写进文件，stdout 只留目录。调用方（通常是子会话）把目录交给主线，
      原文留在盘上，需要精确引用时按行号 grep 那几行。
    * 目录每行带行号，形如 "L  412  第 37 张 · 栈的应用"，可直接
      `grep -n "栈的应用" material/chapter-9-full.txt` 定位。
    * 幻灯片顺序取自 presentation.xml 的 sldIdLst，不靠文件名猜。

约定：
    * 只解 zip 读 XML，不依赖第三方库，不改动 pptx。
    * 表格按行输出，单元格之间用 " | " 分隔。
    * 幻灯片编号、页码、页眉页脚等非正文占位符跳过。
    * 输出文件一律 UTF-8；stdout 跟随控制台编码。
"""

import argparse
import datetime
import glob
import os
import posixpath
import re
import sys
import zipfile
import xml.etree.ElementTree as ET

A = "http://schemas.openxmlformats.org/drawingml/2006/main"
P = "http://schemas.openxmlformats.org/presentationml/2006/main"
R = "http://schemas.openxmlformats.org/officeDocument/2006/relationships"
NS = {"a": A, "p": P, "r": R}

SKIP_PH = {"sldNum", "dt", "ftr", "hdr"}


def local(tag):
    return tag.rsplit("}", 1)[-1]


def slide_order(z):
    """按 sldIdLst 的顺序返回 slide xml 的完整路径。"""
    names = set(z.namelist())
    order = []
    try:
        pres = ET.fromstring(z.read("ppt/presentation.xml"))
        rels = ET.fromstring(z.read("ppt/_rels/presentation.xml.rels"))
        target = {}
        for rel in rels:
            target[rel.get("Id")] = rel.get("Target") or ""
        lst = pres.find("p:sldIdLst", NS)
        if lst is not None:
            for sid in lst.findall("p:sldId", NS):
                rid = sid.get("{%s}id" % R)
                tgt = target.get(rid, "").lstrip("/")
                if not tgt:
                    continue
                full = posixpath.normpath(tgt) if tgt.startswith("ppt/") \
                    else posixpath.normpath(posixpath.join("ppt", tgt))
                if full in names:
                    order.append(full)
    except (KeyError, ET.ParseError):
        pass
    if not order:
        cand = [n for n in names if re.match(r"^ppt/slides/slide\d+\.xml$", n)]
        order = sorted(cand, key=lambda n: int(re.search(r"(\d+)\.xml$", n).group(1)))
    return order


def is_content_shape(sp):
    ph = sp.find("p:nvSpPr/p:nvPr/p:ph", NS)
    if ph is None:
        return True
    return ph.get("type") not in SKIP_PH


def walk(elem, lines):
    """按文档顺序收集文本行。段落成行，表格行成一行。"""
    name = local(elem.tag)
    if name == "p":
        text = " ".join("".join(elem.itertext()).split())
        if text:
            lines.append(text)
        return
    if name == "tbl":
        for tr in elem.findall("a:tr", NS):
            cells = [" ".join("".join(tc.itertext()).split())
                     for tc in tr.findall("a:tc", NS)]
            if any(cells):
                lines.append(" | ".join(cells))
        return
    if name == "sp" and not is_content_shape(elem):
        return
    for child in elem:
        walk(child, lines)


def slide_title(tree):
    if tree is None:
        return ""
    for sp in tree.iter("{%s}sp" % P):
        ph = sp.find("p:nvSpPr/p:nvPr/p:ph", NS)
        if ph is None or ph.get("type") not in ("title", "ctrTitle"):
            continue
        for para in sp.findall(".//a:p", NS):
            text = " ".join("".join(para.itertext()).split())
            if text:
                return text
    return ""


def extract(path):
    with zipfile.ZipFile(path) as z:
        slides = []
        for idx, name in enumerate(slide_order(z), 1):
            root = ET.fromstring(z.read(name))
            tree = root.find("p:cSld/p:spTree", NS)
            lines = []
            if tree is not None:
                walk(tree, lines)
            title = slide_title(tree)
            if lines and lines[0] == title:
                lines = lines[1:]
            slides.append((idx, title, lines))
    return slides


def render(stem, slides, stamp):
    out = ["# %s" % stem, "# %d 张幻灯片 · 提取于 %s" % (len(slides), stamp), ""]
    outline = []
    for idx, title, lines in slides:
        label = title or "(无标题)"
        if len(label) > 60:
            label = label[:60] + "…"
        outline.append((len(out) + 1, "第 %d 张 · %s" % (idx, label)))
        out.append("## 第 %d 张 · %s" % (idx, label))
        out.extend(lines or ["(本页无可提取文本)"])
        out.append("")
    return "\n".join(out), outline


def main():
    ap = argparse.ArgumentParser(
        description="把 pptx 正文抽成按页分节的纯文本，stdout 只回报目录。")
    ap.add_argument("files", nargs="+", help="pptx 路径，可用通配符")
    ap.add_argument("--out", default="material", help="输出目录，默认 material/")
    ap.add_argument("--stdout", action="store_true", help="正文打到 stdout，不写文件")
    ap.add_argument("--brief", action="store_true", help="只回报路径、页数、行数")
    args = ap.parse_args()

    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")

    paths = []
    for pattern in args.files:
        hit = glob.glob(pattern)
        paths.extend(hit or [pattern])

    stamp = datetime.date.today().isoformat()
    failed = 0

    for path in paths:
        if not os.path.isfile(path):
            print("跳过（不存在）：%s" % path)
            failed += 1
            continue
        try:
            slides = extract(path)
        except zipfile.BadZipFile:
            print("跳过（不是有效的 pptx）：%s" % path)
            failed += 1
            continue

        stem = os.path.splitext(os.path.basename(path))[0]
        body, outline = render(stem, slides, stamp)
        total = body.count("\n")

        if args.stdout:
            print(body)
            continue

        os.makedirs(args.out, exist_ok=True)
        dest = os.path.join(args.out, stem + ".txt")
        existed = os.path.exists(dest)
        with open(dest, "w", encoding="utf-8", newline="\n") as f:
            f.write(body)

        print("%s%s  ·  %d 张  ·  %d 行" % (
            dest.replace("\\", "/"), "（覆盖）" if existed else "",
            len(slides), total))
        if not args.brief:
            for lineno, label in outline:
                print("  L%5d  %s" % (lineno, label))

    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
