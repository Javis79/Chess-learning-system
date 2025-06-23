# README - System Nauki Gry w Szachy

## Opis Projektu

System nauki gry w szachy jest dedykowany dla początkujących szachistów, którzy chcą poprawić swoje umiejętności i poznać zasady gry. Projekt oparty jest na mikrokontrolerze STM32, który zarządza interakcją z użytkownikiem oraz prezentuje ruchy na szachownicy.

Projekt ma na celu pomoc w nauce poprzez wizualne wskazywanie odpowiednich pól szachownicy oraz wyświetlanie notacji ruchów na wyświetlaczu 2.8 cala z interfejsem SPI. System wykorzystuje matrycę LED do podświetlania odpowiednich pól oraz umożliwia łatwe śledzenie postępów w grze.

## Zawartość Projektu

1. **Szachownica LED**: Matryca LED podświetlająca odpowiednie pola na planszy szachowej.
2. **Notacja Ruchów**: Wyświetlanie ruchów w odpowiedniej notacji na wyświetlaczu 2.8 cala z SPI.
3. **Mikrokontroler STM32**: Centralny układ sterujący systemem, odpowiedzialny za obsługę wyświetlacza i matrycy LED.
4. **Potencjalny Rozwój**:

   * **Wykrywanie Poprawności Ruchów**: Każde pole szachownicy będzie traktowane jako przycisk, a system będzie rozpoznawał odkliknięcie klawisza w celu sprawdzenia poprawności ruchu.
   * **Zegar Szachowy**: Wyświetlanie zegara szachowego na wyświetlaczu 2.8 cala, umożliwiającego śledzenie czasu każdej ze stron.

## Wymagania Systemowe

* **Mikrokontroler**: STM32F401RE ARM Cortex M4
* **Matryca LED**: 8x8 
* **Wyświetlacz**: 2.8 cala SPI
* **Zasilanie**: 5V 
* **Oprogramowanie**: STM32CubeIDE

## Wykorzystane Technologie

* **Mikrokontroler STM32**: Platforma sprzętowa do realizacji projektu.
* **SPI**: Interfejs do komunikacji z wyświetlaczem.
* **Matryca LED**: Wyświetlanie pól szachowych i interakcja z użytkownikiem.
* **C**: Język programowania do realizacji logiki sterującej mikrokontrolerem.

## Autor

Praca inżynierska jest tworzona przez Krystiana Bajerskiego, studenta elektroniki na AGH pod przewodnictwem dr inż. Agnieszki Dąbrowskiej-Boruch.  
