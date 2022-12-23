Write a program that finds the current in each connection in a circuit.

The circuit consists of a power supply, nodes, and connections. The power supply has a fixed electric potential difference across node ~1~ and node ~N~, allowing current to flow into the node ~1~ and out of the node ~N~ throughout the circuit. Each node is associated with an electric potential. Each connection joins two nodes and has a fixed resistance.

Two physical laws govern the flow of current in an electric circuit:

Ohm's law: The electric potential difference across a connection is the product of the resistance of the connection and the current passing through it. Current flows from a node with a higher electric potential to a node with a lower electric potential.

Kirchhoff's current law: The sum of currents flowing into a node equals the sum of currents flowing out of it. This applies to both nodes within the circuit and nodes connected to the power supply.

## Input Specification

The first line contains three integers, ~N~, ~M~, and ~V~, representing the number of nodes, the number of connections, and the potential difference of the power supply.

The following ~M~ lines each contains ~3~ integers, ~a_i~, ~b_i~, and ~R_i~, representing a connection with a resistance of ~R_i~ units connecting joints ~a_i~ and ~b_i~.

## Output Specification

Output ~M~ lines, each with a decimal number, the current across each connection, in the same order as listed in the input. The current across connection ~i~ is positive if it flows from node ~a_i~ to node ~b_i~ and vice versa. Your solution will be considered correct if it has an absolute or relative error less than ~10^{-5}~ compared to the reference solution.

## Constraints

~2\le N \le 10000~, ~1 \le M \le 30000~, ~1\le V\le 10^9~, ~1\le a_i,b_i \le N~, ~1\le R_i\le 10^5~.

In test cases worth ~40\%~ of the total points, ~N \le 600~.

For any two nodes in the circuit, there is a path formed by one or more connections between them.

It is guaranteed that there is a unique solution satisfying both Ohm's law and Kirchhoff's current law.

**Note from the problem author:** It is recommended to use a programming language that is not too slow (like C/C++) to solve this problem.

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

The solved circuit is shown in the diagram below, where ~V~, ~A~, ~\Omega~ are units for electric potential, current, and resistance. Note that the solution for electric potentials is not unique, since the solution is still valid after adding all electric potentials by a constant. Also note that this is not a formal circuit diagram.

<latex>
\documentclass{standalone}
\usepackage{tikz}
\usetikzlibrary{arrows}
\usetikzlibrary{positioning}
\usetikzlibrary{shapes.geometric}
\usepackage{amsmath}
\begin{document}
\begin{tikzpicture}[
endnode/.style={diamond, draw=black, fill=none, very thick, minimum size=1em},
node/.style={circle, draw=black, fill=none, very thick, minimum size=1em},
]
\node[endnode, label={$36V$}](n1){1};
\node[node, label={$27V$}](n2)[right=of n1]{2};
\node[node, label={$12V$}](n3)[right=of n2]{3};
\node[endnode, label={$0V$}](n4)[right=of n3] {4};
\draw[->] (n1.east) -- node[above]{$6\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$1.5A$}++(0.99,0) -- (n2.west);
\draw[->] (n2.east) -- node[above]{$10\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$1.5A$}++(0.99,0) -- (n3.west);
\draw[->] (n3.east) -- node[above]{$8\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$1.5A$}++(0.99,0) -- (n4.west);
\end{tikzpicture}
\end{document}
</latex>

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

<latex>
\documentclass{standalone}
\usepackage{tikz}
\usetikzlibrary{arrows}
\usetikzlibrary{positioning}
\usetikzlibrary{shapes.geometric}
\usepackage{amsmath}
\begin{document}
\begin{tikzpicture}[
endnode/.style={diamond, draw=black, fill=none, very thick, minimum size=1em},
node/.style={circle, draw=black, fill=none, very thick, minimum size=1em},
]
\node[endnode, label={$43V$}](n1){1};
\node[node, label={$28V$}](n2)[right=of n1]{2};
\node[node, label=left:{$24V$}](n3)[below=of n2]{3};
\node[node, label=right:{$16V$}](n4)[right=of n3]{4};
\node[node, label={$10V$}](n5)[right=of n2]{5};
\node[endnode, label={$0V$}](n6)[right=of n5]{6};
\draw[->] (n1.east) -- node[above]{$6\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$2.5A$}++(0.99,0) -- (n2.west);
\draw[->] (n2.east) -- node[above]{$12\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$1.5A$}++(0.99,0) -- (n5.west);
\draw[->] (n5.east) -- node[above]{$4\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$2.5A$}++(0.99,0) -- (n6.west);
\draw[->] (n2.south) -- node[left]{$4\Omega$}++(0,-0.99) -- node{}++(0,0.99) -- node[right]{$1A$}++(0,-0.99) -- (n3.north);
\draw[->] (n3.east) -- node[above]{$8\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$1A$}++(0.99,0) -- (n4.west);
\draw[->] (n4.north) -- node[left]{$6\Omega$}++(0,0.99) -- node{}++(0,-0.99) -- node[right]{$1A$}++(0,0.99) -- (n5.south);
\end{tikzpicture}
\end{document}
</latex>

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

## Explanation for Sample 3

For compactness, all values in the diagram are rounded to 2 decimal places. You program still needs to output enough decimal places to pass the autograder.

<latex>
\documentclass{standalone}
\usepackage{tikz}
\usetikzlibrary{arrows}
\usetikzlibrary{positioning}
\usetikzlibrary{shapes.geometric}
\usepackage{amsmath}
\begin{document}
\begin{tikzpicture}[
endnode/.style={diamond, draw=black, fill=none, very thick, minimum size=1em},
node/.style={circle, draw=black, fill=none, very thick, minimum size=1em},
]
\node[endnode, label={$5.11V$}](n1){1};
\node[node, label={$-0.22V$}](n2)[right=of n1]{2};
\node[node, label=below:{$0V$}](n3)[below=of n2]{3};
\node[endnode, label=below:{$-4.89V$}](n4)[right=of n3]{4};
\draw[->] (n1.east) -- node[above]{$6\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$0.89A$}++(0.99,0) -- (n2.west);
\draw[->] (n3.east) -- node[above]{$5\Omega$}++(0.99,0) -- node{}++(-0.99,0) -- node[below]{$0.98A$}++(0.99,0) -- (n4.west);
\draw[->] (n2.south) -- node[]{$\begin{matrix}5\Omega\\-0.04A\end{matrix}$}++(0,-0.99) -- (n3.north);
\draw[->] (n1.south) -- node[below]{$\begin{matrix}5\Omega\\1.02A\end{matrix}$}++(0.7,-0.6) -- (n3.west);
\draw[->] (n2.east) -- node[right]{$\begin{matrix}5\Omega\\0.93A\end{matrix}$}++(0.7,-0.6) -- (n4.north);
\end{tikzpicture}
\end{document}
</latex>
