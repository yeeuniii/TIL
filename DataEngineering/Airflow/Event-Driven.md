전통적인 Apache Airflow가 정해진 시각에 작업을 수행하는 배치(Batch) 중심의 스케줄러였다면, 최근의 Airflow는 특정 이벤트 발생에 즉각 반응하는 이벤트 기반(Event-driven) 아키텍처로 그 패러다임이 확장되고 있다. 이는 데이터 파이프라인의 실시간성과 효율성을 극대화하는 핵심적인 변화이다.

## Airflow Event-Driven

### 1. Using Airflow Sensors
가장 고전적인 방식으로, 특정 조건이 충족될 떄까지 대기하는 특수한 연산자이다.

* **작동 방식:** 주기적으로 외부 상태를 확인(Polling)하여 이벤트 발생 시 다음 작업을 실행한다. (예: `S3KeySensor`, `ExternalTaskSensor`)
* **장점:** 구현이 매우 간단하고 직관적
* **단점:** 대기하는 동안 Airflow 워커(Worker)의 슬롯을 계속 차지하므로 리소스 낭비가 발생

### 2. Using Deferrable Operators
센서의 리소스 낭비 문제를 해결하기 위해 도입된 방식이다.

* **작동 방식:** 대기 상태에 들어가면 워커 슬롯을 반환하고, `Triggerer`라는 별도의 가벼운 프로세스에 polling을 맡긴다. 이벤트 발생 시 다시 워커를 할당받아 작업이 실행된다.
* **장점:** 수천 개의 태스크가 동시에 대기하더라도 리소스 사용량이 매우 적고, 비동기(Async) 처리에 최적화되어 있다.

### 3. Using Dataset
Airflow 2.4 버전에 도입된 기능으로, 시간 중심이 아닌 **데이터의 변화**를 기준으로 DAG를 실행합니다.

* **작동 방식:**
    * **Producer DAG:** 데이터를 생성하고 특정 `Dataset`을 업데이트했다고 선언합니다.
    * **Consumer DAG:** 해당 `Dataset`이 업데이트되면 자동으로 실행됩니다.
* **장점:** DAG 간의 의존성을 명확하게 관리할 수 있으며, 복잡한 파이프라인을 유기적으로 연결



### 4. Using Airflow REST API
Airflow 외부의 시스템(예: 웹 앱, AWS Lambda)에서 직접 Airflow를 깨우는 방식

* **작동 방식:** 외부 시스템에서 Airflow의 REST API 엔드포인트(`. /api/v1/dags/{dag_id}/dagRuns`)로 POST 요청을 보낸다.
* **장점:** Airflow 환경 밖에서 발생하는 완전히 독립적인 이벤트(예: 사용자의 버튼 클릭, 외부 메시지 큐 등)에 반응할 수 있다.

---

## 요약

| 방식 | 주요 특징 | 추천 상황 |
| :--- | :--- | :--- |
| **Sensors** | 단순 폴링(Polling) | 대기 시간이 짧고 구조가 단순할 때 |
| **Deferrable** | 리소스 효율적 대기 | 대기 시간이 길거나 동시 실행이 많을 때 |
| **Datasets** | 데이터 기반 의존성 | 상속 관계에 있는 데이터 파이프라인 관리 |
| **REST API** | 외부 시스템 연동 | 외부 서비스에서 직접 DAG를 호출해야 할 때 |

---

### 결론
과거의 Airflow가 "매일 오전 8시"에 작동하는 로봇이었다면, 현재의 Airflow는 **"데이터가 도착하면", "파일이 생성되면"** 즉시 반응하는 능동적인 시스템으로 진화했다. 리소스 효율성을 생각한다면 **Deferrable Operators**를, 데이터 흐름의 정합성을 생각한다면 **Datasets** 기능을 적극 활용하는 것이 좋다.


### 참고
[Event-based DAGs in airflow](https://medium.com/codex/event-based-dags-in-airflow-7c8ee851665f)
