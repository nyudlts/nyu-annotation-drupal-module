<?php

/**
 * Prepares the noscript tag which is used when JavaScript is not available.
 *
 * @param $variables
 *   An array containing a "disqus" array, containing the following items:
 *     - "domain": The domain associated with this Disqus account.
 *     - "title": The title of the thread.
 *     - "developer": Whether or not testing is enabled.
 *     - "url": The disqus_url variable (http://disqus.com/docs/help/#faq-16).
 */
function theme_annotation_noscript($variables = array()) {
  $annotation = $variables['annotation'];
  
  /*
   * Return the comment markup.
   */ 
  return '<noscript><p>' . l( t('View the annotation thread.'), $annotation['environment'] . '/?url=' . urlencode($annotation['url']) ) . '</p></noscript>';
}