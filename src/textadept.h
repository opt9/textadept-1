// Copyright 2007 Mitchell mitchell<att>caladbolg.net. See LICENSE.

#ifndef TEXTADEPT_H
#define TEXTADEPT_H

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define PLAT_GTK 1
#include <Scintilla.h>
#include <SciLexer.h>
#include <ScintillaWidget.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// globals
extern GtkWidget
  *window, *focused_editor, *command_entry,
  *pm_container, *pm_entry, *pm_view,
  *findbox, *find_entry, *replace_entry;
extern GtkTreeStore *pm_store;
extern lua_State *lua;
static const char *textadept_home = "/usr/share/textadept/";

// textadept.c
void create_ui();
GtkWidget* new_scintilla_window(sptr_t default_id=NULL);
void remove_scintilla_window(GtkWidget *editor);
void new_scintilla_buffer(ScintillaObject *sci, bool create=true,
                          bool addref=true);
void remove_scintilla_buffer(sptr_t doc);
void split_window(GtkWidget *editor, bool vertical=true);
bool unsplit_window(GtkWidget *editor);
void resize_split(GtkWidget *editor, int pos, bool increment=true);
void set_statusbar_text(const char *text);
void set_docstatusbar_text(const char *text);
void command_toggle_focus();

static long SS(ScintillaObject *sci, unsigned int msg, unsigned long wParam=0,
               long lParam=0) {
  return scintilla_send_message(sci, msg, wParam, lParam);
}
static long SSS(ScintillaObject *sci, unsigned int msg, const char *wParam=0,
                const char *lParam=0) {
  return scintilla_send_message(sci, msg, reinterpret_cast<long>(wParam),
                                reinterpret_cast<long>(lParam));
}

// lua_interface.c
void l_init(int argc, char **argv);
void l_close();
void l_load_script(const char *script_file);
void l_add_scintilla_window(GtkWidget *editor);
void l_remove_scintilla_window(GtkWidget *editor);
void l_goto_scintilla_window(GtkWidget *editor, int n, bool absolute=true);
void l_set_view_global(GtkWidget *editor);
int  l_add_scintilla_buffer(sptr_t doc);
void l_remove_scintilla_buffer(sptr_t doc);
void l_goto_scintilla_buffer(GtkWidget *editor, int n, bool absolute=true);
void l_set_buffer_global(ScintillaObject *sci);

void l_handle_error(lua_State *lua, const char *errmsg=0);
bool l_handle_signal(const char *s);
bool l_handle_keypress(int keyval, GdkEventKey *event);
void l_handle_completion(const char *command);
void l_handle_scnnotification(SCNotification *n);

void l_ta_command(const char *command);

bool l_pm_get_contents_for(const char *entry_text, bool expanding=false);
void l_pm_populate(GtkTreeIter *initial_iter=NULL);
void l_pm_get_full_path(GtkTreePath *path);
void l_pm_perform_action();
void l_pm_popup_context_menu(GdkEventButton *event, GCallback callback);
void l_pm_perform_menu_action(const char *menu_item);

void l_find(const char *ftext, int flags, bool next=true);
void l_find_replace(const char *rtext);
void l_find_replace_all(const char *ftext, const char *rtext, int flags);

// pm.c
GtkWidget* pm_create_ui();
void pm_toggle_focus();
void pm_open_parent(GtkTreeIter *iter, GtkTreePath *path);
void pm_close_parent(GtkTreeIter *iter, GtkTreePath *path);
void pm_activate_selection();
void pm_popup_context_menu(GdkEventButton *event, GCallback callback);
void pm_process_selected_menu_item(GtkWidget *menu_item);

// find_replace.c
GtkWidget *find_create_ui();
void find_toggle_focus();

#endif
