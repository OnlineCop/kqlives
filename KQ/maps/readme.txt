Map Editor for KQ

In order to use the map editor, Mapdraw, you will need to compile the source.  To run the binary, you need to change to the maps/ directory and invoke Mapdraw.  (Mapdraw does not currently support command-line arguments.)

You will see a level editor with map stats on the bottom and tiles on the right.  There is no current GUI menus, so you will have to make use of the keyboard shortcuts (hit F1 to see a list of possible keys).

To load a previously saved map, press F2.  It will prompt you for the map name.  You will need to supply Mapdraw with the path and/or filename of the .MAP file.

The maps used in KQ have 3 layers:
- Layer 1 is the background layer.
- Layer 2 is the middle layer.  Some maps will draw the middle layer above the Player's sprite.  Most, however, will simply draw over the background but under the Player.  This includes dishes on a table background tile, shrubs covering an item on the ground, etc.
- Layer 3 is the foreground layer.  This is drawn over the top of the Player's sprite, such as tall bookshelves, treetops, or doorways and ceilings that partially obfuscate the Player.

The maps also have 4 attributes saved on each individual map:
- Shadows are drawn over EVERYTHING.  This is a partially transparent layer where, when the Player is standing under it, part or all of their sprite will be lightened or darkened.  (Key: S)
- Obstacles are placed on the map wherever a sprite is not able to walk onto.  Be sure, when making or modifying a map, that all areas of the map are enclosed with an obstacle so the Player doesn't walk outide of the area s/he is supposed to stay within.  (Key: O)
  - The most common obstacle used is the SQUARE, where nothing can move onto the tile from any 4 directions.
  - The other types are the T-shaped obstacles.  These will block movement only in one direction, meaning you can enter the tile from 3 directions, but cannot move onto or off of the tile from the 4th.
- Entities are, for the most part, hard-coded onto the maps.  (Key: E)
  - You can modify their attributes such as initial direction they are facing, whether or not they face the Player when being talked to, their sprite (what they look like), etc. by pressing F12.
- Zones are used in the Lua files (in the scripts/ directory) to trigger a response when a player walks over the specified tile.

You can draw to the individual layers by selecting Layer 1, 2, or 3 (Key: 1, 2, or 3).  If you want to view more than one layer (as indicated in the F1 help screen), press 4-7.

Attributes are toggled.  That means that if you select Shadows, for example (Key: S), you will be in draw mode for all shadows on the map, and no other layer is affected.

The + and - keys (either by the backspace key or on the number pad) are used to choose which tile (they are on the right-side menu) or Attribute (select between different Shadows or Obstacles, for example) you will draw to the map.

There are other key shortcuts as well, but you can experiment with them with the F1 Help menu.


===============================
Keys:

NOTE: These are drawing modes, which allow you to modify the map
1   - Layer 1
2   - Layer 2
3   - Layer 3

NOTE: These are viewing modes, which won't modify the map
4   - Layers 1+2
5   - Layers 1+3
6   - Layers 2+3
7   - Layers 1+2+3

NOTE: Attributes can be "toggled".  You can only draw the Attribute to the
      map if it is showing AND it is the current drawing mode.  You can have
      multiple Attributes showing at once, but only one can be the current
      drawing mode at any given time.  Displaying Layers or toggling the
      Attribute again turns it off.
F11 - Toggle Entites Attribute
S   - Toggle Shadows Attribute
O   - Toggle Obstacles Attribute
Z   - Toggle Zone Attribute

C   - View Layers 1-3, plus Entities and Shadows Attributes
A   - Turn Layers 1-3, plus all four Attributes

T   - Block copy (left click starts, right click finishes block copy)
P   - Paste the copied block/tile (right-click pastes to user-specified map)

W   - Wipe the contents of the current map
D   - Displace the location of one or more Entities
E   - Empty the contents of the clipboard
R   - Resize the map's height and width (fixed the bug in this one!)
J   - Copy Layers 1-3 to a mini PCX image
F1  - Load a saved map
F2  - Create a new map
F3  - Save the current map
F4  - Erase a Layer from the map
F5  - Load a PCX image and convert it to a map
F6  - Change all the instances of one tile in a map to another
F7  - Remove all Obstructions from the map
F8  - Remove all Shadows from the map
F9  - Copy from one layer to another
F10 - Map description (what the player sees when entering a town, cave, etc.)
F12 - Entity Modification Mode
ESC - Stop Block copying

+/- keys:
If you are in Entities mode, selects an entity (a whole lot to choose from)
If you are in Shadows mode, selects a shadow type (11 to choose from)
If you are in Obstacles mode, selects an obstacle (5 to choose from)
If you are in Zones mode, select a zone (up to 255; this is what the LUA
 files refer to )
If you are in regular Layer 1-3 mode, select one of the icons from the
 right-hand menu

Up, Down, Left, Right: Move the view window around
PgUp, PgDn, Tab, Backspace, Home, End: Move more quickly

You can use the mouse to select a tile from the tile map in the right-hand
menu.  You can also change map options in the bottom menu.


===============================
The code is fairly well-commented, so everything should be self-explanitory.
These few key changes were implemented for ease-of-use, and keys are easily
redefined if they're undesirable.


===============================
Changes, updates:

* You can change the tileset by clicking on the " Icon: *** " PCX name and it
  shows the results immediately across the map.
* Function to select the tile under the mouse cursor (its icon in the tileset
  menu is updated to show which tile you've selected).
  * Added right-click tile-grabbing to "grab" the tile and then set the
    draw_mode to the current layer.
* Added help menu (F1).
* Added extra file open/save checks.
* Fixed multiple memory bugs and map-resize issues.


===============================
Known bugs:

* If the view window is at the bottom of the map when the mapsize is reduced, it will crash the editor.  Either don't be at the bottom of the map when you decrease its height, or don't shrink the map :-)


Questions and comments (donations, threats...) are of course welcome.

-TeamTerradactyl :: teamterradactyl@users.sourceforge.net
