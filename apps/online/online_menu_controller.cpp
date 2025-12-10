#include "online_menu_controller.h"

#include "app.h"

namespace Online {

OnlineMenuController::OnlineMenuController(
    Escher::StackViewController* parentResponder,
    ParametersController* onlineController)
    : Escher::ListWithTopAndBottomController(parentResponder, &m_messageView),
      m_messageView(I18n::Message::ParameterChoose, k_messageFormat),
      m_parametersController(onlineController) {
  selectRow(0);
}

void OnlineMenuController::viewWillAppear() {
  int nRows = numberOfRows();
  for (int i = 0; i < nRows; i++) {
    m_cells[i].label()->setMessage(
        App::GetOnlineData()->labelForParameter(parameterAtIndex(i)));
    m_cells[i].subLabel()->setMessage(
        App::GetOnlineData()->sublabelForParameter(parameterAtIndex(i)));
  }
  m_selectableListView.reloadData(false);
  ViewController::viewWillAppear();
}

bool OnlineMenuController::handleEvent(Ion::Events::Event event) {
  // canBeActivatedByEvent can be called on any cell with chevron
  if (m_cells[0].canBeActivatedByEvent(event)) {
    App::GetOnlineData()->setUnknown(parameterAtIndex(innerSelectedRow()));
    stackOpenPage(m_parametersController);
    return true;
  }
  return popFromStackViewControllerOnLeftEvent(event);
}

const char* OnlineMenuController::title() const {
  return I18n::translate(App::GetOnlineData()->menuTitle());
}

int OnlineMenuController::numberOfRows() const {
  return App::GetOnlineData()->numberOfUnknowns();
}

KDCoordinate OnlineMenuController::nonMemoizedRowHeight(int row) {
  return protectedNonMemoizedRowHeight(&m_cells[row], row);
}

uint8_t OnlineMenuController::parameterAtIndex(int index) const {
  // Parameters are displayed in the same order as the enum order.
  assert(index >= 0 && index < App::GetOnlineData()->numberOfUnknowns());
  return index;
}

}  // namespace Online
