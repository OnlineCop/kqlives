Mapdraw Info:

Hope you like the changes I made; they are, of course, open for debate :-)

===============================
Keys:

NOTE: You can only edit layers with these:
1-3 - Layers 1-3 (can edit Layers)

NOTE: You can't edit layers with these:
4-7 - Multi-layers (can't edit Layers)

NOTE: These Attributes can be toggled.  Displaying Layers turns them off by
      default.  You can only draw to the the map if the Attribute is the
      Active mode.  Toggling it again will hide the Attribute.  You can have
      multiple Attributes on at once.
F11 - Toggle Entites Attribute
S   - Toggle Shadows Attribute
O   - Toggle Obstacles Attribute
Z   - Toggle Zone Attribute

C   - View Layers 1-3, plus Entities and Shadows Attributes
A   - Turn on all Layers and all Attributes
0 (zero) - Turn off all Layers and Attributes

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

Fix the Attribute problem in 'mapedit.c':
This is where the Attributes need to toggle on or off, regardless of which
Layers are showing.  The current bug is that the Attributes will turn on all
3 Layers when they are showing, then the Layers turn back off when the
Attributes are turned off.



Known bugs:

'atype' is defined to be 0 or 1, but in the following maps, this is not the
case:

bridge.map
fort.map
grotto.map
tower.map

Is there a reason that these don't follow suit?  If there is, let me know.
If not, let's fix them.



Questions and comments (donations, threats...) are of course welcome.

teamterradactyl@users.sourceforge.net
