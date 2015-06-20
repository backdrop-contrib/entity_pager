<?php

/**
 * @file
 * A class to manage advice when using the Entity Pager module.
 */

/**
 * Class entity_pager_advice.
 *
 * Provides a central and compact section of code where all performance advice
 * logic and messages are handled.
 */
abstract class entity_pager_advice extends entity_pager_sets {

  /**
   * Check performance
   * See if the option in the View is set to set to log performance.
   * see: View > Format > Settings > Log performance suggestions
   * @return bool
   */
  private function check_performance() {

    $check_performance = FALSE;
    $settings = $this->getSettings($this->getView());  // get Views settings.

    if ($settings['next_prev']['log_performance']) {
      $check_performance = TRUE;
    }
    return $check_performance;
  }

  /**
   * Get Cache status of View.
   * @return bool
   */
  private function views_caching_is_used() {

    $view = $this->getView();
    $cache_value = $view->query->pager->display->handler->default_display
      ->display->display_options['cache']['type'];

    $cache_result = TRUE;
    if ($cache_value == 'none') {
      $cache_result = FALSE;
    }
    return $cache_result;
  }

  /**
   * Run advice logic.
   * Run the logic that manages performance advice.
   * @param $advice_type
   * @param $issue
   */
  protected function run_advice_logic($advice_type, $issue) {

    if ($this->check_performance()) {
      // log performance suggestions.
      if ($advice_type == 'quantity') {
        // Coding note:  ideally nested 'if' statements should be avoided,
        // but the whole purpose of this little logic method is to sort
        // the logic in one manageable place. The if statements are simple
        // (single purpose) meaning they are easy to work down logically.
        if ($this->getViewResultCount() > 500) {
          if ($this->views_caching_is_used()) {
            // do nothing
          }
          else {
            $this->logAdvice($advice_type, $issue);
          }
        }
      }
      else {
        $this->logAdvice($advice_type, $issue);
      }
    }
  }

  /**
   * Set Performance Message.
   * @param $advice_type
   * @param $issue
   */
  private function logAdvice($advice_type, $issue) {

    $view = $this->getView();

    switch ($advice_type) {
      case 'no-records':
        watchdog('entity_pager', "Performance Advice: in the View ('@view_name'),
                the issue ('@reason'), means you are needlessly attempting to
                displaying an empty Entity Pager block on this page.
                To ignore this advice, you can turn this alert off at:
                @view_name view > Format > Settings > Log performance
                suggestions.",
          array(
            '@view_name' => $view->human_name,
            '@reason' => $issue
          ), WATCHDOG_INFO);
        break;
      case 'quantity':
        watchdog('entity_pager', "Performance Advice: in the View ('@view_name'),
                the issue ('@reason'). At the moment there are 500+ records in
                your pager. The Entity Pager module is constructed to be fast
                keeping processing to a minimum, that said, it might still be a
                good idea to cache the Views Query Result see:
                @view_name view > Other > Caching. (Note: do not cache the
                Rendered output). To ignore this advice, you can turn this
                alert off at: @view_name view > Format > Settings >
                Log performance suggestions.",
          array(
            '@view_name' => $view->human_name,
            '@reason' => $issue
          ), WATCHDOG_INFO);
    }
  }
}



