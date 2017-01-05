# Conceptual Model

The following describes the model to be implemented. It combines a city-scale building energy model with a people activity model.

## Sets

* P: people
* B: buildings
* K: time steps
* A = {work, sleep, shower, transit, ...}: people activities

## Parameters

* T<sub>out, k</sub>: outdoor temperature at time k
* &delta;<sub>thermal</sub>: vector of thermal building parameters
* r<sub>b</sub>: position vector of building b
* v<sub>people</sub>: average transit speed of people
* &Gamma;<sub>p, b, a, k</sub>: restriction of activity a at place b for person p at time k (0 = activity not possible, 1 = activity possible)

(possible more restrictions apply to &Gamma;<sub>p, b, a</sub>: only one work place, sleep only at home, etc.)

## People Movement Model

* r<sub>p, k</sub>: position vector of person p at time k
* P<sub>b, k</sub> &isin; P: people in building b at time k

where

r'<sub>p, k</sub> = v<sub>people</sub> * (r<sub>b<sub>destination</sub></sub> - r<sub>b<sub>source</sub></sub>)

r<sub>p, k</sub> = r<sub>b</sub> &forall; p &isin; P<sub>b, k</sub>

See `conceptual-activity-model.md` for more details.

## People Activity Model

* A<sub>current</sub>: P x K x A &#8614; A

See `conceptual-activity-model.md` for more details.

## Building Energy Model

* T<sub>in, b, k</sub>: indoor temperature in building b at time k
* P<sub>thermal, b, k</sub>: heating or cooling power in building b at time k

where

P<sub>thermal, b, k</sub> = P<sub>thermal, b, k</sub>(T<sub>in, b, k</sub>, &delta;<sub>thermal</sub>, T<sub>out, k</sub>, &theta;<sub>set, b, k, H</sub>, &theta;<sub>set, b, k, C</sub>) &forall; b &isin; B, k &isin; K

T<sub>in, b, k</sub> = T<sub>in, b, k</sub>(T<sub>in, b, k-1</sub>, &delta;<sub>thermal</sub>, T<sub>out, k-1</sub>, P<sub>thermal, b, k-1</sub>)

&theta;<sub>set, b, k, H</sub> = &theta;<sub>set, b, k, H</sub>(P<sub>b, k</sub>)

&theta;<sub>set, b, k, C</sub> = &theta;<sub>set, b, k, C</sub>(P<sub>b, k</sub>)
