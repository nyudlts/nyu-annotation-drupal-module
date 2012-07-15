<?php

/**
 * Configure Annotation form
 */
function annotation_settings($form, &$form_state) {
  
  $form['service'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service settings'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );

  $form['service']['annotation_environment_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server URL'),
    '#default_value' => variable_get( 'annotation_environment_url', '' ),
    '#size' => 60,
    '#maxlength' => 256,
    '#description' => t('URL that identifies the Annotation server and request path to be used. E.g. http://host.bobst.nyu.edu'),
    '#required' => TRUE,
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );
  
  /*
   * Visibility settings
   */
  $form['visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility'),
    '#group' => 'settings',
  );
  
  $types = node_type_get_types();
  $options = array();
  
  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }
  
  $form['visibility']['annotation_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Apply comments to only the following node types.'),
    '#default_value' => variable_get('annotation_node_types', array()),
    '#options' => $options,
  );
  
  $form['visibility']['annotation_location'] = array(
    '#type' => 'select',
    '#title' => t('Location'),
    '#description' => t('Display the Annotation annotations in the given location. When "Block" is selected, the comments will appear in the <a href="@annotationannotations">Annotation annotations block</a>.', array('@annotationannotations' => url('admin/structure/block'))),
    '#default_value' => variable_get('annotation_location', 'content_area'),
    '#options' => array(
      'content_area' => t('Content Area'),
      'block' => t('Block'),
    ),
  );
  
  $form['visibility']['annotation_weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    '#description' => t('When the annotations are displayed in the content area, you can change the position at which they will be shown.'),
    '#default_value' => variable_get('annotation_weight', 50),
    '#options' => drupal_map_assoc(array(-100, -75, -50, -25, 0, 25, 50, 75, 100)),
    '#states' => array(
      'visible' => array(
        'select[name="annotation_location"]' => array('value' => 'content_area'),
      ),
    ),
  );
  
  /*
   * Behavior settings
   */
  $form['behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Behavior'),
    '#group' => 'settings',
  );
  
  $form['behavior']['annotation_userapikey'] = array(
    '#type' => 'textfield',
    '#title' => t('User API Key'),
    '#description' => t('The API key of the administrator account on Annotation. You can get yours <a href="@key">here</a>.', array('@key' => 'http://annotation.nyu.edu/api/get_my_key/')),
    '#default_value' => variable_get('annotation_userapikey', ''),
  );
  
  $form['behavior']['annotation_inherit_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inherit User Credentials'),
    '#description' => t("When enabled and a user is logged in, the Annotation 'Post as Guest' login form will be pre-filled with the user's name and email address."),
    '#default_value' => variable_get('annotation_inherit_login', TRUE),
  );
  $form['behavior']['annotation_developer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Testing'),
    '#description' => t('When enabled, uses the <a href="http://annotation.nyu.edu/docs/help/">annotation_developer</a> flag to tell Annotation that you are in a testing environment. Threads will not display on the public community page with this set.'),
    '#default_value' => variable_get('annotation_developer', FALSE),
  );
  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#group' => 'settings',
    '#description' => t('Use these settings to configure the more advanced uses of Annotation. You can find more information about these in the <a href="@applications">Applications</a> section of Annotation. To enable some of these features, you will require a <a href="@addons">Annotation Add-on Package</a>.', array(
      '@applications' => 'http://annotation.nyu.edu/api/applications/',
      '@addons' => 'http://annotation.nyu.edu/addons/',
    )),
  );
  $form['advanced']['annotation_publickey'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Key'),
    '#default_value' => variable_get('annotation_publickey', ''),
  );
  $form['advanced']['annotation_secretkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('annotation_secretkey', ''),
  );
  $form['advanced']['dannotation_sso'] = array(
    '#type' => 'checkbox',
    '#title' => t('Single Sign-On'),
    '#description' => t('Provide <a href="@sso">Single Sign-On</a> access to your site.', array(
      '@sso' => 'http://annotation.nyu.edu/api/sso/',
    )),
    '#default_value' => variable_get('annotation_sso', FALSE),
    '#states' => array(
      'visible' => array(
        'input[name="annotation_publickey"]' => array('empty' => FALSE),
        'input[name="annotation_secretkey"]' => array('empty' => FALSE),
      ),
    ),
  );
  
  return system_settings_form($form);
}

function annotation_test_submit($form, &$form_state) {
  $ping = annotation_server_status($form_state['values']['url'], $form_state['values']['service_class']);
  if ($ping) {
    drupal_set_message(t('Your site has contacted the Annotation server.'));
  }
  else {
    drupal_set_message(t('Your site was unable to contact the Annotation server.'), 'error');
  }
  $form_state['rebuild'] = TRUE;
}

/**
* Menu callback; Automatically closes the window after the user logs in.
*/
function annotation_closewindow() {
  drupal_add_js('window.close();', 'inline');
  return t('Thank you for logging in. Please close this window, or <a href="@clickhere">click here</a> to continue.', array('@clickhere' => 'javascript:window.close();'));
}