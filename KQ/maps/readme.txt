Mapdraw Info:

There have been a few changes to the map editor.

* Added entry-checking so too-large or too-small values aren't accepted.
* You can change the tileset by clicking on the " Icon: *** " PCX name and it
  shows the results immediately across the map.
* Increased the length of the song's filename to 18 characters.
* Function to select the tile under the mouse cursor (its icon in the tileset
  menu is updated to show which tile you've selected).
  * Added right-click tile-grabbing to first "grab" the tile the change to one
    of the drawing modes for quick placement of the tile.
* Added help menu (F1).
* Added extra file open/save checks.
* Fixed multiple memory bugs and map-resize issues.


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
redefined if they're undesirable.  I do have a couple TODOs that I need help
with:

===============================
TODO

The Zone Attribute is displayed using a crosshair with the letter 'z' in the
middle.  This needs to be changed to be a crosshair with the number of the
zone in the middle so it's easier to see which zone is where.



Fixes to maps:

- Moved most tiles from Layer 3 to Layer 2
- Fixed 'atype' for all entities in following maps:
  ('atype' should be 0 or 1; these had values > 1)

  bridge.map
  fort.map
  grotto.map
  tower.map



Questions and comments (donations, threats...) are of course welcome.

-TeamTerradactyl
