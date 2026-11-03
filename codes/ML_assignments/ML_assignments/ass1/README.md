# ML Assignment 1: Uber Fare Prediction

---

## 1. Problem Statement & SPPU Syllabus Mapping
Predict the price of an Uber ride given pickup and drop-off coordinates and timestamps.

### Required Tasks:
1. **Pre-process the dataset** (Handle null values, convert timestamps, drop irrelevant columns, calculate travel distance).
2. **Identify outliers** (Use IQR and Z-score methods).
3. **Check the correlation** (Pearson correlation between fare amount and other features).
4. **Implement Linear Regression and Random Forest Regression models** (Built from scratch with vectorization and tree splits).
5. **Evaluate the models and compare respective scores** ($R^2$, RMSE, MAE).

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 What is Regression?
In Machine Learning, **Regression** is used when we want to predict a **continuous numeric number** (like price, temperature, weight, distance). 
- If we predict a label ("Spam" or "Not Spam"), that is **Classification**.
- If we predict a dollar amount (e.g., "$15.50"), that is **Regression**.

### 2.2 Why Distance Calculation (Haversine Formula)?
We are given latitudes and longitudes, which are points on a 3D curved sphere (Earth), not on a flat sheet of paper.
- Euclidean distance ($\sqrt{(x_2-x_1)^2 + (y_2-y_1)^2}$) gives erroneous distances on a sphere.
- **Haversine Formula** calculates the great-circle distance between two points on a sphere of radius $R \approx 6371\text{ km}$:
  $$a = \sin^2\left(\frac{\Delta \phi}{2}\right) + \cos(\phi_1)\cos(\phi_2)\sin^2\left(\frac{\Delta \lambda}{2}\right)$$
  $$c = 2 \cdot \text{atan2}(\sqrt{a}, \sqrt{1-a})$$
  $$d = R \cdot c$$

### 2.3 What are Outliers & How Do We Detect Them?
An outlier is an extreme or erroneous data point that lies abnormally far from other values (e.g., fare of -$5.00 or $50,000, or coordinates located at (0, 0) in the Atlantic Ocean).
- **IQR (Interquartile Range) Method**:
  - Divide sorted data into four quarters. $Q_1$ is 25th percentile, $Q_3$ is 75th percentile.
  - $\text{IQR} = Q_3 - Q_1$
  - Any point below $Q_1 - 1.5 \times \text{IQR}$ or above $Q_3 + 1.5 \times \text{IQR}$ is an outlier.
- **Z-Score Method**:
  - $z = \frac{x - \mu}{\sigma}$ (measures how many standard deviations $x$ is from the mean $\mu$).
  - If $|z| > 3$, the point is extremely rare (< 0.3% probability under normal distribution).

### 2.4 Correlation Analysis
Correlation ($r$) measures the strength and direction of the linear relationship between two variables:
$$r = \frac{\sum (x - \bar{x})(y - \bar{y})}{\sqrt{\sum (x - \bar{x})^2 \sum (y - \bar{y})^2}}$$
- $r = +1$: Perfect positive relationship (as distance increases, fare increases).
- $r = 0$: No linear relationship.
- $r = -1$: Perfect negative relationship.

### 2.5 Linear Regression vs. Random Forest Regression
| Feature | Linear Regression | Random Forest Regressor |
| :--- | :--- | :--- |
| **Model Type** | Parametric, Linear | Non-parametric, Ensemble of Decision Trees |
| **Formula / Mechanism** | $\hat{y} = w_1 x_1 + \dots + w_n x_n + b$ | Averages predictions of $B$ independent trees: $\hat{y} = \frac{1}{B}\sum_{b=1}^B T_b(x)$ |
| **Optimization** | Batch Gradient Descent / Normal Equation | Bagging (Bootstrap Aggregation) + Feature Randomness |
| **Non-linear Data** | Poor (assumes linear line/hyperplane) | Excellent (captures complex interactions) |
| **Outlier Sensitivity** | High | Low |

### 2.6 Evaluation Metrics
1. **$R^2$ (Coefficient of Determination)**:
   $$R^2 = 1 - \frac{\text{SS}_{\text{res}}}{\text{SS}_{\text{tot}}} = 1 - \frac{\sum (y_i - \hat{y}_i)^2}{\sum (y_i - \bar{y})^2}$$
   Measures what proportion of variance in fare is explained by features (closer to 1.0 is better).
2. **RMSE (Root Mean Squared Error)**:
   $$\text{RMSE} = \sqrt{\frac{1}{n}\sum_{i=1}^n (y_i - \hat{y}_i)^2}$$
   Penalizes large errors heavily; in same units as the target ($).
3. **MAE (Mean Absolute Error)**:
   $$\text{MAE} = \frac{1}{n}\sum_{i=1}^n |y_i - \hat{y}_i|$$
   Average absolute error in dollars.

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Loading & Cleaning Coordinates
```python
df = pd.read_csv('uber.csv')
df.drop(columns=['Unnamed: 0', 'key'], inplace=True)
df.dropna(inplace=True)
# Filter invalid coordinates and negative fares
df = df[(df['fare_amount'] > 0) & (df['fare_amount'] < 500)]
df = df[(df['pickup_latitude'].between(-90, 90)) & (df['pickup_longitude'].between(-180, 180))]
df = df[(df['dropoff_latitude'].between(-90, 90)) & (df['dropoff_longitude'].between(-180, 180))]
```
*Why?* The Kaggle dataset contains missing dropoff coordinates and zero coordinates `(0.0, 0.0)` which represent Null Island in the Gulf of Guinea.

### Step 2: Feature Engineering (Haversine Distance & Time Features)
```python
df['distance_km'] = haversine_distance(
    df['pickup_latitude'], df['pickup_longitude'],
    df['dropoff_latitude'], df['dropoff_longitude']
)
df['pickup_datetime'] = pd.to_datetime(df['pickup_datetime'])
df['hour'] = df['pickup_datetime'].dt.hour
df['day'] = df['pickup_datetime'].dt.day
df['month'] = df['pickup_datetime'].dt.month
df['year'] = df['pickup_datetime'].dt.year
df['dayofweek'] = df['pickup_datetime'].dt.dayofweek
```
*Why?* Raw timestamps and raw latitudes are meaningless to linear algorithms. Breaking time into `hour`, `day`, `month`, and computing actual `distance_km` exposes direct predictive signals.

### Step 3: Outlier Removal & Correlation
```python
# Remove zero distance or absurd distances (> 100 km within NYC city trips)
df = df[(df['distance_km'] > 0.1) & (df['distance_km'] < 100)]
corr = df.corr()
print(corr['fare_amount'].sort_values(ascending=False))
```
`distance_km` shows the strongest positive correlation with `fare_amount` (~0.85+).

### Step 4: Standard Scaling & Train-Test Split (Manual)
```python
class StandardScalerManual:
    def fit(self, X):
        self.mean_ = np.mean(X, axis=0)
        self.std_ = np.std(X, axis=0) + 1e-8
        return self
    def transform(self, X):
        return (X - self.mean_) / self.std_
```
*Why?* Linear regression with Gradient Descent fails or diverges if features have drastically different scales (e.g. `distance_km` in range 0-50 vs `year` in range 2009-2015).

### Step 5: Linear Regression with Gradient Descent
```python
class LinearRegressionScratch:
    def fit(self, X, y):
        n_samples, n_features = X.shape
        self.weights = np.zeros(n_features)
        self.bias = 0.0
        for _ in range(self.n_iters):
            y_pred = np.dot(X, self.weights) + self.bias
            dw = (2 / n_samples) * np.dot(X.T, (y_pred - y))
            db = (2 / n_samples) * np.sum(y_pred - y)
            self.weights -= self.lr * dw
            self.bias -= self.lr * db
```
Uses MSE loss derivative with respect to weights ($dw$) and bias ($db$) to iteratively update parameters downhill.

### Step 6: Random Forest Regressor from Scratch
```python
class RandomForestRegressorScratch:
    # Uses bootstrap sampling (sampling with replacement)
    # Builds an ensemble of DecisionTreeRegressor nodes splitting on variance reduction
```

---

## 4. Expected Results & Comparison

| Metric | Linear Regression | Random Forest Regressor | Best Performer |
| :--- | :---: | :---: | :---: |
| **Train $R^2$** | ~0.76 - 0.79 | ~0.88 - 0.92 | Random Forest |
| **Test $R^2$** | ~0.75 - 0.78 | ~0.82 - 0.86 | Random Forest |
| **Test RMSE** | ~$4.50 - $4.90 | ~$3.60 - $4.10 | Random Forest |
| **Test MAE** | ~$2.80 - $3.10 | ~$2.10 - $2.40 | Random Forest |

**Reason**: Random Forest captures non-linear relationships (such as base fare + traffic surge multipliers during rush hours) which a single straight hyperplane cannot fit.

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: Why did you drop the `key` and `Unnamed: 0` columns?**
> *Answer:* They are arbitrary primary keys and row indices from the CSV export. They have zero causal relationship with the fare price; leaving them in could lead to spurious correlations.

**Q2: Why is the Haversine formula preferred over Euclidean distance?**
> *Answer:* The Earth is approximately spherical (geoid). Euclidean distance treats coordinates as planar $(x, y)$, causing massive distortion over geographic distances. Haversine accounts for the Earth's curvature ($R = 6371\text{ km}$).

**Q3: Why must we scale features for Linear Regression with Gradient Descent, but it's optional for Decision Trees?**
> *Answer:* Gradient Descent updates weights proportional to feature magnitude ($X^T(y_{pred} - y)$), causing zigzagging or divergence if scales differ. Decision Trees and Random Forests only evaluate orthogonal splits ($x_j \le c$) based on sorted order, making them invariant to monotonic scaling.

**Q4: What causes $R^2$ to be negative?**
> *Answer:* $R^2$ is negative when the model's predictions are worse than simply predicting the mean of the target variable ($\bar{y}$) for all samples.

**Q5: What is the difference between MAE and RMSE?**
> *Answer:* MAE penalizes all errors linearly ($|y - \hat{y}|$), while RMSE squares errors before averaging. Hence, RMSE heavily penalizes large errors and is more sensitive to outliers.
