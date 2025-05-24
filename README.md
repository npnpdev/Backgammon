# TermiGammon

[English](#english-version) | [Polski](#wersja-polska)

---

## English Version

### Project Description

**TermiGammon** is a terminal-based Backgammon game written in C, using the **ncurses** library for board, menu, and piece rendering. All game logic is implemented in C99 without external dependencies.

### Key Features

* **Complete Backgammon Rules**: movement, hitting, bar, bearing off, final scoring
* **ncurses Interface**: dynamic rendering of board, points, bar, and context menus
* **Save/Load Functionality**: save to and load from slots 1–5 and initial state via `start.txt`
* **Step-by-Step Replay**: rewind/forward moves with Left/Right arrows + Enter, jump to game end with `k`
* **Keyboard Controls**: detailed in the Controls section below

### Requirements & Constraints

* **Language**: C (C99 standard)
* **Library**: ncurses only
* **Platform**: Linux, macOS, Windows (with ncurses)

### Controls

#### Main Menu (`optionsSystem`)

* **KEY\_UP / KEY\_DOWN**: navigate options `PLAY`, `INFO`, `EXIT`
* **Enter (10)**: confirm selection

#### In-Game Options (`inGameOptions`) — before rolling dice

* **`s`**: open save-slot menu (slots 1–5), navigate with KEY\_UP/KEY\_DOWN, confirm with Enter
* **`l`**: open load-slot menu (slots 1–5), navigate with KEY\_UP/KEY\_DOWN, confirm with Enter
* **`p`**: restart game from `start.txt`
* **KEY\_LEFT**: rewind one sub-move (press Enter after each KEY\_LEFT to apply)
* **KEY\_RIGHT**: forward one sub-move (press Enter after each KEY\_RIGHT to apply)
* **`k`**: jump to end of saved game (replay all moves)
* **Enter (10)**: proceed to normal turn (roll dice, select pieces, move)

#### Pawn Count Selection (`pawnsOption`)

* **KEY\_UP**: change number of pieces to move (1–4 depending on dice)
* **Enter (10)**: confirm count

#### Field Number Input (`fieldNumber`)

* **Numeric keys**: enter source field number for the move
* **Enter**: confirm field number
* **`99`**: immediate program exit (debug/force quit)

#### First Roll (`firstRoll`)

* **Enter (10)**: continue after each player’s roll to see who starts

### Repository Structure

```text
TermiGammon/
├── main.c       # single source file with all logic
├── start.txt    # initial game state (used by restart)
├── save1.txt…save5.txt  # save slots created in working directory
├── stepSave.txt # intermediate state for step-by-step replay
└── README.md    # this documentation
```

### Architecture & Design

* **UI Module**: `menuFrame()`, `board()`, `drawPawn()`, etc., separate rendering from logic
* **Game Logic Module**: `pawnMoveSystem()`, `checkPawn()`, `endGame()` implement Backgammon rules
* **I/O Module**: `save()`, `load()`, `stepSave()`, `stepMove()` manage persistence and replay
* **Data Structures**: `Board` (array of pawn counts), `Column` (positions and counts), `Pawn` (player ID + coordinates)

---

## Wersja polska

### Opis projektu

**TermiGammon** to terminalowa implementacja gry Backgammon w C (standard C99), wykorzystująca bibliotekę **ncurses** do rysowania planszy, menu i pionków. Cała logika gry jest zaimplementowana w jednym pliku `main.c` bez dodatkowych zależności.

### Kluczowe funkcje

* **Pełne zasady Backgammon**: ruch pionków, bicie, bar, zdejmowanie pionków, punktacja końcowa
* **Interfejs ncurses**: dynamiczne rysowanie planszy, punktów, baru i menu kontekstowego
* **Zapis/Odczyt stanu gry**: sloty zapisu 1–5 i restart z pliku `start.txt`
* **Odtwarzanie ruchów krok po kroku**: cofanie/przewijanie strzałkami Lewo/Prawo + Enter, skok na koniec gry klawiszem `k`
* **Sterowanie klawiaturą**: szczegółowo w sekcji Sterowanie

### Wymagania i ograniczenia

* **Język**: C (C99)
* **Biblioteka**: ncurses
* **Platforma**: Linux, macOS, Windows (z ncurses)

### Sterowanie

#### Menu Główne (`optionsSystem`)

* **KEY\_UP / KEY\_DOWN**: nawigacja po opcjach `PLAY`, `INFO`, `EXIT`
* **Enter (10)**: zatwierdzenie wyboru

#### Opcje w grze (`inGameOptions`) — przed rzutem kostką

* **`s`**: otwiera menu slotów zapisu (1–5), nawigacja KEY\_UP/KEY\_DOWN, zatwierdzenie Enter
* **`l`**: otwiera menu slotów odczytu (1–5), nawigacja KEY\_UP/KEY\_DOWN, zatwierdzenie Enter
* **`p`**: restart gry z pliku `start.txt`
* **KEY\_LEFT**: cofnięcie jednego pod-ruchu (po każdej strzale Enter)
* **KEY\_RIGHT**: przewinięcie jednego pod-ruchu (po każdej strzale Enter)
* **`k`**: skok na koniec zapisanej partii
* **Enter (10)**: przejście do normalnej fazy (rzut kostkami, ruch)

#### Wybór liczby pionków (`pawnsOption`)

* **KEY\_UP**: zmiana liczby pionków do ruchu (1–4)
* **Enter (10)**: zatwierdzenie

#### Wprowadzenie numeru pola (`fieldNumber`)

* **Klawisze numeryczne**: wpisanie numeru pola źródłowego
* **Enter**: zatwierdzenie numeru
* **`99`**: natychmiastowe zakończenie programu (wyjście awaryjne)

#### Pierwszy rzut (`firstRoll`)

* **Enter (10)**: kontynuacja po rzucie każdego gracza, aby ustalić kolejność

### Struktura repozytorium

```text
TermiGammon/
├── main.c           # jedyny plik źródłowy z logiką gry
├── start.txt        # stan początkowy używany przy restarcie
├── save1.txt…save5.txt # pliki zapisów tworzone w katalogu uruchomienia
├── stepSave.txt     # pośredni stan do odtwarzania kroków
└── README.md        # ta dokumentacja
```

### Architektura i design

* **Moduł UI**: `menuFrame()`, `board()`, `drawPawn()`, itd., oddzielenie rysowania od logiki
* **Moduł logiki gry**: `pawnMoveSystem()`, `checkPawn()`, `endGame()` implementują zasady Backgammon
* **Moduł I/O**: `save()`, `load()`, `stepSave()`, `stepMove()` zarządzają zapisem, odczytem i odtwarzaniem
* **Struktury danych**: `Board` (tablica liczby pionków), `Column` (pozycje i liczba pionków), `Pawn` (ID gracza + współrzędne)

---

## Kontakt / Contact

* **Igor Tomkowicz**
* GitHub: [npnpdev](https://github.com/npnpdev)
* LinkedIn: [Igor Tomkowicz](https://www.linkedin.com/in/igor-tomkowicz-a5760b358/)
* E-mail: [npnpdev@gmail.com](mailto:npnpdev@gmail.com)

---

## Licencja / License

Project available under the **MIT** license. See [LICENSE](LICENSE) for details.
