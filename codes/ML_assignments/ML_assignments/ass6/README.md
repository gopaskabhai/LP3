# ML Assignment 6: Sales Data Customer Segmentation using K-Means and Hierarchical Clustering

---

## 1. Problem Statement & SPPU Syllabus Mapping
Implement **K-Means clustering** and **Hierarchical clustering** on the `sales_data_sample.csv` dataset.
- Determine the optimal number of clusters using the **Elbow Method**.
- Analyze and profile customer/sales order segments.

---

## 2. Theoretical Concepts Made Simple (First-Year Beginner Friendly)

### 2.1 What is Unsupervised Learning & Clustering?
Unlike previous assignments where we had target labels ($y$) such as `fare_amount`, `spam`, `churn`, or `diabetes`, clustering is **Unsupervised Learning**:
- There is **no target label $y$** to guide the algorithm.
- The algorithm must discover natural groupings or patterns purely based on geometric similarities in the feature space.
- **Analogy**: Organizing mixed candies into bowls based on color and shape without knowing the names of the candies.

### 2.2 K-Means Clustering
- **Goal**: Partition $N$ observations into $K$ distinct, non-overlapping clusters.
- **Algorithm (Lloyd's Algorithm)**:
  1. **Initialization**: Choose $K$ initial points as cluster centroids (using `k-means++` to spread them out).
  2. **Assignment Step**: Assign every data point to its nearest centroid based on Euclidean distance:
     $$c^{(i)} = \arg\min_j \|\mathbf{x}^{(i)} - \boldsymbol{\mu}_j\|^2$$
  3. **Update Step**: Recompute each centroid by taking the mean of all points assigned to that cluster:
     $$\boldsymbol{\mu}_j = \frac{1}{|C_j|} \sum_{i \in C_j} \mathbf{x}^{(i)}$$
  4. **Repeat** steps 2 and 3 until centroids stop moving (convergence).

### 2.3 The Elbow Method & WCSS (Inertia)
How do we know what value of $K$ to choose?
- We calculate the **Within-Cluster Sum of Squares (WCSS)** or **Inertia**:
  $$\text{WCSS} = \sum_{k=1}^K \sum_{\mathbf{x} \in C_k} \|\mathbf{x} - \boldsymbol{\mu}_k\|^2$$
- As $K$ increases, WCSS naturally decreases (if $K=N$, WCSS is 0).
- We plot WCSS against $K$. The plot resembles an arm; the point where the rate of decrease abruptly bends and flattens out like an **"elbow"** indicates the optimal trade-off between compactness and complexity.
- We validate this using the **Silhouette Score**:
  $$s(i) = \frac{b(i) - a(i)}{\max(a(i), b(i))}$$
  where $a(i)$ is average intra-cluster distance and $b(i)$ is the nearest-cluster distance. Range is $[-1, 1]$ (higher is better).

### 2.4 Hierarchical (Agglomerative) Clustering
- **Bottom-Up Approach**:
  1. Start with every single data point in its own individual cluster ($N$ clusters).
  2. In each iteration, merge the two clusters that are closest together.
  3. Repeat until all points are merged into one giant cluster.
- **Linkage Criterion (Ward's Method)**:
  Minimizes the total within-cluster variance when two clusters are merged.
- **Dendrogram**: A tree-like diagram showing the exact hierarchy of merges and distance levels. Cutting the dendrogram horizontally at a chosen distance reveals the clusters.

---

## 3. Code Walkthrough Step-by-Step

### Step 1: Loading Dataset & Feature Selection
```python
df = pd.read_csv('sales_data_sample.csv', encoding='unicode_escape')

# Relevant numerical features for transaction profiling:
features = ['QUANTITYORDERED', 'PRICEEACH', 'SALES', 'MSRP']
X = df[features].copy()
```
*Note*: `encoding='unicode_escape'` or `latin1` is needed because the CSV contains international currency symbols or accented characters.

### Step 2: Feature Standardization
```python
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)
```
*Why?* `SALES` has values up to $14,000, while `PRICEEACH` is around $30-$100, and `QUANTITYORDERED` is 10-50. Standardization ensures distance calculations treat each dimension fairly.

### Step 3: Determining Optimal $K$ (Elbow + Silhouette)
```python
wcss = []
silhouette_scores = []
k_range = range(2, 11)

for k in k_range:
    km = KMeans(n_clusters=k, init='k-means++', random_state=42, n_init=10)
    km.fit(X_scaled)
    wcss.append(km.inertia_)
    silhouette_scores.append(silhouette_score(X_scaled, km.labels_))
```
- The Elbow inflection point and highest silhouette score indicate **$K = 3$** (or $K=4$) is optimal.

### Step 4: Fitting K-Means Model ($K = 3$)
```python
kmeans = KMeans(n_clusters=3, init='k-means++', random_state=42, n_init=10)
df['KMeans_Cluster'] = kmeans.fit_predict(X_scaled)
```

### Step 5: Hierarchical Clustering & Dendrogram
```python
from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.cluster import AgglomerativeClustering

# Linkage using Ward's minimum variance method
linked = linkage(X_scaled[:500], method='ward')

agg = AgglomerativeClustering(n_clusters=3, linkage='ward')
df['Hierarchical_Cluster'] = agg.fit_predict(X_scaled)
```

### Step 6: 2D & 3D Visualization using PCA
```python
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_scaled)
# Scatter plot colored by cluster assignments
```

---

## 4. Business Interpretation of the 3 Clusters

| Cluster ID | Cluster Name | Characteristics | Business Strategy |
| :---: | :--- | :--- | :--- |
| **Cluster 0** | **Economy / Small Orders** | Low quantity (< 30), low price (< $60), small total sales (< $2,500). | Offer bulk purchase discounts and cross-sell related accessories. |
| **Cluster 1** | **Standard / Moderate Orders** | Medium quantity (30-45), standard catalog price, mid-tier sales ($3,000 - $5,000). | Loyalty reward programs to encourage repeat orders. |
| **Cluster 2** | **High-Value / Premium Bulk Orders** | Large quantity (> 45), premium items (> $90), highest total sales ($6,000 - $14,000+). | Assign dedicated account managers; VIP priority shipping. |

---

## 5. Practical Viva Questions & Answers for the Examiner

**Q1: What is the main difference between K-Means and Hierarchical Clustering?**
| Feature | K-Means Clustering | Hierarchical Clustering |
| :--- | :--- | :--- |
| **Type** | Partitional (flat) | Nested hierarchy (tree-based) |
| **Preset $K$ required?** | Yes, must specify $K$ upfront | No, inspect tree first to choose cut level |
| **Time Complexity** | Fast: $O(N \cdot K \cdot I \cdot D)$ | Slow: $O(N^2 \log N)$ or $O(N^3)$ |
| **Scalability** | Handles large datasets easily | Limited to small/medium datasets ($N < 10,000$) |

**Q2: What is the `k-means++` initialization algorithm?**
> *Answer:* Random initialization can pick centroids that are close together, trapping the algorithm in poor local minima. `k-means++` picks the first centroid randomly, and then selects subsequent centroids with probability proportional to their squared distance from already chosen centroids, ensuring centroids are widely dispersed.

**Q3: How do you read a Dendrogram?**
> *Answer:* The vertical axis represents the distance/dissimilarity between clusters when they merge. The longer the vertical stems without any horizontal lines crossing them, the more natural and distinct the cluster separation. A horizontal cut across these stems determines the number of clusters.

**Q4: What is the Silhouette Score and what do its values mean?**
> *Answer:* It measures how similar an object is to its own cluster compared to neighboring clusters.
> - **+1.0**: Well-clustered, point is far from neighboring clusters.
> - **0.0**: Point lies on the decision boundary between two clusters.
> - **-1.0**: Point might belong to the wrong cluster.

**Q5: Why did we need PCA to visualize the clusters?**
> *Answer:* The dataset has 4 numerical features (`QUANTITYORDERED`, `PRICEEACH`, `SALES`, `MSRP`), making it impossible to directly plot on a 2D screen. PCA reduces the 4 dimensions to 2 principal components that capture maximum variance, enabling 2D scatter plots of the clusters.
