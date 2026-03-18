AI 요약 (Gemini 3)

---

dbt 개발팀이 권장하는 **"가장 이상적이고 유지보수하기 좋은 프로젝트 구조화(Structuring)"** 에 대한 핵심 베스트 프랙티스를 이해하기 쉽게 요약해 드릴게요.

---

dbt 공식 문서의 베스트 프랙티스(Best Practice) 가이드에 따르면, dbt 프로젝트는 데이터를 소스 중심(Source-conformed)에서 비즈니스 중심(Business-conformed)으로 변환해가는 3가지 주요 계층(Staging, Intermediate, Marts)으로 구성하며, 프로젝트가 커짐에 따라 폴더와 파일명 규칙을 일관되게 가져가는 것을 가장 중요하게 강조합니다.

### 1. 🏗️ Staging (원자적 레고 블록 만들기)
Staging 계층은 Raw 데이터를 dbt의 세계로 가져오는 첫 관문입니다.
* **핵심 역할**: Raw 테이블과 **1:1로 매핑**되어 기초적인 정제(Renaming, Type casting, 기본 연산)를 수행합니다.
* **절대 금지 (Anti-patterns)**: 이 단계에서는 **다른 테이블과의 JOIN이나 복잡한 Aggregation(집계)을 절대 하지 않습니다.** 원래 데이터의 Grain(행의 기준, 예: 1줄 = 1건의 결제)을 그대로 유지해야 합니다.
* **명명 규칙 (Naming)**: `stg_[소스명]__[엔티티명]s.sql` 형식을 권장합니다. (예: `stg_stripe__payments.sql`) 
* **설정 (Config)**: 일반적으로 `view` 형태로 Materialize 하여, 최신 데이터를 유지하고 웨어하우스 공간을 절약합니다.

### 2. ⚙️ Intermediate (목적을 가진 복잡한 중간 변환)
Marts를 만들기 위해 여러 테이블을 합치거나 복잡한 계산을 해야 할 때 거쳐가는 징검다리입니다.
* **핵심 역할**: 너무 많은 로직이 Marts 모델 하나에 쏠리지 않도록, 복잡한 JOIN, 데이터 단위 변경(Fan-outs/Collapsing), 무거운 연산을 분리해냅니다.
* **언제 사용하나요?**: Marts 모델에서 4~5개 이상의 개념을 조인해야 하거나, 로직이 너무 길고 복잡해져서 디버깅이 어려울 때 사용합니다. **최종 사용자(BI 툴 등)에게 노출되어서는 안 됩니다.**
* **명명 규칙**: `int_[엔티티명]s_[동사]s.sql` 형식을 권장합니다. (예: `int_payments_pivoted_to_orders.sql`)
* **설정 (Config)**: 성능이나 재사용성에 따라 `ephemeral`이나 `view`로 구성합니다.

### 3. 🎯 Marts (비즈니스 요구사항의 최종 종착지)
데이터 분석가나 비즈니스 이해관계자(Stakeholder)가 실제로 쿼리하고 시각화하는 최종 테이블입니다.
* **핵심 역할**: 부서나 비즈니스 영역(Finance, Marketing 등)의 요구사항에 맞게 넓고(Wide), 풍부한(Rich) 데이터를 제공합니다. 데이터를 정규화(Normalization)하는 것이 아니라, 사용하기 편하게 **역정규화(Denormalization)** 하여 하나의 넓은 테이블에 모든 필요한 정보를 담는 것을 목표로 합니다.
* **명명 규칙**: 사용자가 이해하기 쉬운 명확한 영단어를 사용합니다. (예: `orders.sql`, `dim_customers.sql`, `fct_events.sql`)
* **설정 (Config)**: 조회 성능을 극대화하기 위해 일반적으로 `table`이나 `incremental`로 Materialize 합니다.

### 4. 🧩 The Rest of the Project (나머지 구성요소들)
* **Base Models (`base_`)**: 만약 Staging을 만들기 전에 동일한 스키마의 과거/현재 테이블을 UNION 해야 하거나, 삭제된 데이터(deletes table)를 미리 JOIN 해서 걸러내야 하는 특수한 경우에만 사용합니다.
* **Utilities (`models/utilities/`)**: 날짜 테이블(date spines) 등 프로젝트 전반에서 공통으로 쓰는 도구 성격의 모델을 모아둡니다.
* **Seeds (`seeds/`)**: 우편번호 매핑, 국가 코드 등 자주 변하지 않는 작고 정적인 매핑 데이터를 CSV로 관리할 때 씁니다. (Raw 데이터를 대체하는 용도로 쓰면 안 됩니다!)
* **YAML 설정 파일 (`.yml`)**: 폴더별로 하나의 설정 파일을 두는 것을 권장합니다. 파일명 앞에 언더스코어(`_`)를 붙여서 탐색기 최상단에 고정시킵니다. (예: `models/staging/stripe/_stripe__models.yml`)

<br>

> 공식 문서   
> https://docs.getdbt.com/best-practices/how-we-structure/1-guide-overview?version=1.11   
> https://docs.getdbt.com/best-practices/how-we-structure/2-staging?version=1.11   
> https://docs.getdbt.com/best-practices/how-we-structure/3-intermediate?version=1.11   
> https://docs.getdbt.com/best-practices/how-we-structure/4-marts?version=1.11   
> https://docs.getdbt.com/best-practices/how-we-structure/5-the-rest-of-the-project?version=1.11   