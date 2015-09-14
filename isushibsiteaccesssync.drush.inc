<?php

/**
 * @file
 * Make a drush command to sync users
 */

/**
 * Implements hook_drush_command().
 */
function isushibsiteaccesssync_drush_command() {
  $items = array();

  $items['sync-users'] = array(
    'description' => 'Sync access list with Active Directory group(s)',
    'callback' => 'isushibsiteaccesssync_run_drush_command',
  );

  return $items;
}

/**
 * Force the sync via drush command.
 */
function isushibsiteaccesssync_run_drush_command() {
  variable_set('isushibsiteaccesssync_day_of_year', 0);
  isushibsiteaccesssync_cron();
}

