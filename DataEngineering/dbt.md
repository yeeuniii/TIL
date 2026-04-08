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

`profiles.yml` 설정을 통해 개발(Dev)과 운영(Prod) 환경을 엄격히 분리한다. 권장하는 구조는 스키마 분리로, 하나의 데이터베이스 내에서 서로 다른 스키마를 사용하여 환경을 격리하는 방식이다.

### 3.1 데이터 흐름과 설정 파일의 역할
dbt 프로젝트에서 데이터의 출처(Read)와 목적지(Write)는 서로 다른 설정 파일에서 독립적으로 관리된다.

*   **Source (원천 - Read): `sources.yml`**
    *   dbt가 데이터를 읽어오는 대상이다. Airbyte와 같은 추출 도구가 데이터를 적재해두는 `raw` 스키마 등을 정의하며, 접속 환경(`target`)과 관계없이 원천 데이터의 고정된 절대 경로를 참조한다.
*   **Destination (대상 - Write): `profiles.yml` (schema)**
    *   dbt가 가공한 결과를 물리적인 테이블로 생성하는 기본 작업 공간이다. 이곳에 명시된 스키마(`target_schema`)는 dbt가 `CREATE TABLE` 문을 실행할 때 기준이 되는 베이스캠프 역할을 한다.

#### 3.2 커스텀 경로 제어
dbt는 기본적으로 **`target_schema` + `custom_schema`** 를 결합하여 최종 위치를 결정한다. 하지만 실무 운영 환경에서는 연동 시스템의 안정성을 위해 이 결합 로직을 차단하고 고정 경로를 사용하며, 이를 위해 `generate_schema_name.sql` 매크로를 오버라이딩한다.

*   **샌드박스(Sandbox) 모드의 한계:** 
    dbt의 기본 동작은 개인별 테스트 공간을 생성하는 데 유리하지만, 실행 환경(target)에 따라 테이블 주소가 가변적으로 변하므로 Superset이나 Airflow와 같은 외부 연동 시스템의 설정이 빈번하게 깨지는 문제를 야기한다.
*   **고정 경로 모드의 실무적 이점:**
    1.  **연동 안정성:** 시각화 도구 및 다운스트림 시스템이 참조하는 데이터베이스 경로를 절대적으로 고정하여 운영 사고를 방지한다.
    2.  **코드 수정 없는 환경 전환:** profiles.yml의 target 설정(dev/prod)만 변경하면, SQL 소스 코드를 수정하지 않고도 데이터 적재 위치를 개발 서버와 운영 서버 사이에서 즉시 전환할 수 있다. 이는 수동 수정으로 인한 운영 사고를 원천 차단한다.
    3.  **명확한 거버넌스:** dbt의 자동 명명 규칙에 의존하지 않고, 데이터 엔지니어가 설계한 물리적 데이터 구조를 명확하게 통제하고 강제할 수 있다.


> 관련 공식 문서  
> https://docs.getdbt.com/docs/build/custom-schemas#how-does-dbt-use-custom-schemas

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
    engine='OLAP',
    keys=['id', 'name', 'created_date'],
    table_type='PRIMARY',
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
  - `default` (default): 단순히 중복 제거 없이 새 레코드를 추가한다. table_type에 따라서 upsert가 될 수도 있음. (ex. PRIMARY 테이블인 경우)
  - `insert_overwrite`: 특정 파티션을 모두 덮어쓰기
  - `dynamic_overwrite`: 특정 파티션이 없으면 새로 만들고, 있으면 덮어쓰기


정리하면,   
`table_type = PRIMARY` + `incremental_strategy = append`  
→ 코드는 단순하지만 UPSERT를 지원한는 가장 추천하는 기본 전략. 


> 관련 공식문서  
> https://docs.getdbt.com/docs/build/materializations?version=1.11  
> https://docs.getdbt.com/docs/build/incremental-models?version=1.11  
> https://docs.starrocks.io/docs/3.4/integrations/dbt/ 



## 5. dbt Macro 

dbt 매크로는 SQL에 프로그래밍 로직(Jinja)을 결합하여 사용하는, 일종의 재사용 가능한 함수이다. 반복되는 SQL 코드를 줄이고, 데이터 웨어하우스의 기본 동작을 우리 프로젝트에 맞게 재정의할 때 사용한다.

### 5.1 dbt 표준 매크로: `is_incremental()`
증분 빌드(`incremental`) 모델에 사용되는 핵심 매크로이다.   
현재 dbt 실행이 '증분 모드'인지 판별한다. 아래 조건을 모두 만족하는 경우 True를 반환한다. 
- 모델이 이미 데이터베이스 내 테이블로 존재하는 경우
- full-refresh flag로 실행되지 않은 경우
- incremental 전략인 경우

모델 코드 하단의 `WHERE` 절에서 사용하여, 최신 데이터만 읽어오도록 필터링이 필요한 경우 사용한다. 전체 데이터 스캔을 방지하여 쿼리 비용을 줄이고 빌드 속도를 높일 수 있다.

```sql
{% if is_incremental() %}
  WHERE event_timestamp > (SELECT MAX(event_timestamp) FROM {{ this }})
{% endif %}
  ```

### 5.2 매크로의 탐색 우선순위 (Hierarchy)
dbt는 매크로 호출 시 아래 순서대로 탐색하며, 동일한 이름이 있다면 상위 단계가 하위 단계를 **오버라이딩(Overriding)** 한다. 

1.  **사용자 프로젝트 (`macros/*.sql`)**: 우리가 직접 만든 로직
2.  **설치된 패키지/어댑터 (`dbt-starrocks` 등)**: 엔진 전용 로직
3.  **dbt 코어 (Global)**: 공통 기본 로직

### 5.3 StarRocks 어댑터 오버라이딩
dbt의 기본 매크로 동작이 우리 환경(StarRocks + Airflow)과 맞지 않는 경우, 동일한 이름의 매크로 파일을 만들어 재정의한 사례이다.

#### ① `generate_schema_name.sql` 

dbt가 모델을 데이터베이스에 적재할 때 사용하는 **스키마 명명 규칙을 재정의**하기 위해 사용한다. 

* **`target_schema`**: `profiles.yml`에 정의된 기본 스키마이다. 
* **`custom_schema`**: 모델의 `config`나 `dbt_project.yml`에서 별도로 지정한 스키마 이름이다.
* dbt는 **`target_schema` + `custom_schema`** 형태의 스키마 이름을 갖는다.
* 모델에 `custom_schema` 설정이 존재하는 경우, `target_schema`를 결합하지 않고 **사용자가 지정한 이름 그대로를 반환**하도록 오버라이딩하였다.
* 이를 통해 개발/운영 환경이나 실행 주체에 관계없이 데이터 계층(raw, stg, mart)에 따른 **고정된 스키마 명칭을 강제**함으로써 외부 시스템과의 정밀한 연동을 보장한다.

```jinja
{% macro generate_schema_name(custom_schema_name, node) -%}
    {%- if custom_schema_name is none -%}
        {{ target.schema }}
    {%- else -%}
        {{ custom_schema_name | trim }}
    {%- endif -%}
{%- endmacro %}
```


#### ② `make_temp_relation.sql`

Airflow에서 여러 개의 Mart 모델을 동시에 실행(`dbt run`)할 경우, dbt가 생성하는 임시 테이블 이름이 중복되어 서로 충돌하는 현상이 발생한다. `roimon` 프로젝트의 경우 pid, category, account 별로 서로 다른 DAG에서 dbt가 실행되기 때문에, 임시 테이블 네이밍 규칙에 해당 정보들을 포함시켜 고유한 이름을 갖도록 하였다. 따라서 다수의 dbt DAG이 병렬로 실행되어도 **Race Condition(동시성 충돌)이 발생하지 않고 안전하게 데이터를 적재**할 수 있다.

```jinja
{% macro make_temp_relation(base_relation, suffix='__dbt_tmp') %}
  {#-- This ensures microbatch batches get unique temp relations to avoid clobbering --#}
  {% if suffix == '__dbt_tmp' and model.batch %}
    {% set suffix = suffix ~ '_' ~ model.batch.id %}
  {% endif %}

  {#-- 커스텀 로직: 여러 Airflow 태스크가 동시에 dbt run을 실행할 때 발생하는 임시 테이블 충돌 방지 --#}
  {#-- pid_category_account 형태로 Suffix 추가 --#}
  {% set custom_suffix = [] %}
  
  {% if var('target_pid', False) %}
    {% do custom_suffix.append(var('target_pid') | replace("-", "_")) %}
  {% endif %}
  
  {% if var('target_category', False) %}
    {% do custom_suffix.append(var('target_category') | replace("-", "_")) %}
  {% endif %}
  
  {% if var('target_account', False) %}
    {% do custom_suffix.append(var('target_account') | replace("-", "_")) %}
  {% endif %}

  {% if custom_suffix | length > 0 %}
    {% set suffix = suffix ~ '_' ~ custom_suffix | join('_') %}
  {% endif %}

  {{ return(adapter.dispatch('make_temp_relation', 'dbt')(base_relation, suffix)) }}
{% endmacro %}
```

---

## 6. 모델 실행 (Execution)

dbt 프로젝트에 작성된 `.sql` 파일들(SELECT 쿼리)을 실제 데이터베이스의 뷰나 테이블로 빌드(Build)하는 과정이다. 주로 `dbt run` 명령어를 사용하며, 데이터의 계보(Lineage)와 상태에 따라 똑똑하게 실행 대상을 선택할 수 있다.

### 6.1 dbt run 
- 프로젝트 내의 **모든 모델**을 순서대로 실행한다. 
- 데이터가 많아지면 전체 실행은 부담이 될 수 있으므로, 아래의 선택자(`--select`)를 주로 사용한다.

### 6.2 Graph operators
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

### 6.3 강제 전체 새로고침 (Full Refresh)
증분 모델(`incremental`)은 평소에 `is_incremental()` 로직을 타고 새로운 데이터(INSERT)나 변경된 데이터(UPSERT)만 처리한다. 하지만 기존 데이터를 아예 지우고 처음부터 다시 쌓아야 할 때 `--full-refresh` 플래그를 사용한다.

```
dbt run --select model_name --full-refresh
```
* **동작 원리:** 데이터베이스 엔진에게 "테이블이 이미 존재하더라도 **무시하고 통째로 삭제(DROP)한 뒤, CTAS로 처음부터 다시 만들어라!**" 라고 명령한다. 이때 `is_incremental()`은 `False`로 인식되어 전체 기간의 데이터를 조회하게 된다.
* **언제 사용하나요?**
    1.  **스키마(Schema)가 변경되었을 때:** 컬럼이 추가되거나 데이터 타입이 바뀌었을 때.
    2.  **비즈니스 로직이 크게 변경되었을 때:** 과거 데이터까지 전부 새로운 계산식을 적용해야 할 때.
    3.  **데이터가 심각하게 꼬였을 때:** 엉킨 과거 데이터를 초기화하고 원천부터 다시 깔끔하게 적재(Backfill)하고 싶을 때.


### 6.4 Options
dbt 실행 시 프로젝트 설정 파일(`dbt_project.yml`)과 접속 정보 파일(`profiles.yml`)의 위치를 명시적으로 지정할 수 있다. 옵션을 생략할 경우 dbt는 기본 탐색 경로를 찾아간다.

* **`--project-dir [PATH]`**
    * **역할:** `dbt_project.yml` 파일이 위치한 디렉토리를 지정한다.
    * **기본값:** 현재 명령어를 실행 중인 디렉토리(Current Working Directory)이다.
    * 터미널의 현재 위치가 dbt 프로젝트 루트 폴더가 아닐 때, 혹은 특정 프로젝트를 외부에서 호출할 때 사용한다.

* **`--profiles-dir [PATH]`**
    * **역할:** `profiles.yml` 파일이 위치한 디렉토리를 지정한다.
    * **기본값:** 사용자 홈 디렉토리 하위의 전역 설정 폴더인 `~/.dbt/` 이다.
    * **로컬 관리:** 보안상의 이유로 계정 정보를 프로젝트 폴더 내부에 함께 두고 관리하고 싶을 때 사용한다. (예: `dbt run --profiles-dir .`)
    * **환경 격리:** 운영 서버나 CI/CD 환경에서 특정 안전한 경로에 있는 프로필을 참조해야 할 때 사용한다.

* **`--vars '[JSON_STRING]'`**
    * **역할:** 모델 내부의 `{{ var('variable_name') }}` 값을 런타임에 외부에서 동적으로 주입한다.
    * **활용:** 특정 기간(start_date, end_date)이나 특정 매체(target_pid)에 대해서만 필터링하여 실행하고 싶을 때 유용하다.
    * JSON 형식을 사용하며, 공백이나 특수문자 처리를 위해 작은따옴표(`'`)로 감싸서 전달하는 것을 권장


#### 실행 예제

```bash
dbt run \
  --profiles-dir . \
  --select gdata \
  --vars '{
    "target_pid": "apple", 
    "target_category": "store", 
    "start_date": "2026-03-20", 
    "end_date": "2026-03-24"
  }'
```

## 7. 실무 운영

### 7.1 모델 계층화 

* **Staging:** 가장 원자적인(atomic) 기본 단위. raw data를 가져와서 데이터 타입 변환, 컬럼명 통일, 간단한 필터링 등의 초기 정제 작업만 수행하는 1:1 매핑 단계. 복잡한 transformation 지양
  * 1:1 매핑이란, **1개의 raw data table = 1개의 dbt staging model** 을 의미함. 
* **Intermediate**: smarts로 가기 위한 복잡한 논리 연산을 수행. 테이블 join 및 집계와 같은 로직이 사용됨
* **Marts**: 우리가 실질적으로 필요한 최종 결과물. 

[best-practices](https://docs.getdbt.com/best-practices/how-we-structure/1-guide-overview)


---

### 참고
https://zzsza.github.io/data-engineering/2025/01/16/dbt-core/

공식문서

https://docs.getdbt.com/docs/introduction?version=1.11   
https://docs.getdbt.com/docs/local/install-dbt?version=1.11   
https://docs.getdbt.com/docs/local/dbt-core-environments?version=1.11   
https://docs.getdbt.com/docs/build/sql-models?version=1.11   
https://docs.getdbt.com/reference/commands/run?version=1.11   
https://docs.getdbt.com/reference/node-selection/graph-operators?version=1.11   
