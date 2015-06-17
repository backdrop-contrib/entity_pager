<?php

/**
 * @file
 * General setup base Class for Entity Pager module.
 */

//todo: sort this section.
/**
 * Class entity_pager.
 *
 * Provides a base Class setup for the entity_pager_out Class.
 *
 */
abstract class entity_pager_advice extends entity_pager {

  /**
   * Check performance
   * See if the option in the View is set to set to log performance.
   * see: View > Format > Settings > Log performance suggestions
   * @return bool
   */
  protected function check_performance(){

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
  protected function views_caching_is_used(){

    $view = $this->getView();
    $cache_value = $view->query->pager->display->handler->default_display
      ->display->display_options['cache']['type'];

    $cache_result = TRUE;
    if ($cache_value == 'none'){
      $cache_result = FALSE;
    }
    return $cache_result;
  }

  /**
   * Run advice logic.
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
          if ($this->views_caching_is_used()){
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
  protected function logAdvice($advice_type, $issue){

    $view = $this->getView();

    switch ($advice_type) {
      case 'no-records':
        watchdog('entity_pager', "Performance Advice: in the View ('@view_name'),
                the issue ('@reason'), means you are needlessly attempting to
                displaying an empty Entity Pager block on this page.
                If you want, you can turn this alert off see:
                @view_name view > Format > Settings > Log performance suggestions.",
          array('@view_name' => $view->human_name, '@reason' => $issue), WATCHDOG_INFO);
        break;
      case 'quantity':
        watchdog('entity_pager', "Performance Advice: in the View ('@view_name'),
                the issue ('@reason'), means you are needlessly attempting to
                displaying an empty Entity Pager block on this page.
                If you want, you can turn this alert off see:
                @view_name view > Format > Settings > Log performance suggestions.",
          array('@view_name' => $view->human_name, '@reason' => $issue), WATCHDOG_INFO);
    }


  }






}



