# MJS

A minimal terminal text editor written in C, built from scratch.

> **Current phase: 0.1 — Text Viewer.**
> mjs can open a file and load it into memory, but it can't edit anything yet.

---

## Table of Contents

- [About](#about)
- [Project Status](#project-status)
- [Current Features](#current-features)
- [Architecture](#architecture)
- [Build & Run](#build--run)
- [Known Limitations](#known-limitations)
- [Roadmap](#roadmap)
- [Phase Log](#phase-log)

---

## About

**MJS** is a learning-oriented project: a text editor implemented in plain C, one small, reviewable step at a time. The goal is to understand how editors work internally (buffers, cursor, input handling, rendering) instead of wrapping an existing library.

Design principles:

- **Small milestones** — every phase ships something that runs.
- **Plain C, minimal dependencies** — standard library first.
- **Documented phases** — each phase updates this README so changes stay easy to review.

---

## Project Status

| Area                    | Status                |
| ----------------------- | --------------------- |
| File loading            | Done                  |
| Dynamic text buffer     | Done                  |
| Displaying file content | Done (dump to stdout) |
| Cursor                  | Not started           |
| Editing (insert/delete) | Not started           |
| Keyboard input          | Not started           |
| UI / rendering          | Not started           |
| Saving files            | Not started           |

At this stage mjs is a **viewer**, not yet an editor.

---

## Current Features

1. **Open a file from the command line**
   The file path is taken as the first argument. If the file can't be opened, mjs prints an error and exits with code `1`.
2. **Load the file into a dynamic buffer**
   The file is read character by character and appended to a `TextBuffer`.
3. **Print the buffer**
   After loading, the whole buffer is printed to the terminal, then the memory is freed.

### Flow

```
argv[1] ──► fopen ──► readFileByCharacter ──► TextBuffer ──► print ──► free
                            │
                            └─ addCharToBuffer (grows capacity ×2 when full)
```

---

## Architecture

Everything currently lives in a single source file.

### `TextBuffer`

```c
typedef struct {
    size_t size;      // number of characters currently stored
    size_t capacity;  // allocated size in bytes
    char  *data;      // heap-allocated character array
} TextBuffer;
```

### Functions

| Function              | Responsibility                                                                                        |
| --------------------- | ----------------------------------------------------------------------------------------------------- |
| `addCharToBuffer`     | Appends one char. When `size == capacity`, doubles the capacity via `realloc`.                        |
| `readFileByCharacter` | Reads a `FILE*` with `fgetc` until EOF, appending each char to the buffer.                            |
| `main`                | Parses args, opens the file, initializes the buffer (initial capacity: 16), loads, prints, cleans up. |

---

## Build & Run

**Requirements:** a C compiler (GCC or Clang).

```bash
# build
gcc -Wall -Wextra -o mjs mjs.c

# run
./mjs <filename>
```

Example:

```bash
./mjs notes.txt
```

Sample output:

```
File has opened successfuly!

Added more capacity to buffer
Current size: 32Bytes
...

File content:
<file content here>

Free buffer memory
```

---

## Known Limitations

Tracked here so they can be fixed deliberately. Most of them are planned for [Phase 0.2](#phase-02--cleanup--hardening-next); the rest are deferred.

- **Viewer only** — no cursor, no editing, no saving, no keyboard handling.
- **Whole file in memory** — the entire file is loaded before anything is shown.
- **Debug logging in the buffer path** — a message is printed on every capacity growth (noisy for large files, and it will interfere with a real UI).
- **Errors go to `stdout`** — error messages should use `stderr` and end with a newline.
- **Silent data loss on allocation failure** — if `realloc` fails, `addCharToBuffer` prints a message and returns; the caller doesn't know the character was dropped.
- **`fgetc` result stored in `char`** — `fgetc` returns `int`; storing it in `char` can misbehave with `EOF` on some platforms. The current `feof` check works, but it should be reworked.
- **Format specifier mismatch** — `size_t` is printed with `%d`; it should be `%zu`.
- **No null terminator / binary safety** — the buffer is a raw byte array (fine for editing, but don't treat it as a C string). Files containing `\0` are not specially handled.
- **Encoding** — no UTF-8 awareness; each byte is treated as one character.
- **Doubling growth has no overflow check** on `capacity * 2`.

---

## Roadmap

Planned work, in order:

### Phase 0.2 — Cleanup & Hardening (next)

Before adding new features, we fix most of the issues listed in [Known Limitations](#known-limitations) so the editing work starts on a better codebase. The goal is **"mostly fixed", not "perfect"** — we ship a cleaner base, then move on.

**In scope:**

- [ ] Store the `fgetc` result in an `int` and rework the EOF check
- [ ] Use `%zu` for `size_t` values
- [ ] Send error messages to `stderr` and end them with a newline
- [ ] Make `addCharToBuffer` report failure to its caller (e.g. return a status code) so a failed `realloc` is no longer silent
- [ ] Remove the capacity-growth debug print (or put it behind a debug flag)
- [ ] Add an overflow check when doubling the capacity

**Explicitly deferred (not part of this phase):**

- Loading the whole file into memory
- UTF-8 awareness
- Special handling of binary files / `\0` bytes

**Done when:** the viewer behaves exactly as in 0.1, with the in-scope items above resolved and the Known Limitations section updated to reflect what's left.

### Phase 0.3 — Cursor & Editing (core)

- [ ] Add cursor position to editor state
- [ ] Insert a character at the cursor position (not only at the end)
- [ ] Delete a character (backspace / delete)
- [ ] Move the cursor within the buffer

### Phase 0.4 — Keyboard Input

- [ ] Read keypresses from the terminal
- [ ] Raw mode / disable line buffering and echo
- [ ] Map keys to actions (arrows, backspace, enter, quit)

### Phase 0.5 — UI / Rendering

- [ ] Render the buffer as a screen (rows/columns)
- [ ] Draw the cursor
- [ ] Basic status bar (file name, position)
- [ ] Scrolling for content larger than the terminal

### Later

- [ ] Save file
- [ ] Refactor into multiple modules (`buffer`, `file`, `input`, `render`)
- [ ] Line-based data structure or gap buffer for efficient edits
- [ ] UTF-8 support
- [ ] Search
- [ ] Undo/redo

---

## Phase Log

| Phase | Summary                                                                                             |
| ----- | --------------------------------------------------------------------------------------------------- |
| 0.1   | File is opened from CLI, loaded char-by-char into a dynamic `TextBuffer`, and printed. Viewer only. |

---

## Contributing

This is a two-person learning project. Keep changes small, one concern per commit, and update the **Project Status**, **Roadmap**, and **Phase Log** sections when a phase changes.
