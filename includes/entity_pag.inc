<?php

/**
 * @file
 * General setup base Class for Entity pager module.
 */


/**
 * Class EntityPag.
 *
 * Provides a base Class setup for the EntityPager Class.
 * This Class contains house keeping functions for EntityPager.
 * EntityPager inherits this class and does the Business Logic.
 */
abstract class EntityPag {

  protected $entity;
  protected $result;
  protected $entity_info;
  protected $outSettings;

  /**
   * Setup Entity pager with minimum values to work with.
   * @param $view
   */
  public function __construct($view) {
    // set function to allow for dependency injection and coding best practices.
    $this->setView($view);
    $this->outSettings = new stdClass();
    $this->establishEntity($view);
  }

  /**
   * Generic 'Setter' for fields.
   * @param $field
   * @param $value
   */
  protected function setField($field, $value) {
    if (!isset($this->outSettings)) {
      $this->outSettings = new stdClass();
    }
    $this->outSettings->$field = $value;
  }

  /**
   * Generic 'Getter' for fields.
   * @param $field
   * @return bool
   */
  protected function getField($field) {

    if (isset($this->outSettings->$field)) {
      return $this->outSettings->$field;
    }
    return FALSE;
  }

  /**
   * Set Entity.
   * Store the entity object. e.g. $node
   * @param $entity
   */
  public function setEntity($entity) {
    $this->entity = $entity;
  }

  /**
   * Get Entity.
   * Get the entity object. e.g. $node
   * @return $entity
   */
  public function getEntity() {
    return $this->entity;
  }

  /**
   * Set View.
   * @param $view
   */
  public function setView($view) {
    $this->view = $view;
  }

  /**
   * Get View.
   * @param $view
   */
  public function getView() {
    return $this->view;
  }

  /**
   * Get Views Settings.
   * Get the values input in Views > Settings > Options from the View.
   * To allow the class to take advantage of dependency injection
   * the whole $view has been injected into the class. This
   * function uses the part of the view that is to be consistently used
   * to get the options settings that where selected in the View for this
   * Entity pager.
   * @param $view
   * @return mixed
   */
  public function getSettings($view) {
    return $view->style_plugin->options;
  }

  /**
   * Get View Result.
   * Get the part of the View object that contains the Views result data.
   * @param $view
   * @return mixed
   */
  public function getViewResult($view) {
    return $view->result;
  }

  /**
   * Get All URL.
   * This 'get' could have been pulled from the dynamic function, but
   * listing it's full name means IDE's will prompt (help) developers with
   * the possible 'get' choices. This is helpful for speedy development
   * when sharing code on Open Source projects.
   * @return mixed
   */
  public function getAllUrl() {
    return $this->getField('all_url');
  }

  /**
   * Get All Title.
   * @return mixed
   */
  public function getAllTitle() {
    return $this->getField('all_title');
  }

  /**
   * Set Link All Url.
   * This 'set' could have been left as a dynamic function
   * (i.e. setField(), but listing it's full name (i.e. setAllUrl() )
   * means developers IDEs will prompt (help) them with the possible
   * 'set' choices.  This IDE prompting makes it easy for other developers
   * to work with the code quickly and easily on this Open Source project.
   * @param $url
   */
  public function setAllUrl($url) {
    $this->setField('all_url', $url);
  }

  /**
   * Set All Title.
   * For more info see description for: function setAllUrl($url)
   * @param $title
   */
  public function setAllTitle($title) {
    $this->setField('all_title', $title);
  }

  /**
   * Get Fields
   * Get the fields returned from a View
   * @return mixed
   */
  public function getFields(){
    $view = $this->getView();
    return $view->query->pager->display->handler->handlers['field'];
  }

  /**
   * Set Field Name of ID field.
   * Work out which field 'id' is to be used.
   * @param $result
   * @return string
   */
  protected function setNidFieldName() {

    $this->getFields();

    if (isset($this->entity_info['alias'])){
      $field_name = $this->entity_info['alias'];
    } else
    {
      $field_name = $this->entity_info['field'];
    }
    $this->outSettings->id_field_name = $field_name;

    return $this->outSettings->id_field_name;
  }

  /**
   * Get Nid Field Name.
   * Get the name of the Field that is to be used for the nid value.
   *
   * @return bool
   */
  protected function getIdFieldName() {

    if (isset($this->outSettings->id_field_name)) {
      return $this->outSettings->id_field_name;
    }
    else {
      drupal_set_message(
        'Entity pager Module:  you need to select a nid in your View',
        'warning');
      return FALSE;  // this should never happen.
    }
  }

  /**
   * Establish Entity
   *
   * Use View result to establish the Entity type of interest.
   * i.e. a node entity or a user entity etc..
   * Then load that type of entity from the menu object.
   * Now store all the information that has been worked out so it can be
   * conveniently used by this Class.
   * @param $view
   */
  protected function establishEntity($view){

    $entity_info = $this->establishEntityDetails($view);
    $entity = menu_get_object($entity_info['entity_type']);
    $this->setEntity($entity);
    $this->setEntityInfo($entity_info);
    $this->setNidFieldName();
  }

  /**
   * Set the Entity Information
   *
   * This stores generic information about the Entity type.
   * e.g. the entity is a 'node' and its main id field is 'nid'
   * It is required to make the module universal, so it can process entities
   * as oppose to being hardcoded and only working for nodes.
   * @param $entity_info
   */
  protected function setEntityInfo($entity_info){
    $this->entity_info = $entity_info;
  }

  /**
   * Get Entity Information
   *
   * Get the information about the Entity type.
   * @param null $field
   * @return bool | $entity_info
   */
  public function getEntityInfo($field = null){

    $entity_info = FALSE;
    if (is_null($field)){
      // return whole array
      $entity_info = $this->entity_info;
    }
    elseif (isset($this->entity_info[$field])) {
      // return a specific field
      $entity_info = $this->entity_info[$field];
    }
    return $entity_info;
  }

  /**
   * Establish the Entity of interest from the View.
   *
   * Use the View result to automatically establish the Entity type of interest.
   * e.g. node or user etc..
   *
   * @param $view
   * @return mixed
   */
  function establishEntityDetails($view){

    $entity_keys = $this->getEntityKeys();

    $view_fields = $view->query->fields;
    // reverse order to get the last id e.g. nid or uid
    rsort($view_fields);
    foreach($view_fields as $view_field){

      if (in_array($view_field['field'], $entity_keys)){
        $entity_flip = array_flip($entity_keys);
        $id_field = $view_field;
        $id_field['entity_type'] = $entity_flip[$view_field['field']];
        break;
      }
    }

    return $id_field;

  }

  /**
   * Get Entity Key
   *
   * Get the Key field of an entity, it's main ID field.
   * e.g. node = nid, user = uid etc..
   * @return mixed
   */
  function getEntityKeys(){

    $entities = entity_get_info();
    foreach($entities as $key => $value){
      $entity_keys[$key] = $value['entity keys']['id'];
    }
    return $entity_keys;
  }

}