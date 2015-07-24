<?php

/**
 * @file
 * Provide views data for the Entity Pager module.
 */

/**
 * Implements hook_views_plugins().
 */
function entitypager_views_plugins() {
  $plugins = array(
    // Display, style, row, argument default, argument validator and access.
    'style' => array(
      'entitypager' => array(
        'title' => t('Entity Pager'),
        'help' => t('Displays extra information on a Entity such as Next and Previous links'),
        'handler' => 'views_plugin_style_entitypager',
        'theme' => 'views_view_table',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-table',
      ),
    ),
  );

  return $plugins;
}
