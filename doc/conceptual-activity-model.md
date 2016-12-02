# Conceptual Model of Activity Choice

This is the conceptual model of activity choice of one person. Activity choices are made stochastically through a time-heterogenous first-order Markov chain. Location choices are made stochastically based on preferences and travel distances.

This model is a sub-model of the overall conceptual model, so have a first look over there.

## Parameters

* Pr<sub>p, a<sub>p, k-1</sub>, a<sub>p, k</sub>, k</sub>: probability for person p to move at time k from activity a<sub>k-1</sub> to a<sub>k</sub> &forall; p &isin; P, k &isin; K, a<sub>p, k-1</sub> &isin; A, a<sub>p, k</sub> &isin; A
* &Phi;<sub>p, b, a</sub>: preference of person p to perform activity a in building b &forall; p &isin; P, b &isin; B, a &isin; A
* &Delta;t<sub>a</sub>: time needed for activity a &forall; a &isin; A

## Choice Model

Whenever &Delta;t<sub>a</sub> of last activity has elapsed for a person p &isin; P:

* a<sub>p, k, choice</sub> &isin; A: a first-order Markov chain determining next activity based on previous
* b<sub>p, k</sub> &isin; B: the building in which an activity shall be performed at time k (based on personal preference and stochastic choice, for example through [Roulette Wheel Selection](https://en.wikipedia.org/wiki/Fitness_proportionate_selection))

where:

a<sub>p, k, choice</sub> = a<sub>p, k, choice</sub>(a<sub>p, k-1</sub>, p, k)

b<sub>p, k</sub> = b<sub>p, k</sub>(a<sub>p, k, choice</sub>, p, k, r<sub>b<sub>k</sub></sub>, r<sub>p, k</sub>)

if r<sub>b<sub>p, k</sub></sub> == r<sub>p, k</sub>:

* a<sub>p, k + s</sub> = a<sub>p, k, choice</sub> &forall; s &isin; [0, 1, ..., &Delta;t<sub>a</sub>]
* r'<sub>p, k + s</sub> = **0** &forall; s &isin; [0, 1, ..., &Delta;t<sub>a</sub>]

else:

* N = ceil( ||r<sub>b<sub>p, k</sub></sub> - r<sub>p, k</sub>|| / v<sub>people</sub> ) *(travel time)*
* a<sub>p, k+n</sub> = 'transit' &forall; n &isin; [0, 1, ..., N-1]
* r'<sub>p, k+n</sub> = v<sub>people</sub> * (r<sub>b<sub>p, k</sub></sub> - r<sub>p, k</sub>) &forall; n &isin; [0, 1, ..., N-1]
* a<sub>p, k+N+s</sub> = a<sub>p, k, choice</sub> &forall; s &isin; [0, 1, ..., &Delta;t<sub>a</sub>]
* r'<sub>p, k+N+s</sub> = **0** &forall; s &isin; [0, 1, ..., &Delta;t<sub>a</sub>]

## Open Questions

* How to synch people in a single household?
