Write a program that finds the current in each resistor in a circuit.

The circuit consists of a power supply, joints, and resistors. The power supply has a fixed electric potential difference across two joints, allowing current to flow into the joint ~1~ and out of the joint ~N~ throughout the circuit. Each joint is associated with an electric potential. Each resistor connects two joints and has a fixed resistance.

Two physical laws govern the flow of current in an electric circuit:

Ohm's law: The electric potential difference across a resistor is the product of the resistance in the resistor and the current passing through it. Current flows from a joint with a higher electric potential to a joint with a lower electric potential.

Kirchhoff's first law: The sum of currents flowing into a joint equals the sum of currents flowing out of it. This applies to both a joint within a circuit and a joint connected to the power supply.

## Input Specification

The first line contains three integers, ~N~, ~M~, and ~V~, representing the number of joints, the number of resistors, and the potential difference of the power supply in volts.

The following ~M~ lines each contain ~3~ integers, ~a_i~, ~b_i~, and ~R_i~, representing a resistor with a resistance of ~R_i~ ohms connecting joints ~a_i~ and ~b_i~.

## Output Specification

Output ~M~ lines, each with a floating point number, the current across each resistor in amps, in the same order as listed in the input. The current across the resistor is positive when it flows from joint ~a_i~ to joint ~b_i~ and vice versa. Your solution will be considered correct if it has an absolute or relative error less than ~10^{-5}~ compared to the reference solution.

## Constraints

~2\le N \lt 7500~, ~1 \le M \le 30000~, ~N+M \lt 31000~, ~1\le V\le 10^9~, ~1\le a_i,b_i \le N~, ~1\le R_i\le 10^5~.

In test cases worth ~40\%~ of the total points, ~M \le 500~, ~N+M \lt 550~.

Each joint is connected to at least one resistor. There is a path formed by resistors connecting arbitrary two joints in the circuit.

The input guarantees that there is a unique solution satisfying both Ohm's law and Kirchhoff's first law.

## Sample Input 1

```
4 3 36
1 2 6
2 3 10
3 4 8
```

## Sample Output 1

```
1.500000
1.500000
1.500000
```

## Explanation for Sample 1

The resistors are connected in a line. The resistance of the circuit is the sum of all resistors' resistance, and all resistors have the same current.

## Sample Input 2

```
6 6 43
1 2 6
2 3 4
2 5 12
3 4 8
4 5 6
5 6 4
```

## Sample Output 2

```
2.500000
1.000000
1.500000
1.000000
1.000000
2.500000
```

## Explanation for Sample 2

For readers with knowledge of parallel and series circuits, the circuit has two resistors and one group of resistors connected in series. The group of resistors has two lines connected in parallel, one contains one resistor and another contains three resistors in series.

## Sample Input 3

```
4 5 10
1 2 6
1 3 5
2 3 5
2 4 5
3 4 5
```

## Sample Output 3

```
0.888889
1.022222
-0.044444
0.933333
0.977778
```

## Note for Sample 3

This circuit cannot be solved using the way you may have learned in grade-school science. This is the case for most test cases in this problem.