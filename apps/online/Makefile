SOURCES_apps += $(addprefix $(PATH_apps)/online/,$(addsuffix :-test, \
  app.cpp \
  interest_menu_controller.cpp \
  menu_controller.cpp \
  parameters_controller.cpp \
  result_controller.cpp \
  online_icon.png \
) \
  data.cpp \
  test/online.cpp:+test \
)

$(call depends_on_image,$(PATH_apps)/online/app.cpp,$(PATH_apps)/online/online_icon.png)

$(call i18n_with_universal,online/i18n/base)