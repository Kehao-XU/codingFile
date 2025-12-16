import numpy as np
import tiktoken
from datasets import load_dataset

enc = tiktoken.get_encoding("cl100k_base")

def tok_len(s: str) -> int:
    return len(enc.encode(s))

# 说明：
# FLORES-200 在 HuggingFace 上的语言代码需要你确认具体字段名
# 常见写法是 flores200 的 devtest，并用类似 "eng_Latn", "kaz_Cyrl", "uzn_Latn" 这样的代码
ds = load_dataset("facebook/flores", "all", split="devtest")  # 可能需要按实际数据集名调整

def avg_tps(lang_code: str, text_field: str = "sentence", n: int = 1000):
    rows = ds.filter(lambda x: x["lang"] == lang_code).select(range(n))
    lens = [tok_len(x[text_field]) for x in rows]
    return float(np.mean(lens))

tps_en  = avg_tps("eng_Latn")
tps_kaz = avg_tps("kaz_Cyrl")
tps_uzb = avg_tps("uzn_Latn")

rtc_kaz = tps_kaz / tps_en
rtc_uzb = tps_uzb / tps_en

print("TPS_en:", tps_en)
print("TPS_kaz:", tps_kaz, "RTC_kaz:", rtc_kaz)
print("TPS_uzb:", tps_uzb, "RTC_uzb:", rtc_uzb)