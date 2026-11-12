
추가적인 쓰기 작업과 저장 공간을 활용하여 데이터베이스 테이블의 검색 속도를 향상시키기 위한 자료구조. 
 책 뒤의 색인(찾아보기)을 생각하면 이해하기 쉽다. 

**왜?**
- 조건에 맞는 데이터를 찾을 때의 비용(I/O)을 줄이기 위해

**장점**
- 쿼리 속도 향상. 
- CPU, 디스크 부담 줄어듦. 

**비용**
- 저장 공간을 추가로 사용해야 함. 
- 적재, 컴팩션과 같은 데이터 쓰기 비용이 늘어남. 

인덱스를 추가한다고 무조건 조회 성능이 좋아지지 않는다. 조건에 맞지 않는 인덱스는 효과가 없고 비용만 늘어난다. 

## OLTP와 OLAP에서의 인덱스의 차이 

|       | OLTP            | OLAP                                         |
| ----- | --------------- | -------------------------------------------- |
| 대표 구조 | B+Tree          | Sparse index, min/max, inverted, BloomFilter |
| 목적    | 특정 행 위치를 정확히 찾기 | 읽지 않아도 되는 블록 건너뛰기 (data skipping)            |
| 단위    | 행 (row)         | 세그먼트, 페이지(수천 행 묶음)                           |

---

## Doris index 종류

### Point Query Indexes
- 주로 point query를 빠르게 할 때 사용한다. 
- 인덱스로 WHERE 조건을 만족하는 행을 찾아 그 행만 바로 읽는 방식
- 조건에 해당하는 행 적을 때 효과적이다. 

#### 1. **Prefix Index**
- sort key 순서로 데이터를 정렬해 저장하고 1024행마다 sparse prefix index를 하나씩 만든다. 
- prefix index는 sort key 앞에서부터 최대 36바이트
	- VARCHAR 컬럼을 만나면 그 컬럼의 앞 20바이트까지만 넣고 prefix가 끝남. 그 뒤 컬럼은 36바이트가 남아 있어도 들어가지 않는다.
- Built-in
- sort key 컬럼 순서대로 조건을 걸어야만 인덱스를 사용한다. 
```
KEY(logdate, repoid, stage)
WHERE logdate = .. AND repoid = ..  → prefix 사용 O
WHERE repoid = .. 만               → prefix 사용 X (첫 컬럼을 건너뜀)
```

> sparse prefix index: 인덱스를 모든 행이 아니라 일정 간격마다 하나씩 만든다. 즉, 범위 탐색
	
**동작 방식**
- 1024행 묶음마다 첫 번째 행의 sort key 값만 저장한다. 
- 쿼리가 들어오면 index 항목을 binary search 해서 "읽을 행 범위"를 정한다. 
- 범위 안의 행은 한 행씩 실제 조건과 비교해 필터한다. → 결과는 정확한 행이 아니라 행 범위.
- 데이터가 sort key 순서로 정렬되어 있기 때문에 가능한 방식이다. 
- 비유: 전화번호부 각 페이지 맨 위의 머리글("김가영 ~ 김민수"). 머리글만 보고 펼칠 페이지를 고른 뒤, 그 페이지 안에서 이름을 찾는다.

**예**
`DUPLICATE KEY(logdate, repoid, stage)` 테이블의 한 segment

```
정렬된 데이터 (segment 내부)                prefix index (1024행마다 1개)
──────────────────────────────            ─────────────────────────────
row 0     2026-04-13 | bubblepop | ...  ←── [0]  (2026-04-13, bubblepop)
...
row 1024  2026-04-13 | bubblepop | ...  ←── [1]  (2026-04-13, bubblepop)
...
row 2048  2026-04-13 | dart      | ...  ←── [2]  (2026-04-13, dart)
...
row 3072  2026-04-13 | dart      | ...  ←── [3]  (2026-04-13, dart)
...
row 4096  2026-04-13 | unclebill | ...  ←── [4]  (2026-04-13, unclebill)
```

`WHERE logdate = '2026-04-13' AND repoid = 'dart'` 이면
1. index 항목을 binary search 한다. 시작은 [1] (1024 ~ 2047행 사이에서 dart가 시작될 수 있음), 끝은 dart보다 큰 값이 시작되는 [4].
2. 1024 ~ 4095행만 읽고 나머지는 읽지 않는다.
3. 읽은 행을 한 행씩 조건과 비교해 dart가 아닌 행은 걸러낸다. (경계 묶음에서는 조건에 안 맞는 행도 조금 읽게 됨 = sparse의 대가)

#### 2. Inverted Index (2.0+)
- 세그먼트마다 값에서 그 값을 가진 행 번호로 연결되는 역색인. 
	- 각 값을 row id 집합에 대응시키는 posting list가 만들어짐. 
- 예전의 BITMAP index는 deprecated 되었고, inverted index로 대체되었다. 
- **언제?** 조건에 맞는 행의 비율이 낮을 때

**예**
WHERE user_id = 'A'이면 인덱스에서 {0, 3}을 바로 얻고, 그 행만 읽는다. 
```
원본 (segment 내부)          inverted index
row 0: user_id = A          A → {0, 3}
row 1: user_id = B          B → {1}
row 2: user_id = C          C → {2, 4}
row 3: user_id = A
row 4: user_id = C
```


### Skip Indexes
- 주로 분석 쿼리를 빠르게 할 때 사용한다.
- 인덱스로 WHERE 조건을 만족하지 않는 데이터 블록을 가려내 건너뛰고, 조건을 만족할 수 있는 데이터 블록만 읽는다. 그 다음 행 단위로 필터링하여 조건에 만족하는 행을 찾는다. 
- 조건을 만족하는 행이 많을 때 더 효과적이다. 

#### 1. ZoneMap Index
- 각 컬럼의 통계를 자동으로 관리한다. 데이터 파일(segment)과 데이터 블록(page)마다 min/max/null 값 존재 여부를 기록한다. 
- 데이터가 정렬되어 있고, 군집되어 있을수록 효과가 크다. 
- Built-in

#### 2. BloomFilter Index
- 페이지마다 bloom filter를 하나씩 두고, bloom filter는 값이 들어 있는지 물으면 둘 중 하나로 답한다. 
	- 확실히 없음 -> skip
	- 있을 수도 있음 -> 페이지 읽고 실제로 필터 (기본 오탐률 bloom_filter_fpp = 0.05)
```
WHERE user_id = 'A'
page 1: bloom → 확실히 없음 → skip
page 2: bloom → 있을 수도   → 읽고 실제로 필터
page 3: bloom → 확실히 없음 → skip
```

- **언제?** 카디널리티가 높은 컬럼일 때
	- 값 종류가 적으면 bloom filter가 거의 모든 페이지에서 있을 수도 있음 을 답하면서 skip 효과를 거의 볼 수 없다. 


## Inverted vs BloomFilter

|          | Inverted                       | BloomFilter         |
| -------- | ------------------------------ | ------------------- |
| 판단 단위    | 행                              | 페이지                 |
| 정확도      | 정확함                            | 오탐 있음 ("있을 수도")     |
| 지원 조건    | `=`, `IN`, 범위, fulltext search | `=`, `IN`만          |
| 저장·쓰기 비용 | 큼                              | 작음                  |
| 잘 맞는 경우  | 조건에 맞는 행의 비율이 낮은 필터            | 카디널리티가 높은 컬럼의 등치 조회 |

 
 
> 참고
> https://doris.apache.org/docs/3.x/table-design/index/index-overview