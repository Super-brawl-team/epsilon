#ifndef ONLINE_APP_H
#define ONLINE_APP_H

#include <apps/i18n.h>
#include <apps/shared/math_app.h>
#include <escher/stack_view_controller.h>
#include <omg/ring_buffer.h>

#include "data.h"
#include "online_menu_controller.h"
#include "menu_controller.h"
#include "parameters_controller.h"
#include "result_controller.h"

namespace Online {

class App : public Shared::MathApp {
 public:
  // Descriptor
  class Descriptor : public Escher::App::Descriptor {
   public:
    I18n::Message name() const override { return I18n::Message::OnlineApp; }
    I18n::Message upperName() const override {
      return I18n::Message::OnlineAppCapital;
    }
    const Escher::Image* icon() const override;
  };

  // Snapshot
  class Snapshot : public Shared::SharedApp::Snapshot {
   public:
    /* At most 3 nested menus from MenuController :
     * OnlineMenuController, ParametersController and ResultController */
    constexpr static uint8_t k_maxDepth = 3;

    App* unpack(Escher::Container* container) override;
    const Descriptor* descriptor() const override;
    void reset() override;
    OMG::RingBuffer<Escher::ViewController*, k_maxDepth>* pageQueue() {
      return &m_pageQueue;
    }
    Data* data() { return &m_data; }

   private:
    OMG::RingBuffer<Escher::ViewController*, k_maxDepth> m_pageQueue;
    Data m_data;
  };

  static App* app() { return static_cast<App*>(Escher::App::app()); }
  static OnlineData* GetOnlineData() {
    return app()->snapshot()->data()->onlineData();
  }
  static void SetModel(bool selectedModel) {
    return app()->snapshot()->data()->setModel(selectedModel);
  }

  Snapshot* snapshot() const {
    return static_cast<Snapshot*>(Escher::App::snapshot());
  }

  void didBecomeActive(Escher::Window* window) override;

  // Navigation
  void willOpenPage(Escher::ViewController* controller) override;
  void didExitPage(Escher::ViewController* controller) override;

 private:
  App(Snapshot* snapshot);

  // Controllers
  ResultController m_resultController;
  ParametersController m_parametersController;
  OnlineMenuController m_onlineMenuController;
  MenuController m_menuController;
  Escher::StackViewController::Default m_stackViewController;
};

}  // namespace Online

#endif
