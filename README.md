# TermiGammon

> Pełna, terminalowa implementacja gry Backgammon w C z wykorzystaniem biblioteki ncurses.

---

## Spis treści

* [Opis projektu](#opis-projektu)
* [Kluczowe funkcje](#kluczowe-funkcje)
* [Wymagania i ograniczenia](#wymagania-i-ograniczenia)
* [Budowa i uruchomienie](#budowa-i-uruchomienie)
* [Sterowanie](#sterowanie)
* [Struktura repozytorium](#struktura-repozytorium)
* [Architektura i design](#architektura-i-design)
* [Kontakt](#kontakt)
* [Licencja](#licencja)

---

## Opis projektu

**TermiGammon** to w pełni funkcjonalna gra Backgammon uruchamiana w terminalu. Wykorzystuje bibliotekę **ncurses** do rysowania planszy, menu i pionków, a całość logiki gry zaimplementowano w C bez dodatkowych zewnętrznych bibliotek.

---

## Kluczowe funkcje

* **Pełne reguły Backgammon**: ruchy, bicie pionków, bar, dom, punktacja końcowa.
* **Interfejs ncurses**: dynamiczne rysowanie planszy, kolumn, pasków i menu kontekstowego.
* **Menu startowe i in-game**: wybór gry, podgląd informacji, zapisy/odczyty stanu.
* **Obsługa plików**: zapis i przywracanie partii („save/load”), krok po kroku odtwarzanie ruchów.
* **Sterowanie klawiaturą**: wsparcie dla klawiszy strzałek, Enter, liter (`s` – save, `l` – load, `p` – restart, `k` – zakończenie).

---

## Wymagania i ograniczenia

* **Język**: C (standard C99)
* **Biblioteka**: tylko `ncurses` (bez innych zależności)
* **Moduły**: każda funkcja krótka i jednofunkcyjna (zgodnie z wymaganiami zadania)
* **Środowisko**: Linux / macOS / Windows (z pakietem ncurses)

---
## Struktura repozytorium

```
TermiGammon/
├── main.c                  # jedyny plik źródłowy z całą logiką
├── saves/                  # folder z plikami zapisu (save1.txt…)
└── README.md               # ten plik
```

---

## Architektura i design

* **Moduł UI**: funkcje `menuFrame()`, `board()`, `drawPawn()`, itd. izolują rysowanie od logiki.
* **Moduł logiki gry**: `pawnMoveSystem()`, `checkPawn()`, `endGame()` definiują zasady Backgammon.
* **Moduł I/O**: `save()`, `load()`, `stepSave()` i `stepMove()` odpowiadają za trwałość stanu i odtwarzanie ruchów.
* **Struktura danych**: `Board` (tablica pionków), `Column` (pozycje i liczba pionków), `Pawn` (gracz + współrzędne).

Dzięki prostym, jednozadaniowym funkcjom i bez zewnętrznych zależności projekt jest lekki, łatwy do kompilacji i utrzymania.

---

## Kontakt

* **Autor**: Igor Tomkowicz
* GitHub: [npnpdev](https://github.com/npnpdev)
* E-mail: [npnpdev@gmail.com](mailto:npnpdev@gmail.com)

---

## Licencja

Projekt dostępny na licencji **MIT**.
