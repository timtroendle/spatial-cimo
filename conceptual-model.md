# Conceptual Model

The following describes the model to be implemented. It combines a city-scale building energy model with a people activity model.

## Sets

* P: people
* B: buildings
* K: time steps
* A = {work, sleep, shower, transit, ...}: people activities

## Parameters

* &delta;<sub>thermal</sub>: vector of thermal building parameters
* &theta;<sub>set, a, H</sub>: heating set point relating to activity a
* &theta;<sub>set, a, C</sub>: cooling set point relating to activity a
* P<sub>elec, b, k, fix</sub>: base active power load for building b at time k
* P<sub>elec, a</sub>: active power load related to activity a
* P<sub>water, a</sub>: load for heating water related to activity a
* r<sub>b</sub>: position vector of building b
* v<sub>people</sub>: average transit velocity of people
* &Gamma;<sub>p, b, a</sub>: restriction of activity a at place b for person p (0 = activity possible, 1 = activity not possible)

where

&sum;<sub>b</sub><sup>B</sup> &Gamma;<sub>p, b, a</sub> >= 1 &forall; p &isin; P, a &isin; A

(possible more restrictions apply to &Gamma;<sub>p, b, a</sub>: only one work place, sleep only at home, etc.)

## Transit Model

* r<sub>p, k</sub>: position vector of person p at time k
* P<sub>b, k</sub> &isin; P: people in building b at time k

where

r'<sub>p, k</sub> = v<sub>people</sub> * (r<sub>b<sub>destination</sub></sub> - r<sub>b<sub>source</sub></sub>)

r<sub>p, k</sub> = r<sub>b</sub> &forall; p &isin; P<sub>b, k</sub>

## People Activity Model

* A<sub>current</sub>: P x K x A &#8614; {0, 1}

where &sum;<sub>a</sub><sup>A</sup> A<sub>current</sub>(p, k, a) = 1 &forall; p &isin; P, b &isin; B

## Building Energy Model

* P<sub>thermal, b, k</sub>: heating or cooling power in building b at time k
* P<sub>elec, b, k</sub>: electrical power in building b at time k
* P<sub>water, b, k</sub>: power usage for hot water in building b at time k

where

P<sub>elec, b, k</sub> = P<sub>elec, b, k, fix</sub> + &sum;<sub>a</sub><sup>A</sup>&sum;<sub>p</sub><sup>P<sub>b, k</sub></sup> A<sub>current</sub>(p, k, a) * P<sub>elec, a</sub> &forall; b &isin; B, k &isin; K

P<sub>water, b, k</sub> = &sum;<sub>a</sub><sup>A</sup>&sum;<sub>p</sub><sup>P<sub>b, k</sub></sup> A<sub>current</sub>(p, k, a) * P<sub>water, a</sub> &forall; b &isin; B, k &isin; K

P<sub>thermal, b, k</sub> = P<sub>thermal, b, k</sub>(P<sub>thermal, b, k-1</sub>, &delta;<sub>thermal</sub>, &theta;<sub>set, b, k, H</sub>, &theta;<sub>set, b, k, C</sub>) &forall; b &isin; B, k &isin; K

&theta;<sub>set, b, k, H</sub> = min<sub>P<sub>b, k</sub></sub> &sum;<sub>a</sub><sup>A</sup> A<sub>current</sub>(p, k, a) * &theta;<sub>set, a, H</sub> &forall; b &isin; B, k &isin; K

&theta;<sub>set, b, k, C</sub> = max<sub>P<sub>b, k</sub></sub> &sum;<sub>a</sub><sup>A</sup> A<sub>current</sub>(p, k, a) * &theta;<sub>set, a, C</sub> &forall; b &isin; B, k &isin; K
