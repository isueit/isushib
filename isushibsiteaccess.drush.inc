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
      'drush shib-addusers user1 user2' => 'Removes access of user1 and user2',
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
  $requested_roles = split(',', drush_get_option('roles'));
  $assignable_roles = isushibsiteaccess_available_roles();

  foreach ($args as $name) {
    // First argument is the name of the command, i.e. shib-addusers or shib
    if ($name == "shib-addusers" || $name == "shib") {
      continue;
    }

    // If user_validate_name() returns anything it's a bad sign.
    if (user_validate_name($name)) {
      drush_log($name . " is not a valid username", 'error');
      continue;
    }
    // Does user already exist in Drupal?
    $u = user_load_by_name($name);
    $status = $u ? t('current') : t('future');
    $user_exists = !empty($u);

    // Give user access
    $fuid = isushibsiteaccess_add_name_to_list($name, $u, 'drush-addusers');
    if ($fuid) {
      drush_log("Added " . $status . " user " . $name , 'success');
    }
    else {
      drush_log($name . " was already on the access list", 'warning');
    }

    // Add roles, if any were given
    foreach ($requested_roles as $role_name) {
      // Check for empty role name
      $role_name = trim($role_name);
      if (empty($role_name)) {
        continue;
      }

      // Check if it's an assignable rol and that the role actually exists
      if (in_array($role_name, $assignable_roles) && ($role = user_role_load_by_name($role_name))) {
        if ($user_exists) {
          // Assign role for current user
          user_multiple_role_edit(array($u->uid), 'add_role', $role->rid);
        }
        else {
          // Assign role for future user
          isushibsiteaccess_assign_role_to_future_user(isushibsiteaccess_get_fuid($name), $role->rid);
        }
        drush_print("Added role " . $role_name . " for " . $status . " user " . $name . ".");
      }
      else {
        drush_log($role_name . ' is not an assignable role.', 'warning');
      }
    }
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

    $fuid = isushibsiteaccess_get_fuid($name);
    if ($fuid) {
      isushibsiteaccess_revolk_user($name, isushibsiteaccess_get_fuid($name), 'drush-rmusers');
    }
    else {
      drush_log("User " . $name . " not on the access list", 'warning');
    }
  }
}
