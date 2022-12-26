[user:harry7557558] just finished his first term in university. A common problem that University of Toronto engineering science students are asked to do in their first years is analyzing a truss structure, where [user:harry7557558] has learned the method of joints and the method of sections to find the forces in truss members, as well as the principle of virtual work for determining truss deflection. However, ~~frustrated with marks he lost in quizzes due to mistakes~~ unsatisfied with the amount of calculation involved in applying these methods by hand, [user:harry7557558] is going to write a program to solve trusses. He indeed has already have his truss solver script working, and he's challenging you to write something similar.

A truss is a structure consisting of joints and members. A joint can be imagined as a hinge where members are connected to, which is either a free (movable) joint or a pin (fixed) joint in this problem. A member is a structural component connecting two joints that can carry tension or compression, where tension is represented as a positive internal force and compression is represented as a negative one. The force exerted by a member on a joint has the same magnitude as the member's internal force, and its direction is toward the member when the member's internal force is positive and away from the member when the internal force is negative.

External forces like gravity are applied to joints. Newton's second law states when an object is in equilibrium, the sum of all forces acting on it must be zero. When Newton's second law is applied to a joint, the sum of applied forces and forces exerted by members equals zero. When Newton's second law is applied to the entire truss structure, fixed joints must generate reaction forces to balance applied forces.

The following diagram shows a 2D truss with ~2~ fixed joints, ~3~ free joints, and ~7~ members. One member is subjected to a ~100\mathrm{kN}~ applied force.

[truss diagram]

To analyze a truss subject to applied forces, [user:harry7557558] was taught to first find the reaction forces from the equilibrium of forces and [moments](https://en.wikipedia.org/wiki/Torque), and then solve for all member forces using equilibrium at free joints. However, this does not work for all cases. For example, the following 2D truss has ~3~ fixed joints and therefore ~6~ force components, but there are only ~3~ equations from the equilibrium of force and moment. In addition, the truss has ~10~ member forces to solve for, but there are only ~8~ equilibrium equations from ~4~ free joints. Therefore, neither reaction forces nor member forces can be solved uniquely from equilibrium.

[statically indeterminate truss]

[user:harry7557558] intuitively thought about solving this type of structures from deflection, or, the deviation of free joints from the original position after the force is applied. The following method (called the principle of virtual work) was taught to [user:harry7557558] to find truss deflection ~\delta~, assuming the reaction and member forces can be uniquely solved from equilibrium. Note that the method only finds a linear approximation of the deflection of a single free joint in a particular direction.

 - Solve for the forces in truss members subject to applied forces, denoted as ~F~;
 - Apply a dummy force ~P^\ast~ on the joint of interest in the direction of interest;
 - Solve for the truss member forces subject to only the dummy force, denoted as ~F^\ast~;
 - Solve ~P^\ast\delta=\sum_{i}F^\ast_ik_iF_i~ to find the deflection ~\delta~.

Here, ~k~ is the axial stiffness of the member, which is the same as the spring constant you may have seen in grade-school physics courses. For truss members with identical materials and cross section areas, ~k~ is inversely proportional to the member length ~L~, and their product ~kL=EA~ will be given to you in this problem. ~L~ is the Euclidean distance between the initial locations of the two joints the member connects to.

Starting from equilibrium and the principle of virtual work, [user:harry7557558] discovered a way to find all reaction forces, member forces, and deflections in a truss. He tested his method on several 3D trusses. Can you beat him?


# Input Specification

The first line of input contains four integers, ~N~, the number of joints in the truss, ~S~, the number of fixed joints, ~M~, the number of members, and ~EA~, the product of axial stiffness ~k~ and length ~L~ for members in Newtons.

The next ~N~ lines each contains four integers, ~x_i~, ~y_i~, ~z_i~, the coordinates of the ~i~th joint in millimeters, and ~w_i~, representing the force applied on the ~i~th joint is ~(0, 0, -w_i)~ in Newtons.

The next line contains ~S~ integers between ~1~ and ~N~, the indices of fixed joints.

The next two lines each contains ~M~ integers, ~1\lt a_j\lt N~ and ~1\lt b_j\lt N~ for ~1\lt j\lt M~, representing there is a member connecting joints with indices ~a_j~ and ~b_j~.


# Output Specification

Output ~M~ numbers, the forces in each member, on a single line, separated by ~M-1~ spaces. Output a positive number for tension and a negative number for compression. [user:harry7557558] only asks you for the member forces because it is the last step in his "intended" solution. Since [user:harry7557558] hates sig figs, in this problem, you can pass if all numbers in your program's output have an absolute or relative error less than ~10^{-3}~ compared to [user:harry7557558]'s solution.


# Sample Input

```
8 4 16 60000000
0 0 0 0
2000 0 0 0
4000 0 0 0
0 2000 0 0
2000 2000 0 0
4000 2000 0 0
500 1000 -1000 5000
3500 1000 -1000 1000
1 2 3 6
1 1 1 2 2 2 2 3 3 4 4 5 5 5 6 7
2 4 7 3 5 7 8 6 8 5 7 6 7 8 8 8
```

# Sample Output

```
0 0 5959 0 0 -3036 5210 0 -3041 0 0 7500 5154 -5154 4500 446.5
```

You are allowed to have more decimal places in your output.


# Visual of the Sample Case

Black arrows represent applied forces and magenta arrows represent reaction forces. A red/orange color on a member represents a high tension force, a blue color represents a high compression force, and members with smaller (including zero) forces are in green.


# Test Data

There are ~12~ test cases for this problem worth equal points. All test cases have the following constraints:
 - For each connected component in the truss, there are at least ~3~ non-parallel fixed joints.
 - There is no part in the truss that can rotate freely around another part. In simple words, you won't see a "hanging chain" or a "movable hinge."
 - For arbitrary joint ~i~ and ~j~, the deflection-force gradient ~\dfrac{\partial u_j}{\partial P_i}~ exists. In other words, the linear approximation in the principle of virtual work applies.

The parameters for each test case is given in the following table:




