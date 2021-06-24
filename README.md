# put-oscp-lab_final
The following is a final project of Operating Systems with Concurrent Programming laboratories of 2nd semester at PUT Artificial Intelligence.

## Task
Prepare solution for one of the final assignments from the course book:
- **The museum** - Image a small museum with 2 halls: A and B. A is the hall one enters while stepping into museum and getting to B requires entering A first. The halls have maximum visitor capacitance `Na` and `Nb` `(Nb < Na)`. People may enter to see the exhibitions from A and leave or proceed from A to B and then leave (passing hall A, occupying them for the time of leaving). Write programs implementing synchronization algorithm which allows:
  - visiting the museum at the same time by as many people as possible
  - leaving the museum by people visiting hall B in the shortest possible time
- **Producer-consumer problem revisited** - Write a program implementing producer and the consumer in a generalized version that makes it possible to run several prod. and cons. at the same time. A buffer of N slots should allow for running up to N concurrent processes, producing and consuming different items. The processes should run in loops with different and variable speeds. The ordering of items being consumed should reflect production and times.

## Solution
I chose the first problem. While designing the solution, we have to take into account deadlock possibility. 

### Disclaimer
The solution depends on whether we know the visitors' intentions in advance. The point is, with such a setup there is a possibility for a deadlock. It happens if both B and A are full and all the people from A want to visit B instead of walk out.

### Intentions known in advance
If we know in advance, a synchronization algorithm can be implemented, such that it prevents deadlock. Then the whole hall capacities can be used. It may be achieved using an additional medium - a gatekeeper. The gatekeeper would let people in or make them wait, according to the current conditions in A and B. People are let in A such that there is always either free space or a one person which does not visit B. If a visitor in B wants to leave, it lets the gatekeeper know. If there are people willing to leave B and A is full, the gatekeeper would prevent new visitors from coming in. As soon as (possibly the only) person from A which does not want to visit B exits, willing people exit B. Once that happens, the new visitors may enter.

### Intentions not known in advance
If not, the simpliest (and only, if we don't want to force the visitors out) option is to prevent deadlock by letting at max `Na - 1` people to A at the same time. I think deadlock prevention is the best option for this exact application. One could approach the problem by detecting and recovering from deadlock, but that would require forcing at least one person from A out and the museum's rating would go down really fast ;)

### My approach
Based on the task's formulation, namely:
```
"(...) People visiting the museum enter hall A, watch the exhibition,
and decide whether they want to continue by proceeding to hall B. (...)"
```
It appears to me we're dealing with not knowing the intentions in advance. For that reason, I'm going to use deadlock prevention with the max working visitor limit `Na - 1`.

### 1. Visiting the museum at the same time by as many people as possible
This part of the exercise would be more complex if the intentions were known in advance. In such case, there could be a system of privileging some visitors over the others. There would be two lines, one for people visiting only A and one for people visiting A and B. B visitors would be privileged, as they would increase the number of people in both A and B. If additionally the time of visit was known in advance, the order of visitors could be optimized even more. But in the real museum application, such a privileging system would be quite outrageous, as well as forcing people to set and adhere to some time of visit. Not even mentioning forcing people to wait, just to let in only batches of visitors at a time.

For that reason, the only restriction placed is the deadlock prevention - always keeping one free place in A reserved for people leaving B. This approach is to high extent random - achieving as many visitors at the same time as possible depends solely on the external factors, such as the peak hours, order of visitors A with respect to visitors A&B, time of visit, etc. That's the only reasonable option which doesn't place any weird, unjust, outrageous, or simply wrong restrictions on the visitors.

### 2. Leaving the museum by people visiting hall B in the shortest possible time
This part of the exercise could be optimized without the real-world limitations. Keep in mind leaving B and then A would take some time just to get to the doors. The shortest time of leaving B is allowed if the algorithm ensures there's always a free place in A whenever someone exits B. As discussed previously, forcing people out of A is outrageous and everyone's time of visit is not known.

There's an option to achieve a guaranteed free space in A by making people entering B still taking up a place in A. That gets rid off the problem of having a single reserved space in A, but also reduces the maximum working capacity of the whole museum to just `Na`. For that reason, in real life such an algorithm could be optimized by finding the best number of places in A reserved for people leaving B. But for the sake of this task, the algorithm will reserve a place for each B visitor.

## Example invocation
todo
