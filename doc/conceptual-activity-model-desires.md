# Conceptual Model of Rational Activity Choice

*This is an alternative model to the one described in `conceptual-activity-model.md` and its use is currently discouraged.*

This is the conceptual model of activity choice of one person. It is a Rational Choice Model
in which people choose their activities based on predictions of fulfilment of their desires.

Desires are modelled as leaking stocks. Activities refill stocks or accelerate their depletion. People's decisions are based on a model predictive control approach in which people choose a day plan that optimises the level of their desires.

This model is a sub-model of the overall conceptual model, so have a first look over there.

## Sets

* S = {energy, hunger, household_budget, ...}: stocks
* D = {energy, hunger, ...} &isin; S: desires
* K+: time steps over forecast horizon
* A = {work, sleep, shower, transit, ...}: people activities

## Parameters

* k<sub>0</sub>: current time
* T<sub>s</sub>: decay time of stock s &forall; s &isin; S
* w<sub>d</sub>: weight of desire d &forall; d &isin; D
* l<sub>s</sub> &isin; [0, 1] &isin; &real;: level of stock s at time k<sub>0</sub> &forall; s &isin; S
* &Delta;l<sub>s, a</sub>: level change of stock s for activity a per time step &forall; s &isin; S, a &isin; A
* &Delta;t<sub>a</sub>: time needed for activity a &forall; a &isin; A

## Variables

* a<sub>k</sub> &isin; A: chosen activity at time k &forall; k &isin; K+
* b<sub>k</sub> &isin; B: the building in which an activity shall be performed at time k &forall; k &isin; K+
* u<sub>k</sub> &isin; A x B = (a<sub>k</sub>, b<sub>k</sub>)

## Auxiliary Variables

These are not free variables, but only introduced for to simplify notation.


* l<sup>^</sup><sub>s, k</sub>: predicted level of stock s at time k &forall; s &isin; S, k &isin; K

## Constraints

* a<sub>k + 1</sub> = a<sub>k</sub> if k - k<sub>a, start</sub> < &Delta;t<sub>a</sub>
* l<sup>^</sup><sub>s, k+1</sub> = l<sup>^</sup><sub>s, k</sub> * e<sup>ln(0.5) / T<sub>s</sub></sup> + &Delta;l<sub>s, a<sub>k</sub></sub>
* if r<sub>b<sub>k</sub></sub> &ne; r<sub>p, k</sub>:
    * a<sub>k</sub> = 'transit'
    * r'<sub>p, k</sub> = v<sub>people</sub> * (r<sub>b<sub>k</sub></sub> - r<sub>p, k</sub>)

 else:
    * r'<sub>p, k</sub> = 0
* &Gamma;<sub>p, b<sub>k</sub>, a<sub>k</sub>, k</sub> = 1

## Objective Function

J(u) =  &sum;<sub>k</sub><sup>K+</sup> &sum;<sub>d</sub><sup>D</sup> |1 - l<sup>^</sup><sub>d, k</sub>| * w<sub>d</sub>

min<sub>u</sub> J(u)

## Open Questions

* Do we need a threshold per desire that defines when a desire becomes relevant?
* How to synch household stocks among people in the household?
* Which optimisation algorithm?
