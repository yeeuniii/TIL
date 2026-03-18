# 🛠️ dbt (Data Build Tool) 

## 1. dbt의 정의와 목적

dbt는 데이터 웨어하우스 내에서 **데이터 변환(Data Transformation)** 에 특화된 도구로, ELT 파이프라인 중 **'T'** 를 담당하는 프레임워크.

* **핵심 목표:** 비즈니스 로직을 반영하는 **SELECT 쿼리** 작성에 집중.
* **주요 이점:**
  * **No Boilerplate:** 반복되는 코드 없이 효율적인 개발 가능.
  * **모듈화 및 재사용:** 모델 간의 의존성 관리가 용이함.
  * **테스트 및 문서화:** 데이터 품질 테스트와 명세서 자동 생성 지원.
  * **State-aware 오케스트레이션:** 변경 사항을 감지하여 필요한 모델만 선택적으로 구축.



## 2. 구성 요소: Language & Engine

dbt는 단순한 실행기를 넘어 개발 환경 전반을 제공한다.

* **dbt Language:** `SQL(SELECT) + Jinja(Template) + YAML(Config)`의 조합.
* **dbt Engine:**
  * **dbt Core:** Python 기반의 오픈소스 엔진.
  * **dbt Fusion:** **Rust** 기반의 고성능 엔진. 컴파일 및 SQL 분석 속도가 비약적으로 빠르며, VS Code 익스텐션을 통해 활용 가능.
  * **dbt Cloud:** SaaS 형태의 유료 서비스로 웹 UI 환경 제공.



## 3. 개발 및 운영 환경 분리

`profiles.yml` 설정을 통해 개발(Dev)과 운영(Prod) 환경을 엄격히 분리한다.

* 권장하는 구조는 스키마 분리로, 하나의 데이터베이스 내의 다른 스키마를 사용하는 것이다. 
  * 계층 구조: `Database > Schema > Table`
* **환경별 타겟:**
  * **Dev:** 개인별 스키마(`dbt_<username>`)를 사용하여 동료와 작업 충돌 방지.
  * **Prod:** `analytics`, `public`과 같은 공용 운영 스키마 사용.


## 4. dbt Models 

dbt에서 **모델**은 프로젝트의 가장 기본 단위이다.  각 모델은 하나의 `.sql` 파일로 존재하며, 그 안에는 단 하나의 **SELECT 문**만 포함된다.
즉, 데이터를 어떻게 변형(Transform)할 것인지 정의한 SELECT 쿼리 파일이다. 모델 간의 참조(`ref`)를 통해 데이터의 흐름(Lineage)을 자동으로 파악하고 관리한다.   

 **모델의 흐름:** `원천 데이터(Raw Data) → dbt Model(Transform) → 최종 데이터셋(Data Set)`


### 4.1 Materialization

`Config` 설정을 통해 이 모델을 데이터베이스에 **어떤 형태로 저장할 것인지** 결정한다.
전략은 table, view, incremental, ephemeral, materialized view 5가지가 있고, 어댑터 종류마다 세부 설정이 다르다. 

#### **view** (논리적 뷰)
- 실행할 때마다 뷰를 다시 생성하고, 뷰를 다른 모델에서 참조 시 쿼리가 실행된다. (`create view as`)
- 열 이름 변경이나 열 형식 변경과 같은 중요한 변환 작업이 수행되지 않는 모델에 적합하다. 

#### **table** (물리적 테이블)
- CTAS(Create Table As SELECT): 실행할 때마다 테이블이 생성된다. 
- 쿼리 속도는 빠르지만 빌드 비용 발생. 
- raw data의 변경이 자동으로 반영되지 않는다.
- 변환(transformtaion) 작업이 무거운 경우 사용을 권장

#### **incremental** (증분 빌드)
- 마지막으로 실행된 모델의 데이터가 추가 또는 업데이트된다. (UPSERT)
- CTAS + INSERT
  - 처음 실행하면(,i.e. 테이블이 없는 경우), 모든 데이터를 변환 후 테이블을 생성하여 저장한다.
  - 이후 재실행하면(,i.e. 테이블이 이미 존재하는 경우), 필터링된 데이터만 추가된다. 
- 따라서 변환해야 하는 데이터의 양이 제한되면서 실행 시간이 크게 단축된다.
- dbt run 이 너무 느린 경우 사용을 추천 (처음부터 incremental 모델 사용을 권장하지 않음)

> `is_incremental()` macro   
> incremental 모델에 가장 중요한 매크로로,  아래 조건을 모두 만족하는 경우, True를 반환한다.   
> 이 매크로를 사용하여 증분을 구현할 수 있다. 
> - 모델이 이미 데이터베이스 내 테이블로 존재하는 경우
> - full-refresh flag로 실행되지 않은 경우
> - incremental 전략인 경우

#### **ephemeral** (임시 모델)
- DB에 물리적으로 저장되지 않고, 다른 모델에서 참조할 때만 CTE로 삽입된다.
- 두 개 이하의 하위 모델에서의, 또한 가벼운 transformation 작업을 권장한다.

#### **materialized view**
- 논리 뷰처럼 쿼리 로직을 가지고 있지만, 내부적으로 테이블처럼 물리적인 데이터를 저장한다.
- 따라서 쿼리 속도가 빠르면서 설정에 따라서 데이터를 업데이트할 수 있다.
- dbt의 view + table의 장점을 합친 전략


### 4.2 starrocks configuration
#### table

```sql
{{ config(
    materialized='table',
    engine='OLAP',-- 
    keys=['id', 'name', 'created_date'],
    table_type='PRIMARY', -- 
    distributed_by=['id'],
    buckets=3,
    partition_by=['created_date'],
    properties=[
        {"replication_num": "1"}
    ]
) }}

SELECT 
    id,
    name,
    email,
    created_date,
    last_modified_date
FROM {{ source('your_source', 'users') }}
```


- `engine`
  - 스토리지 엔진 (default: OLAP)
- `table_type`
  - 테이블 모델 타입
  - `DUPLICATE` (default): 중복을 허용하면서 insert
  - `PRIMARY` (recommend): upsert & delete 지원
  - `UNIQUE`: 유일성 강제
- `keys`
  - `table_type`이 `PRIMARY` 또는 `UNIQUE`인 경우, 중복 제거의 기준이 되는 키
  - `table_type`이 `DUPLICATE`인 경우, 정렬 키
- `distributed_by`
  - 데이터를 분산 저장할 기준 컬럼
- `buckets`
  - 버킷 수
- `partition_by` & `partition_by_init`
  -  파티셔닝
- `properties`
  - 기타 고급 속성
  - replication 개수 등이 될 수 있음.


#### incremental
```sql
{{ config(
    materialized='incremental',
    unique_key='id',
    table_type='PRIMARY',
    keys=['id'],
    distributed_by=['id'],
    incremental_strategy='default'
) }}

SELECT
    id,
    user_id,
    event_name,
    event_timestamp,
    properties
FROM {{ source('raw', 'events') }}

{% if is_incremental() %}
    WHERE event_timestamp > (SELECT MAX(event_timestamp) FROM {{ this }})
{% endif %}
```

- `incremental_strategy` 
  - `append` (default): 단순히 중복 제거 없이 새 레코드를 추가한다. table_type에 따라서 upsert가 될 수도 있음. (ex. PRIMARY 테이블인 경우)
  - `insert_overwrite`: 특정 파티션을 모두 덮어쓰기
  - `dynamic_overwrite`: 특정 파티션이 없으면 새로 만들고, 있으면 덮어쓰기


정리하면,   
`table_type = PRIMARY` + `incremental_strategy = append`  
→ 코드는 단순하지만 UPSERT를 지원한는 가장 추천하는 기본 전략. 


> 관련 공식문서  
> https://docs.getdbt.com/docs/build/materializations?version=1.11  
> https://docs.getdbt.com/docs/build/incremental-models?version=1.11  
> https://docs.starrocks.io/docs/3.4/integrations/dbt/ 



---

## 5. 모델 실행 (Execution)

dbt 프로젝트에 작성된 `.sql` 파일들(SELECT 쿼리)을 실제 데이터베이스의 뷰나 테이블로 빌드(Build)하는 과정이다. 주로 `dbt run` 명령어를 사용하며, 데이터의 계보(Lineage)와 상태에 따라 똑똑하게 실행 대상을 선택할 수 있다.

### 5.1 dbt run 
- 프로젝트 내의 **모든 모델**을 순서대로 실행한다. 
- 데이터가 많아지면 전체 실행은 부담이 될 수 있으므로, 아래의 선택자(`--select`)를 주로 사용한다.

### 5.2 Graph operators
모델 간의 의존성(Upstream/Downstream)을 파악하여 **특정 모델과 그 주변부만 선택적으로 실행**할 때 `--select` (또는 `-s`) 옵션과 `+` 연산자를 활용한다. 특정 매체 데이터만 재수집되었거나, 특정 로직만 수정했을 때 매우 유용하다.

* **`dbt run --select model_name`**
    * 딱 지정한 `model_name` 모델 하나만 실행한다.
* **`dbt run --select +model_name` (상위 모델 포함)**
    * 지정한 모델과, 그 모델에 데이터를 공급하는 **모든 부모(Upstream) 모델들**을 순서대로 실행한다.
    * *용도:* 최종 마트 테이블을 보기 전에, 원천 데이터부터 타고 내려오는 모든 가공 단계를 최신화하고 싶을 때.
* **`dbt run --select model_name+` (하위 모델 포함)**
    * 지정한 모델과, 그 모델의 데이터를 참조하는 **모든 자식(Downstream) 모델들**을 순서대로 실행한다.
    * *용도:* `stg_applovin...` 같은 특정 중간 모델의 로직을 수정했을 때, 그 변경사항을 최종 마트 테이블까지 쭉 전파시키고 싶을 때.
* **`dbt run --select +model_name+` (전체 계보 실행)**
    * 해당 모델을 기준으로 위아래로 연결된 **모든 데이터 파이프라인(Lineage)** 을 싹 다 실행한다.

### 5.3 강제 전체 새로고침 (Full Refresh)
증분 모델(`incremental`)은 평소에 `is_incremental()` 로직을 타고 새로운 데이터(INSERT)나 변경된 데이터(UPSERT)만 처리한다. 하지만 기존 데이터를 아예 지우고 처음부터 다시 쌓아야 할 때 `--full-refresh` 플래그를 사용한다.

```
dbt run --select model_name --full-refresh
```
* **동작 원리:** 데이터베이스 엔진에게 "테이블이 이미 존재하더라도 **무시하고 통째로 삭제(DROP)한 뒤, CTAS로 처음부터 다시 만들어라!**" 라고 명령한다. 이때 `is_incremental()`은 `False`로 인식되어 전체 기간의 데이터를 조회하게 된다.
* **언제 사용하나요?**
    1.  **스키마(Schema)가 변경되었을 때:** 컬럼이 추가되거나 데이터 타입이 바뀌었을 때.
    2.  **비즈니스 로직이 크게 변경되었을 때:** 과거 데이터까지 전부 새로운 계산식을 적용해야 할 때.
    3.  **데이터가 심각하게 꼬였을 때:** 엉킨 과거 데이터를 초기화하고 원천부터 다시 깔끔하게 적재(Backfill)하고 싶을 때.


---

### 참고
https://zzsza.github.io/data-engineering/2025/01/16/dbt-core/

공식문서

https://docs.getdbt.com/docs/introduction?version=1.11
https://docs.getdbt.com/docs/local/install-dbt?version=1.11
https://docs.getdbt.com/docs/local/dbt-core-environments?version=1.11
https://docs.getdbt.com/docs/build/sql-models?version=1.11
