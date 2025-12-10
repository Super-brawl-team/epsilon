#ifndef ONLINE_MENU_CONTROLLER_H
#define ONLINE_MENU_CONTROLLER_H

#include <escher/chevron_view.h>
#include <escher/menu_cell.h>
#include <escher/message_text_view.h>
#include <escher/selectable_list_view_controller.h>
#include <escher/stack_view_controller.h>
#include <escher/view_controller.h>
#include <ion/events.h>

#include "online_menu_controller.h"

namespace Online {

class MenuController
    : public Escher::UniformSelectableListController<
          Escher::MenuCell<Escher::MessageTextView, Escher::MessageTextView,
                           Escher::ChevronView>,
          2> {
 public:
  MenuController(Escher::StackViewController* parentResponder,
                 OnlineMenuController* onlineMenuController);
  bool handleEvent(Ion::Events::Event e) override;

  constexpr static int k_indexOfSimpleOnline = 0;
  constexpr static int k_indexOfCompoundOnline = 1;

 protected:
  void handleResponderChainEvent(ResponderChainEvent event) override;

 private:
  OnlineMenuController* m_onlineMenuController;
};

}  // namespace Online

#endif
