"""Adjust SC202CS IPA tuning data for M5Tab5 camera:
  - LSC corner gains +50% (fix vignetting)
  - CCM strength reduced to 85% (fix color cast)
  - Saturation increased (more vivid)
  - Low-light CCM now uses softened matrix instead of identity
"""
import json, math, copy

with open("sc202cs_default.json") as f:
    data = json.load(f)

acc = data["SC202CS"]["acc"]

# --- LSC: increase corner gains ---
for tbl in acc["lsc"]["table"]:
    for ch in ("calibrations_r_tbl", "calibrations_gr_tbl",
               "calibrations_gb_tbl", "calibrations_b_tbl"):
        arr = tbl[ch]
        h, w = 13, 21  # grid size
        for r in range(h):
            for c in range(w):
                idx = r * w + c
                v = arr[idx]
                # distance from center (normalised 0..1)
                dc = (c - (w-1)/2) / ((w-1)/2)
                dr = (r - (h-1)/2) / ((h-1)/2)
                d = math.sqrt(dc*dc + dr*dr)
                # boost: (v - 1.0) * (1.0 + 0.5 * d^2) + 1.0
                arr[idx] = (v - 1.0) * (1.0 + 0.50 * d * d) + 1.0

# --- CCM: reduce strength by blending towards identity ---
ALPHA = 0.85  # 1.0 = full correction, 0.0 = identity

def blend_ccm_3x3(m):
    """M' = I + alpha * (M - I) for 3x3 nested list"""
    return [
        [ (1.0 if i == j else 0.0) + ALPHA * (m[i][j] - (1.0 if i == j else 0.0))
          for j in range(3) ]
        for i in range(3)
    ]

def blend_ccm_flat(m):
    """M' = I + alpha * (M - I) for flat 9-element list"""
    out = []
    for i in range(3):
        for j in range(3):
            ident = 1.0 if i == j else 0.0
            out.append(ident + ALPHA * (m[i*3 + j] - ident))
    return out

# Low-luma CCM (flat format)
ccm_low = acc["ccm"]["low_luma"]
ccm_low["matrix"] = blend_ccm_flat(ccm_low["matrix"])

# Main CCM table
for entry in acc["ccm"]["table"]:
    entry["matrix"] = blend_ccm_3x3(entry["matrix"])

# --- BLC: slight black-level tweak ---
for blc in acc["blc"]["blc_table"]:
    blc["blc_param"]["blc_top_left"] = 20
    blc["blc_param"]["blc_top_right"] = 20
    blc["blc_param"]["blc_bottom_left"] = 20
    blc["blc_param"]["blc_bottom_right"] = 20

# --- Saturation: more vivid ---
for s in acc["saturation"]:
    s["value"] = min(255, int(s["value"] * 1.20))

with open("sc202cs_tuned.json", "w") as f:
    json.dump(data, f, indent=4)

print("Tuned JSON written to sc202cs_tuned.json")
print("Now run: esp_ipa/tools/config/esp_ipa_config.py -i sc202cs_tuned.json -o sc202cs_ipa_config.c")
