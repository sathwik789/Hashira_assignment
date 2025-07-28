# 🔐 Shamir's Secret Sharing - C++ Implementation

This project implements **Shamir's Secret Sharing** (SSS) algorithm in C++. Given multiple encoded shares of a secret in various numeric bases, it reconstructs the original secret using **Lagrange interpolation**.

---

## 📚 What is Shamir's Secret Sharing?

Shamir's Secret Sharing is a **cryptographic algorithm** that allows a secret (e.g., a password or cryptographic key) to be divided into `n` pieces (called **shares**) such that:

- Any **k ≤ n** shares can reconstruct the secret.
- Any group of fewer than `k` shares **cannot** determine the secret.

This is done using **polynomials over finite fields**. The secret is embedded as the constant term of a random polynomial of degree `k - 1`. Each share is a point `(x, y)` on this polynomial.

---

## 🛠️ How It Works (In This Project)

1. **Input** is provided in a `input.JSON` file.
2. Each test case contains:
   - Total number of shares (`n`)
   - Minimum threshold to reconstruct (`k`)
   - Each share consists of:
     - An `x` value (index)
     - A `value` in a specific base (e.g., binary, base-16)
3. We convert all `value`s to decimal `y` values using their base.
4. We apply **Lagrange Interpolation** using combinations of `k` shares to estimate the constant term of the polynomial (the secret).
5. The most frequently estimated constant is selected as the **final secret**.

---

## 🧮 Math Behind the Code

### ➤ Lagrange Interpolation

To find the secret (constant term of polynomial), we use:

\[
f(0) = \sum_{i=0}^{k-1} y_i \cdot \prod_{\substack{j=0 \\ j \neq i}}^{k-1} \frac{0 - x_j}{x_i - x_j}
\]

Where:
- \( (x_i, y_i) \) are known shares
- \( f(0) \) is the constant term (our secret)

We use **rational arithmetic (fractions)** to avoid floating point errors.

---

## 📂 File Structure

📁 Hashira/
├── input.JSON # Input data for test cases
├── json.hpp # library to read adn extract data from json file
├── task.cpp # Main C++ implementation
├── README.md # You're reading this! 

## 🔢 Example: `input.JSON`

```json
{
  "testcase1": {
    "keys": {
      "n": 4,
      "k": 3
    },
    "1": { "base": "10", "value": "4" },
    "2": { "base": "2", "value": "111" },
    "3": { "base": "10", "value": "12" },
    "6": { "base": "4", "value": "213" }
  },
  "testcase2": {
    "keys": {
      "n": 10,
      "k": 7
    },
  }
}


🖥️ How to Compile and Run
Prerequisites:
C++ compiler (g++)
json.hpp header file (from nlohmann/json)

✅ Steps:
Place your input.JSON and task.cpp in the same directory.
Open terminal in that directory and run:
g++ task.cpp -o task
./task


✅ Expected Output:
Secret from Test Case 1: 3
Secret from Test Case 2: 79836264049851
