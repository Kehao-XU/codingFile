# -*- coding: utf-8 -*-
"""Build a .pptx from slides.json (Chinese-friendly, 16:9, clean light theme)."""

import json
import sys
from pathlib import Path

from pptx import Presentation
from pptx.dml.color import RGBColor
from pptx.enum.text import PP_ALIGN, MSO_ANCHOR
from pptx.util import Inches, Pt, Emu
from pptx.oxml.ns import qn

W, H = Inches(13.333), Inches(7.5)

INK = RGBColor(0x1B, 0x21, 0x2C)
MUTED = RGBColor(0x6B, 0x72, 0x80)
ACCENT = RGBColor(0x2F, 0x6F, 0xED)
ACCENT_SOFT = RGBColor(0xE8, 0xEF, 0xFD)
WARN = RGBColor(0xD9, 0x53, 0x3B)
BG = RGBColor(0xFF, 0xFF, 0xFF)
PANEL = RGBColor(0xF5, 0xF7, 0xFA)
LINE = RGBColor(0xE2, 0xE6, 0xEC)
WHITE = RGBColor(0xFF, 0xFF, 0xFF)

FONT = "Microsoft YaHei"
MONO = "Consolas"


def set_font(run, size, bold=False, color=INK, font=FONT):
    run.font.size = Pt(size)
    run.font.bold = bold
    run.font.color.rgb = color
    run.font.name = font
    rPr = run._r.get_or_add_rPr()
    ea = rPr.find(qn("a:ea"))
    if ea is None:
        ea = rPr.makeelement(qn("a:ea"), {})
        rPr.append(ea)
    ea.set("typeface", font)


def add_text(slide, left, top, width, height, text, size=18, bold=False,
             color=INK, align=PP_ALIGN.LEFT, font=FONT, anchor=MSO_ANCHOR.TOP,
             line_spacing=1.15):
    box = slide.shapes.add_textbox(left, top, width, height)
    tf = box.text_frame
    tf.word_wrap = True
    tf.vertical_anchor = anchor
    tf.margin_left = 0
    tf.margin_right = 0
    tf.margin_top = 0
    tf.margin_bottom = 0
    p = tf.paragraphs[0]
    p.alignment = align
    p.line_spacing = line_spacing
    r = p.add_run()
    r.text = text
    set_font(r, size, bold, color, font)
    return box


def add_rect(slide, left, top, width, height, fill, line=None, radius=False):
    from pptx.enum.shapes import MSO_SHAPE
    shape = slide.shapes.add_shape(
        MSO_SHAPE.ROUNDED_RECTANGLE if radius else MSO_SHAPE.RECTANGLE,
        left, top, width, height)
    shape.fill.solid()
    shape.fill.fore_color.rgb = fill
    if line is None:
        shape.line.fill.background()
    else:
        shape.line.color.rgb = line
        shape.line.width = Pt(0.75)
    shape.shadow.inherit = False
    if radius:
        shape.adjustments[0] = 0.08
    return shape


def draw_background(prs, slide, number, total):
    add_rect(slide, 0, 0, W, H, BG)
    # thin top rule
    add_rect(slide, 0, 0, W, Emu(45720), ACCENT)
    # footer
    add_text(slide, Inches(0.7), H - Inches(0.55), Inches(8), Inches(0.3),
             "DataStructure Labs  ·  代码分析报告", 10, False, MUTED)
    add_text(slide, W - Inches(1.6), H - Inches(0.55), Inches(0.9), Inches(0.3),
             f"{number:02d} / {total:02d}", 10, False, MUTED, PP_ALIGN.RIGHT)


def style_cell(cell, text, size=12, bold=False, color=INK, fill=None,
               align=PP_ALIGN.LEFT):
    cell.text = ""
    tf = cell.text_frame
    tf.word_wrap = True
    cell.margin_left = Inches(0.08)
    cell.margin_right = Inches(0.08)
    cell.margin_top = Inches(0.04)
    cell.margin_bottom = Inches(0.04)
    cell.vertical_anchor = MSO_ANCHOR.MIDDLE
    p = tf.paragraphs[0]
    p.alignment = align
    r = p.add_run()
    r.text = str(text)
    set_font(r, size, bold, color)
    if fill is not None:
        cell.fill.solid()
        cell.fill.fore_color.rgb = fill
    else:
        cell.fill.background()


def add_table(slide, headers, rows, left, top, width, col_widths=None,
              font_size=12, header_size=12, row_h=Inches(0.38)):
    n_rows, n_cols = len(rows) + 1, len(headers)
    height = row_h * n_rows
    shape = slide.shapes.add_table(n_rows, n_cols, left, top, width, height)
    table = shape.table
    table.first_row = False
    table.horz_banding = False

    if col_widths:
        total = sum(col_widths)
        for i, cw in enumerate(col_widths):
            table.columns[i].width = Emu(int(width * cw / total))
    for r in range(n_rows):
        table.rows[r].height = row_h

    for c, head in enumerate(headers):
        style_cell(table.cell(0, c), head, header_size, True, WHITE, ACCENT)
    for r, row in enumerate(rows, start=1):
        fill = PANEL if r % 2 == 1 else WHITE
        for c in range(n_cols):
            val = row[c] if c < len(row) else ""
            style_cell(table.cell(r, c), val, font_size, False, INK, fill)
    return shape


def slide_title(prs, data):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    add_rect(slide, 0, 0, W, H, INK)
    add_rect(slide, 0, 0, Inches(0.18), H, ACCENT)
    add_text(slide, Inches(1.0), Inches(2.05), Inches(11), Inches(0.4),
             data.get("kicker", ""), 14, True, RGBColor(0x8A, 0xB4, 0xFF))
    add_text(slide, Inches(1.0), Inches(2.55), Inches(11), Inches(1.2),
             data["title"], 44, True, WHITE, line_spacing=1.1)
    if data.get("subtitle"):
        add_text(slide, Inches(1.0), Inches(3.75), Inches(10.5), Inches(0.9),
                 data["subtitle"], 18, False, RGBColor(0xC9, 0xD1, 0xDD))
    if data.get("meta"):
        add_text(slide, Inches(1.0), H - Inches(1.15), Inches(11), Inches(0.4),
                 data["meta"], 12, False, RGBColor(0x9A, 0xA3, 0xB2))
    return slide


def slide_section(prs, data, number, total):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    add_rect(slide, 0, 0, W, H, PANEL)
    add_rect(slide, 0, 0, Inches(0.14), H, ACCENT)
    add_text(slide, Inches(1.0), Inches(2.7), Inches(2.0), Inches(1.6),
             data.get("index", ""), 76, True, ACCENT_SOFT)
    add_text(slide, Inches(2.2), Inches(2.85), Inches(10), Inches(1.0),
             data["title"], 36, True, INK)
    if data.get("subtitle"):
        add_text(slide, Inches(2.2), Inches(3.85), Inches(9.6), Inches(0.8),
                 data["subtitle"], 16, False, MUTED)
    add_text(slide, W - Inches(1.6), H - Inches(0.55), Inches(0.9), Inches(0.3),
             f"{number:02d} / {total:02d}", 10, False, MUTED, PP_ALIGN.RIGHT)
    return slide


def slide_bullets(prs, data, number, total):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    draw_background(prs, slide, number, total)

    top = Inches(0.6)
    if data.get("kicker"):
        add_text(slide, Inches(0.7), top, Inches(10), Inches(0.3),
                 data["kicker"].upper(), 11, True, ACCENT)
        top += Inches(0.35)
    add_text(slide, Inches(0.7), top, Inches(11.9), Inches(0.75),
             data["title"], 28, True, INK)
    bullets = data.get("bullets", [])
    size = data.get("font_size", 16)
    step = [Inches(0.52) if isinstance(b, dict) and b.get("head") else Inches(0.62)
            for b in bullets]
    block_h = sum(step, Inches(0))
    area_top = top + Inches(0.95)
    area_bottom = H - (Inches(1.45) if data.get("note") else Inches(1.05))
    avail = area_bottom - area_top
    offset = int((avail - block_h) / 2)
    offset = min(offset, int(Inches(0.6)))
    y = area_top + (Emu(offset) if block_h < avail else Inches(0))
    idx = 0
    for item in bullets:
        if isinstance(item, dict):
            head, body = item.get("head", ""), item.get("body", "")
        else:
            head, body = "", str(item)
        add_rect(slide, Inches(0.75), y + Inches(0.11), Inches(0.09), Inches(0.09), ACCENT)
        if head:
            add_text(slide, Inches(1.05), y, Inches(2.15), Inches(0.4), head, size, True, INK)
            add_text(slide, Inches(3.2), y, Inches(9.3), Inches(0.4), body, size, False, INK)
        else:
            add_text(slide, Inches(1.05), y, Inches(11.4), Inches(0.4), body, size, False, INK)
        y += step[idx]
        idx += 1
    if data.get("note"):
        add_text(slide, Inches(1.05), H - Inches(1.25), Inches(11.2), Inches(0.55),
                 data["note"], 12, False, MUTED)
    return slide


def slide_table(prs, data, number, total):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    draw_background(prs, slide, number, total)

    top = Inches(0.6)
    if data.get("kicker"):
        add_text(slide, Inches(0.7), top, Inches(10), Inches(0.3),
                 data["kicker"].upper(), 11, True, ACCENT)
        top += Inches(0.35)
    add_text(slide, Inches(0.7), top, Inches(11.9), Inches(0.7),
             data["title"], 26, True, INK)

    rows = data["rows"]
    row_h = data.get("row_h", 0.38)
    if isinstance(row_h, (int, float)):
        row_h = Inches(row_h)
    block_h = Emu(row_h * (len(rows) + 1))
    area_top = top + Inches(0.95)
    area_bottom = H - (Inches(1.35) if data.get("note") else Inches(1.0))
    avail = area_bottom - area_top
    offset = int((avail - block_h) / 2)
    offset = min(offset, int(Inches(0.6)))
    table_top = area_top + (Emu(offset) if block_h < avail else Inches(0))
    add_table(slide, data["headers"], rows, Inches(0.7), table_top,
              Inches(11.9), data.get("col_widths"), data.get("font_size", 12),
              data.get("header_size", 12), row_h)
    if data.get("note"):
        add_text(slide, Inches(0.7), H - Inches(1.15), Inches(11.8), Inches(0.55),
                 data["note"], 12, False, MUTED)
    return slide


def slide_cards(prs, data, number, total):
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    draw_background(prs, slide, number, total)

    top = Inches(0.6)
    if data.get("kicker"):
        add_text(slide, Inches(0.7), top, Inches(10), Inches(0.3),
                 data["kicker"].upper(), 11, True, ACCENT)
        top += Inches(0.35)
    add_text(slide, Inches(0.7), top, Inches(11.9), Inches(0.7),
             data["title"], 26, True, INK)

    cards = data.get("cards", [])
    n = len(cards)
    gap = Inches(0.22)
    total_w = Inches(11.9)
    cw = Emu(int((total_w - gap * (n - 1)) / n))
    x = Inches(0.7)
    y = top + Inches(1.05)
    ch = Inches(3.8) if data.get("tall") else Inches(3.2)
    for card in cards:
        add_rect(slide, x, y, cw, ch, PANEL, LINE, radius=True)
        add_rect(slide, x, y, cw, Emu(45720), ACCENT)
        add_text(slide, x + Inches(0.25), y + Inches(0.28), cw - Inches(0.5), Inches(0.4),
                 card.get("head", ""), 16, True, ACCENT)
        add_text(slide, x + Inches(0.25), y + Inches(0.72), cw - Inches(0.5), Inches(0.5),
                 card.get("title", ""), 18, True, INK)
        body = card.get("body", "")
        add_text(slide, x + Inches(0.25), y + Inches(1.4), cw - Inches(0.5),
                 ch - Inches(1.6), body, data.get("font_size", 13), False, MUTED,
                 line_spacing=1.35)
        x = Emu(x + cw + gap)
    return slide


def main():
    src = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("slides.json")
    out = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("output.pptx")
    data = json.loads(src.read_text(encoding="utf-8-sig"))

    prs = Presentation()
    prs.slide_width, prs.slide_height = W, H

    slides = data["slides"]
    total = len(slides)
    for i, s in enumerate(slides, start=1):
        kind = s.get("type", "bullets")
        if kind == "title":
            slide_title(prs, s)
        elif kind == "section":
            slide_section(prs, s, i, total)
        elif kind == "table":
            slide_table(prs, s, i, total)
        elif kind == "cards":
            slide_cards(prs, s, i, total)
        else:
            slide_bullets(prs, s, i, total)

    out.parent.mkdir(parents=True, exist_ok=True)
    prs.save(out)
    print(f"saved {out} ({out.stat().st_size} bytes, {total} slides)")


if __name__ == "__main__":
    main()
