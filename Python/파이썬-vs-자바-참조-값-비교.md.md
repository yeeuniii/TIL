### 값 🆚 객체 참조 비교

|  | Python | Java |
| --- | --- | --- |
| 객체 참조 비교 | is | == |
| 값 비교 | == | .equals() |

- Python은 객체의 id가 같은지 비교한다고 한다.

#### 정리하면

- **Python의 `is` ≈ Java의 `==`** (참조 비교)
- **Python의 `==` ≈ Java의 `.equals()`** (값 비교)