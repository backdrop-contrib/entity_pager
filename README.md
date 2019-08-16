Entity Pager
============

Provides next and previous navigation on entities.

You can add multiple different navigation blocks on the same entity.
This module combines Views, Blocks, Tokens and has its own internal intelligent
advice system with run fast with the minimum of processing.

It is simple to use, allowing you can create Navigation blocks in a few seconds.
For more complex pagers, it can combine with the Entity Reference, References,
and Taxonomy modules to create generic sub-navigation on related entities.

e.g. navigate through a Brands products or go back to the Brand page itself.

Video Demonstration
-------------------

[See demo](https://youtu.be/oJUcjKLz8ls). The video was recorded for Drupal 7 but is
still relevant for Backdrop CMS.

Installation
------------

Install this module using the official [Backdrop CMS instructions](https://backdropcms.org/guide/modules).

Instructions
------------

This module works on any Entity, but for simplicity these instructions will
walk you through adding an Entity Pager to a node.

Quick Instructions
------------------

1. Create a View that lists the IDs (nid) of the nodes you wish to be part of
your pager.
2. Select the Format type of the View as: Entity Pager.
3. Place the Views Block on the Node page. If the current Node has a nid that
is in the Views nid listing, then the Entity Pager module will automatically
display a Pager.

(note: don't worry about giving the module the nid of the current node, the
module automatically figures out all that stuff).

Detailed Instructions
---------------------

These instructions will walk you through adding an Entity Pager to a node.

1. Create a View.
   - i.e. Structure > Views > Add new view
   - note: you must have the Views UI module turned on to see the Views interface
2. Fill in the box: View name.
3. Click to create a block (not a page).
4. In the display format select: Entity Pager
5. For the box: Items per page
   - enter: 0
   - (note: entering 0 selects all items)
6. Leave the box: Use a pager
   - unselected.
7. Click the button: Continue & edit
8. In the Fields section, add the field:   Content: Nid
   - (note: to create a Pager on Nodes use the Nid field, for a Pager of Users
    the Uid field and so on)
9. Remove any other fields (e.g. Title) from the View, they are not needed and
will only unnecessarily make the database search more resource heavy on the
system. i.e. just add the one field to the View, the field nid.

(TIP: you can create your View however you like, but removing unneeded lookups
is the most efficient on system resources. You probable also don't need the
default sort criteria: Content: Post date (desc). Backdrop normally returns the
content in a sensible ID order without processing the Post date.

Instructions or other Entities
------------------------------

These instructions have been created for:  Nodes
To place an Entity Pager block for other Entities (e.g. Users) simply reread
the instructions above replacing the work 'node' with 'user' and instead
of 'nid' use 'uid'.

Instructions with Screenshots
-----------------------------

[See instructions with screenshots](http://www.jmokay.com/drupal/entity-pager-module).

Demonstration Block/View
------------------------

When turning on this module an Entity Pager demonstration View block is
created called: Entity Pager Example.

Enable this block in a region of your theme on a page with a node on. This
will display an Entity Pager block for nodes.

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

Maintainers
-----------

This module is seeking maintainers.

Credit
------

Ported to Backdrop CMS by Herb v/d Dool.

Originally written for Drupal by [James Lawson](https://www.drupal.org/u/jmokay).
