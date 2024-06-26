<?php

/**
 * @file
 * Definition of views_handler_relationship_node_term_data.
 */

/**
 * Relationship handler to return the taxonomy terms of nodes.
 *
 * @ingroup views_relationship_handlers
 */
class views_taxonomy_edge_2_handler_relationship_node_term_edge extends views_handler_relationship {
  function init(&$view, &$options) {
    parent::init($view, $options);

    // Convert legacy vids option to machine name vocabularies.
    if (!empty($this->options['vids'])) {
      $vocabularies = taxonomy_vocabulary_load_multiple();
      foreach ($this->options['vids'] as $vid) {
        if (isset($vocabularies[$vid]->machine_name)) {
          // Store the vocabulary machine name under vocabularies.
          $this->options['vocabularies'][$vocabularies[$vid]->machine_name] = $vocabularies[$vid]->machine_name;
        }
      }
      unset($this->options['vids']); // Remove the vids from options after conversion.
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['vocabularies'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $vocabularies = taxonomy_vocabulary_load_multiple();
    $options = array();
    foreach ($vocabularies as $voc) {
      $options[$voc->machine_name] = check_plain($voc->name);
    }

    $form['vocabularies'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vocabularies'],
      '#description' => t('Choose which vocabularies you wish to relate. Remember that every term found will create a new record, so this relationship is best used on just one vocabulary that has only one term per node.'),
    );
    parent::options_form($form, $form_state);
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = 'taxonomy_term_edge';

    if (!array_filter($this->options['vocabularies'])) {
      $taxonomy_index = $this->query->add_table('taxonomy_index', $this->relationship);
      $def['left_table'] = $taxonomy_index;
      $def['left_field'] = 'tid';
      $def['field'] = 'tid';
      $def['type'] = empty($this->options['required']) ? 'LEFT' : 'INNER';
    }
    else {
      // If vocabularies are supplied join a subselect instead
      $def['left_table'] = $this->table_alias;
      $def['left_field'] = 'nid';
      $def['field'] = 'nid';
      $def['type'] = empty($this->options['required']) ? 'LEFT' : 'INNER';

      $query = db_select('taxonomy_term_edge', 'td');
      $query->addJoin($def['type'], 'taxonomy_vocabulary', 'tv', 'td.vocabulary = tv.vocabulary');
      $query->addJoin($def['type'], 'taxonomy_index', 'tn', 'tn.tid = td.' . $this->real_field);
      $query->condition('tv.machine_name', array_filter($this->options['vocabularies']));
      $query->addTag('term_access');
      $query->fields('td');
      $query->fields('tn', array('nid'));
      $def['table formula'] = $query;
    }

    $join = new views_join();

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, 'taxonomy_term_edge', $this->relationship);
  }
}
