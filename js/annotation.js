/**
 * @file
 * JavaScript for the Annotation Drupal module.
 */

(function ($) {

  /**
   * Drupal Disqus behavior.
   */
  Drupal.behaviors.annotation = {
    attach: function (context, settings) {
	  
	  if ( window.console || false ) console.log('Annotation');
	  
      $('body').once('annotation', function() {
        if (settings.annotation || false) {
        }
      });
    }
  };
})(jQuery);