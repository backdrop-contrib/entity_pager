<?php

/**
 * @file
 * Provides the Business Logic for the entity_pager module.
 */

/**
 * Class entity_pager_out.
 *
 * The business logic class.
 *
 * The main class for the entity_pager module that does Business Logic.
 * For best practice Business Logic is kept separate from general Class
 * maintenance tasks. This class extents entity_pager which performs
 * general Class duties.
 */
class entity_pager_out extends entity_pager {

  /**
   * Entity Pager return links.
   *
   * The main method for returning links & counts etc.. for Entity Pager.
   * Use this to get Entity Pager links.
   *
   * @return array|bool|void
   */
  public function get_entity_pager() {

    $entity = $this->getEntity();
    $view = $this->getView();

    if (!$entity) {
      return FALSE;
    }

    $settings = $this->getSettings($view);
    $entity_type = $this->getEntityInfo('entity_type');

    // process to 'All' link.
    $this->token_link(
      $entity_type,
      $entity,
      $settings['next_prev']['link_all_url'],
      $settings['next_prev']['link_all_text']
    );

    // create links for 'Next', 'All' and 'Prev' values.
    $links = $this->calculate_next_all_prev_values_from_view($entity, $view);

    return $links;
  }

  /**
   * Process Token link.
   *
   * The 'All' link can have tokens set in it in the Views > settings form
   * 'Entity Pager' section.  This function processes the values added.
   * e.g. it can turn things like entity references into Titles with links.
   *
   * @param $entity_type
   * @param $entity
   * @param $url
   * @param $title
   */
  private function token_link($entity_type, $entity, $url, $title) {

    $detokened = array();
    $detokened['url'] = token_replace($url, array($entity_type => $entity));
    $detokened['title'] = token_replace($title, array($entity_type => $entity));

    if ($detokened['url'] == $url) {
      // no tokens used
      $new_url = $url;
    }
    elseif (drupal_valid_path($detokened['url'])) {
      // a valid url e.g. edit node
      $new_url = $detokened['url'];
    }
    else {
      // entity reference process
      $new_url = $this->entity_ref_url_from_token($entity_type, $entity, $url);
    }
    $this->setAllUrl($new_url);
    $this->setAllTitle($detokened['title']);
  }

  /**
   * Process Entity Reference tokens.
   *
   * If it is established the Tokens are Entity References, this method
   * converts them into useful information.
   * @param $entity_type
   * @param $entity
   * @param $link_with_token
   * @return bool|mixed|null|string
   */
  protected function entity_ref_url_from_token($entity_type, $entity, $link_with_token) {

    $link_all_url = '';

    // get URL of token
    // example token:  [node:field_company]
    $trimmed = trim($link_with_token, '[]');
    $token_in_url = explode(':', $trimmed);
    $field = $token_in_url[1];  // e.g. field_company

    $this->setField('nid_field', $field); // set the nid field

    // e.g. $node->field_company['und'][0]['entity']->nid;
    $field_long = $entity->$field;

    // e.g. $field_value = $field_value[LANGUAGE_NONE][0]['entity']->nid;
    $field_value = $field_long[LANGUAGE_NONE][0]['target_id'];

    if (isset($field_value) && is_numeric($field_value)) {
      $link_all_url = drupal_get_path_alias($entity_type . '/' . $field_value);
    }

    return $link_all_url;
  }

  /**
   * Calculate the values for the links.
   * e.g.
   *    < prev   All  next >
   *
   * @param $entity
   * @param $view
   * @return array|mixed|void
   */
  private function calculate_next_all_prev_values_from_view($entity, $view) {

    // extract the info required from the View.
    $settings = $this->getSettings($view); // info from Views > Format > settings
    $view_result = $this->getViewResult($view); // results of the view.
    // field
    $field_name = $this->getIdFieldName();
    // entity
    $entity_type = $this->getEntityInfo('entity_type');
    $entity_field = $this->getEntityInfo('field');
    // next or previous
    $next = FALSE;
    $match = FALSE;
    // set short, convenient incoming values to process below.
    $n = array(
      'cur' => '',
      'count' => count($view_result),
      'next' => $settings['next_prev']['link_next'],
      'prev' => $settings['next_prev']['link_prev'],
    );
    // set defaults for output.
    $links = array(
      'next' => $n['next'],
      'prev' => $n['prev'],
      'count' => '',
    );

    $cnt = $current_cnt = 0;

    foreach ($view_result as $result) {

      $cnt++;
      // keep a record of the previous nid in the loop, so when the nid is
      // matched, we can reference the previous nid to create the 'prev' link
      $n['cur_prev'] = $n['cur'];
      $n['cur'] = $result->$field_name; // the current value

      if ($next) {
        $links['next'] = l($n['next'], $entity_type . '/' . $result->$field_name, $options = array());
        break;
      }

      if ($entity->$entity_field == $result->$field_name) {
        // must be beyond the first record to have a previous
        $match = TRUE;
        $current_cnt = $cnt;
        $links['prev'] = $n['prev'];
        if ($cnt > 1) {
          $links['prev'] = l($n['prev'], $entity_type . '/' . $n['cur_prev'], $options = array());
        }
        $next = TRUE; // there is a next value
      }
    }

    if (!$match) {
      // there are no matches.
      if ($settings['next_prev']['log_performance']){
        // log performance suggestions.
        watchdog('entity_pager', 'Performance Advice:  You are needlessly
          attempting to displaying an empty Entity Pager block on this page.
          The Entity Pager block has a corresponding View and the results
          for this View do not contain the current entity (page).
          Note: if you want, you can turn this alert off in the View.',
          NULL, WATCHDOG_INFO);
      }
      return;  // return nothing.
    }

    // display the Count stats
    if ($settings['next_prev']['display_count']) {
      $links['count'] = t('@cnt of <span class="total">@count</span>',
        array(
          '@cnt' => number_format($current_cnt),
          '@count' => number_format($n["count"]),
        )
      );
    }

    // create the link for the 'all' link.
    $all_title = $this->getAllTitle();
    $all_url = $this->getAllUrl();
    if ($all_title && $all_url) {
      $links['all_link'] = l($all_title, $all_url, $options = array());
    }

    $links = $this->orderLinks($links);
    return $links;
  }

  /**
   * Order links
   *
   * Put links (and count) in the most appropriate order.
   * If extra links have been added from the 4 in the orders array below,
   * they will still appear in the links array.
   * @param $links
   * @return mixed
   */
  private function orderLinks($links) {
    // define the most usable array order.
    $orders = array('prev', 'all_link', 'next', 'count');
    foreach ($orders as $order) {
      if (isset($links[$order])) {
        $link = $links[$order]; // remove key => value link from middle of array
        unset($links[$order]); // add the key => value link to the end of  array
        $links[$order] = $link;
      }
    }
    return $links;
  }

}