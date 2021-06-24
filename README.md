# put-oscp-lab_final
The following is a final project of Operating Systems with Concurrent Programming laboratories of 2nd semester at PUT Artificial Intelligence.

## Task
Prepare solution for one of the final assignments from the course book:
- **The museum** - Image a small museum with 2 halls: A and B. A is the hall one enters while stepping into museum and getting to B requires entering A first. The halls have maximum visitor capacitance Na and Nb (Nb < Na). People may enter to see the exhibitions from A and leave or proceed from A to B and then leave (passing hall A, occupying them for the time of leaving). Write programs implementing synchronization algorithm which allows:
  - visiting the museum at the same time by as many people as possible
  - leaving the museum by people visiting hall B in the shortest possible time
- **Producer-consumer problem revisited** - Write a program implementing producer and the consumer in a generalized version that makes it possible to run several prod. and cons. at the same time. A buffer of N slots should allow for running up to N concurrent processes, producing and consuming different items. The processes should run in loops with different and variable speeds. The ordering of items being consumed should reflect production and times.

## Solution

DISCLAIMER - The solution depends on whether the visitor makes up their intentions in advance. If so, a mechanism preventing deadlock can be implemented, such that exactly Na people can be visiting A at the same time. If not, the simpliest (and only, if we don't want to make the customers leave A) way to prevent deadlock is to actually let at max Na-1 people to A at the same time.

At the time of writing this, I opt for the museum problem. These are the initial thoughts on how the algorithm can work:
1. todo

todo

2. There is an additional medium - a gatekeeper. The gatekeeper does not add up to the number of visitors, thus there's no need to account for it in Na or Nb. If a person visiting B wants to leave, it let's the gatekeeper know. The gatekeeper keeps the count of people willing to leave hall B. New people may enter hall A only if there are no people waiting to leave B.

The reasoning for that is, people leaving B still have to wait for A to have enough free room to let them go through it. If A happens to be full, the only fair thing to do is to wait until somebody leaves A (one cannot just throw out a person and interrupt their visit). However, that lead up to another problem - if both A and B are full and all people from A want to visit B, a deadlock occurs. For that reason, there should be more restrictions on entering the museum. The gatekeeper shall know the visitors' intentions in advance. It can then decide whether the next person can enter or should wait.
