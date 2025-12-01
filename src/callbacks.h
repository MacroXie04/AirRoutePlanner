#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "globals.h"
#include <FL/Fl_Multiline_Output.H>
#include <bobcat_ui.h>
#include <button.h>
#include <dropdown.h>
#include <input.h>
#include <int_input.h>

using namespace bobcat;

// Setup all UI callbacks
void setupFindRouteCallback(Button *findBtn, Dropdown *fromDd, Dropdown *toDd,
                            Dropdown *prefDd,
                            Fl_Multiline_Output *resultOutput);

void setupCopyCallback(Button *copyBtn, Fl_Multiline_Output *resultOutput);

void setupAirportCallbacks(Button *addBtn, Button *editBtn, Button *delBtn,
                           Button *loadBtn, Input *codeInput, Input *nameInput,
                           IntInput *xInput, IntInput *yInput);

void setupRouteCallbacks(Button *addBtn, Button *editBtn, Button *delBtn,
                         Button *loadBtn, IntInput *costInput,
                         IntInput *timeInput);

#endif // CALLBACKS_H
