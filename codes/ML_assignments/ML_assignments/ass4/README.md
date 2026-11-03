# ML Assignment 4: Gradient Descent Algorithm

---

## 1. Problem Statement & SPPU Syllabus Mapping
Implement the **Gradient Descent Algorithm** from scratch to find the local minima of a function.
- Specifically find the local minima of:
  $$y = (x + 3)^2$$
  starting from the initial point:
  $$x_0 = 2$$

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 What is Gradient Descent?
Gradient Descent is the foundational optimization algorithm of Machine Learning and Deep Learning. 
- **Analogy**: Imagine you are blindfolded on a foggy mountain and want to reach the lowest point of the valley. You feel the slope of the ground under your feet:
  - If the slope goes down towards the left, you step left.
  - If the slope goes down towards the right, you step right.
  - You keep taking small steps downhill until the ground feels flat under your feet (slope $\approx 0$).

### 2.2 Mathematical Derivation
Let the objective function be:
$$y = f(x) = (x + 3)^2$$

1. **Calculate the Derivative (Slope / Gradient)**:
   Using power rule and chain rule:
   $$f'(x) = \frac{dy}{dx} = 2(x + 3) \cdot \frac{d}{dx}(x + 3) = 2(x + 3)$$

2. **The Update Rule**:
   To move opposite to the direction of steepest increase, we subtract the gradient scaled by a step size $\eta$ (learning rate):
   $$x_{t+1} = x_t - \eta \cdot f'(x_t)$$

3. **Theoretical Minimum**:
   Setting $f'(x) = 0$:
   $$2(x + 3) = 0 \implies x = -3$$
   $$y_{\min} = (-3 + 3)^2 = 0$$
   Therefore, the true minimum is located at **$x^* = -3$**, where **$y^* = 0$**.

### 2.3 The Role of the Learning Rate ($\eta$)
The learning rate controls how big of a step we take at each iteration:
- **Too Small ($\eta = 0.001$)**: The algorithm takes minuscule steps; takes thousands of iterations to converge.
- **Optimal ($\eta = 0.1$ to $0.2$)**: Smooth, steady, fast convergence directly to $x = -3$.
- **Too Large ($\eta = 0.8$ to $0.9$)**: Overshoots back and forth across the valley (oscillates).
- **Catastrophic ($\eta \ge 1.0$)**: Diverges! When $\eta = 1.0$, $x_{t+1} = x_t - 1.0 \times 2(x_t + 3) = -x_t - 6$. When $\eta > 1.0$, steps get exponentially larger and explode to $\pm \infty$.

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Function and Gradient Definitions
```python
def objective_function(x):
    return (x + 3)**2

def derivative(x):
    return 2 * (x + 3)
```

### Step 2: Implementation of Gradient Descent from Scratch
```python
def gradient_descent(func, grad, start_x, learning_rate=0.1, precision=1e-6, max_iters=1000):
    x = start_x
    history = []
    
    for i in range(max_iters):
        y = func(x)
        slope = grad(x)
        history.append({'iteration': i, 'x': x, 'y': y, 'gradient': slope})
        
        step = learning_rate * slope
        # Stopping criteria: step smaller than precision threshold
        if abs(step) < precision:
            break
            
        x = x - step
        
    return x, history
```

### Step 3: Execution from $x_0 = 2$ with $\eta = 0.1$
```python
optimal_x, history = gradient_descent(
    objective_function, derivative,
    start_x=2.0, learning_rate=0.1, precision=1e-6, max_iters=100
)
```

---

## 4. Iteration Trace (First 5 Iterations & Final Convergence)

| Iteration | Current $x_t$ | Objective $y = (x_t+3)^2$ | Gradient $f'(x_t) = 2(x+3)$ | Step $\eta \cdot f'(x)$ | Next $x_{t+1}$ |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | **2.000000** | 25.000000 | 10.000000 | 1.000000 | 1.000000 |
| **1** | **1.000000** | 16.000000 | 8.000000 | 0.800000 | 0.200000 |
| **2** | **0.200000** | 10.240000 | 6.400000 | 0.640000 | -0.440000 |
| **3** | **-0.440000** | 6.553600 | 5.120000 | 0.512000 | -0.952000 |
| **4** | **-0.952000** | 4.194304 | 4.096000 | 0.409600 | -1.361600 |
| ... | ... | ... | ... | ... | ... |
| **~40** | **-2.999995** | $\approx 0.000000$ | $0.000009$ | $< 10^{-6}$ | **-3.000000** |

- **Result**: Algorithm smoothly converges to **$x \approx -3.0000$** and minimum value **$y \approx 0.0000$** in ~40 iterations.

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: What is a gradient mathematically?**
> *Answer:* A gradient is the vector of first-order partial derivatives of a multi-variable function ($\nabla f$). It points in the direction of the greatest rate of increase of the function. Its magnitude indicates how steep the slope is.

**Q2: Why do we subtract the gradient instead of adding it?**
> *Answer:* Because the gradient points in the direction of *steepest ascent* (uphill). Since our goal is minimization, we move in the opposite direction (steepest descent) by subtracting the gradient ($-\eta \nabla f$). Adding it would perform Gradient *Ascent* (maximization).

**Q3: What are the primary stopping criteria for Gradient Descent?**
> *Answer:*
> 1. Absolute change in $x$ between successive iterations drops below a tolerance: $|x_{t+1} - x_t| < \epsilon$.
> 2. The gradient magnitude is approximately zero: $\|\nabla f(x)\| < \epsilon$.
> 3. Reaching a pre-defined maximum number of iterations (`max_iters`).

**Q4: What is the difference between Batch Gradient Descent, Stochastic Gradient Descent (SGD), and Mini-batch Gradient Descent?**
> *Answer:*
> - **Batch GD**: Computes the gradient over the entire dataset before making one update. Highly stable, but slow on large datasets.
> - **SGD**: Computes the gradient and updates weights using only one random sample at a time. Very fast, but noisy and oscillates.
> - **Mini-batch GD**: The industry standard; computes gradients on small batches (e.g. 32, 64 samples), combining speed with computational stability.

**Q5: Can Gradient Descent get stuck in local minima? How is that solved?**
> *Answer:* Yes, for non-convex loss surfaces, standard GD can get trapped in sub-optimal local minima or saddle points. Solutions include:
> 1. Adding **Momentum** (incorporating past velocity).
> 2. Using adaptive optimizers like **Adam** or **RMSprop**.
> 3. Random restarts from different initial points ($x_0$).
