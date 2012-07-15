<?php

/**
 * @file
 * Views integration with the Disqus module.
 */

/**
 * Implements hook_views_data_alter().
 */
function annotation_views_data_alter(&$data) {
 // Number of Annotation comments made on the given node.
  $data['node']['annotation_comment_count'] = array(
    'field' => array(
      'title' => t('Annotation Comment Count'),
      'help' => t('The number of Annotation annotations made on the post. Note that this will not work in the preview.'),
      'handler' => 'views_handler_field_node_annotation_annotation_count',
    ),
  );
}

/**
 * Field handler to present the number of Annotation annotations on a node.
 */
class views_handler_field_node_annotation_annotation_count extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  function query() {
    // Override parent::query() without altering query.
  }

  /**
   * When rendering the field.
   */
  function render($values) {
    // Ensure Annotation annotations are available on the node user has access to edit this node.
    $node = node_load($values->nid);
    if (user_access('view annotation annotations') && isset($node->annotation)) {
      // Extract the Annotation values.
      $annotation = $node->annotation;
      // Build a renderable array for the link.
      $content = array(
        '#theme' => 'link',
        '#text' => t('Annotations'),
        '#path' => $disqus['identifier'],
        '#options' => array(
          'fragment' => 'annotation_thread',
          'attributes' => array(
            // Identify the node for Annotation with the unique identifier:
            // http://docs.disqus.com/developers/universal/#comment-count
            'data-annotation-identifier' => $annotation['identifier'],
          ),
          'html' => FALSE,
        ),
      );

      /**
       * This attaches disqus.js, which will look for the DOM variable
       * disqusComments which is set below. When found, the disqus javascript
       * api replaces the html element with the attribute:
       * "data-disqus-identifier" and replaces the element with the number of
       * comments on the node.
       */
      $content['#attached'] = array(
        'js' => array(
          array('data' => drupal_get_path('module', 'annotation') . '/annotation.js'),
          array(
            'data' => array('annotationComments' => $disqus['domain']),
            'type' => 'setting',
          ),
        ),
      );
      return drupal_render($content);
    }
  }
}