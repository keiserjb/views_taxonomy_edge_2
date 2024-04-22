<?php

/**
 * Implements hook_views_data().
 */
function views_taxonomy_edge_2_views_data() {
  $data = array();

  $data['taxonomy_term_edge']['table']['group'] = t('Taxonomy edge');
  $data['taxonomy_term_edge']['table']['join'] = array(
    'taxonomy_vocabulary' => array(
      'left_field' => 'vocabulary',
      'field' => 'vocabulary',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    'node' => array(
      'left_table' => 'taxonomy_index',
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    'taxonomy_index' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  $data['taxonomy_term_edge']['vid'] = array(
    'title' => t('Vocabulary'),
    'help' => t('Filter the results of "Taxonomy: Term" to a particular vocabulary.'),
    'filter' => array(
      'handler' => 'views_handler_filter_vocabulary_vid',
    ),
  );

  $data['taxonomy_term_edge']['parent'] = array(
    'argument' => array(
      'title' => t('Has taxonomy term ID or children'),
      'help' => t('Display content if it has the selected taxonomy terms, or children of the selected terms.'),
      'handler' => 'views_taxonomy_edge_2_handler_argument_term_edge_node_tid_depth',
      'name table' => 'taxonomy_term_data',
      'name field' => 'name',
      'empty field name' => t('Uncategorized'),
      'numeric' => TRUE,
      'accept depth modifier' => TRUE,
      'zero is null' => TRUE,
    ),
  );

  $data['taxonomy_term_edge']['tid'] = array(
    'argument' => array(
      'title' => t('Has taxonomy term ID or ancestor'),
      'help' => t('Display content if it has the selected taxonomy terms, or a parent of the selected terms.'),
      'handler' => 'views_taxonomy_edge_2_handler_argument_term_edge_node_tid_depth',
      'name table' => 'taxonomy_term_data',
      'name field' => 'name',
      'empty field name' => t('Uncategorized'),
      'numeric' => TRUE,
      'accept depth modifier' => TRUE,
    ),
  );

  $data['taxonomy_term_edge']['distance'] = array(
    'filter' => array(
      'title' => t('Taxonomy term depth'),
      'help' => t('Filter "Taxonomy: Term" based on distance from parent'),
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function views_taxonomy_edge_2_views_data_alter(&$data) {
  $data['node']['term_edge_node_parent'] = array(
    'title' => t('Taxonomy terms or term children on node'),
    'help' => t('Relate nodes to taxonomy terms, specifiying which vocabulary or vocabularies to use. This relationship will cause duplicated records if there are multiple terms.'),
    'relationship' => array(
      'handler' => 'views_taxonomy_edge_2_handler_relationship_node_term_edge',
      'label' => t('term'),
      'table' => 'taxonomy_term_edge',
      'real field' => 'parent',
      'base' => 'taxonomy_index',
      'base field' => 'tid',
      'skip base' => 'taxonomy_index',
    ),
  );

  $data['node']['term_edge_node_tid'] = array(
    'title' => t('Taxonomy terms or term parents on node'),
    'help' => t('Relate nodes to taxonomy terms, specifiying which vocabulary or vocabularies to use. This relationship will cause duplicated records if there are multiple terms.'),
    'relationship' => array(
      'handler' => 'views_taxonomy_edge_2_handler_relationship_node_term_edge',
      'label' => t('term'),
      'table' => 'taxonomy_term_edge',
      'real field' => 'tid',
      'base' => 'taxonomy_index',
      'base field' => 'tid',
      'skip base' => 'taxonomy_index',
    ),
  );
}

?>

