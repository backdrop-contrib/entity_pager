<?php

/**
 * @file
 * Contains the Entity pager style plugin.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_entity_pager extends views_plugin_style {

  /**
   * Render the display in this style.
   */
  function render() {
    if ($this->uses_row_plugin() && empty($this->row_plugin)) {
      debug('views_plugin_style_default: Missing row plugin');
      return;
    }

    $EntPag = new EntityPager($this->view);
    if ($links = $EntPag->getEntityPager()) {
      $variables['links'] = $links;
      return theme('entity_pager', $variables);
    };
  }

//  /**
//   * Get Entity Choices
//   *
//   * Creat an array of entity choices for a Form API ratio button.
//   * @return array
//   */
//  function getEntityChoices(){
//    $entities = entity_get_info();
//
//    $choices = array();
//    foreach($entities as $entity_name => $value){
//      $choices[$entity_name] = $entity_name;
//    }
//    return $choices;
//  }

  /**
   * Options form in Views for Entity pager.
   *
   * @param $form
   * @param $form_state
   */
  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    // setup an options form that
    $form['next_prev'] = array(
      '#type' => 'fieldset',
      '#title' => t('Next Previous links'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'next_prev',
      '#weight' => 0,
    );

    $form['next_prev']['link_next'] = array(
      '#title' => t('Next label'),
      '#description' => t('The text to display for the Next link.'),
      '#type' => 'textfield',
      '#default_value' =>
        (empty($this->options['next_prev']['link_next'])
          ? 'next >' : $this->options['next_prev']['link_next']),
    );

    $form['next_prev']['link_prev'] = array(
      '#title' => t('Previous label'),
      '#description' => t('The text to display for the Previous link.'),
      '#type' => 'textfield',
      '#default_value' =>
        (empty($this->options['next_prev']['link_prev'])
          ? '< prev' : $this->options['next_prev']['link_prev']),
    );

    $form['next_prev']['link_all_url'] = array(
      '#title' => t('List All URL'),
      '#description' =>
        t('The <strong>URL</strong> of the listing Link
          (e.g. a Views listing page) or you can use:
          <ul>
              <li><strong>@front</strong> for the <strong>homepage</strong></li>
              <li>a <strong>Node</strong>
              <a href="/admin/help/token"><strong>Token</strong></a>. The node
              token can also be an <strong>entity reference</strong> if the node
              has one. </br> e.g. [node:field_company]
          </ul>', array('@front' => '<front>')),
      '#type' => 'textfield',
      '#default_value' =>
        (empty($this->options['next_prev']['link_all_url'])
          ? '<front>' : $this->options['next_prev']['link_all_url']),
    );

    $form['next_prev']['link_all_text'] = array(
      '#title' => t('List All label'),
      '#description' =>
        t("The <strong>text</strong> to display for the <strong>List All URL
          </strong>.
          <ul>
              <li>When an <strong>entity reference</strong> is used in
              the <strong>List All URL</strong> box above, just add the same
              entity reference in this box and the referenced
              <strong>Node Title</strong> will automatically be displayed.
              </li>
          </ul>"
        ),
      '#type' => 'textfield',
      '#default_value' =>
        (empty($this->options['next_prev']['link_all_text'])
          ? 'Home' : $this->options['next_prev']['link_all_text']),
    );

    $form['next_prev']['display_count'] = array(
      '#title' => t('Display count'),
      '#description' => t('Display the number of records e.g. 5 of 8'),
      '#type' => 'checkbox',
      '#default_value' => (empty($this->options['next_prev']['display_count']))
        ? FALSE : $this->options['next_prev']['display_count'],
      '#weight' => 10,
    );
//
//    $form['next_prev']['entity_type'] = array(
//      '#type' => 'select',
//      '#title' => t('Entity type'),
//      '#default_value' => 'node',
//      '#options' => $this->getEntityChoices(),
//      '#description' =>
//        t('Please select the entity type of interest.'),
//      '#weight' => 20,
//    );

    $form['spacer_one'] = array(
      '#markup' => '&nbsp;',
    );

    // Remove unwanted inherited form elements
    $form['grouping'] = '';

    if ($this->uses_fields()) {
      $form['row_class'] = '';
    }
    $form['default_row_class'] = '';
    $form['row_class_special'] = '';
  }
}
