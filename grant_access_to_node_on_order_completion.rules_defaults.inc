<?php
/**
 * @file
 * grant_access_to_node_on_order_completion.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function grant_access_to_node_on_order_completion_default_rules_configuration() {
  $items = array();
  $items['rules_grant_access_to_node_if_product_points_to_one'] = entity_import('rules_config', '{ "rules_grant_access_to_node_if_product_points_to_one" : {
      "LABEL" : "Grant access to node if product points to one",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "content_access_rules" ],
      "USES VARIABLES" : { "line_item" : { "label" : "Line Item", "type" : "commerce_line_item" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "line-item" ], "field" : "commerce_product" } },
        { "entity_has_field" : {
            "entity" : [ "line-item:commerce-product" ],
            "field" : "field_class_refernce"
          }
        }
      ],
      "DO" : [
        { "content_access_action_user_grant" : {
            "node" : [ "line-item:commerce-product:field-class-refernce" ],
            "content_access_user_view" : [ "line-item:order:owner" ],
            "content_access_user_update" : [ "" ],
            "content_access_user_delete" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_grant_access_to_node_on_order_completion'] = entity_import('rules_config', '{ "rules_grant_access_to_node_on_order_completion" : {
      "LABEL" : "Grant access to node on order completion",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "entity" ],
      "ON" : { "commerce_order_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:state" ], "value" : "completed" } }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "commerce-order:commerce-line-items" ] },
            "ITEM" : { "line_item" : "Line item" },
            "DO" : [
              { "component_rules_grant_access_to_node_if_product_points_to_one" : { "line_item" : [ "line_item" ] } }
            ]
          }
        },
        { "drupal_message" : { "message" : "Access granted" } }
      ]
    }
  }');
  return $items;
}
