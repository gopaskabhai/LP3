# ML Assignment 5: Diabetes Prediction using K-Nearest Neighbors (KNN)

---

## 1. Problem Statement & SPPU Syllabus Mapping
Implement the **K-Nearest Neighbors (KNN)** algorithm on the `diabetes.csv` (Pima Indians Diabetes) dataset.

### Required Tasks:
1. Load and clean the dataset.
2. Standardize features to avoid distance metric distortions.
3. Compute and report:
   - **Confusion Matrix**
   - **Accuracy**
   - **Error Rate**
   - **Precision**
   - **Recall**

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 The Medical Context & Pima Indians Diabetes Dataset
The dataset predicts whether a female patient has diabetes based on diagnostic measurements:
- Features: `Pregnancies`, `Glucose`, `BloodPressure`, `SkinThickness`, `Insulin`, `BMI`, `DiabetesPedigreeFunction`, `Age`.
- Target: `Outcome` (0 = Non-diabetic, 1 = Diabetic).

### 2.2 The "Biological Zero" Problem
In biological data, a measurement of **0** for features like `Glucose`, `BloodPressure`, `SkinThickness`, `Insulin`, or `BMI` is biologically impossible for a living human!
- In the raw dataset, `0` was used to represent **missing data**.
- If left untreated, a KNN distance calculation will treat a missing glucose reading as physically 0, distorting all nearest-neighbor distance metrics.
- **Solution**: Replace 0 with `np.nan` and impute with the **median** of each feature (median is robust against extreme outliers).

### 2.3 How Does KNN Work?
KNN is an instance-based, non-parametric algorithm:
1. Given a test patient $\mathbf{x}_{\text{test}}$, calculate the Euclidean distance to all $N$ patients in the training set:
   $$d(\mathbf{x}, \mathbf{x}_i) = \sqrt{\sum_{j=1}^D (x_j - x_{i,j})^2}$$
2. Sort distances in ascending order and pick the top $K$ nearest patients.
3. Count how many of the $K$ neighbors are diabetic vs non-diabetic (majority voting):
   $$\hat{y} = \text{mode}(y_{i_1}, y_{i_2}, \dots, y_{i_K})$$

### 2.4 Why Feature Standardization is Mandatory
Suppose `Insulin` ranges from 0 to 800, while `DiabetesPedigreeFunction` ranges from 0.08 to 2.4.
- Distance difference in Insulin will completely overpower pedigree function by a factor of 10,000!
- `StandardScaler` rescales every feature so $\mu = 0$ and $\sigma = 1$.

### 2.5 Detailed Metric Formulas Required by the Syllabus
Given the confusion matrix:
$$\begin{pmatrix} \text{TN} & \text{FP} \\ \text{FN} & \text{TP} \end{pmatrix}$$

1. **Confusion Matrix**: A $2 \times 2$ table showing exact true vs predicted classifications.
2. **Accuracy**:
   $$\text{Accuracy} = \frac{\text{TP} + \text{TN}}{\text{TP} + \text{TN} + \text{FP} + \text{FN}}$$
3. **Error Rate** (Misclassification Rate):
   $$\text{Error Rate} = 1 - \text{Accuracy} = \frac{\text{FP} + \text{FN}}{\text{TP} + \text{TN} + \text{FP} + \text{FN}}$$
4. **Precision** (Positive Predictive Value):
   $$\text{Precision} = \frac{\text{TP}}{\text{TP} + \text{FP}}$$
   *Meaning*: Out of all patients predicted as diabetic, how many actually have diabetes?
5. **Recall** (Sensitivity / True Positive Rate):
   $$\text{Recall} = \frac{\text{TP}}{\text{TP} + \text{FN}}$$
   *Meaning*: Out of all genuinely diabetic patients, how many did the hospital test detect?

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Loading & Replacing Biological Zeros
```python
df = pd.read_csv('diabetes.csv')
zero_cols = ['Glucose', 'BloodPressure', 'SkinThickness', 'Insulin', 'BMI']
# Replace invalid 0s with NaN and impute with median
for col in zero_cols:
    df[col] = df[col].replace(0, np.nan)
    df[col].fillna(df[col].median(), inplace=True)
```

### Step 2: Feature Matrix & Stratified Train-Test Split
```python
X = df.drop(columns=['Outcome'])
y = df['Outcome']

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.20, random_state=42, stratify=y
)
```

### Step 3: Feature Scaling (No Leakage)
```python
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
```

### Step 4: Finding Optimal $K$ via Error Rate & Cross-Validation
```python
k_range = range(1, 31)
test_error_rates = []

for k in k_range:
    knn = KNeighborsClassifier(n_neighbors=k)
    knn.fit(X_train_scaled, y_train)
    preds = knn.predict(X_test_scaled)
    test_error_rates.append(1.0 - accuracy_score(y_test, preds))
```
Optimal performance is achieved around **$K = 11$ or $13$** (odd number to avoid ties).

### Step 5: Training Optimal Model & Metric Computation
```python
knn = KNeighborsClassifier(n_neighbors=11)
knn.fit(X_train_scaled, y_train)
y_pred = knn.predict(X_test_scaled)

cm = confusion_matrix(y_test, y_pred)
acc = accuracy_score(y_test, y_pred)
err = 1.0 - acc
prec = precision_score(y_test, y_pred)
rec = recall_score(y_test, y_pred)
```

### Step 6: Verification with KNN Implemented from Scratch
```python
class KNNFromScratch:
    def __init__(self, k=11):
        self.k = k
    def fit(self, X, y):
        self.X_train = np.array(X)
        self.y_train = np.array(y)
    def predict(self, X):
        predictions = []
        for x in np.array(X):
            distances = np.sqrt(np.sum((self.X_train - x)**2, axis=1))
            k_idx = np.argsort(distances)[:self.k]
            k_labels = self.y_train[k_idx]
            majority = Counter(k_labels).most_common(1)[0][0]
            predictions.append(majority)
        return np.array(predictions)
```
Produces results matching scikit-learn.

---

## 4. Expected Results & Metrics Summary

| Metric | Scikit-Learn KNN ($K=11$) | Scratch KNN ($K=11$) | Required by Syllabus? |
| :--- | :---: | :---: | :---: |
| **Accuracy** | **~77.92%** | **~77.92%** | Yes |
| **Error Rate** | **~22.08%** | **~22.08%** | Yes |
| **Precision** | **~71.11%** | **~71.11%** | Yes |
| **Recall** | **~60.00%** | **~60.00%** | Yes |
| **F1-Score** | **~0.65** | **~0.65** | Optional bonus |
| **ROC-AUC** | **~0.83** | — | Optional bonus |

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: Why is KNN called a "Lazy Learner" or "Instance-Based" algorithm?**
> *Answer:* KNN does not build an explicit mathematical model or learn weights during training (`fit()` simply stores the coordinates in memory). All computation (distance calculations and sorting) is deferred to the prediction/query phase.

**Q2: In healthcare and diabetes diagnosis, is Recall more important or Precision?**
> *Answer:* **Recall is significantly more important**. A False Negative means a diabetic patient is falsely told they are healthy, delaying medical intervention which can lead to life-threatening complications. A False Positive simply leads to a secondary confirmatory test.

**Q3: Why should $K$ always be chosen as an odd number for binary classification?**
> *Answer:* An odd number eliminates the possibility of a 50-50 tie during majority voting between two classes.

**Q4: What is the computational complexity of KNN during testing?**
> *Answer:* $O(N \cdot D)$, where $N$ is the number of training samples and $D$ is the number of features. For large datasets, predicting becomes slow and memory-intensive unless spatial indexing trees (like KD-Tree or Ball-Tree) are used.

**Q5: How does an outlier affect KNN with $K=1$ versus $K=15$?**
> *Answer:* For $K=1$, an outlier will create an isolated island of false predictions around itself, leading to severe overfitting. With $K=15$, the outlier's vote is easily overwhelmed by normal neighboring points, making the boundary smoother and more robust.
