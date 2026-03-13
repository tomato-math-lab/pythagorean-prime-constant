## 🍅 The Tomato-Math-Lab Theorem: The Dual-Prime Pythagorean Conjecture

### 1. The Structure

For any $n \in \mathcal{N}_{tomato}$, we define a unique Pythagorean triple $\{a, b, c\}$ where both $a$ and $c$ are prime numbers:

$$a = 2n + 1 \in \mathbb{P}$$

$$c = 2n^2 + 2n + 1 \in \mathbb{P}$$

The remaining side $b$ is given by:

$$b = 2n(n+1)$$

### 2. The Series

We define the **Tomato Series** $S$ as the sum of the reciprocals of these specific $n$ values:

$$S = \sum_{\substack{2n+1 \in \mathbb{P} \\ 2n^2+2n+1 \in \mathbb{P}}} \frac{1}{n}$$

### 3. The Discovery

Numerical evidence up to $n = 56,446,464$ suggests that this series converges to a "deep irrational pit":

$$S = 1 + \frac{1}{2} + \frac{1}{5} + \frac{1}{9} + \frac{1}{14} + \dots \longrightarrow \frac{3\pi}{4}$$

------

### 🛠️ Verification (How to Run)
To reproduce the results, use the Microsoft Visual C++ compiler (VS2022). Run the following command in the Developer PowerShell/Command Prompt:

```bash
cl /O2 /W3 dpex.c
dpex.exe
```