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

  protected $entity;       // e.g. $node
  protected $view;         // the information returned from the View
  protected $entity_info;  // generic information about Entities.
  protected $outSettings;  // the information to be passed to the template.

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
   * Store the entity object. e.g. $node or $user etc..
   * @param $entity
   */
  public function setEntity($entity) {
    $this->entity = $entity;
  }

  /**
   * Get Entity.
   * Get the entity object. e.g. $node or $user etc..
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
   * @return mixed
   */
  public function getView() {
    return $this->view;
  }

  /**
   * Get Views Settings.
   * Get the values input in Views > Settings > Options from the View.
   * To allow the class to take advantage of dependency injection
   * the whole $view has been injected into the class. This
   * function uses the part of the view data that is to be consistently used
   * to get the options settings.
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
   * Form more information see comments about: getAllUrl()
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
   * Work out and store the field 'id' that is to be used.
   * @return mixed
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
   * Get ID Field Name.
   * Get the name of the Field that is to be used for the ID value.
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
   * Then store all the information that has been worked out so it can be
   * conveniently and efficiently used by this Class.
   * @param $view
   */
  protected function establishEntity($view){

    $entity_info = $this->establishEntityInfo($view);
    $entity = menu_get_object($entity_info['entity_type']);
    $this->setEntity($entity);
    $this->setEntityInfo($entity_info);
    $this->setNidFieldName();
  }

  /**
   * Establish the Entity Info from the View.
   * Use the View result to automatically establish the Entity type of interest.
   * e.g. node or user etc..
   * Then return an array of information about the entity type and how the View
   * is using it.
   * e.g
   *  entity_type = "user"
   *  field       = "uid"
   *  table       = "users"
   *  alias       = "uid"    // this what the View is calling the field.
   *
   * @param $view
   * @return mixed
   */
  function establishEntityInfo($view){

    $entity_info = NULL;
    $entity_keys = $this->getEntityKeys();

    $view_fields = $view->query->fields;
    // reverse order to get the last id e.g. nid or uid
    rsort($view_fields);
    foreach($view_fields as $view_field){

      if (in_array($view_field['field'], $entity_keys)){
        $entity_flip = array_flip($entity_keys);
        $entity_info = $view_field;
        $entity_info['entity_type'] = $entity_flip[$view_field['field']];
        break;
      }
    }
    return $entity_info;
  }

  /**
   * Get Entity Keys.
   * Get the key ID fields of each entity.
   *  e.g.
   *   node = nid
   *   user = uid
   * @return mixed
   */
  protected function getEntityKeys(){

    $entity_keys = array();
    $entities = entity_get_info();
    foreach($entities as $key => $value){
      $entity_keys[$key] = $value['entity keys']['id'];
    }
    return $entity_keys;
  }

  /**
   * Set Entity Information.
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
   * Get Entity Information.
   * Get generic information about the Entity Type.
   * If no field is declared it will return the complete array of Entity
   * Information. If a $field is entered only information on that field
   * is returned.
   *
   * @param null $field
   * @return bool | array $entity_info
   */
  public function getEntityInfo($field = NULL){

    $entity_info = FALSE;
    if (is_null($field)){
      $entity_info = $this->entity_info;          // the whole array
    }
    elseif (isset($this->entity_info[$field])) {
      $entity_info = $this->entity_info[$field];  // a specific field
    }
    return $entity_info;
  }
}