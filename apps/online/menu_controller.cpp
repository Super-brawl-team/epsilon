#include "menu_controller.h"

#include <apps/i18n.h>
#include <assert.h>
#include <escher/container.h>

#include "app.h"

using namespace Online;

MenuController::MenuController(Escher::StackViewController* parentResponder,
                               OnlineMenuController* onlineMenuController)
    : Escher::UniformSelectableListController<
          Escher::MenuCell<Escher::MessageTextView, Escher::MessageTextView,
                           Escher::ChevronView>,
          k_numberOfCells>(parentResponder),
      m_onlineMenuController(onlineMenuController) {
  selectRow(0);
  cell(k_indexOfSimpleOnline)
      ->label()
      ->setMessage(I18n::Message::SimpleOnline);
  cell(k_indexOfSimpleOnline)
      ->subLabel()
      ->setMessage(I18n::Message::SimpleOnlineDescription);
  cell(k_indexOfCompoundOnline)
      ->label()
      ->setMessage(I18n::Message::CompoundOnline);
  cell(k_indexOfCompoundOnline)
      ->subLabel()
      ->setMessage(I18n::Message::CompoundOnlineDescription);
  centerTable(Escher::Metric::DisplayHeightWithoutTitleBar);
}

void MenuController::handleResponderChainEvent(
    Responder::ResponderChainEvent event) {
  if (event.type == ResponderChainEventType::HasBecomeFirst) {
    m_selectableListView.reloadData();
  } else {
    Escher::UniformSelectableListController<
        Escher::MenuCell<Escher::MessageTextView, Escher::MessageTextView,
                         Escher::ChevronView>,
        k_numberOfCells>::handleResponderChainEvent(event);
  }
}

bool MenuController::handleEvent(Ion::Events::Event event) {
  // canBeActivatedByEvent can be called on any cell with chevron
  if (cell(0)->canBeActivatedByEvent(event)) {
    bool simpleOnlineRowSelected = (selectedRow() == k_indexOfSimpleOnline);
    assert(simpleOnlineRowSelected ||
           selectedRow() == k_indexOfCompoundOnline);

    // Set the online data model
    App::SetModel(simpleOnlineRowSelected);
    m_onlineMenuController->selectFirstCell();
    stackOpenPage(m_onlineMenuController);
    return true;
  }
  return false;
}
