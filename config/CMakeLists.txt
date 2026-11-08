# Модуль в самом репозитории конфигурации: свои поведения
#   &dsptog  - вкл/выкл экран
#   &lflash  - короткая вспышка светодиодов (0 = красный, 1 = зелёный)
zephyr_library()
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY src/behavior_display_toggle.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_RGB_UNDERGLOW src/behavior_led_flash.c)
