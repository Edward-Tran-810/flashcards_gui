# Flashcards GUI

This is simple flashcard study app built with **Qt/C++**.

You can create decks (groups of cards), add cards to them, and study
the cards through a graphical interface.

## Features

- View all decks
- View all cards in a deck
- Add decks and cards from a file
- Add a new deck through the app
- Remove a deck
- Remove a card from a deck
- Exit the app
- A card element that can:
  - Add a new card
  - Edit an existing card
  - Flip the card (show one side, then the other)
  - Study the card (answer and get checked)

## How the project is organized

The code is split into two parts: the **logic** (backend) and the
**interface** (frontend).

**Logic (backend)** - handles the data, no UI code here:
- `deckmanager` - manages all decks (add, find, remove, load from file)
- `deck` - manages the cards inside one deck
- `card` - one single flashcard (its fields, its ID, answer checking)

**Interface (frontend)** - everything the user sees and clicks:
- `mainwindow` - the main window (deck list, buttons)
- `cardwidget` - the card element on screen, inherited from `QWidget`
- `studydialog` - the window used when studying a card

`CardWidget` is reused for adding, editing, and studying cards - it just
shows different fields depending on what mode it's in.

## How cards are picked when studying

Cards are picked in **random order** from the deck, so you don't just
memorize the order they were added in.

## How to build and run

Open the `.pro` file in Qt Creator and click Run.

Or from the terminal:

\`\`\`bash
qmake flashcards_gui.pro
make
./flashcards_gui
\`\`\`

## More info

See [\'instructions.txt\'](./instructions.txt) for detailed usage instructions.

## Author

Manh Tran - Tampere University (TUNI)
