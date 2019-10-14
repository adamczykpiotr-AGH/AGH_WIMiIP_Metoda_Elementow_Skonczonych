# AGH_WIMiIP_Metoda_Elementow_Skonczonych
Laboratoria z MES na AGH WIMiIP [prowadzący: dr inż. Piotr Kustra]

## Obiekty klas:

GlobalData:
```cpp
GlobalData(double height, double length, uint64_t mH, uint64_t mL);
```

Grid:
```cpp
Grid(GlobalData data);
```

Element:
```cpp
Element(Node& first, Node& second, Node& third, Node& fourth);
```
Node:
```cpp
Node(uint64_t x, uint64_t y, uint16_t boundaryCondition = 0) {
```

## Dodatkowe uwagi
Każdy obiekt klasy ma przeładowany operator >>
