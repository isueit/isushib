<?php

/**
 * @file
 * Drush commands to add and remove users from site access list
 */

/**
 * Implements hook_drush_command()
 */
function isushibsiteaccess_drush_command() {
  $items = array();

  $items['shib-addusers'] = array(
    'description' => 'Add user(s) to the shibboleth site access list',
    'arguments' => array(
      'users' => 'List of users to add',
    ),
    'options' => array(
      'roles' => 'List of roles to include with each user',
    ),
    'examples' => array(
      'drush shib-addusers user1' => 'Adds user1 with no future roles',
      'drush shib-addusers user1 user2 --roles=site-editor,site-munger' => 'Adds user1 and user2 to the access list, giving them the future role of site-editor and site-munger',
    ),
  );

  $items['shib-rmusers'] = array(
    'description' => 'Remove user(s) to the shibboleth site access list',
    'arguments' => array(
      'users' => 'List of users to remove',
    ),
    'examples' => array(
      'drush shib-addusers user1 user2' => 'Removes access of user1 and user2',
    ),
  );

  return $items;
}

/**
 * Add user(s) to the access list
 */
function drush_isushibsiteaccess_shib_addusers() {
  $args = drush_get_arguments();
  $options = drush_get_option('roles');

  foreach ($args as $name) {
    // First argument is the name of the command, ie shib-addusers
    if ($name == "shib-addusers") {
      continue;
    }

    // If user_validate_name() returns anything it's a bad sign.
    if (user_validate_name($name)) {
      drush_log($name . " is not a valid username\n", 'error');
      continue;
    }

    // User will be added here

    // Roles will be added here
  }
}

/**
 * Remove user(s) to the access list
 */
function drush_isushibsiteaccess_shib_rmusers() {
  $args = drush_get_arguments();
}
