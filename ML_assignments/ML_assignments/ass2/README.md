# ML Assignment 2: Email Spam Detection using KNN and SVM

---

## 1. Problem Statement & SPPU Syllabus Mapping
Classify emails into:
- **Normal State (0)**: Not Spam (Ham)
- **Abnormal State (1)**: Spam

### Required Tasks:
1. Load and explore the `emails.csv` dataset (3000 word-frequency features).
2. Preprocess data (drop identifier `Email No.`, separate $X$ and $y$, train-test split 80:20, feature scaling).
3. Implement **K-Nearest Neighbors (KNN)** and tune hyperparameter $K$.
4. Implement **Support Vector Machine (SVM)** and test linear vs non-linear (RBF) kernels.
5. Analyze and compare their performance using **Accuracy, Precision, Recall, F1-Score, Confusion Matrix, and ROC-AUC curves**.

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 The Email Spam Problem as Binary Classification
We are given an email represented as a row of 3,000 numbers. Each column represents a specific English word (e.g. `"free"`, `"offer"`, `"meeting"`, `"urgent"`), and the value is how many times that word appears in that email.
- **Input vector $X \in \mathbb{R}^{3000}$**: Word frequency counts.
- **Output $y \in \{0, 1\}$**: 0 for Normal (Ham), 1 for Spam.

### 2.2 K-Nearest Neighbors (KNN)
- **Analogy**: *"Tell me who your 5 closest neighbors are, and I'll tell you who you are."*
- **Mechanism**:
  1. KNN stores all training samples in memory (it is a **lazy learner**; no explicit training phase).
  2. When a new email arrives, it calculates the **Euclidean distance** from this email to every training email:
     $$d(p, q) = \sqrt{\sum_{i=1}^D (p_i - q_i)^2}$$
  3. It picks the $K$ closest neighbors and takes a **majority vote**.
- **Choosing $K$**:
  - $K=1$: Overfits easily (memorizes noise and typos).
  - Very large $K$: Underfits (smooths out boundaries, dominated by majority class).
  - Rule of thumb: $K = \sqrt{N}$ and an odd number to avoid tie-breaks.

### 2.3 Support Vector Machines (SVM)
- **Analogy**: Drawing the widest possible highway between two rival camps.
- **Mechanism**:
  1. SVM finds an optimal hyperplane $\mathbf{w}^T \mathbf{x} + b = 0$ that separates Spam from Not-Spam with the **maximum margin** ($2 / \|\mathbf{w}\|$).
  2. The points closest to the hyperplane that dictate its orientation and width are called **Support Vectors**.
- **Kernel Trick**:
  - What if the classes cannot be separated by a straight line or flat plane?
  - SVM uses a mathematical mapping function $\phi(\mathbf{x})$ to project data into a higher-dimensional space where linear separation is possible, without explicitly calculating the coordinates (**Kernel Trick**).
  - Common Kernels:
    - **Linear**: $K(x, z) = x^T z$ (Fast, ideal for high-dimensional text data).
    - **RBF (Radial Basis Function)**: $K(x, z) = \exp(-\gamma \|x - z\|^2)$ (Captures complex non-linear spheres/islands).

### 2.4 Why Feature Scaling (StandardScaler) is Crucial?
- Both KNN and SVM rely heavily on geometric distance:
  - If feature $A$ (e.g. word `"the"`) ranges from 0 to 50, and feature $B$ (e.g. word `"lottery"`) ranges from 0 to 1, feature $A$ would artificially dominate distance by a factor of 2500!
  - `StandardScaler` standardizes each feature to mean $\mu = 0$ and standard deviation $\sigma = 1$:
    $$z = \frac{x - \mu}{\sigma}$$

### 2.5 Evaluation Metrics for Email Classification
In spam filtering, **False Positives** (marking an important college/job email as Spam) are much worse than **False Negatives** (letting a junk email into the inbox).
- **Confusion Matrix**:
  - **TP (True Positive)**: Spam correctly caught as Spam.
  - **TN (True Negative)**: Ham correctly left in Inbox.
  - **FP (False Positive)**: Important Ham wrongly thrown into Spam (Worst disaster!).
  - **FN (False Negative)**: Spam leaked into Inbox (Mild annoyance).
- **Formulas**:
  $$\text{Accuracy} = \frac{\text{TP} + \text{TN}}{\text{TP} + \text{TN} + \text{FP} + \text{FN}}$$
  $$\text{Precision} = \frac{\text{TP}}{\text{TP} + \text{FP}} \quad (\text{How many identified spams were actually spam?})$$
  $$\text{Recall} = \frac{\text{TP}}{\text{TP} + \text{FN}} \quad (\text{What \% of total spam emails did we catch?})$$
  $$\text{F1-Score} = 2 \times \frac{\text{Precision} \times \text{Recall}}{\text{Precision} + \text{Recall}}$$

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Loading Dataset & Splitting Features
```python
df = pd.read_csv('emails.csv')
# emails.csv structure: 'Email No.', 3000 word columns, 'Prediction'
X = df.drop(columns=['Email No.', 'Prediction'])
y = df['Prediction']
```

### Step 2: 80:20 Train-Test Split & Scaling
```python
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.20, random_state=42, stratify=y
)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)  # Note: transform only, prevents leakage!
```

### Step 3: Finding Optimal $K$ for KNN (Elbow Method)
```python
k_values = [1, 3, 5, 7, 9, 11, 15]
error_rates = []
for k in k_values:
    knn = KNeighborsClassifier(n_neighbors=k)
    knn.fit(X_train_scaled, y_train)
    preds = knn.predict(X_test_scaled)
    error_rates.append(np.mean(preds != y_test))
```
$K=5$ or $K=7$ yields the lowest test error while preventing overfitting.

### Step 4: Training SVM & Kernel Comparison
```python
kernels = ['linear', 'rbf', 'poly', 'sigmoid']
for k in kernels:
    svm_temp = SVC(kernel=k, random_state=42)
    svm_temp.fit(X_train_scaled, y_train)
    # Evaluate accuracy and execution time
```
SVM with `kernel='linear'` or `kernel='rbf'` achieves exceptional accuracy (> 98%).

### Step 5: Visualizing Decision Boundary using PCA (2D)
```python
pca = PCA(n_components=2)
X_train_pca = pca.fit_transform(X_train_scaled)
# Train models on 2D space to draw contour decision boundaries
```

---

## 4. Performance Comparison: KNN vs SVM

| Metric | K-Nearest Neighbors (KNN, $K=5$) | Support Vector Machine (SVM, Linear/RBF) | Superior Model |
| :--- | :---: | :---: | :---: |
| **Accuracy** | ~87% - 90% | **~98% - 99%** | **SVM** |
| **Precision** | ~75% - 82% | **~96% - 98%** | **SVM** |
| **Recall** | ~80% - 85% | **~96% - 98%** | **SVM** |
| **F1-Score** | ~0.80 - 0.85 | **~0.97 - 0.98** | **SVM** |
| **ROC-AUC** | ~0.92 | **~0.99** | **SVM** |
| **Inference Speed** | Slow ($O(N \cdot D)$ per email) | Instant ($O(N_{\text{support}} \cdot D)$) | **SVM** |

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: Why does SVM dramatically outperform KNN on this email dataset?**
> *Answer:* High dimensionality (Curse of Dimensionality: 3000 features). In 3000 dimensions, data points become equidistant in Euclidean space, degrading KNN's distance metric. SVM thrives in high-dimensional spaces because it only depends on finding an optimal hyper-separating plane supported by key boundary vectors.

**Q2: What is the Curse of Dimensionality?**
> *Answer:* As feature dimensions increase, the volume of the feature space grows exponentially, making the available data sparse. Distance metrics (Euclidean) lose contrast and become uniform, hurting distance-based models like KNN.

**Q3: What are Support Vectors in SVM?**
> *Answer:* Support vectors are the data coordinates that lie closest to the decision boundary (on the margin boundaries). The location and orientation of the separating hyperplane depend exclusively on these support vectors; moving non-support vectors does not change the hyperplane.

**Q4: In spam detection, which metric is more critical: Precision or Recall?**
> *Answer:* Precision is more critical. If Precision is low, legitimate emails (college notifications, job offers) are classified as spam (False Positives), potentially causing severe loss. High precision ensures flagged spam is indeed spam.

**Q5: Why did you scale using `scaler.fit_transform(X_train)` and then `scaler.transform(X_test)`?**
> *Answer:* To avoid **Data Leakage**. The test set simulates unseen future emails. Computing the mean and variance on the test set would leak information about its distribution into the training pipeline.
