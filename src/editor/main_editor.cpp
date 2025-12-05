//
// Created by Tyler Haas on 12/4/25.
//

// TODO create an entire editor
// no biggie...

#include "Editor.hpp"

int main(int /*argc*/, char** argv) {
    Editor editor{argv};
    editor.setup();
    return editor.loop();
}
