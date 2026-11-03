# ML Assignment 3: Bank Customer Churn Prediction using Artificial Neural Networks (ANN)

---

## 1. Problem Statement & SPPU Syllabus Mapping
Given a bank customer's profile, build a neural network-based classifier that determines whether the customer will leave (churn = 1) or stay (churn = 0) in the next 6 months.

### Required Tasks:
1. **Read the dataset** (`Churn_Modelling.csv`, 10,000 samples, 14 features).
2. **Distinguish feature ($X$) and target ($y$) set** and divide into training (80%) and testing (20%) sets.
3. **Normalize the train and test data** (StandardScaler).
4. **Initialize and build the model**, identify points of improvement, and implement them.
5. **Print accuracy score and confusion matrix**.

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 What is Customer Churn?
Customer churn refers to the percentage of customers who stop doing business with an entity (in this case, closing their bank accounts). Predicting churn helps banks proactively offer incentives (loans, rewards) before customers leave.

### 2.2 What is an Artificial Neural Network (ANN)?
An ANN is inspired by biological neurons in the human brain:
1. **Input Layer**: Receives features ($x_1, x_2, \dots, x_m$ like credit score, age, balance).
2. **Hidden Layer(s)**: Intermediate neurons that extract non-linear patterns.
   - Each neuron computes a weighted sum plus a bias:
     $$z = \sum_{j=1}^m w_j x_j + b = \mathbf{w}^T \mathbf{x} + b$$
   - This sum is passed through an **Activation Function** $a = f(z)$.
3. **Output Layer**: Produces the final prediction probability $\hat{y} \in [0, 1]$.

```
[Inputs: Age, Balance, etc.]
            \
             ---> [Hidden Layer: Dense + ReLU] ---> [Hidden Layer: Dense + ReLU] ---> [Output: Sigmoid (0 to 1)]
            /
```

### 2.3 Key Deep Learning Terms Explained:
- **Activation Functions**:
  - **ReLU (Rectified Linear Unit)**: $f(z) = \max(0, z)$. Fast, eliminates the vanishing gradient problem for positive inputs; used in hidden layers.
  - **Sigmoid**: $\sigma(z) = \frac{1}{1 + e^{-z}}$. Maps any number to $(0, 1)$; ideal for the output neuron in binary classification (probability of churn).
- **Loss Function — Binary Cross-Entropy**:
  $$L(y, \hat{y}) = -\left[ y \log(\hat{y}) + (1 - y) \log(1 - \hat{y}) \right]$$
  Penalizes confident wrong predictions heavily.
- **Optimizer — Adam (Adaptive Moment Estimation)**:
  An advanced gradient descent algorithm that computes adaptive learning rates for each parameter using running averages of both gradients and squared gradients (combining Momentum + RMSprop).

### 2.4 Why Address Class Imbalance? (Points of Improvement)
- In the Kaggle dataset:
  - Stayed (0): ~80%
  - Churned (1): ~20%
- If a naive model predicts `0` for every customer, it achieves **80% accuracy** while catching 0 churners!
- **Identified Points of Improvement**:
  1. **Compute Class Weights**: Penalize the model more when it misses a churner (minority class):
     $$w_1 = \frac{N_{\text{total}}}{2 \times N_{\text{churn}}}$$
  2. **Add Dropout Regularization**: Randomly deactivate 15-20% of neurons during training to prevent co-adaptation and overfitting.
  3. **Batch Normalization**: Stabilizes and speeds up training by zero-centering inputs to each layer.
  4. **Callbacks (EarlyStopping & ReduceLROnPlateau)**: Stop training when validation loss stops improving, preventing over-training.
  5. **Threshold Tuning**: Lower decision threshold from 0.50 to ~0.35–0.40 to capture more churners.

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Data Cleaning & Feature Separation
```python
df = pd.read_csv('Churn_Modelling.csv')
# RowNumber, CustomerId, Surname have no predictive power
X = df.drop(columns=['RowNumber', 'CustomerId', 'Surname', 'Exited'])
y = df['Exited']

# One-hot encode categorical features ('Geography' and 'Gender')
X = pd.get_dummies(X, columns=['Geography', 'Gender'], drop_first=True)
```

### Step 2: Train-Test Split & Normalization
```python
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.20, random_state=42, stratify=y
)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
```

### Step 3: Baseline Neural Network Architecture
```python
baseline_model = Sequential([
    Dense(16, activation='relu', input_dim=X_train_scaled.shape[1]),
    Dense(8, activation='relu'),
    Dense(1, activation='sigmoid')
])
baseline_model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
baseline_model.fit(X_train_scaled, y_train, epochs=40, batch_size=32, validation_split=0.2)
```

### Step 4: Improved Neural Network Architecture
```python
# Compute class weights
class_weights = compute_class_weight('balanced', classes=np.unique(y_train), y=y_train)
weight_dict = {0: class_weights[0], 1: class_weights[1]}

improved_model = Sequential([
    Dense(32, activation='relu', input_dim=X_train_scaled.shape[1]),
    BatchNormalization(),
    Dropout(0.2),
    Dense(16, activation='relu'),
    BatchNormalization(),
    Dropout(0.15),
    Dense(1, activation='sigmoid')
])

callbacks = [
    EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True),
    ReduceLROnPlateau(monitor='val_loss', factor=0.5, patience=5)
]

improved_model.fit(
    X_train_scaled, y_train,
    epochs=60, batch_size=32,
    validation_split=0.2,
    class_weight=weight_dict,
    callbacks=callbacks
)
```

---

## 4. Evaluation & Performance Comparison

| Metric | Baseline ANN | Improved ANN (with Class Weights & Dropout) |
| :--- | :---: | :---: |
| **Accuracy** | ~85.5% | ~81.5% - 83.5% |
| **Recall (Churn = 1)** | ~45% - 50% | **~75% - 80%** (Massive Improvement!) |
| **Precision (Churn = 1)** | ~70% | ~53% - 58% |
| **F1-Score (Churn = 1)** | ~0.55 | **~0.63 - 0.66** |
| **ROC-AUC** | ~0.84 | **~0.86 - 0.87** |

> **Key Examiner Takeaway**: While baseline accuracy looks slightly higher, it missed over half the churners! The improved model catches **~75-80% of churners**, saving the bank significant revenue. In churn modeling, **Recall is far more important than raw Accuracy**.

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: Why did you use `drop_first=True` during One-Hot Encoding?**
> *Answer:* To avoid the **Dummy Variable Trap** (multicollinearity). If a customer can only be from France, Germany, or Spain, knowing they are not from France and not from Spain guarantees they are from Germany. Including all three creates linearly dependent columns.

**Q2: What is the purpose of Dropout in Deep Learning?**
> *Answer:* Dropout is a regularization technique where random neurons are deactivated during training with probability $p$. This prevents neurons from co-adapting and memorizing the training data, drastically reducing overfitting.

**Q3: Why is Sigmoid used in the final layer instead of ReLU?**
> *Answer:* Sigmoid outputs values strictly bounded between 0 and 1, which can be interpreted directly as probabilities ($\hat{y} = P(\text{Churn}=1|\mathbf{x})$). ReLU outputs unbounded values from 0 to $+\infty$, which cannot represent binary probability.

**Q4: What is Early Stopping and why is it useful?**
> *Answer:* Early Stopping monitors a validation metric (such as `val_loss`). When the validation metric stops improving for a specified number of epochs (`patience`), training is halted and weights from the best epoch are restored, preventing over-training.

**Q5: What is the difference between an Epoch, a Batch, and an Iteration?**
> *Answer:*
> - **Epoch**: One complete forward and backward pass of the entire training dataset.
> - **Batch Size**: The number of training samples processed before updating the model weights.
> - **Iteration**: The number of batches needed to complete one epoch ($N / \text{batch\_size}$).
