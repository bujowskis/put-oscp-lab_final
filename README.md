# put-oscp-lab_final
The following is a final project of Operating Systems with Concurrent Programming laboratories of 2nd semester at PUT Artificial Intelligence.

## Task
Prepare solution for one of the final assignments from the course book:
- **The museum** - Image a small museum with 2 halls: A and B. A is the hall one enters while stepping into museum and getting to B requires entering A first. The halls have maximum visitor capacitance `Na` and `Nb` `(Nb < Na)`. People may enter to see the exhibitions from A and leave or proceed from A to B and then leave (passing hall A, occupying them for the time of leaving). Write programs implementing synchronization algorithm which allows:
  - visiting the museum at the same time by as many people as possible
  - leaving the museum by people visiting hall B in the shortest possible time
- **Producer-consumer problem revisited** - Write a program implementing producer and the consumer in a generalized version that makes it possible to run several prod. and cons. at the same time. A buffer of N slots should allow for running up to N concurrent processes, producing and consuming different items. The processes should run in loops with different and variable speeds. The ordering of items being consumed should reflect production and times.

I chose the first problem.

## Solution

### Disclaimer
The solution depends on whether we know the visitors' intentions in advance or not. The point is, with such a setup there is a deadlock possibility. It happens if both B and A are full and all the people from A want to visit B instead of walk out. I consider both cases below.

### Intentions known in advance
If the intentions are known, a synchronization algorithm can be implemented such that it prevents deadlock. Then the whole hall capacities can be used. It may be achieved using an additional medium - a gatekeeper. The gatekeeper would let people in or make them wait, according to the current conditions in A and B. People are let in A such that there is always either free space or at least one person which does not visit B. If a visitor in B wants to leave, it lets the gatekeeper know. If there are people willing to leave B and A is full, the gatekeeper would prevent new visitors from coming in. As soon as (possibly the only) person from A which does not want to visit B leaves, people may exit B. Once that happens, the new visitors may enter.

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

There's an option to achieve a guaranteed free space in A by making people entering B still hold up a place in A. That gets rid off the deadlock problem (`Na` people can be visiting A at the same time), but also makes the whole museum's capacity just `Na`. For that reason, in real life such an algorithm could be optimized by finding the best number of places in A reserved for people leaving B. But for the sake of this task, the algorithm will reserve a place for each B visitor.

## Example invocation

### Disclaimer
Adding visitors differentiates between "only A" and "A and B" visitors. However, from the algorithms' points of view, there is no distinction between those. They are unaware of that and that's why the "no distinction" property holds.

Also, the added visitors will be coming back after some time. The fact they're the same processes doesn't play any role.

### How to start?
Run the `Makefile`. The whole project is built around the `./museum` executable. It is the main entry point to all the functionalities. By executing
```
./museum man
```
or
```
./museum    
```
We get the following manual:
```
The program is a solution to the museum problem. The available commands:

Starting the museum:
  ./museum crt no Na Nb     creates museum maximizing no. of visitors at the same time
  ./museum crt time Na Nb   creates museum minimizing time to leave hall B
  ./museum dst              destroys the museum
                    
Adding visitors:            (X = 1, 2, 4 or 8)
  ./museum A X              adds X visitors that will not enter B
  ./museum B X              adds X visitors that will enter B
```
In general, the approach is as follows:
1. Create one of the museum instances
2. Add as many A and B visitors as you want (the program waits for ctrl+D, so that you can first add all the visitors)
3. Run the visitors - they are distinguished by their pid's. Their status, as well as the museum's can be monitored.
4. After finishing and ending all the processes with ctrl+C, run `./museum dst` to unlink the created semaphores

Below are exemplary, specific invocations - step by step. The commands should be run in separate terminals, unless stated otherwise.

### Maximize visitors - step by step
(museum specifications: Na = 10, Nb = 5, 8A, 6B visitors)

1. `./museum crt no 10 5` - this terminal will enable monitoring the museum's state
2. `./museum A 8` - terminal dedicated to 8 A visitors
4. `./museum B 4` - terminal dedicated to the first 4 B visitors
5. `./museum B 2` - terminal dedicated to the last 2 B visitor
6. `ctrl+D` in all visitors' terminals to run them relatively close to each other

At this point, you can watch how the visitors interact with the museum, hall occupation, the working A occupation (max limit is always `Na - 1`), and the time elapsed since start.

7. `ctrl+C` in all terminals to terminate the processes
8. `./museum dst` in one of the terminals to unlink the semaphores

### Minimize time of exiting hall B - step by step
(museum specifications: Na = 9, Nb = 6, 10A, 9B visitors)

1. `./museum crt time 9 6` - this terminal will enable monitoring the museum's state
2. `./museum A 8` - terminal dedicated to the first 8 A visitors
3. `./museum A 2` - terminal dedicated to the last 2 A visitors
4. `./museum B 8` - terminal dedicated to the first 8 B visitors
5. `./museum B 1` - terminal dedicated to the last 1 B visitor
6. `ctrl+D` in all visitors' terminals to run them relatively close to each other

At this point, you can watch how the visitors interact with the museum, hall occupation, A hall occupation with reserved spot for every B visitor, and the time elapsed since start. Also, each B visitor shows its time of exiting the museum starting from B. This time is equal to 2 seconds (1 for exiting B, 1 for exiting A) with some insignificant fraction. There is always a place in A for every B visitor. That means the fraction comes from the time of two `sem_post` operations.

7. `ctrl+C` in all terminals to terminate the processes
8. `./museum dst` in one of the terminals to unlink the semaphores
