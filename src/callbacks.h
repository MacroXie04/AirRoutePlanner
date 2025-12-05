#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "globals.h"
#include <bobcat_ui.h>
#include <button.h>
#include <dropdown.h>
#include <memo.h>

using namespace bobcat;

// Setup all UI callbacks
void setupFindRouteCallback(Button *findBtn, Dropdown *fromDd, Dropdown *toDd,
                            Dropdown *prefDd, Memo *resultOutput);

void setupCopyCallback(Button *copyBtn, Memo *resultOutput);

#endif // CALLBACKS_H
