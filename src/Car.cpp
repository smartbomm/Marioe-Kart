#include <Car.h>

struct car *newestCar = (struct car *)malloc(sizeof(struct car));
;                                    // Zeiger auf Datensatz des zuletzt eingelesenen Autos
struct car *nextReadCar = newestCar; // Zeiger auf Datensatz des zuletzt ausgelesenen Autos

void addCar(uint8_t id)
{ // Lese Auto in Liste ein
    struct car *newCar;
    if (newestCar->id == 99)
    { // 1. Aufruf
        newestCar->id = id;
    }
    else
    {
        newCar = (struct car *)malloc(sizeof(struct car));
        newestCar->next = newCar;
        newestCar = newCar;
    }
    if (nextReadCar->id == 99)
        nextReadCar = newCar;
}

uint8_t getCar()
{ // Lese Auto aus Liste aus und lösche es
    uint8_t id = nextReadCar->id;
    if (id != 99)
    {
        if (nextReadCar->next == NULL)
        {
            nextReadCar->id = 99;
        }
        else
        {
            struct car *nextCar = nextReadCar->next;
            free(nextReadCar);
            nextReadCar = nextCar;
        }
    }
    return id;
}