/*
    This file is part of KQ.

      Copyright (C) 2006 Günther Brammer

    KQ is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    KQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with KQ; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <glade/glade.h>

#include "mapdraw2.h"
#include "mapedit2.h"

#define GLADE_FILENAME "mapdraw2.glade"

static GtkWidget *map_drawing_area;
static GtkWidget *tile_drawing_area;
static GtkWidget *palette_drawing_area;
static GtkWidget *text_current_value;
static GtkWindow *window;
static GtkWindow *entitydialog;
static GtkListStore *layers;
static GtkListStore *eprops;
static char *current_filename;
static unsigned int current_tile;
static unsigned int current_layer;
static int selected_entity;
static int map_has_changed;
enum
{
   LAYER_SHOW_COLUMN,
   LAYER_NAME_COLUMN,
   LAYER_INDEX_COLUMN,
   LAYER_N_COLUMNS
};
unsigned int layer_showing_flags = LAYER_1_FLAG | LAYER_2_FLAG | LAYER_3_FLAG | SHADOW_FLAG | ENTITIES_FLAG;
unsigned int active_x = 0;
unsigned int active_y = 0;

void map_change (unsigned int x, unsigned int y)
{
   gtk_widget_queue_draw_area (map_drawing_area, x * 16, y * 16, 16, 16);
   map_has_changed = TRUE;
}

static void on_mainwindow_destroy (GtkMenuItem * item, gpointer userdata)
{
   gtk_main_quit ();
}

static void on_info_activate (GtkMenuItem * item, GtkWindow * parent_window)
{
   GladeXML *xml = glade_xml_new (GLADE_FILENAME, "aboutdialog", NULL);
   GtkWidget *dialog = glade_xml_get_widget (xml, "aboutdialog");
   gtk_dialog_run (GTK_DIALOG (dialog));
   gtk_widget_destroy (dialog);
   g_object_unref (xml);
}

static void load_map_and_update_stuff (char *filename)
{
   do_load_map (filename);
   gtk_widget_set_size_request (map_drawing_area, gmap.xsize * 16, gmap.ysize * 16);
   if (map_drawing_area->window)
      gdk_window_invalidate_rect (map_drawing_area->window, &map_drawing_area->allocation, FALSE);
   if (tile_drawing_area->window)
      gdk_window_invalidate_rect (tile_drawing_area->window, &tile_drawing_area->allocation, FALSE);
   gtk_widget_set_size_request (palette_drawing_area, -1, max_sets * ICONSET_SIZE * 16);
   if (palette_drawing_area->window)
      gdk_window_invalidate_rect (palette_drawing_area->window, &palette_drawing_area->allocation, FALSE);
}

static void on_open_activate (GtkMenuItem * item, GtkWindow * parent_window)
{
   GtkWidget * dialog = gtk_file_chooser_dialog_new ("Open File",
                                         parent_window,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

   if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
      if (current_filename)
         g_free (current_filename);
      current_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      load_map_and_update_stuff (current_filename);
      map_has_changed = FALSE;
   }
   gtk_widget_destroy (dialog);
}

static void on_save_as_activate (GtkMenuItem * item, GtkWindow * parent_window)
{
   GtkWidget * dialog = gtk_file_chooser_dialog_new ("Save File",
                                         parent_window,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
   gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

   if (current_filename) {
      gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), current_filename);
   } else {
      //gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), default_folder_for_saving);
      gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled.map");
   }

   if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
      if (current_filename)
         g_free (current_filename);
      current_filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      do_save_map (current_filename);
      map_has_changed = FALSE;
   }
   gtk_widget_destroy (dialog);
}

static void on_save_activate (GtkMenuItem * item, GtkWindow * parent_window)
{
   if (current_filename) {
      do_save_map (current_filename);
      map_has_changed = FALSE;
   } else {
      on_save_as_activate (item, parent_window);
   }
}

static gboolean on_mainwindow_delete_event (GtkWidget * widget, GdkEvent * event, gpointer user_data)
{
   if (map_has_changed) {
      GtkWidget * dialog = gtk_message_dialog_new (GTK_WINDOW (widget),
                                                  0,
                                                  GTK_MESSAGE_QUESTION,
                                                  GTK_BUTTONS_NONE,
                                                  "Save map before closing?");
      gtk_dialog_add_buttons (GTK_DIALOG (dialog),
                              GTK_STOCK_CLOSE, GTK_RESPONSE_REJECT,
                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
      switch (gtk_dialog_run (GTK_DIALOG (dialog))) {
      case GTK_RESPONSE_REJECT:
         map_has_changed = FALSE;
         break;
      case GTK_RESPONSE_ACCEPT:
         on_save_activate (0, GTK_WINDOW (widget));
         break;
      default:
         break;
      }
      gtk_widget_destroy (dialog);
   }
   if (!map_has_changed)
      gtk_widget_destroy (widget);
   return map_has_changed;
}

static void on_quit_activate (GtkMenuItem * item, GtkWindow * parent_window)
{
   on_mainwindow_delete_event (GTK_WIDGET (parent_window), NULL, NULL);
}

static void on_palette_button_press_event (GtkWidget * widget, GdkEventButton * event, gpointer user_data)
{
   current_tile = ((int) event->y / 16) * (widget->allocation.width / 16) + (int) event->x / 16;
   gtk_widget_queue_draw_area (tile_drawing_area, 0, 0, 16, 16);
}

static gboolean on_map_button_press_event (GtkWidget * widget, GdkEventButton * event, gpointer user_data)
{
   unsigned int x = event->x / 16;
   unsigned int y = event->y / 16;
   unsigned int i = 0;

   /* current_value is a generic string that the user can edit before clicking
    * on the map. It is the box in the bottom left corner. */
   const char * current_value = gtk_entry_get_text(GTK_ENTRY(text_current_value));
   char ch[32];

   /* Ignore double and triple clicks. Treat them as multiple normal clicks.
   * Without this line, you end up getting two normal clicks and a third, double
   * click for every two, fast, consecutive clicks */
   if (event->type == GDK_2BUTTON_PRESS || event->type == GDK_3BUTTON_PRESS)
      return FALSE;

   /* Can't edit what you can't see. This prevents accidents. */
   if (!(current_layer & layer_showing_flags))
      return FALSE;

   active_x = x;
   active_y = y;

   /* What is the gdk constant for left mouse button? */
   if (event->button == 1) {
      switch(current_layer) {
      case LAYER_1_FLAG:
      case LAYER_2_FLAG:
      case LAYER_3_FLAG:
         if (event->state & GDK_CONTROL_MASK) {
            current_tile = get_tile_at (x, y, current_layer);
            gtk_widget_queue_draw_area (tile_drawing_area, 0, 0, 16, 16);
         } else {
            set_tile_at (current_tile, x, y, current_layer);
         }
         break;
      case OBSTACLES_FLAG:
         set_obstacle_at(OBSTACLES_CYCLE, x, y);
         break;
      case ENTITIES_FLAG:
         gtk_widget_show (GTK_WIDGET (entitydialog));
         selected_entity = get_entity_at (x, y);
         if (selected_entity == -1) {
            selected_entity = do_place_entity (x, y);
         }
         if (selected_entity == -1) {
            gtk_widget_hide (GTK_WIDGET (entitydialog));
         } else {
            fill_entity_model (eprops, selected_entity);
         }
         break;
      case ZONES_FLAG:
         // If there is a zone here, fill the current value with the zone's value.
         // If there is no zone, fill the zone with the value in current_value.
         if (!(i = get_zone_at(x, y))) {
            i = atoi(current_value);
            set_zone_at(i, x, y);
         }
         sprintf(ch, "%d", i);
         gtk_entry_set_text(GTK_ENTRY(text_current_value), ch);
         break;
      case MARKERS_FLAG:
      // If there is a marker here, fill current_value edit box
      // with value of marker.
         if (get_marker_value(x, y)) {
            strcpy(ch, (char *) get_marker_value(x, y));
            gtk_entry_set_text(GTK_ENTRY(text_current_value), ch);
         } else {
         // If there is no marker here, put one here, with current value of edit box.
            set_marker_at_loc(current_value, x, y);
         }
         break;
      case SHADOW_FLAG:
      default:
         break;
      } /* End of switch */

   /* What is the gdk constant for right mouse button? */
   } else if (event->button == 3) {
      switch(current_layer) {
      case LAYER_1_FLAG:
      case LAYER_2_FLAG:
      case LAYER_3_FLAG:
            set_tile_at (0, x, y, current_layer);
            break;
      case OBSTACLES_FLAG:
            set_obstacle_at(0, x, y);
            break;
      case ZONES_FLAG:
         set_zone_at(0, x, y);
         break;
      case MARKERS_FLAG:
         remove_marker(x, y);
         break;
      case ENTITIES_FLAG:
         selected_entity = get_entity_at(x, y);
         remove_entity (selected_entity);
         selected_entity = -1;
         break;
      case SHADOW_FLAG:
      default:
         break;
      } /* End of switch */

   /* What is the gdk constant for middle mouse button? */
   } else if (event->button == 2) {
      switch(current_layer) {
      case ZONES_FLAG:
         set_zone_at(ZONES_UP, x, y);
         i = get_zone_at(x, y);
         sprintf(ch, "%d", i);
         gtk_entry_set_text(GTK_ENTRY(text_current_value), ch);
         break;
      default:
         break;
      }
   }
   gtk_widget_grab_focus(text_current_value);
   return FALSE;
}

static void on_text_current_value_changed(GtkEntry * entry, gpointer user_data)
{
   char * current_value = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
   int i = 0;

   switch(current_layer) {
   case ZONES_FLAG:
      i = atoi(current_value);
      set_zone_at(i, active_x, active_y);
      break;
   case MARKERS_FLAG:
      set_marker_at_loc(current_value, active_x, active_y);
      break;
   }
}

static void on_deletebutton_clicked (GtkButton * button, gpointer user_data)
{
   map_change (gent[selected_entity].tilex, gent[selected_entity].tiley);
   remove_entity (selected_entity);
   selected_entity = -1;
   gtk_widget_hide (GTK_WIDGET (entitydialog));
}

static gboolean on_entitydialog_delete_event (GtkWidget * w, GdkEvent * e, gpointer user_data)
{
   gtk_widget_hide(GTK_WIDGET(entitydialog));
   return TRUE;
}

static void eprops_edited_callback (GtkCellRendererText * cell, gchar * path_string, gchar * new_text, gpointer user_data)
{
   int oldx = gent[selected_entity].tilex;
   int oldy = gent[selected_entity].tiley;
   map_change (gent[selected_entity].tilex, gent[selected_entity].tiley);
   change_entity_model (eprops, selected_entity, path_string, new_text);
   if (oldx != gent[selected_entity].tilex || oldy != gent[selected_entity].tiley)
      map_change (gent[selected_entity].tilex, gent[selected_entity].tiley);
}

static gboolean on_map_expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
   cairo_t *cr = gdk_cairo_create (widget->window);

   gdk_cairo_rectangle (cr, &event->area);
   cairo_clip (cr);

   do_draw_map (cr, &event->area, layer_showing_flags);

   cairo_destroy (cr);
   return TRUE;
}

static gboolean on_palette_expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
   cairo_t *cr = gdk_cairo_create (widget->window);

   gdk_cairo_rectangle (cr, &event->area);
   cairo_clip (cr);

   do_draw_palette (cr, &event->area, widget->allocation.width, current_layer, current_tile);

   cairo_destroy (cr);
   return TRUE;
}

static gboolean on_currenttile_expose_event (GtkWidget * widget, GdkEventExpose * event, gpointer data)
{
   cairo_t *cr = gdk_cairo_create (widget->window);

   gdk_cairo_rectangle (cr, &event->area);
   cairo_clip (cr);

   do_draw_tile (cr, current_tile);

   cairo_destroy (cr);
   return TRUE;
}

static void on_layer_toggle (GtkCellRendererToggle * cell_renderer, gchar * path_str, gpointer user_data)
{
   GtkTreeModel *model = (GtkTreeModel *) user_data;
   GtkTreeIter iter;
   GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
   unsigned int layer_flag;

   /* get toggled iter */
   gtk_tree_model_get_iter (model, &iter, path);
   gtk_tree_model_get (model, &iter, LAYER_INDEX_COLUMN, &layer_flag, -1);

   /* do something with the value */
   layer_showing_flags ^= layer_flag;

   /* set new value */
   gtk_list_store_set (GTK_LIST_STORE (model), &iter, LAYER_SHOW_COLUMN, (layer_showing_flags & layer_flag) != 0, -1);

   /* clean up */
   gtk_tree_path_free (path);

   gdk_window_invalidate_rect (map_drawing_area->window, &map_drawing_area->allocation, FALSE);
}

static void on_layerselection_changed (GtkTreeSelection * treeselection, gpointer user_data)
{
   GtkTreeModel *model;
   GtkTreeIter iter;
   if (gtk_tree_selection_get_selected (treeselection, &model, &iter)) {
      gtk_tree_model_get (model, &iter, LAYER_INDEX_COLUMN, &current_layer, -1);
   }
}

static void init_entitydialog (void)
{
   // fill the entitydialog
   GladeXML *xml = glade_xml_new (GLADE_FILENAME, "entitydialog", NULL);
#define SIGNAL_CONNECT(s,p) glade_xml_signal_connect_data (xml, #s, G_CALLBACK(s), p)
   SIGNAL_CONNECT (on_deletebutton_clicked, NULL);
   SIGNAL_CONNECT (on_entitydialog_delete_event, NULL);
   SIGNAL_CONNECT (gtk_widget_hide, NULL);
#undef SIGNAL_CONNECT
   entitydialog = GTK_WINDOW (glade_xml_get_widget (xml, "entitydialog"));

   eprops = create_entity_model ();

   GtkTreeView *view = GTK_TREE_VIEW (glade_xml_get_widget (xml, "entity"));
   gtk_tree_view_set_model (view, GTK_TREE_MODEL (eprops));

   GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
   GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("Property", renderer, "text",
                                                                         ENTITY_PROP_COLUMN, NULL);
   gtk_tree_view_append_column (view, column);

   renderer = gtk_cell_renderer_text_new ();
   g_object_set (renderer, "editable", TRUE, NULL);
   g_signal_connect (renderer, "edited", (GCallback) eprops_edited_callback, NULL);
   column = gtk_tree_view_column_new_with_attributes ("Value", renderer, "text", ENTITY_VALUE_COLUMN, NULL);
   gtk_tree_view_append_column (view, column);
   g_object_unref (xml);
}

void mainwindow (int *argc, char **argv[])
{
   gtk_init (argc, argv);
   GladeXML *xml = glade_xml_new (GLADE_FILENAME, "mainwindow", NULL);

   /* get a widget (useful if you want to change something) */
   window = GTK_WINDOW (glade_xml_get_widget (xml, "mainwindow"));
   map_drawing_area = glade_xml_get_widget (xml, "map");
   tile_drawing_area = glade_xml_get_widget (xml, "currenttile");
   palette_drawing_area = glade_xml_get_widget (xml, "palette");
   text_current_value = glade_xml_get_widget (xml, "text_current_value");

   /* connect signal handlers */
#define SIGNAL_CONNECT(s,p) glade_xml_signal_connect_data (xml, #s, G_CALLBACK(s), p)
   SIGNAL_CONNECT (on_mainwindow_destroy, NULL);
   SIGNAL_CONNECT (on_mainwindow_delete_event, NULL);
   SIGNAL_CONNECT (on_open_activate, window);
   SIGNAL_CONNECT (on_save_activate, window);
   SIGNAL_CONNECT (on_save_as_activate, window);
   SIGNAL_CONNECT (on_quit_activate, window);
   SIGNAL_CONNECT (on_map_expose_event, NULL);
   SIGNAL_CONNECT (on_palette_expose_event, NULL);
   SIGNAL_CONNECT (on_info_activate, window);
   SIGNAL_CONNECT (on_currenttile_expose_event, NULL);
   SIGNAL_CONNECT (on_map_button_press_event, NULL);
   SIGNAL_CONNECT (on_palette_button_press_event, NULL);
   SIGNAL_CONNECT (on_text_current_value_changed, NULL);
   SIGNAL_CONNECT (on_deletebutton_clicked, NULL);
   SIGNAL_CONNECT (on_entitydialog_delete_event, NULL);
   SIGNAL_CONNECT (gtk_widget_hide, NULL);
#undef SIGNAL_CONNECT

   // Fill the list
   layers = gtk_list_store_new (LAYER_N_COLUMNS, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_UINT);

   GtkTreeIter iter, layer1_iter;
#define ADD_LAYER(f, n) \
   gtk_list_store_append (layers, &iter); \
   gtk_list_store_set (layers, &iter, \
                       LAYER_SHOW_COLUMN, (layer_showing_flags & f) != 0, \
                       LAYER_NAME_COLUMN, n, \
                       LAYER_INDEX_COLUMN, f, -1)
   ADD_LAYER (MAGIC_PINK_FLAG, "Pink");
   ADD_LAYER (LAYER_1_FLAG, "Layer 1");
   layer1_iter = iter;
   ADD_LAYER (LAYER_2_FLAG, "Layer 2");
   ADD_LAYER (LAYER_3_FLAG, "Layer 3");
   ADD_LAYER (SHADOW_FLAG, "Shadows");
   ADD_LAYER (OBSTACLES_FLAG, "Obstacles");
   ADD_LAYER (ZONES_FLAG, "Zones");
   ADD_LAYER (MARKERS_FLAG, "Markers");
   ADD_LAYER (ENTITIES_FLAG, "Entities");
#undef ADD_LAYER

   GtkTreeView *view = GTK_TREE_VIEW (glade_xml_get_widget (xml, "layers"));
   g_signal_connect (gtk_tree_view_get_selection (view), "changed", G_CALLBACK (on_layerselection_changed), NULL);
   gtk_tree_view_set_model (view, GTK_TREE_MODEL (layers));

   GtkCellRenderer *renderer = gtk_cell_renderer_toggle_new ();
   g_signal_connect (renderer, "toggled", G_CALLBACK (on_layer_toggle), layers);

   GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes ("", renderer, "active",
                                                                         LAYER_SHOW_COLUMN, NULL);
   gtk_tree_view_append_column (view, column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes ("Layer", renderer, "text", LAYER_NAME_COLUMN, NULL);
   gtk_tree_view_append_column (view, column);

   renderer = gtk_cell_renderer_text_new ();
   /*gtk_tree_view_column_pack_end (column, renderer, TRUE);
      gtk_tree_view_column_set_attributes (column, renderer, "text", LAYER_NAME_COLUMN, NULL); */

   gtk_tree_selection_select_iter (gtk_tree_view_get_selection (view), &layer1_iter);
   g_object_unref (xml);
   init_entitydialog ();

   if (*argc > 1) {
      current_filename = g_strdup ((*argv)[1]);
      load_map_and_update_stuff (current_filename);
   }

   gtk_main ();
}

void error_load (const char *filename)
{
   GtkWidget *dialog = gtk_message_dialog_new (window,
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE,
                                               "Error loading file '%s'",
                                               filename);
   gtk_dialog_run (GTK_DIALOG (dialog));
   gtk_widget_destroy (dialog);
}
