<div align="center">

<!-- Animated typing header -->
[![Typing SVG](https://readme-typing-svg.demolab.com?font=Fira+Code&size=28&duration=3000&pause=1000&color=FFB347&center=true&vCenter=true&width=700&lines=⌨️+Corne+ZMK+Config;42+Keys+%7C+6+Layers;Per-Key+RGB+%7C+Animations;QWERTY+%2B+Colemak-DH)](https://git.io/typing-svg)

<br/>

</div>

---

## 🎮 About This Config

**Corne 42-key split keyboard** with ZMK firmware featuring:
- **6 fully-featured layers** with per-key RGB color indicators
- **Smart layer toggling** — Colemak-DH accessible without disrupting other layers
- **Conditional layers** — hold two buttons to unlock special functions
- **Игровой макрос** — двойное нажатие F3 на слое Adjust выполняет `F3↓ D F3↑`; Shift+Backspace = Delete
- **Complete mouse control** — move, click, scroll on dedicated layer
- **RGB customization** — 6 distinct color schemes, brightness/hue controls

---

## 📊 Layer Overview

| # | Layer | Activation | RGB Color | Purpose |
|---|-------|-----------|-----------|---------|
| **0** | 🟣 QWERTY | Default | `#100010` Dark Purple | Typing base layout |
| **1** | 🟠 Colemak-DH | 4-key combo | `#FF6600` Orange | Ergonomic layout (alternative to QWERTY) |
| **2** | 🟢 Lower | Hold `MO2` | `#00FF00` Green | Numbers, media, arrows |
| **3** | 🔵 Raise | Hold `MO3` | `#0000FF` Blue | Symbols, Bluetooth |
| **4** | 🟣 Adjust | `MO2` + `MO3` | `#FF4400` (left) / `#00AAFF` (right) | Mouse, F-keys, RGB/display, ZMK Studio unlock |
| **5** | 🟣 RGB | `TO 5` | `#FF00FF` Magenta | RGB brightness/hue/effect controls |

---

## ⌨️ Layer Diagrams

### Layer 0 — 🟣 QWERTY (Default)

<div align="center">

![Layer 0](https://img.shields.io/badge/Layer-0%20QWERTY-100010?style=for-the-badge&logoColor=white&label=Default%20Base&labelColor=1a1a1a)

| TAB | Q | W | E | R | T | — | Y | U | I | O | P | BSPC/DEL |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | **A** | **S** | **D** | **F** | **G** | — | **H** | **J** | **K** | **L** | **;** | **'** |
| **LCTRL** | **Z** | **X** | **C** | **V** | **B** | — | **N** | **M** | **,** | **.** | **/** | **ESC** |
| | | | **LGUI** | **MO2** | **SPACE** | — | **ENTER** | **MO3** | **RALT** | | | |

</div>

---

### Layer 1 — 🟠 Colemak-DH (Alternate Base)

<div align="center">

![Layer 1](https://img.shields.io/badge/Activation-4--Key%20Combo-FF6600?style=for-the-badge&logo=lightning&logoColor=white&label=LGUI%20%2B%20MO2%20%2B%20MO3%20%2B%20RALT&labelColor=1a1a1a)

| TAB | Q | W | F | P | B | — | J | L | U | Y | ; | BSPC/DEL |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | **A** | **R** | **S** | **T** | **G** | — | **M** | **N** | **E** | **I** | **O** | **'** |
| **LCTRL** | **X** | **C** | **D** | **V** | **Z** | — | **K** | **H** | **,** | **.** | **/** | **ESC** |
| | | | **LGUI** | **MO2** | **SPACE** | — | **ENTER** | **MO3** | **RALT** | | | |

</div>

> **✨ Key difference**: Colemak-DH is optimized for finger rolling and reduces same-hand bigrams.
> Нижний ряд слева: `X C D V Z` (вариант Angle).

> **🚀 Important:** Switching to L1 does NOT break your access to L2/L3/L4!
> - `MO2` (LOWER) works **on top of** L1 base, just like L0
> - `MO3` (RAISE) works **on top of** L1 base  
> - `MO2` + `MO3` still activates L4 (Adjust) regardless of base layer

---

### Layer 2 — 🟢 Lower (Numbers, Media, Navigation)

<div align="center">

![Layer 2](https://img.shields.io/badge/Activation-Hold%20MO2-00FF00?style=for-the-badge&logoColor=white&label=Any%20Base%20Layer&labelColor=1a1a1a)

| TAB | 1 | 2 | 3 | 4 | 5 | — | 6 | 7 | 8 | 9 | 0 | BSPC/DEL |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | — | — | **◀◀** | **▶ ⏸** | **▶▶** | — | **←** | **↓** | **↑** | **→** | — | — |
| **LCTRL** | — | — | **🔊-** | **🔇** | **🔊+** | — | — | — | — | — | — | — |
| | | | **LGUI** | **TRANS** | **SPACE** | — | **ENTER** | **TRANS** | **RALT** | | | |

**Features:**
- Numpad on top row (1-0)
- Media controls: Previous/Play/Next
- Arrow keys (vim-style: ←↓↑→)
- Volume control: Decrease/Mute/Increase

</div>

---

### Layer 3 — 🔵 Raise (Symbols, Bluetooth)

<div align="center">

![Layer 3](https://img.shields.io/badge/Activation-Hold%20MO3-0000FF?style=for-the-badge&logoColor=white&label=Any%20Base%20Layer&labelColor=1a1a1a)

| TAB | ! | @ | # | $ | % | — | ^ | & | * | ( | ) | BSPC/DEL |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | — | — | — | — | — | — | **-** | **=** | **[** | **]** | **\\** | **`** |
| **LCTRL** | — | — | **📱◀** | **📱✕** | **📱▶** | — | **_** | **+** | **{** | **}** | **\|** | **~** |
| | | | **LGUI** | **TRANS** | **SPACE** | — | **ENTER** | **TRANS** | **RALT** | | | |

**Features:**
- All punctuation symbols
- Bluetooth management: Previous profile / Clear **current** profile / Next profile
- Right side: `- = [ ] \ `` ` and their Shift variants `_ + { } | ~` on the row below

</div>

---

### Layer 4 — 🟣 Adjust (Mouse, F-Keys, Display Control)

<div align="center">

![Layer 4](https://img.shields.io/badge/Activation-Conditional-FF4400?style=for-the-badge&logoColor=white&label=MO2%20%2B%20MO3&labelColor=1a1a1a)

| — | — | — | — | **RGB** | **L5** | — | **🖱L** | **🖱R** | — | — | — | **DSP** |
|---|---|---|---|------|-----|----|---------|---------|---|---|---|---|
| **F1** | **F2** | **F3 / ×2 = макрос** | **F4** | **F5** | **F6** | — | **←** | **↓** | **↑** | **→** | — | — |
| **F7** | **F8** | **F9** | **F10** | **F11** | **F12** | — | — | **⬇** | **⬆** | — | — | **UNLOCK** |
| | | | **LGUI** | **TRANS** | **SPACE** | — | **ENTER** | **TRANS** | **RALT** | | | |

**Features:**
- **F1–F12 keys** on left side
- **Mouse movement**: ← ↓ ↑ → (move cursor)
- **Mouse buttons**: 🖱L (left click), 🖱R (right click)
- **Scroll**: ⬆ ⬇ (vertical scrolling)
- **RGB** (top row, 5th key): toggle RGB on/off
- **DSP** (top row, rightmost key): toggle the OLED on/off (`&dsptog`, see below)
- **F3**: tap = `F3`, double tap = macro `F3↓ D F3↑`
- **UNLOCK**: unlock ZMK Studio (live keymap editor)

</div>

---

### Layer 5 — 🟣 RGB Controls (LED Customization)

<div align="center">

![Layer 5](https://img.shields.io/badge/Activation-Toggle%20TO5-FF00FF?style=for-the-badge&logoColor=white&label=RGB%20Controls&labelColor=1a1a1a)

| 🔆- | 🔆+ | — | — | **L0** | — | — | — | — | — | — | — | **UNLOCK** |
|-----|-----|---|---|-----|----|---|---|---|---|---|---|----------|
| **🌈H-** | **🌈H+** | **🎨S-** | **🎨S+** | — | — | — | — | — | — | — | — | — |
| **◀◀** | **🎨OFF** | **▶▶** | **⚡-** | **⚡+** | — | — | — | — | — | — | — | — |
| | | | — | — | — | — | — | — | — | | | |

**Controls:**
- **🔆-/+** — Brightness down / up
- **🌈H-/+** — Hue down / up (color wheel)
- **🎨S-/+** — Saturation down / up
- **🎨OFF** — Toggle RGB on/off
- **◀◀ / ▶▶** — Previous / Next effect
- **⚡-/+** — Effect speed down / up
- **L0** — Return to QWERTY (note: if you were on Colemak, you return to QWERTY)

</div>

---

## ✨ Unique Features Explained

### 1️⃣ Combo-Activated Colemak-DH Toggle

**The Problem:** Switching between QWERTY and Colemak usually requires a dedicated key. This wastes space on a 42-key board.

**The Solution:** A 4-key combo on the bottom row:

```zmk
combo_colemak_on {
    key-positions = <36 37 40 41>;  // LGUI, MO2, MO3, RALT
    bindings = <&to 1>;             // Jump to Colemak
    layers = <0>;                   // Only on QWERTY
    timeout-ms = <50>;
};

combo_colemak_off {
    key-positions = <36 37 40 41>;  // Same positions
    bindings = <&to 0>;             // Jump back to QWERTY
    layers = <1>;                   // Only on Colemak
    timeout-ms = <50>;
};
```

**How it works:**
- Press all 4 keys (`LGUI` + `MO2` + `MO3` + `RALT`) **simultaneously** (within 50 ms)
- On QWERTY (L0) → switches to Colemak (L1)
- On Colemak (L1) → switches back to QWERTY (L0)

**Why it's powerful:**
- **No accidental triggers** — 4 keys pressed at once is deliberate
- **Doesn't break other layers** — Lower/Raise/Adjust still work on L1
- **Stays out of the way** — uses bottom row modifiers you're already touching

---

### 2️⃣ Conditional Layers (FN_MO13 from QMK)

**The Problem:** You want a "super layer" that only activates when holding TWO specific buttons. No extra key needed.

**The Solution:**

```zmk
conditional_layers {
    tri_layer {
        if-layers = <2 3>;      // If LOWER (2) AND RAISE (3) held...
        then-layer = <4>;       // ...activate Adjust (4)
    };
};
```

**Behavior:**
- Hold `MO2` (LOWER) alone → Layer 2 activates
- Hold `MO3` (RAISE) alone → Layer 3 activates
- Hold `MO2` + `MO3` together → Layer 4 (Adjust) activates **instead**

**Why ZMK is better than QMK here:**
- No weird hacks or placeholder bindings
- Automatically activates when both conditions are met
- Clean, declarative syntax

---

### 3️⃣ Mod-Morph: Shift+Backspace = Delete

**The Problem:** You want Backspace to delete right (Delete key) when Shift is held, but you don't have space for both keys.

**The Solution:**

```zmk
bspc_del: backspace_delete {
    compatible = "zmk,behavior-mod-morph";
    #binding-cells = <0>;
    bindings = <&kp BSPC>, <&kp DEL>;
    mods = <(MOD_LSFT|MOD_RSFT)>;  // Trigger on left or right Shift
};
```

**Behavior:** (used on the top-right key of layers 0–3)
- Normal tap: `Backspace`
- `Shift` + tap: `Delete` (deletes character to the right)



---

### 4️⃣ Tap Dance: F3 vs Game Macro (M1)

**The Problem:** You want F3 for one thing, but also a macro `F3 (hold) + D (release F3)` for a game. One key, two functions.

**The Solution:**

```zmk
macros {
    macro_m1: macro_m1 {
        compatible = "zmk,behavior-macro";
        bindings =
            <&macro_press>, <&kp F3>,    // Hold F3
            <&macro_tap>, <&kp D>,       // Tap D
            <&macro_release>, <&kp F3>; // Release F3
    };
};

td_f3_m1: tap_dance_f3_m1 {
    compatible = "zmk,behavior-tap-dance";
    bindings = <&kp F3>, <&macro_m1>;
    tapping-term-ms = <180>;  // 180ms to distinguish tap vs double-tap
};
```

**Behavior:**
- Single tap: `F3` (activates a menu/mode in-game)
- Double tap (within 180ms): Macro `F3↓ D F3↑` (performs special action while holding F3)

Bound to the **F3** key on the Adjust layer (layer 4).

---

### 5️⃣ Mouse Control on Adjust Layer

**The Problem:** You want mouse control without external hardware.

**The Solution:** Dedicated mouse layer with movement, clicking, and scrolling:

```zmk
// Adjust layer (Layer 4)
&mmv MOVE_LEFT   // Move cursor left
&mmv MOVE_RIGHT  // Move cursor right
&mmv MOVE_UP     // Move cursor up
&mmv MOVE_DOWN   // Move cursor down

&mkp MB1         // Left click
&mkp MB2         // Right click

&msc SCRL_UP     // Scroll up
&msc SCRL_DOWN   // Scroll down
```

**Activation:** Hold `MO2` + `MO3` simultaneously. Move = keys under `H J K L`, buttons = top row (`Y` `U`), scroll = bottom row (`M` `,`).

**Speed control:** defined at the top of `config/corne.keymap`:
```zmk
#define ZMK_POINTING_DEFAULT_MOVE_VAL 1200  // Cursor speed
#define ZMK_POINTING_DEFAULT_SCRL_VAL 10    // Scroll speed
```

---

### 6️⃣ RGB Toggle and Display Toggle (Adjust layer)

- Top row, 5th key: `&rgb_ug RGB_TOG` — RGB on/off (RGB is **off** after power-up)
- Top row, rightmost key: `&dsptog` — blanks / unblanks the OLED on the half where it runs
  (both halves react, each keeps its own state). It sends the OLED's own "display off" command.
  The source is in `config/src/behavior_display_toggle.c` (a small Zephyr module that lives in this repo).
- There is deliberately **no** external-power (`EP_TOG`) key: its state is saved in flash, and a saved
  "off" cuts the OLED power right after boot (the screen blinks and dies). If the screens stay dark,
  flash `settings_reset.uf2` first and then the normal firmware.

---

### 8️⃣ OLED Screens (nice_oled)

| Half | Widget |
|------|--------|
| **Left** (central) | Responsive Bongo Cat (reacts to every key press), modifier symbols, BT profile number |
| **Right** (peripheral) | Gem animation, connection icon, battery |

- The small digit on the left screen is the **Bluetooth profile number (1–5)**, not the layer. It changes only
  when you switch profiles (`BT_NXT`/`BT_PRV` on the Raise layer). The current layer is shown by the RGB color.
- The screens use the `nice_oled` shield, taken from my fork
  [`NoFilterA1/zmk-nice-oled`](https://github.com/NoFilterA1/zmk-nice-oled) (branch `nzmk`). It is
  [`tokyo2006`'s LVGL 9 port](https://github.com/mctechnology17/zmk-nice-oled/pull/37) of
  [`mctechnology17/zmk-nice-oled`](https://github.com/mctechnology17/zmk-nice-oled) plus two small fixes:
  the `zmk_endpoints_selected()` name used by the `darknao/zmk` fork, and the responsive Bongo Cat frames.
- Widget positions are Kconfig options (`CONFIG_NICE_OLED_WIDGET_*_CUSTOM_X/Y` in `config/corne.conf`).
  The module was designed for a different canvas size, so some positions are tuned by hand.

---

### 7️⃣ Per-Key RGB by Layer (Visual Layer Indicator)

The keyboard shows **which layer you're on** by the color of the entire keyboard:

**RGB color mapping:**
- **L0 (QWERTY)** → `#100010` Dark Purple (base)
- **L1 (Colemak)** → `#FF6600` Orange
- **L2 (Lower)** → `#00FF00` Green (bright indicator)
- **L3 (Raise)** → `#0000FF` Blue
- **L4 (Adjust)** → `#FF4400` (left half) + `#00AAFF` (right half)
- **L5 (RGB)** → `#FF00FF` Magenta

**Why this matters:**
- No mental load checking layer status — look at keyboard color
- Physical feedback that you switched layers
- Useful in the dark or while gaming
- **RGB is OFF after power-up.** Turn it on with the RGB key on the Adjust layer (layer 4, 5th key of the top row)
- Effects/brightness can be changed on Layer 5 (RGB controls); layer colors themselves are set in `config/corne.keymap`

---

## 🎨 RGB Color Legend

```
┌─────────────────────────────────────────┐
│  Layer 0 (QWERTY)                       │
│  ██████████████████████████████████████ │  #100010 Dark Purple
│                                          │
│  Layer 1 (Colemak-DH)                   │
│  ██████████████████████████████████████ │  #FF6600 Orange
│                                          │
│  Layer 2 (Lower)                        │
│  ██████████████████████████████████████ │  #00FF00 Green
│                                          │
│  Layer 3 (Raise)                        │
│  ██████████████████████████████████████ │  #0000FF Blue
│                                          │
│  Layer 4 (Adjust) — left / right halves│
│  ██████████████████████████████████████ │  #FF4400 Orange
│  ██████████████████████████████████████ │  #00AAFF Sky Blue
│                                          │
│  Layer 5 (RGB Controls)                 │
│  ██████████████████████████████████████ │  #FF00FF Magenta
└─────────────────────────────────────────┘
```

---

## 🔧 Build & Flash

**Automatic (recommended):** every push to GitHub starts the *Build ZMK* workflow
(`.github/workflows/build.yml`). It reads `build.yaml` and builds both halves
(`corne_left nice_oled` with ZMK Studio, `corne_right nice_oled`).
Download the `firmware` artifact from the *Actions* tab.

**Flash:** double-tap reset on the nice!nano → it appears as a USB drive →
copy the matching `.uf2` (`corne_left…` / `corne_right…`) onto it. Repeat for the second half.

**Local build** (Docker, same image as CI; about 2 minutes after the first download):

```bash
mkdir ws && cp -r config ws/config && cd ws
docker run --rm -u $(id -u):$(id -g) -e HOME=/tmp -v "$PWD":/ws zmkfirmware/zmk-build-arm:stable bash -c '
  cd /ws && west init -l config && west update && west zephyr-export
  west build -p -s zmk/app -d out/left  -b nice_nano/nrf52840 -S studio-rpc-usb-uart -- \
    -DSHIELD="corne_left nice_oled" -DZMK_CONFIG=/ws/config -DCONFIG_ZMK_STUDIO=y
  west build -p -s zmk/app -d out/right -b nice_nano/nrf52840 -- \
    -DSHIELD="corne_right nice_oled" -DZMK_CONFIG=/ws/config'
# result: out/left/zephyr/zmk.uf2, out/right/zephyr/zmk.uf2
```

Use `-p` (pristine) every time: without it west silently reuses the old CMake cache and ignores a changed shield.

**Debug build** (left half over USB): add `-S zmk-usb-logging` (and e.g. `-DCONFIG_ZMK_LOG_LEVEL_DBG=y`), then read `/dev/ttyACM*`
(115200 baud). This is how the "dark OLED" problem was found.

**Reset saved settings:** build with `-DSHIELD=settings_reset` (empty `ZMK_CONFIG` dir) and flash that `.uf2` before the normal one.
It clears saved state such as external power and Bluetooth bonds (re-pair the keyboard afterwards).

Dependencies (`config/west.yml`) are pinned to exact commits: the `darknao/zmk` fork (`zmk-perkey_ug`, per-key RGB, Zephyr 4.1 / LVGL 9)
and the `nice_oled` fork described above.

---

## 🩺 Known issues

- **Bluetooth log error on layer change:** `bt_l2cap: Expecting 1 ref, got 0` appears in the debug log every time the central sends the layer state
  to the right half (the per-key RGB fork syncs layers). Not fixed; suspected to be related to rare freezes after idle.
- **Deep sleep is disabled** (`CONFIG_ZMK_SLEEP=n`) while that is investigated.
- **Third-party code:** the `nice_oled` port is not tested by its author on this OLED layout; positions may need further tuning.

---

## 🎮 Pro Tips

1. **Learn Colemak gradually:**
   - Start on QWERTY (L0) using Lower/Raise for numbers/symbols
   - Switch to Colemak (L1) when comfortable
   - Your muscle memory for L2/L3/L4 transfers perfectly

2. **Use Adjust for everything:**
   - Mouse speed is set by `ZMK_POINTING_DEFAULT_MOVE_VAL` at the top of the keymap
   - Need a function key? Adjust layer has F1–F12

3. **Game macros:**
   - Double-tap F3 on the Adjust layer runs the `F3↓ D F3↑` macro
   - Add more macros via new `macros {}` sections in keymap

4. **RGB customization:**
   - On the Adjust layer press the `L5` key to enter RGB Controls
   - Use `RGB_HUI` to cycle hue, `RGB_SAI` for saturation
   - Press `L0` (top row, 5th key) to return to QWERTY

---

## 📚 Resources

- **ZMK Documentation:** https://zmk.dev/
- **Corne PCB Guide:** https://github.com/foostan/crkbd
- **ZMK Behaviors:** https://zmk.dev/docs/behaviors/
- **RGB Controls:** https://zmk.dev/docs/features/underglow/

---

<div align="center">

**Built with ⌨️ for comfort, customization, and speed**

Questions? Check the ZMK Discord or open an issue.

</div>
