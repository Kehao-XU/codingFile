# -*- coding: utf-8 -*-
"""Rough layout check: estimate wrapped text height vs available space."""
import sys, math
from pptx import Presentation
from pptx.util import Emu

EMU_IN = 914400
SLIDE_H = 7.5


def em_width(text, size_pt):
    w = 0.0
    for ch in text:
        o = ord(ch)
        if o > 0x2E80:          # CJK / fullwidth
            w += 1.0
        elif ch in "ilI.,:;'|! ":
            w += 0.30
        else:
            w += 0.55
    return w * size_pt


def check(path):
    prs = Presentation(path)
    problems = []
    for idx, slide in enumerate(prs.slides, 1):
        for shp in slide.shapes:
            if not shp.has_table:
                continue
            tbl = shp.table
            top_in = shp.top / EMU_IN
            total_h = 0.0
            for r, row in enumerate(tbl.rows):
                need = 0.0
                for c, cell in enumerate(row.cells):
                    text = cell.text
                    if not text:
                        continue
                    size = 12
                    for p in cell.text_frame.paragraphs:
                        for run in p.runs:
                            if run.font.size:
                                size = run.font.size.pt
                    col_w = tbl.columns[c].width / EMU_IN
                    avail = (col_w - 0.16) * 72  # points, minus margins
                    lines = max(1, math.ceil(em_width(text, size) / avail))
                    need = max(need, lines * size * 1.22 / 72 + 0.10)
                total_h += max(need, row.height / EMU_IN)
            bottom = top_in + total_h
            if bottom > SLIDE_H - 0.45:
                problems.append(
                    f"slide {idx}: table bottom {bottom:.2f}in exceeds {SLIDE_H-0.45:.2f}in "
                    f"(top {top_in:.2f}, est height {total_h:.2f})")
        # bullets overflow
        for shp in slide.shapes:
            if not shp.has_text_frame:
                continue
            txt = shp.text_frame.text
            if len(txt) < 120:
                continue
            size = 16
            for p in shp.text_frame.paragraphs:
                for run in p.runs:
                    if run.font.size:
                        size = run.font.size.pt
            w_in = shp.width / EMU_IN
            lines = math.ceil(em_width(txt, size) / ((w_in - 0.05) * 72))
            h = lines * size * 1.35 / 72
            bottom = shp.top / EMU_IN + h
            if bottom > SLIDE_H - 0.35:
                problems.append(f"slide {idx}: textbox bottom {bottom:.2f}in (len {len(txt)})")
    return problems


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "output/DataStructure-Labs-分析报告.pptx"
    probs = check(path)
    if not probs:
        print("OK: no likely overflow detected")
    else:
        print("\n".join(probs))
