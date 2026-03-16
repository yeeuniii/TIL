# 🛠️ dbt (Data Build Tool) 

## 1. dbt의 정의와 목적

dbt는 데이터 웨어하우스 내에서 **데이터 변환(Data Transformation)** 에 특화된 도구로, ELT 파이프라인 중 **'T'** 를 담당하는 프레임워크입니다.

* **핵심 목표:** 비즈니스 로직을 반영하는 **SELECT 쿼리** 작성에 집중.
* **주요 이점:**
  * **No Boilerplate:** 반복되는 코드 없이 효율적인 개발 가능.
  * **모듈화 및 재사용:** 모델 간의 의존성 관리가 용이함.
  * **테스트 및 문서화:** 데이터 품질 테스트와 명세서 자동 생성 지원.
  * **State-aware 오케스트레이션:** 변경 사항을 감지하여 필요한 모델만 선택적으로 구축.



## 2. 구성 요소: Language & Engine

dbt는 단순한 실행기를 넘어 개발 환경 전반을 제공합니다.

* **dbt Language:** `SQL(SELECT) + Jinja(Template) + YAML(Config)`의 조합.
* **dbt Engine:**
  * **dbt Core:** Python 기반의 오픈소스 엔진.
  * **dbt Fusion:** **Rust** 기반의 고성능 엔진. 컴파일 및 SQL 분석 속도가 비약적으로 빠르며, VS Code 익스텐션을 통해 활용 가능.
  * **dbt Cloud:** SaaS 형태의 유료 서비스로 웹 UI 환경 제공.



## 3. 개발 및 운영 환경 분리

`profiles.yml` 설정을 통해 개발(Dev)과 운영(Prod) 환경을 엄격히 분리합니다.

* 권장하는 구조는 스키마 분리로, 하나의 데이터베이스 내의 다른 스키마를 사용하는 것이다. 
  * **계층 구조:** `Database > Schema > Table`
* **환경별 타겟:**
  * **Dev:** 개인별 스키마(`dbt_<username>`)를 사용하여 동료와 작업 충돌 방지.
  * **Prod:** `analytics`와 같은 공용 운영 스키마 사용.


## 4. dbt Models (데이터 모델링)

dbt에서 **모델**은 프로젝트의 가장 기본 단위입니다. 각 모델은 하나의 `.sql` 파일로 존재하며, 그 안에는 단 하나의 **SELECT 문**만 들어갑니다.

> 데이터를 어떻게 변형(Transform)할 것인지 정의한 SELECT 쿼리 파일

* **모델의 흐름:** `원천 데이터(Raw Data) → dbt Model(Transform) → 최종 데이터셋(Data Set)`
* **특징:** 모델 간의 참조(`ref`)를 통해 데이터의 흐름(Lineage)을 자동으로 파악하고 관리합니다.

### Materialization (구체화 전략)

`Config` 설정을 통해 이 모델을 데이터베이스에 **어떤 형태로 저장할 것인지** 결정합니다.

| 전략 (Materialized) | 특징 | 상세 설명 |
| --- | --- | --- |
| **View** | **논리적 뷰** | DB에 실제 데이터를 저장하지 않고 쿼리만 저장. 호출 시마다 쿼리 실행. |
| **Table** | **물리적 테이블** | 매번 기존 데이터를 Drop하고 새로 생성(전체 적재). 쿼리 속도는 빠르지만 빌드 비용 발생. |
| **Incremental** | **증분 빌드** | 변경되거나 추가된 데이터만 **Upsert** (성능 최적화). 대용량 데이터 처리에 필수적. |
| **Ephemeral** | **임시 모델** | DB에 물리적으로 저장되지 않고, 다른 모델에서 참조할 때만 CTE(Common Table Expression)로 삽입됨. |

---

## 5. 실무 운영 및 최적화 기법

### ① 데이터 계층화 

* **Staging:** 원천 데이터(Raw)를 1:1로 매핑하여 기본 정제(타입 변경, 필드명 변경).
* **Intermediate**
* **Marts** 

[best-practices](https://docs.getdbt.com/best-practices/how-we-structure/5-the-rest-of-the-project?version=1.11)

### ② 성능 최적화 및 제어

* **Incremental 전략:** `is_incremental()` 매크로와 `unique_key` 설정을 통해 불필요한 전체 재적재 방지.
* **변수(var) 활용:** 실행 시점에 `{{ var('date') }}` 등을 주입하여 유연하게 쿼리 제어.
* **CLI 명령어:**
  * `dbt run`: 모델 실행
  * `dbt test`: 정의된 데이터 품질 테스트 수행
  * `dbt build`: run + test + snapshot 등을 한 번에 실행


### ③ Airflow 연동 (BashOperator)

특정 매체 데이터 유입 시 해당 계보(Lineage)만 업데이트하도록 연산자를 활용합니다.

```
dbt run --select model_name+`
```

- **`model_name+` (뒤에 붙을 때):** 나를 포함해 내가 데이터를 주는 **하위(Downstream)** 모델들 실행.
- **`+model_name` (앞에 붙을 때):** 나를 포함해 나에게 데이터를 주는 **상위(Upstream)** 모델들 실행.
- **`+model_name+` (앞뒤에 붙을 때):** 나를 기준으로 앞뒤에 연결된 **모든 계보(Lineage)** 실행.

---
<!-- 
## 💡 최종 요약

> **"dbt는 SQL을 소프트웨어 엔지니어링의 영역으로 끌어올린 도구입니다."**   
> 데이터 계보(Lineage) 관리, 증분 적재(Incremental), 그리고 Rust 기반의 고성능 엔진(Fusion)을 통해 복잡한 데이터 파이프라인을 안정적이고 빠르게 구축할 수 있게 해줍니다. -->

### 참고
https://zzsza.github.io/data-engineering/2025/01/16/dbt-core/

공식문서

[https://docs.getdbt.com/docs/introduction?version=1.11](https://docs.getdbt.com/docs/introduction?version=1.11)  
[https://docs.getdbt.com/docs/local/install-dbt?version=1.11](https://docs.getdbt.com/docs/local/install-dbt?version=1.11)  
[https://docs.getdbt.com/docs/local/dbt-core-environments?version=1.11](https://docs.getdbt.com/docs/local/dbt-core-environments?version=1.11)  
[https://docs.getdbt.com/docs/build/sql-models?version=1.11](https://docs.getdbt.com/docs/build/sql-models?version=1.11)  
[https://docs.getdbt.com/docs/build/materializations?version=1.11](https://docs.getdbt.com/docs/build/materializations?version=1.11)  