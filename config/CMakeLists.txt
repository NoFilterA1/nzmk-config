# Модуль в самом репозитории конфигурации: свои поведения и движок RGB
#   &dsptog  - вкл/выкл экран
#   &lflash  - короткая вспышка светодиодов (0 = красный, 1 = зелёный)
#   &lrgb    - кнопки RGB с удержанием (выполняется на левой, правая получает состояние через &lsync)
#   &lsync   - состояние RGB от левой к правой
#   &lbat    - пока зажата: каждая клавиша = блок батареи
#   &ugs     - цвет слоя = текущий SOLID (для режима LAY)
#   &ru_caps - Caps Word для русских слов (х ъ ж э б ю ё не обрывают и становятся заглавными)
#   &ugc     - фиксированный цвет слоя, краснеет при Caps
#   &en_caps/&ru_caps - Caps Word (сообщают состояние на экран и подсветку)
#   &snip    - текстовые сниппеты по Ctrl+Shift+Backspace + буква
#   lang_gesture.c - двойной аккорд Ctrl+Shift = Win+K (смена языка)
#   &tapsig  - сигнал "нажата клавиша на левой половине" для кота на правой
zephyr_library()
zephyr_include_directories(include) # nzmk/*.h видны и форку nice_oled
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY src/behavior_display_toggle.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_led_flash.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/nzmk_rgb.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_led_rgb.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_led_sync.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_led_battery.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_ug_solid.c)
zephyr_library_sources(src/behavior_tap_signal.c)
zephyr_library_sources(src/behavior_ru_caps.c)
zephyr_library_sources(src/behavior_en_caps.c)
zephyr_library_sources(src/behavior_snippet.c)
zephyr_library_sources(src/lang_gesture.c)
zephyr_library_sources(src/layer_latch.c)
zephyr_library_sources(src/nzmk_caps.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_ug_caps.c)
zephyr_library_sources(src/tap_signal_event.c)
