<?php

/**
 * @file
 * General setup base Class for Entity Pager module.
 */

/**
 * Class entity_pager.
 *
 * Provides a base Class setup for the entity_pager_out Class.
 * This Class contains house keeping functions for entity_pager.
 * entity_pager_out inherits this class and does the Business Logic.
 *
 * The main things that happening in this class:
 *
 * 1) the $view is used to automatically work out the Entity & Field of interest
 * e.g. it's a 'Node' and the 'nid' is the field of interest.
 * e.g. it's a 'User' and the 'uid' is the field of interest.
 *
 * 2) now the entity type and $field of interest is established, this
 * Entity record is pulled from the database.
 *
 * 3) the specific information of interest for the webpage is then gathered up
 * and made convenient to use & quick to access.  This is done so it is easy
 * to work with when constructing the Business Logic in the sub-class
 * entity_pager_out.
 */
class entity_pager {

  protected $default;

  public function __construct() {
    $this->setDefaults();
  }

  /**
   * Set the default values for Entity Pager.
   */
  protected function setDefaults() {

    $this->default = array(
      'link_next' => 'next >',
      'link_prev' => '< prev',
      'link_all_url' => '<front>',
      'link_all_text' => 'Home',
      'display_count' => TRUE,
      'log_performance' => TRUE,
    );
  }

  /**
   * Get Default value.
   *
   * Use this one method to get all default values in a consistent way.
   * If a field is specified return just that field value.
   * If no field is specified return the whole array.
   *
   * @param null $field
   *
   * @return bool|array|string
   */
  protected function getDefault($field = NULL) {

    $value = FALSE;
    if (is_null($field)) {
      $value = $this->default;
    }
    elseif (isset($this->default[$field])) {
      $value = $this->default[$field];
    }
    return $value;
  }

}
