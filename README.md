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
- **Game-ready macros** — Tap Dance, Mod-morph, combo-activated shortcuts
- **Complete mouse control** — move, click, scroll on dedicated layer
- **RGB customization** — 6 distinct color schemes, brightness/hue controls

---

## 📊 Layer Overview

| # | Layer | Activation | RGB Color | Purpose |
|---|-------|-----------|-----------|---------|
| **0** | 🟣 QWERTY | Default | `#100010` Dark Purple | Typing base layout |
| **1** | 🟠 Colemak-DH | 4-key combo | `#FF6600` Orange | Ergonomic layout (Dvorak alternative) |
| **2** | 🟢 Lower | Hold `MO2` | `#00FF00` Green | Numbers, media, arrows |
| **3** | 🔵 Raise | Hold `MO3` | `#0000FF` Blue | Symbols, Bluetooth |
| **4** | 🟣 Adjust | `MO2` + `MO3` | `#FF4400 / #00AAFF` Mixed | Mouse, F-keys, studio unlock |
| **5** | 🟣 RGB | `TO 5` | `#FF00FF` Magenta | RGB brightness/hue/effect controls |

---

## ⌨️ Layer Diagrams

### Layer 0 — 🟣 QWERTY (Default)

<div align="center">

![Layer 0](https://img.shields.io/badge/Layer-0%20QWERTY-100010?style=for-the-badge&logoColor=white&label=Default%20Base&labelColor=1a1a1a)

| TAB | Q | W | E | R | T | — | Y | U | I | O | P | BSPC |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | **A** | **S** | **D** | **F** | **G** | — | **H** | **J** | **K** | **L** | **;** | **'** |
| **LCTRL** | **Z** | **X** | **C** | **V** | **B** | — | **N** | **M** | **,** | **.** | **/** | **ESC** |
| | | | **LGUI** | **MO2** | **SPACE** | — | **ENTER** | **MO3** | **RALT** | | | |

</div>

---

### Layer 1 — 🟠 Colemak-DH (Alternate Base)

<div align="center">

![Layer 1](https://img.shields.io/badge/Activation-4--Key%20Combo-FF6600?style=for-the-badge&logo=lightning&logoColor=white&label=LGUI%20%2B%20MO2%20%2B%20MO3%20%2B%20RALT&labelColor=1a1a1a)

| TAB | Q | W | F | P | B | — | J | L | U | Y | ; | BSPC |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | **A** | **R** | **S** | **T** | **G** | — | **M** | **N** | **E** | **I** | **O** | **'** |
| **LCTRL** | **X** | **C** | **D** | **V** | **Z** | — | **K** | **H** | **,** | **.** | **/** | **ESC** |
| | | | **LGUI** | **MO2** | **SPACE** | — | **ENTER** | **MO3** | **RALT** | | | |

</div>

> **✨ Key difference**: Colemak-DH is optimized for finger rolling and reduces same-hand bigrams.

> **🚀 Important:** Switching to L1 does NOT break your access to L2/L3/L4!
> - `MO2` (LOWER) works **on top of** L1 base, just like L0
> - `MO3` (RAISE) works **on top of** L1 base  
> - `MO2` + `MO3` still activates L4 (Adjust) regardless of base layer

---

### Layer 2 — 🟢 Lower (Numbers, Media, Navigation)

<div align="center">

![Layer 2](https://img.shields.io/badge/Activation-Hold%20MO2-00FF00?style=for-the-badge&logoColor=white&label=Any%20Base%20Layer&labelColor=1a1a1a)

| TAB | 1 | 2 | 3 | 4 | 5 | — | 6 | 7 | 8 | 9 | 0 | BSPC |
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

| TAB | ! | @ | # | $ | % | — | ^ | & | * | ( | ) | BSPC |
|-----|---|---|---|---|---|----|---|---|---|---|---|------|
| **LSHIFT** | — | — | — | — | — | — | **-** | **=** | **[** | **]** | **\\** | **`** |
| **LCTRL** | — | — | **📱◀** | **📱✕** | **📱▶** | — | **_** | **+** | **{** | **}** | **\|** | **~** |
| | | | **LGUI** | **TRANS** | **SPACE** | — | **ENTER** | **TRANS** | **RALT** | | | |

**Features:**
- All punctuation symbols
- Bluetooth management: Previous / Clear all / Next device
- Shift variants: `-` / `=` / `[` / `]` / `\` `` ` `` (using Shift)

</div>

---

### Layer 4 — 🟣 Adjust (Mouse, F-Keys, Display Control)

<div align="center">

![Layer 4](https://img.shields.io/badge/Activation-Conditional-FF4400?style=for-the-badge&logoColor=white&label=MO2%20%2B%20MO3&labelColor=1a1a1a)

| — | — | — | — | **RGB** | **L5** | — | **🖱L** | **🖱R** | — | — | — | — |
|---|---|---|---|------|-----|----|---------|---------|---|---|---|---|
| **F1** | **F2** | **F3** | **F4** | **F5** | **F6** | — | **←** | **↓** | **↑** | **→** | — | — |
| **F7** | **F8** | **F9** | **F10** | **F11** | **F12** | — | — | **⬇** | **⬆** | — | — | **UNLOCK** |
| | | | **LGUI** | **TRANS** | **SPACE** | — | **ENTER** | **TRANS** | **RALT** | | | |

**Features:**
- **F1–F12 keys** on left side
- **Mouse movement**: ← ↓ ↑ → (move cursor)
- **Mouse buttons**: 🖱L (left click), 🖱R (right click)
- **Scroll**: ⬆ ⬇ (vertical scrolling)
- **RGB_OR_EP** (top-left): 
  - Normal: toggle RGB effects
  - `RALT +` click: toggle display power
- **STUDIO_UNLOCK**: Zephyr Studio debugging

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
- **L0** — Return to default layer

</div>

---

## ✨ Unique Features Explained

### 1️⃣ Combo-Activated Colemak-DH Toggle

**The Problem:** Switching between QWERTY and Colemak usually requires a dedicated key. This wastes space on a 42-key board.

**The Solution:** A 4-key combo on the bottom row:

```zmk
combo_colemak_on {
    key-positions = <36 37 40 41>;  // LGUI, MO2, SPACE, MO3 area
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
- Press all 4 keys (`LGUI` + `MO2` + `SPACE` + `MO3`) **simultaneously**
- On QWERTY (L0) → switches to Colemak (L1)
- On Colemak (L1) → switches back to QWERTY (L0)
- Takes <50ms, no visible lag

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

**Behavior:**
- Normal tap: `Backspace`
- `Shift` + tap: `Delete` (deletes character to the right)

**No QMK-style hacks** — clean, built-in behavior.

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

**Real-world example:** In a game, single F3 opens debug menu, double F3 does power-move combo.

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

**Activation:** Hold `MO2` + `MO3` simultaneously.

**Speed control:** Defined at top of keymap:
```zmk
#define ZMK_POINTING_DEFAULT_MOVE_VAL 1200  // Cursor speed
#define ZMK_POINTING_DEFAULT_SCRL_VAL 10    // Scroll speed
```

---

### 6️⃣ RGB Toggle vs Display Power (Mod-Morph)

**The Problem:** You want to control both keyboard LEDs AND display power, but only one key for both.

**The Solution:**

```zmk
rgb_or_ep: rgb_or_ext_power {
    compatible = "zmk,behavior-mod-morph";
    bindings = <&rgb_ug RGB_TOG>,      // Normal: toggle RGB
               <&ext_power EP_TOG>;     // With RALT: toggle display
    mods = <(MOD_RALT)>;               // Trigger with right Alt
};
```

**Behavior on Adjust layer:**
- Press key: Toggle keyboard RGB effects on/off
- `RALT` + press key: Toggle display power (sleep/wake screen)

---

### 7️⃣ Per-Key RGB by Layer (Visual Layer Indicator)

The keyboard shows **which layer you're on** by the color of the entire keyboard:

**RGB color mapping:**
- **L0 (QWERTY)** → `#100010` Dark Purple (base)
- **L1 (Colemak)** → `#FF6600` Orange (clearly different!)
- **L2 (Lower)** → `#00FF00` Green (bright indicator)
- **L3 (Raise)** → `#0000FF` Blue
- **L4 (Adjust)** → `#FF4400` + `#00AAFF` (alternating rows, mixed)
- **L5 (RGB)** → `#FF00FF` Magenta

**Why this matters:**
- No mental load checking layer status — look at keyboard color
- Physical feedback that you switched layers
- Useful in the dark or while gaming
- Can be customized via Layer 5 (RGB controls)

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
│  Layer 4 (Adjust) — alternating rows   │
│  ██████████████████████████████████████ │  #FF4400 Orange
│  ██████████████████████████████████████ │  #00AAFF Sky Blue
│                                          │
│  Layer 5 (RGB Controls)                 │
│  ██████████████████████████████████████ │  #FF00FF Magenta
└─────────────────────────────────────────┘
```

---

## 🔧 Build & Flash

```bash
# Clone this repo (or it's already in your West workspace)
west update

# Build firmware
west build -b nice_nano_v2 -- -DSHIELD=corne_left

# Flash left half (run once per update)
west flash

# Then switch to right half and repeat:
west build -b nice_nano_v2 -- -DSHIELD=corne_right
west flash
```

**Note:** Uses GitHub Actions for automated builds. Check `.github/workflows/` for CI.

---

## 🎮 Pro Tips

1. **Learn Colemak gradually:**
   - Start on QWERTY (L0) using Lower/Raise for numbers/symbols
   - Switch to Colemak (L1) when comfortable
   - Your muscle memory for L2/L3/L4 transfers perfectly

2. **Use Adjust for everything:**
   - Mouse is slow? Speed it up in Layer 5 (RGB Controls → RGB_SPD)
   - Need a function key? Adjust layer has F1–F12

3. **Game macros:**
   - Tap Dance on Adjust layer is perfect for game combos
   - Add more macros via new `macros {}` sections in keymap

4. **RGB customization:**
   - Press `TO 5` to enter RGB Controls layer
   - Use `RGB_HUI` to cycle hue, `RGB_SAI` for saturation
   - Press key at top-left to return to base layer

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
