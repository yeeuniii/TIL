## Materialized View
> materialized: 구체화된, 실현된.
> 
쿼리 결과를 미리 계산하여 저장하는 실제 테이블.  
- 데이터 양이 많거나 쿼리 연산이 복잡한 경우 발생할 수 있는 성능 부하 완화를 위해 사용됨
- 저장 공간을 차지함. 즉, 실제 데이터만큼의 디스크 용량을 사용함
- 실제 테이블처럼 동작되기 때문에, 인덱스(Index) 추가 가능
- 정적 데이터기 때문에, 원본 데이터 업데이트에 따른 업데이트 전략이 필수


### vs View
1. 데이터 저장 방식
   - View: 가상 테이블. 뷰를 정의하는 쿼리가 실행될떄마다 실시간으로 결괄르 생성하기 때문에, 뷰의 데이터는 실재하지 않음
   - Materialized View: 실제로 데이터를 저장하는 테이블. 쿼리의 결과가 사전에 계산되어 테이블에 저장되기 때문에, 데이터가 실시간으로 업데이트되지 않는 정적 데이터임. 저장 공간을 차지함.
2. 데이터 접근 속도
   - View: 쿼리 실행 시 원본 테이블에 접근하여 데이터 조회 -> 원본 테이블의 성능에 영향을 받음
   - Materialized View: 데이터를 사전에 계산 -> 속도 매우 빠름
3. 데이터 갱신 방식
   - View: 원본 데이터의 변경사항이 즉시 반영
   - Materialized View: 원본 데이터에 변경 사항 발생하면 해당 사항을 수동으로 변경해주어야 함.
4. 뷰 구조 (쿼리 로직) 변경 시, 물리적 데이터 구조가 깨지므로 DROP 후 CREATE 필수
   * 로직 수정 시 DROP & CREATE 가 필요한 이유
     * 일반 뷰는 가상 테이블이므로 쿼리만 바꾸면 되지만, 비동기 MV는 **생성 시점의 로직에 맞춰 이미 물리적인 데이터가 디스크에 적재**되어 있음.
     * 원본 쿼리(`SELECT` 컬럼 변경, `JOIN` 로직 등)가 바뀌면 기존에 쌓아둔 물리적 데이터와 정합성이 완전히 깨지게 됨.


| **구분** | **일반 View** | **Materialized View** |
| --- | --- | --- |
| **추천 상황** | 데이터가 적고 실시간성이 중요할 때 | **데이터가 많고 연산이 복잡할 때** |
| **장점** | 항상 최신 데이터, 저장공간 미사용 | **압도적인 조회 성능, 인덱스 사용 가능** |
| **단점** | 매번 연산하므로 대용량일 때 느림 | 데이터 갱신(Refresh) 전까지는 과거 데이터 |


---

### 분류 In StarRocks / Doris

####  데이터 처리 속도: 동기 vs 비동기
- 동기(synchronous): 기본 데이터 테이블과의 일관성 유지가 필수인 경우 사용.
- 비동기(asynchronous): 최종 일관성은 유지되지만 약간의 딜레이가 존재한다. 높은 최신성이 요구되지 않는 경우에 사용된다.


#### 새로고침(refresh): full vs partition incremental vs real-time
- 비동기
  - full(전체): 모든 데이터를 계산하여 새로고침한다.
  - partition incremental(파티션 증분): 기본 테이블의 증분 업데이트 설정한 파티션 데이터 변경 시, 추가된 파티션만 새로고침하여 데이터를 추가한다.
- 동기
  - real-time(실시간): 기본 테이블과의 일관성 유지가 필수이기 때문에.  

---

### 쿼리 문법 (RDBMS)
#### 1. 생성
```sql
CREATE MATERIALIZED VIEW [view_name] 
AS [raw query]
```

#### 2. 데이터 갱신 (Refresh)
원본 테이블의 최신 데이터를 MV에 반영할 때 사용
```sql
REFRESH MATERIALIZED VIEW [view_name];
```

#### 3. 뷰 삭제
```sql
DROP MATERIALIZED VIEW [view_name];
```

<br>

### 쿼리 문법 (StarRocks)

#### 생성

```sql
CREATE MATERIALIZED VIEW [view_name]
BUILD IMMEDIATE
REFRESH AUTO ON SCHEDULE EVERY 1 DAY STARTS "2026-02-28 02:00:00"
PARTITION BY (logdate)
DISTRIBUTED BY HASH(repoid, stage) BUCKETS 16
AS
[raw query]

```

* **BUILD IMMEDIATE**: 생성 즉시 백그라운드에서 데이터를 계산하여 뷰를 채워 넣음. (반대: `BUILD DEFERRED` - 껍데기(메타데이터)만 먼저 만들고 데이터는 나중에 채움)
* **REFRESH AUTO ...**: 뷰 갱신(업데이트) 스케줄 자동화. 예시의 경우 지정된 시간부터 매일 1회씩 갱신함.
  * `REFRESH ASYNC`: 원본 데이터 변경 시 시스템이 알아서 비동기 갱신
  * `REFRESH MANUAL`: 사용자가 직접 갱신 명령어를 실행할 때만 갱신

* **PARTITION BY**: 특정 컬럼(ex. logdate) 기준으로 데이터를 물리적 파티션으로 분할.
  * **사용 이유**: 데이터 갱신 시 전체 데이터를 다시 계산(Full Refresh)하지 않고, **데이터가 변경된 파티션만 갱신(Partition Incremental)** 하여 리소스를 획기적으로 절약하기 위함.

* **DISTRIBUTED BY HASH(...) BUCKETS N**: 지정한 컬럼들을 해시 연산하여 데이터를 N개의 바구니(Bucket)로 쪼개어 여러 노드에 분산 저장.
  * **사용 이유**: StarRocks(MPP DB)의 병렬 처리 성능을 극대화하려면 데이터가 여러 서버에 골고루 쪼개져 있어야 하기 때문. 조인/그룹화에 자주 쓰이는 키를 설정하는 것이 좋음.



#### 쿼리 조회

```sql
SHOW CREATE MATERIALIZED VIEW [view_name];

```

* **조회 시 `SHOW CREATE`를 사용하는 이유**
  * 비동기 MV는 단순한 가상 테이블이 아니라, 내부적으로 갱신 스케줄, 파티션 전략, 원본 쿼리를 모두 품고 있는 독립적인 백그라운드 작업(Task)에 가까움.
  * 일반적인 `DESC` 명령어로는 단순 컬럼 스키마만 보이므로, MV 생성 시 설정한 모든 속성(DDL 원본)을 온전히 확인하기 위해 사용함.





### 참고
https://2hyes.tistory.com/200  
https://doris.apache.org/docs/3.x/query-acceleration/materialized-view/overview