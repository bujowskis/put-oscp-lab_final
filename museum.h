#ifndef MUSEUM_H
#define MUSEUM_H

/* Prints the user manual to stdout */
void man();

/* Destroys the museum */
void museumDst();

/* Creates a museum maximizing the number of visitors at the same time */
void createNo(const int Na, const int Nb);

/* Creates a museum minimizing the time of leaving hall B */
void createTime(const int Na, const int Nb);

/* Adds x visitors which will enter only hall A */
void museumAddA(const int x);

/* Adds x visitors which will enter both hall A and B */
void museumAddB(const int x);

#endif //MUSEUM_H
