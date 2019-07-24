<?php

/**
 * @file
 * Drush commands to add and remove users from site access list
 */

/**
 * Implements hook_drush_command().
 */
function isushibsiteaccess_drush_command() {
  $items = array();

  $items['shib-addusers'] = array(
    'description' => 'Add user(s) to the shibboleth site access list',
    'aliases' => array('shib'),
    'arguments' => array(
      'users' => 'List of users to add',
    ),
    'options' => array(
      'roles' => 'List of roles to include with each user',
    ),
    'examples' => array(
      'drush shib-addusers user1' => 'Adds user1 with no future roles',
      'drush shib-addusers user1 user2 --roles="content editor, content publisher"' => 'Adds user1 and user2 to the access list, giving them the future roles of content editor and content publisher',
    ),
  );

  $items['shib-rmusers'] = array(
    'description' => 'Remove user(s) to the shibboleth site access list',
    'arguments' => array(
      'users' => 'List of users to remove',
    ),
    'examples' => array(
      'drush shib-rmusers user1 user2' => 'Removes access of user1 and user2',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * @param string $section
 *   The help section.
 *
 * @return string
 *   The help text for the command.
 */
function isushibsiteaccess_drush_help($section) {
  switch ($section) {
    case 'drush:shib-addusers':{
      return 'Add user(s) to the shibboleth site access list.';
    }
    case 'drush:shib-rmusers':{
      return 'Remove user(s) from the shibboleth site access list.';
    }
  }
}


/**
 * Add user(s) to the access list
 */
function drush_isushibsiteaccess_shib_addusers() {
  $args = drush_get_arguments();
  $requested_roles = explode(',', drush_get_option('roles'));
  $module_name = "drush-addusers";
  $vetted_roles = isushibsiteaccess_get_vetted_list_of_roles($requested_roles, $module_name);

  foreach ($args as $name) {
    // First argument is the name of the command, i.e. shib-addusers or shib
    if ($name == "shib-addusers" || $name == "shib") {
      continue;
    }


    // Give user access
    isushibsiteaccess_add_name_to_list($name, $vetted_roles, $module_name);

  }
}

/**
 * Remove user(s) to the access list
 */
function drush_isushibsiteaccess_shib_rmusers() {
  $args = drush_get_arguments();

  foreach ($args as $name) {
    // First argument is the name of the command, i.e. shib-rmusers
    if ($name == "shib-rmusers") {
      continue;
    }

    isushibsiteaccess_revoke_user($name, 'drush-rmusers');
  }
}
