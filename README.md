# AGH_WIMiIP_Metoda_Elementow_Skonczonych
Laboratoria z MES na AGH WIMiIP [prowadzący: dr inż. Piotr Kustra]

## Program składa się z klas głównych:
```cpp
class Node
```
Zawiera parametry dotyczące węzła, pozycję ( x,y ), temperaturę oraz warunek brzegowy

```cpp
class Element
```
Zawiera węzły tworzące element, ich globalne indeksy oraz ilość krawędzi posiadających warunek brzegowy


```cpp
 class GlobalData
```
Agregującej klasy:


```cpp
class Size
```
Zawierającej długość i szerokość obiektu

```cpp
class Grid
```
Zawierającej ilość węzłów wzdłuż i wszerz, liczbę węzłów i liczbę elementów


```cpp
class Temperature
```
Zawierającej temperatury początkowe ciała oraz otoczenia


```cpp
class Time
```

Zawierającej czas procesu oraz krok czasowy
```cpp
class Material
```
Zawierającej dane materiału jak i otoczenia – pojemność cieplną, współczynnik przewodzenia, gęstość ciała oraz współczynnik wymiany ciepła


```cpp
class Grid
```
Zawiera globalne wektory elementów, węzłów, globalne macierze H oraz C, globalny wektor P


```cpp
class Jacobian
```
Generuje jakobian, wyznacznik jakobianu oraz odwrotny jakobian dla wybranego punktu


```cpp
class Surface
```
Tworzy powierzchnię na podstawie obiektu klasy Element, wyznacza długość wybranej krawędzi, generuje funkcje kształtu oraz wraca wyznacznik powierzchni.

```cpp
class UniversalElement
```
Klasa reprezentująca element w układzie lokalnym, zawiera współrzędne w układzie ( ξ,η ) oraz wartości funkcji kształtu

## oraz klas pomocniczych:
```cpp
class Timer
```
Zapewnia pomiar czasu wybranego obszaru kodu z dokładnością do 1 mikrosekundy

```cpp
class Utils
```
Zwiera metodę solver zarządzającą procesem obliczeń, metodę odpowiedzialną za rozwiązywanie układy równań wykorzystującego metodę Gaussa oraz generyczną metodę znajdującą minimalną oraz maksymalną wartość w wektorze ze złożonością O(n)
