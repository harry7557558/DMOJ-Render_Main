# Grade-school CIV

How to solve for the deflections of all joints of a truss using ESC103:

Let the member forces be $F_M$ and applied forces be $F_A$. Both are column vectors.

Find the matrix $C$ such that $CF_M=F_A$, like you did in ESC103 final problem 5.

Then, $C^TCF_M=C^TF_A$, and $F_M=(C^TC)^{-1}C^TF_A$.

The principle of virtual work states $P^\ast\Delta u = \sum\dfrac{F^\ast F_ML}{EA}$, or in matrix form, $CF^\ast=P^\ast I$ if we consider all joints.

Let $P^\ast=1$ unit and $K=\frac{EA}{L}$ be a diagonal matrix of axial stiffnesses. Then, $\Delta u = (F^\ast)^TK^{-1}F_M$.

To find $F^\ast$ using the principle of virtual work, $C^TCF^\ast=C^TI$, or $F^\ast=(C^TC)^{-1}C^T$, $(F^\ast)^T=C(C^TC)^{-1}$.

To find the deflection, first find $F_M$, then find $\Delta L=K^{-1}F_M$, and evaluate $\Delta u=C(C^TC)^{-1}\Delta L$.

For large trusses, most elements of $C$ are zero, and you can take shortcuts when evaluating $Cq$, $C^Tq$, and $(C^TC)^{-1}q$.

If you can solve $C^TCx=b$ in $O(N)$ time, then you can solve for all deflections of a truss in $O(N)$ time.

----

Limitations:

 - Finding matrix $C$ involves solving for reaction forces. More than 2 fixed connections -> statically indeterminate
 - Statically indeterminate truss?? An example is a box with crosses.


# Find deflections first?

Here, we assume the matrix $C$ only considers the internal part, and not the reaction forces.

Let $F_a$ represents applied forces and $F_r$ represents reaction forces and $F_A=F_a+F_r$.

 - $F_r$ can't be trivially solved in 3D? Can rotate about an axis for 2 pin supports, more unknowns than equations for >=3 pin supports.

The equation for the deflection is $u=C(C^TC)^{-1}K^{-1}(C^TC)^{-1}C^TF_A$.

 - If $C^TCF_M=C^TF_A$ has solution(s) but $CF_M=F_A$ doesn't, then $F_A$ isn't at equilibrium.

$u=C(C^TC)^{-1}\Delta L \implies \Delta L=C^Tu$

 - The RHS of this equation isn't affected by statically indeterminates. Is there a way to find $u$ without going through $F_M$, and then find $F_M$ with $K\Delta L$?

$F_M = K C^T u$, $F_A = C K C^T u$.

 - Can be used to find the deflection when $C^TC$ is not full-rank but $CKC^T$ is?

 - Need a more robust way to solve this problem.

Energy: $E = \dfrac{1}{2}\Delta L^T K \Delta L = \dfrac{1}{2}u^TCKC^Tu = \dfrac{1}{2}u^TF_A$.

Find $u$ to minimize energy subject to:
 - $CKC^Tu = F_A$
 - At a fixed point $i$, the deflection $u_i=0$
 - Equilibrium: $\sum F_{ai} + \sum F_{ri} = 0$, $\sum r_i \times F_{ai} + \sum r_i \times F_{ri} = 0$

Second constraint
 - Shrink $u$ to only consider non-fixed points?
 - $u^TF_A=u^T(F_r+F_a)$, $u^TF_r$ is zero because fixed points do not move.

Third constraint
 - Cross product is a linear operator, denoted as $r^\times$.
 - Ignore applied forces at fixed joints because they don't affect deflection; Separate $F_a$ and $F_r$ so they don't share common $i$;
 - $F_a$ is fixed/given, $F_r$ needs to be solved/optimized for;
 - Maybe also put $F_r$ into the list of variables to optimize?

Reformulate:
 - $u$ is a list of deflections at non-fixed joints
 - $H$ is a "shrinked" version of $CKC^T$ to comply with the "shrink" of $u$
 - $S$ is a matrix that maps a "shrinked" $F_r$ to a full-sized force vector
 - $F_a$ is a vector of applied forces, zero at fixed joints
 - Sum of reactions $T=\sum[F_a;r\times F_a]$, reaction operator $R=[\mathbf{1}^T; (r^T)^\times]$
   - Assume $R$ is full rank (6)
 - Find $u$ and $F_r$ to minimize $F_a^Tu$, subject to:
   - $Hu-SF_r=F_a$
   - $RF_r+T=0$
 - There must be something "special" with $H$ for a linear function to have a global minimum

$H$: $CKC^T$ after removing extra columns?
 - $M$ members, $N=N_r+N_a$ joints
 - $CKC^T$ has a dimension of $3N\times3N$
 - $H$ has a dimension of $3N\times3N_a$ because shrinked $u$ has a dimension of $N_a$
 - $H$ is rank-deficient in general because $C$ is often "tall and narrow"
 - Each member has 2 joints therefore each column of $C$ has 2 nonzero elements; $CKC^T$ is sparse therefore $H$ is sparse

Optimization

 - Shapes (without x3): $u$=$N_a$, $F_a$=$N$, $F_r$=$N_r$, $C$=$(N,M)$, $H$=$(N,N_a)$, $S$=$(N,N_r)$, $R$=$(2,N_r)$, $T$=$2$, $\lambda_1$=$N$, $\lambda_2$=$2$

 - Lagrange multiplier $L(u,F_r,\lambda_1,\lambda_2)=F_a^Tu+\lambda_1^T(Hu-SF_r-F_a)+\lambda_2^T(RF_r+T)$
   - $H^T\lambda_1=-F_a$, ($N_a$ equations)
   - $-S^T\lambda_1+R^T\lambda_2=0$, ($N_r$ equations)
   - $Hu-SF_r=F_a$, ($N$ equations)
   - $RF_r=-T$, (2 equations)
   - Number of equations: $N_a+N_r+N+2$
   - Number of unknowns: $N_a+N_r+N+2$
 -
 - $\displaystyle\begin{bmatrix}&&H^T&\\&&-S^T&R^T\\H&-S&&\\&R&&\end{bmatrix}\begin{bmatrix}u\\F_r\\\lambda_1\\\lambda_2\end{bmatrix}=\begin{bmatrix}-F_a\\0\\F_a\\-T\end{bmatrix}$
 - Can $u$ and $F_r$ be uniquely solved? Matrix is rank-deficient??
   - Possible explaination: energies are the same for all statically indeterminate solutions.


# Secondary research - I should have done this first, praxis profs are right.

Contains the equation $F_A=CKC^Tu$: http://web.mit.edu/emech/admin/OldFiles/dontindex-build/full-text/emechbk_5.pdf

**https://engineering.purdue.edu/~aprakas/CE474/CE474-Ch5-StiffnessMethod.pdf**

 - Find stiffness matrix $CKC^T$
 - Split into fixed and nonfixed parts
 - The nonfixed part is guaranteed to be full-rank?!


